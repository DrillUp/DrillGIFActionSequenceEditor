﻿#include "stdafx.h"
#include "s_RmmvCaller_ActionSeq.h"

#include "Source/ProjectModule/S_ProjectManager.h"
#include "Source/ProjectModule/File/S_TempFileManager.h"
#include "Source/PluginModule/StorageData/S_PluginDataContainer.h"
#include "Source/PluginModule/LengthEditor/S_LEAnnotationReader.h"
#include "Source/PluginModule/LengthEditor/S_LEConfigWriter.h"
#include "Source/ActionSeqModule/Data/S_ActionSeqDataContainer.h"
#include "../Base/S_RmmvDataContainer.h"

/*
-----==========================================================-----
		类：		rmmv交互者（动画序列 专用）.cpp
		所属模块：	交互模块
		功能：		与rmmv交互的模块，只考虑 动画序列 相关文件。

		子功能：	->文件
						-> 读取插件文件
						-> 读取系统文件
					
		使用方法：
				>打开：
					
			
-----==========================================================-----
*/

S_RmmvCaller_ActionSeq::S_RmmvCaller_ActionSeq() : P_RmmvCaller(){
	this->init();
}
S_RmmvCaller_ActionSeq::~S_RmmvCaller_ActionSeq() {
}

/* --------------------------------------------------------------
----------RmmvFileManager 单例
*/
S_RmmvCaller_ActionSeq* S_RmmvCaller_ActionSeq::cur_manager = NULL;
S_RmmvCaller_ActionSeq* S_RmmvCaller_ActionSeq::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_RmmvCaller_ActionSeq();
	}
	return cur_manager;
}
/* --------------------------------------------------------------
----------初始化
*/
void S_RmmvCaller_ActionSeq::init(){
}


/*-------------------------------------------------
		工程 - 读取全部相关工程文件
*/
bool S_RmmvCaller_ActionSeq::loadAllRmmvFile(C_RmmvProjectData rmmvProjectData) {
	if (rmmvProjectData.isNull()){ return false; }

	// > 读取插件文件
	QFileInfo plugin_info = S_RmmvDataContainer::getInstance()->getRmmvFile_PluginsData();
	QFile plugin_file(plugin_info.absoluteFilePath());
	if (!plugin_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "错误", "未找到plugins.js文件。", QMessageBox::Yes);
		return false;
	}
	QString plugin_context = plugin_file.readAll();
	S_PluginDataContainer::getInstance()->loadPluginData(plugin_context);		//（读取文件）
	plugin_file.close();

	// > 读取系统文件
	QFileInfo system_info = S_RmmvDataContainer::getInstance()->getRmmvFile_System();
	QFile system_file(system_info.absoluteFilePath());
	if (!system_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "错误", "未找到system.js文件。", QMessageBox::Yes);
		return false;
	}
	QString system_context = system_file.readAll();// （暂无操作）
	system_file.close();

	return true;
}

/*-------------------------------------------------
		工程 - 覆盖资源文件
*/
void S_RmmvCaller_ActionSeq::coverSourceFileToTemp(C_RmmvProjectData rmmvProjectData){
	if (rmmvProjectData.isNull()){ return ; }


	// > 资源文件夹img/Special__actionSeq
	QString temp_path = S_TempFileManager::getInstance()->getTempFileUrl() + "/Special__actionSeq";
	S_TempFileManager::getInstance()->remove_Dir(QDir(temp_path));
	S_TempFileManager::getInstance()->copy_Dir(QDir(rmmvProjectData.getRootPath() + "/img/Special__actionSeq"), QDir(temp_path));

}



/*-------------------------------------------------
		工程 - 打开rmmv（继承）
*/
C_RmmvProjectData S_RmmvCaller_ActionSeq::callRmmvSelect(){
	C_RmmvProjectData result = P_RmmvCaller::callRmmvSelect();
	S_RmmvDataContainer::getInstance()->modify(result);		//打开后，立即记录位置
	return result;
}
/*-------------------------------------------------
		工程 - 保存操作（覆盖到rmmv）
*/
void S_RmmvCaller_ActionSeq::saveDataToRmmv(C_RmmvProjectData rmmvProjectData){
	//（虚函数直接覆写）


	// > 检查插件（强制升级）
	QFileInfo file_COAS = S_RmmvDataContainer::getInstance()->getRmmvFile_Plugin("Drill_CoreOfActionSequence");
	if (file_COAS.exists()){
		QFile file_COAS_file(file_COAS.absoluteFilePath());
		if (!file_COAS_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			//（读取失败时，不操作）
		}
		QString COAS_context = file_COAS_file.readAll();
		file_COAS_file.close();

		// > 强制覆盖
		if (COAS_context.contains("动作序列-%d")){
			QFileInfo COAS_updateFile = QFileInfo(QApplication::applicationDirPath() + "/tools/Drill_CoreOfActionSequence.js");
			S_TempFileManager::getInstance()->copy_File(COAS_updateFile, file_COAS_file);
		}
	}

	// ---------------------------------------------
	// > 读取插件文件内容
	QFileInfo plugin_info = S_RmmvDataContainer::getInstance()->getRmmvFile_PluginsData();
	QFile plugin_file(plugin_info.absoluteFilePath());
	if (!plugin_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "错误", "无法打开plugins.js文件。", QMessageBox::Yes);
		return ;
	}
	QString plugin_context = plugin_file.readAll();
	S_PluginDataContainer::getInstance()->loadPluginDataNoSignal(plugin_context);		//（读取文件）
	plugin_file.close();


	// > 备份文件
	if (rmmvProjectData.optional_backup == true){
		S_TempFileManager::getInstance()->copy_File(plugin_info, plugin_info.absolutePath() + "/plugins_bak.js");
	}


	// ---------------------------------------------
	// > 获取注解 + 实际长度
	QString script_path = QApplication::applicationDirPath() + "/tools/Drill_CoreOfActionSequence.js";
	QFileInfo pluginScript_info = S_RmmvDataContainer::getInstance()->getRmmvFile_Plugin("Drill_CoreOfActionSequence");
	if (pluginScript_info.exists() == false){
		S_TempFileManager::getInstance()->copy_File(QFileInfo(script_path), pluginScript_info);
	}
	C_LEAnnotation* le_annotation = S_LEAnnotationReader::getInstance()->readPlugin(pluginScript_info.absoluteFilePath());
	
	// > 修改数据
	C_PluginData* data = S_PluginDataContainer::getInstance()->getPluginData("Drill_CoreOfActionSequence");
	if (data == nullptr){
		
		data = new C_PluginData();
		data->name = "Drill_CoreOfActionSequence";
		data->status = true;
		S_PluginDataContainer::getInstance()->op_add(data);
	}
	data->description = le_annotation->pluginDesc + "【编辑器编辑】";
	data->parameters = S_ActionSeqDataContainer::getInstance()->getActionSeqData_Object();	//（从 动画序列数据容器 中，获取到参数数据）
	S_PluginDataContainer::getInstance()->op_modify(data);


	// ---------------------------------------------
	// > 写入文件
	if (!plugin_file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "错误", "无法打开plugins.js文件。", QMessageBox::Yes);
		return;
	}
	QString new_context = S_PluginDataContainer::getInstance()->writePluginData(); 
	QTextStream write_stream(&plugin_file);
	QTextCodec *codec2 = QTextCodec::codecForName("utf-8");
	write_stream.flush();
	write_stream.setCodec(codec2);
	write_stream.seek(0);				//光标从0开始写入
	write_stream << new_context;
	plugin_file.close();


	// ---------------------------------------------
	// > 文件清空
	QString src_path = rmmvProjectData.getRootPath() + "/img/Special__actionSeq";
	S_TempFileManager::getInstance()->remove_Dir(src_path);

	// > 文件转移
	QDir(src_path).mkdir(src_path);
	QList<QFileInfo> info_list = S_ActionSeqDataContainer::getInstance()->getAllRelatedFile();
	for (int i = 0; i < info_list.count(); i++){
		QFileInfo info = info_list.at(i);
		S_TempFileManager::getInstance()->copy_File(info.absoluteFilePath(), src_path + "/" + info.fileName());
	}

	// ---------------------------------------------
	// > 插件最大值更新
	QFile script_file(pluginScript_info.absoluteFilePath());
	if (!script_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		//（读取失败时，不操作）
	}else{
		QString script_context = script_file.readAll();
		script_file.close();
		C_LEConfigData le_data = C_LEConfigData();
		C_COAS_Length as_len = S_ActionSeqDataContainer::getInstance()->getActionSeqLength();

		// > 由于这里专门针对 动画序列核心 ，所以参数都知道
		le_data.initParam("Drill_CoreOfActionSequence", "动画序列-%d", as_len.realLen_actionSeq);
		script_context = S_LEConfigWriter::getInstance()->doOverwritePlugin(script_context, le_annotation->getParamByKey("动画序列-%d"), le_data);

		le_data.initParam("Drill_CoreOfActionSequence", "状态元-%d", as_len.realLen_state);
		script_context = S_LEConfigWriter::getInstance()->doOverwritePlugin(script_context, le_annotation->getParamByKey("状态元-%d"), le_data);

		le_data.initParam("Drill_CoreOfActionSequence", "动作元-%d", as_len.realLen_action);
		script_context = S_LEConfigWriter::getInstance()->doOverwritePlugin(script_context, le_annotation->getParamByKey("动作元-%d"), le_data);

		// > 写入脚本
		if (script_context != "" &&
			script_file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text)) {

			QTextStream write_stream(&script_file);
			QTextCodec *codec2 = QTextCodec::codecForName("utf-8");
			write_stream.flush();
			write_stream.setCodec(codec2);
			write_stream.seek(0);
			write_stream << script_context;
			script_file.close();
		}
	}


	// ---------------------------------------------
	// > 文件标记（插件说明）
	QString explain_context = "";
	explain_context = explain_context + "\n  该文件夹来自插件 Drill_CoreOfActionSequence 系统 - GIF动画序列核心。";
	explain_context = explain_context + "\n  这里可以放多张图片组成的GIF哦。";
	explain_context = explain_context + "\n";
	explain_context = explain_context + "\n  ----------------------------------------------------------------------";
	explain_context = explain_context + "\n  该文件由 " + S_ProjectManager::getInstance()->data_ProjectData.getSoftname() + " 自动生成；";
	explain_context = explain_context + "\n";
	explain_context = explain_context + "\n  项目名：" + S_ProjectManager::getInstance()->data_ProjectData.getName();
	explain_context = explain_context + "\n  生成时间：" + QDateTime::currentDateTime().toString("yyyy年MM月dd日 hh时mm分ss秒");
	explain_context = explain_context + "\n  转移文件：" + QString::number(info_list.count()) + "个";
	explain_context = explain_context + "\n  ----------------------------------------------------------------------";
	explain_context = explain_context + "\n";
		
	QString explain_file_name = src_path + "/_插件说明 " + QDate::currentDate().toString("yyyy-MM-dd") + ".txt";
	QFile explain_file(explain_file_name);
	if (!explain_file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text)) {
		return;
	}
	QTextStream write_stream2(&explain_file);
	QTextCodec *codec3 = QTextCodec::codecForName("utf-8");
	write_stream2.flush();
	write_stream2.setCodec(codec3);
	write_stream2.seek(0);				//光标从0开始写入
	write_stream2 << explain_context;
	plugin_file.close();

	QMessageBox::information(nullptr, "提示", "保存成功。", QMessageBox::Yes);
}