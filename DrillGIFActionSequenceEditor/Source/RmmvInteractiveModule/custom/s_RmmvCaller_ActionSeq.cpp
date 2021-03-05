#include "stdafx.h"
#include "s_RmmvCaller_ActionSeq.h"

#include "Source/ProjectModule/s_ProjectManager.h"
#include "Source/ProjectModule/file/s_TempFileManager.h"
#include "Source/PluginModule/storageData/s_PluginDataContainer.h"
#include "Source/ActionSeqModule/actionSeqData/s_ActionSeqDataContainer.h"
#include "s_RmmvDataContainer.h"

/*
-----==========================================================-----
		类：		rmmv交互者（动作序列 专用）.cpp
		所属模块：	交互模块
		功能：		与rmmv交互的模块，只考虑 动作序列 相关文件。

		子功能：	->文件
						-> 读取插件文件
						-> 读取系统文件
					
		使用方法：
				>打开：
					
			
-----==========================================================-----
*/

S_RmmvCaller_ActionSeq::S_RmmvCaller_ActionSeq(){
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
	QString system_context = system_file.readAll();
	// （暂无操作）
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
	S_TempFileManager::getInstance()->copy_Dir(QDir(rmmvProjectData.path + "/img/Special__actionSeq"), QDir(temp_path));

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


	// > 读取插件文件内容
	QFileInfo plugin_info = S_RmmvDataContainer::getInstance()->getRmmvFile_PluginsData();
	QFile plugin_file(plugin_info.absoluteFilePath());
	if (!plugin_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "错误", "未找到plugins.js文件。", QMessageBox::Yes);
		return ;
	}
	QString plugin_context = plugin_file.readAll();
	S_PluginDataContainer::getInstance()->loadPluginDataNoSignal(plugin_context);		//（读取文件）
	plugin_file.close();


	// > 备份文件
	if (rmmvProjectData.optional_backup == true){
		S_TempFileManager::getInstance()->copy_File(plugin_info, plugin_info.absolutePath() + "/plugins_bak.js");
	}


	// > 修改数据
	C_PluginData* data = S_PluginDataContainer::getInstance()->getPluginData("Drill_CoreOfActionSequence");
	if (data == nullptr){
		
		data = new C_PluginData();
		data->name = "Drill_CoreOfActionSequence";
		data->status = true;
		data->description = "[v1.2]        系统 - GIF动画序列核心【编辑器编辑】";
		S_PluginDataContainer::getInstance()->op_add(data);
	}
	data->parameters = S_ActionSeqDataContainer::getInstance()->getActionSeqData();	//（从 动作序列数据容器 中，获取到参数数据）
	S_PluginDataContainer::getInstance()->op_modify(data);


	// > 写入文件
	if (!plugin_file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "错误", "未找到plugins.js文件。", QMessageBox::Yes);
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


	// > 文件清空
	QString src_path = rmmvProjectData.path + "/img/Special__actionSeq";
	S_TempFileManager::getInstance()->remove_Dir(src_path);

	// > 文件转移
	QDir(src_path).mkdir(src_path);
	QList<QFileInfo> info_list = S_ActionSeqDataContainer::getInstance()->getAllRelatedFile();
	for (int i = 0; i < info_list.count(); i++){
		QFileInfo info = info_list.at(i);
		S_TempFileManager::getInstance()->copy_File(info.absoluteFilePath(), src_path + "/" + info.fileName());
	}

	// > 插件更新

	// ...

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