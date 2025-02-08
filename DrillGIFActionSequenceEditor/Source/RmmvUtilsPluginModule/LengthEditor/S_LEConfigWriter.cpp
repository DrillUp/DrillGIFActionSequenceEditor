#include "stdafx.h"
#include "S_LEConfigWriter.h"


/*
-----==========================================================-----
		类：		文本扫描器.h
		版本：		v1.01
		作者：		drill_up
		所属模块：	插件模块
		功能：		提供扫描、覆写直接功能。

		目标：		识别下面的文本，并作修改：
						* @Drill_LE_param "内容-%d"
						* @Drill_LE_parentKey "---内容组%d至%d---"
						* @Drill_LE_var "DrillUp.g_SSpA_context_list_length"

		说明：		这里只提供必要独立功能。
					不考虑流程性的结构。
					不考虑数据存储，只是一个执行工具。

-----==========================================================-----
*/
S_LEConfigWriter::S_LEConfigWriter() : QObject(){
}
S_LEConfigWriter::~S_LEConfigWriter() {
}

/* --------------------------------------------------------------
----------单例
*/
S_LEConfigWriter* S_LEConfigWriter::cur_manager = NULL;
S_LEConfigWriter* S_LEConfigWriter::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_LEConfigWriter();
	}
	return cur_manager;
}


/*-------------------------------------------------
		覆写 - 最大值修改
*/
QString S_LEConfigWriter::doOverwritePlugin(QString context, C_LEAnnotation_Param param, C_LEConfigData config){
	
	// > 最大值检查
	if (config.getConfigLen() <= -1 ){
		QMessageBox::warning(nullptr, "错误", "你需要先配置" + config.getPluginName() + "的最大值。", QMessageBox::Yes);
		return "";
	}
	if (config.getConfigLen() <= 4){
		QMessageBox::warning(nullptr, "错误", "插件" + config.getPluginName() + "参数的最大值不能小于4。", QMessageBox::Yes);
		return "";
	}
	if (param.getRealLen() < 4){
		// （该错误只在单独调用该函数时会出现）
		QMessageBox::warning(nullptr, "错误", "使用修改器函数时，必须要知道修改的目标插件实际长度。\n（通过读取器中读插件获取）", QMessageBox::Yes);
		return "";
	}
	// > 相同最大值不修改
	if (config.getConfigLen() == param.getRealLen()){ return context; }


	QStringList context_list = context.split(QRegExp("(\n\r)|(\n)|(\r\n)"));
	// -----------------------------
	// > 不含分组的生成
	if (!param.isParentGrouping()){

		// > 找到变量起始行
		int i_param = -1;
		for (int i = 0; i < context_list.count(); i++){
			QString text = context_list.at(i);
			if (text.contains(QRegExp(param.getParamCommentRe(1)))){	//必须全词匹配（"@param SV敌人-1" 不能匹配 "敌人-%d" ）
				i_param = i;
				break;
			}
		}

		// > 严重错误检查
		if (i_param == -1){
			QMessageBox::warning(nullptr, "错误", "插件" + config.getPluginName() + "转换时发生了严重错误，该错误绕开了编辑器的检查程序。\n请将该错误的具体情况告知我drill_up。", QMessageBox::Yes);
			return "";
		}

		// > 找到第一个变量结构
		QStringList param_str_list = this->getParamStringList(context_list, param.getParamCommentRe(1));

		// > 迭代遍历删除行
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

		// > 重新生成
		for (int i = 0; i < config.getConfigLen(); i++){
			QString data_str = param_str_list.join("\n");
			data_str = data_str.replace(param.getParamName(1), param.getParamName(i + 1));
			data_str = data_str.replace(QRegExp("@default[^\n]*"), "@default ");		//清空默认值
			data_str += "\n * ";
			context_list.insert(i_param + i, data_str);
		}


	// -----------------------------
	// > 含分组的生成
	}else{

		// > 找到变量起始行
		int i_param = 0;
		for (int i = 0; i < context_list.count(); i++){
			QString text = context_list.at(i);
			if (text.contains(QRegExp(param.getParamCommentRe(1)))){	//必须全词匹配（"@param SV敌人-1" 不能匹配 "敌人-%d" ）
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

		// > 严重错误检查
		if (i_param == -1 || i_parent == -1){
			QMessageBox::warning(nullptr, "错误", "插件" + config.getPluginName() + "转换时发生了严重错误，该错误绕开了编辑器的检查程序。\n请将该错误的具体情况告知我drill_up。", QMessageBox::Yes);
			return "";
		}
		
		// > 找到第一个变量结构
		QStringList param_str_list = this->getParamStringList(context_list, param.getParamCommentRe(1));
		QStringList parent_str_list = this->getParamStringList(context_list, param.getParentCommentRe(1));

		// > 迭代遍历删除行
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
		// > 迭代遍历删除分组
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


		// > 重新生成
		for (int i = 0; i < config.getConfigLen(); i++){
			QString parent_str = "";
			QString param_str = "";
			// > 分组
			if (param.isParentGrouping() && i % 20 == 0){
				parent_str += parent_str_list.join("\n");
				parent_str = parent_str.replace(param.getParentName(1), param.getParentName(i + 1));
				parent_str += "\n * ";
				parent_str += "\n";
			}

			// > 变量
			param_str += param_str_list.join("\n");
			param_str = param_str.replace(param.getParamName(1), param.getParamName(i + 1));
			param_str = param_str.replace(QRegExp("@default[^\n]*"), "@default ");		//清空默认值
			if (param.isParentGrouping()){	//parent修改
				param_str = param_str.replace(param.getParentName(1), param.getParentName(i + 1));
			}
			param_str += "\n * ";
			context_list.insert(i_param + i, parent_str + param_str);
		}
	}

	// > 缩短多余星号
	QString result = context_list.join("\n");
	result = result.replace(QRegExp("([ \t\n\r]+\\*){5,}"),"\n * \n *");
	
	// > 替换
	QString var_str = param.getVarName();
	if (var_str != "" && var_str != "null"){
		QString re_str = var_str.replace(".", "\\.");		//防止误识别正则
		QRegExp re = QRegExp(re_str + "[ ]*=[^=]*\n");
		QString var_defineString = param.getVarName() + " = " + QString::number(config.getConfigLen()) + ";\n";
		result = result.replace(re, var_defineString);
	}

	return result;
}

/*-------------------------------------------------
		覆写 - 根据 "阶段-1" 获取到 "@param 阶段-1" 的全部参数字符串
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
				// > 结构寻找结束
				break;
			}
		}
	}
	return result_list;
}