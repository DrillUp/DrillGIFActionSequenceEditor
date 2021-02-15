#include "stdafx.h"
#include "s_LEConfigWriter.h"


/*
-----==========================================================-----
		�ࣺ		�ı�ɨ����.h
		����ģ�飺	����ģ��
		���ܣ�		�ṩɨ�衢��дֱ�ӹ��ܡ�

		Ŀ�꣺		ʶ��������ı��������޸ģ�
						* @Drill_LE_param "����-%d"
						* @Drill_LE_parentKey "---������%d��%d---"
						* @Drill_LE_var "DrillUp.g_SSpA_context_list_length"

		˵����		����ֻ�ṩ��Ҫ�������ܡ�
					�����������ԵĽṹ��
					���������ݴ洢��ֻ��һ��ִ�й��ߡ�

-----==========================================================-----
*/

S_LEConfigWriter::S_LEConfigWriter() : QObject(){
	init();
}
S_LEConfigWriter::~S_LEConfigWriter() {
}
/*  - - ���� - - */
S_LEConfigWriter* S_LEConfigWriter::cur_manager = NULL;
S_LEConfigWriter* S_LEConfigWriter::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_LEConfigWriter();
	}
	return cur_manager;
}
/*  - - ��ʼ�� - - */
void S_LEConfigWriter::init() {

}


/*-------------------------------------------------
		��д - ���ֵ�޸�
*/
QString S_LEConfigWriter::doOverwritePlugin(QString context, C_LEAnnotation_Param param, C_LEConfigData config){
	
	// > ���ֵ���
	if (config.getConfigLen() <= -1 ){
		QMessageBox::warning(nullptr, "����", "����Ҫ������" + config.getPluginName() + "�����ֵ��", QMessageBox::Yes);
		return "";
	}
	if (config.getConfigLen() <= 4){
		QMessageBox::warning(nullptr, "����", "���" + config.getPluginName() + "���������ֵ����С��4��", QMessageBox::Yes);
		return "";
	}
	// > ��ͬ���ֵ���޸�
	if (config.getConfigLen() == param.getRealLen()){ return context; }
	

	QStringList context_list = context.split(QRegExp("(\n\r)|(\n)|(\r\n)"));
	// -----------------------------
	// > �������������
	if (!param.isParentGrouping()){

		// > �ҵ�������ʼ��
		int i_param = -1;
		for (int i = 0; i < context_list.count(); i++){
			QString text = context_list.at(i);
			if (text.contains(QRegExp(param.getParamCommentRe(1)))){	//����ȫ��ƥ�䣨"@param SV����-1" ����ƥ�� "����-%d" ��
				i_param = i;
				break;
			}
		}

		// > ���ش�����
		if (i_param == -1){
			QMessageBox::warning(nullptr, "����", "���" + config.getPluginName() + "ת��ʱ���������ش��󣬸ô����ƿ��˱༭���ļ�����\n�뽫�ô���ľ��������֪��drill_up��", QMessageBox::Yes);
			return "";
		}

		// > �ҵ���һ�������ṹ
		QStringList param_str_list = this->getParamStringList(context_list, param.getParamCommentRe(1));

		// > ��������ɾ����
		int name_index = param.getRealLen();
		bool deleteOn = false;
		for (int i = context_list.count() - 1; i >= i_param; i--){
			QString text = context_list.at(i).trimmed();
			if (text.contains(QRegExp(param.getParamCommentRe(name_index)))){
				deleteOn = true;
				name_index--;
				context_list.removeAt(i);
				i++;
				continue;
			}
			if (deleteOn == true){
				if (text.contains("@") && !text.contains("@param")){
					context_list.removeAt(i);
					i++;
				}
				else{
					deleteOn = false;
				}
			}
		}

		// > ��������
		for (int i = 0; i < config.getConfigLen(); i++){
			QString data_str = param_str_list.join("\n");
			data_str = data_str.replace(param.getParamName(1), param.getParamName(i + 1));
			data_str = data_str.replace(QRegExp("@default[^\n]*"), "@default ");		//���Ĭ��ֵ
			data_str += "\n * ";
			context_list.insert(i_param + i, data_str);
		}


	// -----------------------------
	// > �����������
	}else{

		// > �ҵ�������ʼ��
		int i_param = 0;
		for (int i = 0; i < context_list.count(); i++){
			QString text = context_list.at(i);
			if (text.contains(QRegExp(param.getParamCommentRe(1)))){	//����ȫ��ƥ�䣨"@param SV����-1" ����ƥ�� "����-%d" ��
				i_param = i;
				break;
			}
		}
		int i_parent = 0;
		for (int i = 0; i < context_list.count(); i++){
			QString text = context_list.at(i);
			if (text.contains(QRegExp(param.getParentCommentRe(1)))){
				i_parent = i;
				break;
			}
		}

		// > ���ش�����
		if (i_param == -1 || i_parent == -1){
			QMessageBox::warning(nullptr, "����", "���" + config.getPluginName() + "ת��ʱ���������ش��󣬸ô����ƿ��˱༭���ļ�����\n�뽫�ô���ľ��������֪��drill_up��", QMessageBox::Yes);
			return "";
		}
		
		// > �ҵ���һ�������ṹ
		QStringList param_str_list = this->getParamStringList(context_list, param.getParamCommentRe(1));
		QStringList parent_str_list = this->getParamStringList(context_list, param.getParentCommentRe(1));

		// > ��������ɾ����
		int name_index = param.getRealLen();
		bool deleteOn = false;
		for (int i = context_list.count() - 1; i >= i_param; i--){
			QString text = context_list.at(i).trimmed();
			if (text.contains(QRegExp(param.getParamCommentRe(name_index)))){
				deleteOn = true;
				name_index--;
				context_list.removeAt(i);
				i++;
				continue;
			}
			if (deleteOn == true){
				if (text.contains("@") && !text.contains("@param")){
					context_list.removeAt(i);
					i++;
				}
				else{
					deleteOn = false;
				}
			}
		}
		// > ��������ɾ������
		name_index = param.getRealLen();
		for (int i = context_list.count() - 1; i >= i_parent; i--){
			QString text = context_list.at(i).trimmed();
			if (text.contains(QRegExp(param.getParentCommentRe(name_index)))){
				deleteOn = true;
				name_index -= 20;
				context_list.removeAt(i);
				i++;
				continue;
			}
			if (deleteOn == true){
				if (text.contains("@") && !text.contains("@param")){
					context_list.removeAt(i);
					i++;
				}
				else{
					deleteOn = false;
				}
			}
		}


		// > ��������
		for (int i = 0; i < config.getConfigLen(); i++){
			QString parent_str = "";
			QString param_str = "";
			// > ����
			if (param.isParentGrouping() && i % 20 == 0){
				parent_str += parent_str_list.join("\n");
				parent_str = parent_str.replace(param.getParentName(1), param.getParentName(i + 1));
				parent_str += "\n * ";
				parent_str += "\n";
			}

			// > ����
			param_str += param_str_list.join("\n");
			param_str = param_str.replace(param.getParamName(1), param.getParamName(i + 1));
			param_str = param_str.replace(QRegExp("@default[^\n]*"), "@default ");		//���Ĭ��ֵ
			if (param.isParentGrouping()){	//parent�޸�
				param_str = param_str.replace(param.getParentName(1), param.getParentName(i + 1));
			}
			param_str += "\n * ";
			context_list.insert(i_param + i, parent_str + param_str);
		}
	}

	// > ���̶����Ǻ�
	QString result = context_list.join("\n");
	result = result.replace(QRegExp("([ \t\n\r]+\\*){5,}"),"\n * \n *");
	
	// > �滻
	QString var_str = param.getVarName();
	if (var_str != "" && var_str != "null"){
		QString re_str = var_str.replace(".", "\\.");		//��ֹ��ʶ������
		QRegExp re = QRegExp(re_str + "[ ]*=[^=]*\n");
		QString var_defineString = param.getVarName() + " = " + QString::number(config.getConfigLen()) + ";\n";
		result = result.replace(re, var_defineString);
	}

	return result;
}

/*-------------------------------------------------
		��д - ���� "�׶�-1" ��ȡ�� "@param �׶�-1" ��ȫ�������ַ���
*/
QStringList S_LEConfigWriter::getParamStringList(QStringList contextList,QRegExp re){
	QStringList result_list = QStringList();
	bool finded = false;
	for (int i = 0; i < contextList.count(); i++){
		QString text = contextList.at(i);
		if (finded == false){
			if (text.contains(re)){
				finded = true;
				result_list.append(text);
			}
		}
		else{	// finded == true
			if (text.contains("@") && !text.contains("@param")){
				result_list.append(text);
			}else{
				// > �ṹѰ�ҽ���
				break;
			}
		}
	}
	return result_list;
}