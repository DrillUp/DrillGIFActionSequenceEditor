#include "stdafx.h"
#include "S_LEAnnotationReader.h"


/*
-----==========================================================-----
		类：		最大值 注解 读取器.h
		所属模块：	插件模块
		功能：		只扫描最大值的相关注解。

		目标：		识别下面的文本：
						* @Drill_LE_param "内容-%d"
						* @Drill_LE_parentKey "---内容组%d至%d---"
						* @Drill_LE_var "DrillUp.g_SSpA_context_list_length"
						* @Drill_LE_editForbidden

		说明：		这里只提供单一操作，不含先后流程。
-----==========================================================-----
*/

S_LEAnnotationReader::S_LEAnnotationReader() : QObject(){
	init();
}
S_LEAnnotationReader::~S_LEAnnotationReader() {
}
/*  - - 单例 - - */
S_LEAnnotationReader* S_LEAnnotationReader::cur_manager = NULL;
S_LEAnnotationReader* S_LEAnnotationReader::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_LEAnnotationReader();
	}
	return cur_manager;
}
/*  - - 初始化 - - */
void S_LEAnnotationReader::init() {

}

/*-------------------------------------------------
		读取 - 获取注解内容
*/
C_LEAnnotation* S_LEAnnotationReader::readPlugin(QFileInfo file){
	C_LEAnnotation* result = new C_LEAnnotation();
	result->pluginName = file.fileName();	//xxxx.js
	result->fullPath = file.absoluteFilePath();

	// > 文件来源
	QFile file_from(result->fullPath);
	if (!file_from.open(QFile::ReadOnly)){
		QMessageBox message(QMessageBox::Critical, ("错误"), ("无法打开文件" + result->pluginName ));
		message.exec();
		return result;
	}
	result->context = file_from.readAll();
	file_from.close();

	// > 扫描器准备
	P_TxtFastReader reader = P_TxtFastReader(result->context);
	reader.prepare_trimAllRows();
	reader.prepare_replaceInStrings(QRegExp("( \\* )|( \\*)|(\\* )|(/\\*:)|(/\\*:ja)"), "");	//预备去掉注释

	// > 获取插件简介
	int i_desc = reader.d_indexOf("@plugindesc", 0);
	if (i_desc != -1){
		QString desc_data = reader.d_rowAt(i_desc);
		result->pluginDesc = desc_data.replace("@plugindesc", "").trimmed();
	}

	// > 获取插件作者
	int i_author = reader.d_indexOf("@author", 0);
	if (i_author != -1){
		QString author_data = reader.d_rowAt(i_author);
		result->pluginAuthor = author_data.replace("@author", "").trimmed();
	}

	// > 禁用情况检查
	if (result->context.indexOf("@Drill_LE_editForbidden") != -1){
		result->paramForbidden = true;
		return result;
	}

	// > 获取插件参数
	int length = reader.d_rowCount();
	QList<int> index_list = reader.d_getAllRowIndexsContains("@Drill_LE_param");
	for (int i = 0; i < index_list.length(); i++){
		int i_param = index_list.at(i);
		QString aaa = reader.d_rowAt(i_param + 1);
		QString bbb = reader.d_rowAt(i_param + 2);
		if (!reader.d_rowAt(i_param + 1).contains("@Drill_LE_parentKey")){ continue; }
		if (!reader.d_rowAt(i_param + 2).contains("@Drill_LE_var")){ continue; }

		// > 获取字符串（会去掉引号）
		QString s_param = reader.d_rowAt(i_param).replace("@Drill_LE_param", "");
		QString s_parentKey = reader.d_rowAt(i_param + 1).replace("@Drill_LE_parentKey", "");
		QString s_var = reader.d_rowAt(i_param + 2).replace("@Drill_LE_var", "");
		s_param = s_param.trimmed().replace("\"", "");
		s_parentKey = s_parentKey.trimmed().replace("\"", "");
		s_var = s_var.trimmed().replace("\"", "");

		// > 加入组
		C_LEAnnotation_Param c_p = C_LEAnnotation_Param();
		c_p.initParam(s_param, s_parentKey, s_var);
		result->paramList.push_back(c_p);
	}

	// > 获取参数最大值
	this->refreshPluginAnnotation(result);

	return result;
}
/*-------------------------------------------------
		读取 - 刷新参数最大值
*/
void S_LEAnnotationReader::refreshPluginAnnotation(C_LEAnnotation* plugin){

	// > 扫描器准备
	QString context = plugin->context;
	P_TxtFastReader reader = P_TxtFastReader(context);
	reader.prepare_trimAllRows();
	reader.prepare_replaceInStrings(QRegExp("( \\* )|( \\*)|(\\* )|(/\\*:)|(/\\*:ja)"), "");	//此行的去掉注释

	// > 获取 - 变量最大值
	for (int i = 0; i < plugin->paramList.count(); i++){
		C_LEAnnotation_Param pluginParam = plugin->paramList.at(i);
		QString var_str = pluginParam.getVarName();
		if (var_str == "" || var_str == "null"){ break; }

		QString re_str = var_str.replace(".", "\\.");		//防止误识别正则
		QRegExp re = QRegExp(re_str + "[ ]*=[^=]*");
		int i_var = context.indexOf(re, 0);
		int i_varEnd = context.indexOf("\n", i_var);
		if (i_var == -1){ continue; }
		if (i_varEnd == -1){ continue; }

		// > 变量值获取
		QString data = context.mid(i_var, i_varEnd - i_var);
		QStringList dataList = data.split("=");
		if (dataList.count() == 0){ continue; }
		int var_length = TTool::_to_int_(dataList.at(1));	//榨取等号后面的int值

		pluginParam.setVarLen(var_length);
		plugin->paramList.replace(i, pluginParam);
	}

	// > 搜索 - 实际最大值
	for (int i = 0; i < plugin->paramList.count(); i++){
		C_LEAnnotation_Param pluginParam = plugin->paramList.at(i);

		int real_length = 0; 
		QStringList param_names = reader.d_getAllRowsContains("@param");
		for (int j = 0; j < param_names.count(); j++){
			QString name = param_names.at(j);
			name = name.replace("@param", "").trimmed();
			
			QString match_str = pluginParam.getParamName(real_length + 1);
			if (match_str == name){
				real_length += 1;
			}
		}

		pluginParam.setRealLen(real_length);
		plugin->paramList.replace(i, pluginParam);
	}

}

/*-------------------------------------------------
		读取 - 插件是否存在"xxx-10"序列格式
*/
bool S_LEAnnotationReader::isPluginIncludedLengthParam(QString context){

	// > 扫描器准备
	P_TxtFastReader reader = P_TxtFastReader(context);
	
	// > 实际变量依次搜索
	QStringList param_names = reader.d_getAllRowsContains(QRegExp("@param[ ]+.*-[0123456789]+"));
	return param_names.count() >= 4;
}

