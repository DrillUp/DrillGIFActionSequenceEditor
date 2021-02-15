#include "stdafx.h"
#include "s_LEAnnotationReader.h"


/*
-----==========================================================-----
		�ࣺ		���ֵ ע�� ��ȡ��.h
		����ģ�飺	���ģ��
		���ܣ�		ֻɨ�����ֵ�����ע�⡣

		Ŀ�꣺		ʶ��������ı���
						* @Drill_LE_param "����-%d"
						* @Drill_LE_parentKey "---������%d��%d---"
						* @Drill_LE_var "DrillUp.g_SSpA_context_list_length"
						* @Drill_LE_editForbidden

		˵����		����ֻ�ṩ��һ�����������Ⱥ����̡�
-----==========================================================-----
*/

S_LEAnnotationReader::S_LEAnnotationReader() : QObject(){
	init();
}
S_LEAnnotationReader::~S_LEAnnotationReader() {
}
/*  - - ���� - - */
S_LEAnnotationReader* S_LEAnnotationReader::cur_manager = NULL;
S_LEAnnotationReader* S_LEAnnotationReader::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_LEAnnotationReader();
	}
	return cur_manager;
}
/*  - - ��ʼ�� - - */
void S_LEAnnotationReader::init() {

}

/*-------------------------------------------------
		��ȡ - ��ȡע������
*/
C_LEAnnotation* S_LEAnnotationReader::readPlugin(QFileInfo file){
	C_LEAnnotation* result = new C_LEAnnotation();
	result->pluginName = file.fileName();	//xxxx.js
	result->fullPath = file.absoluteFilePath();

	// > �ļ���Դ
	QFile file_from(result->fullPath);
	if (!file_from.open(QFile::ReadOnly)){
		QMessageBox message(QMessageBox::Critical, ("����"), ("�޷����ļ�" + result->pluginName ));
		message.exec();
		return result;
	}
	result->context = file_from.readAll();
	file_from.close();

	// > ɨ����׼��
	P_TxtFastReader reader = P_TxtFastReader(result->context);
	reader.prepare_trimAllRows();
	reader.prepare_replaceInStrings(QRegExp("( \\* )|( \\*)|(\\* )|(/\\*:)|(/\\*:ja)"), "");	//Ԥ��ȥ��ע��

	// > ��ȡ������
	int i_desc = reader.d_indexOf("@plugindesc", 0);
	if (i_desc != -1){
		QString desc_data = reader.d_rowAt(i_desc);
		result->pluginDesc = desc_data.replace("@plugindesc", "").trimmed();
	}

	// > ��ȡ�������
	int i_author = reader.d_indexOf("@author", 0);
	if (i_author != -1){
		QString author_data = reader.d_rowAt(i_author);
		result->pluginAuthor = author_data.replace("@author", "").trimmed();
	}

	// > ����������
	if (result->context.indexOf("@Drill_LE_editForbidden") != -1){
		result->paramForbidden = true;
		return result;
	}

	// > ��ȡ�������
	int length = reader.d_rowCount();
	QList<int> index_list = reader.d_getAllRowIndexsContains("@Drill_LE_param");
	for (int i = 0; i < index_list.length(); i++){
		int i_param = index_list.at(i);
		QString aaa = reader.d_rowAt(i_param + 1);
		QString bbb = reader.d_rowAt(i_param + 2);
		if (!reader.d_rowAt(i_param + 1).contains("@Drill_LE_parentKey")){ continue; }
		if (!reader.d_rowAt(i_param + 2).contains("@Drill_LE_var")){ continue; }

		// > ��ȡ�ַ�������ȥ�����ţ�
		QString s_param = reader.d_rowAt(i_param).replace("@Drill_LE_param", "");
		QString s_parentKey = reader.d_rowAt(i_param + 1).replace("@Drill_LE_parentKey", "");
		QString s_var = reader.d_rowAt(i_param + 2).replace("@Drill_LE_var", "");
		s_param = s_param.trimmed().replace("\"", "");
		s_parentKey = s_parentKey.trimmed().replace("\"", "");
		s_var = s_var.trimmed().replace("\"", "");

		// > ������
		C_LEAnnotation_Param c_p = C_LEAnnotation_Param();
		c_p.initParam(s_param, s_parentKey, s_var);
		result->paramList.push_back(c_p);
	}

	// > ��ȡ�������ֵ
	this->refreshPluginAnnotation(result);

	return result;
}
/*-------------------------------------------------
		��ȡ - ˢ�²������ֵ
*/
void S_LEAnnotationReader::refreshPluginAnnotation(C_LEAnnotation* plugin){

	// > ɨ����׼��
	QString context = plugin->context;
	P_TxtFastReader reader = P_TxtFastReader(context);
	reader.prepare_trimAllRows();
	reader.prepare_replaceInStrings(QRegExp("( \\* )|( \\*)|(\\* )|(/\\*:)|(/\\*:ja)"), "");	//���е�ȥ��ע��

	// > ��ȡ - �������ֵ
	for (int i = 0; i < plugin->paramList.count(); i++){
		C_LEAnnotation_Param pluginParam = plugin->paramList.at(i);
		QString var_str = pluginParam.getVarName();
		if (var_str == "" || var_str == "null"){ break; }

		QString re_str = var_str.replace(".", "\\.");		//��ֹ��ʶ������
		QRegExp re = QRegExp(re_str + "[ ]*=[^=]*");
		int i_var = context.indexOf(re, 0);
		int i_varEnd = context.indexOf("\n", i_var);
		if (i_var == -1){ continue; }
		if (i_varEnd == -1){ continue; }

		// > ����ֵ��ȡ
		QString data = context.mid(i_var, i_varEnd - i_var);
		QStringList dataList = data.split("=");
		if (dataList.count() == 0){ continue; }
		int var_length = TTool::_to_int_(dataList.at(1));	//եȡ�Ⱥź����intֵ

		pluginParam.setVarLen(var_length);
		plugin->paramList.replace(i, pluginParam);
	}

	// > ���� - ʵ�����ֵ
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
		��ȡ - ����Ƿ����"xxx-10"���и�ʽ
*/
bool S_LEAnnotationReader::isPluginIncludedLengthParam(QString context){

	// > ɨ����׼��
	P_TxtFastReader reader = P_TxtFastReader(context);
	
	// > ʵ�ʱ�����������
	QStringList param_names = reader.d_getAllRowsContains(QRegExp("@param[ ]+.*-[0123456789]+"));
	return param_names.count() >= 4;
}

