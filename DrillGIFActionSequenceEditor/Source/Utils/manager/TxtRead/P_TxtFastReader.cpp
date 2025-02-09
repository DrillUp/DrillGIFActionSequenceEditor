#include "stdafx.h"
#include "P_TxtFastReader.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		文本快速读取器.cpp
		版本：		v1.12
		作者：		drill_up
		所属模块：	工具模块
		功能：		读取 文本文件的内容，批量操作。
					【可对多篇文本解析，速度快（优先考虑性能）】
					与S_TxtReadingManager功能相似，但是不做额外多余操作。
					以优化性能读取为主。

		注意事项：	1.该管理器只读文本，并返回规范好的文本。
		知识点：	1. text = text + "aaa"; 这样写速度明显低下。
					   text.append("aaa");  建议用该函数快速拼接。
			
		使用方法：
				> 初始化
					P_TxtFastReader reader = P_TxtFastReader(context);
					reader.prepare_trimAllRows();
-----==========================================================-----
*/
P_TxtFastReader::P_TxtFastReader(QString context){
	this->log_context_list = context.split(QRegExp("[\n\r]"));		//（可以包含空行）
	this->m_trim = false;
}
P_TxtFastReader::P_TxtFastReader(QString context, QRegExp re){
	this->log_context_list = context.split(re);
	this->m_trim = false;
}
P_TxtFastReader::~P_TxtFastReader() {
}

/*-------------------------------------------------
		数据 - 获取全部行
*/
QStringList P_TxtFastReader::getAllRows() {
	return this->log_context_list;
}

/*-------------------------------------------------
		全行 - 清理空行
*/
void P_TxtFastReader::clearEmptyRows() {
	TTool::_QStringList_clearEmptyRows_(&this->log_context_list);
	//（只清理空行，不要多操作去执行trimmed）
}
/*-------------------------------------------------
		全行 - 去除注释行（根据首字符）
*/
void P_TxtFastReader::clearComment(QString prefix) {
	QStringList result;
	for (int i = 0; i < this->log_context_list.count(); i++) {
		QString str = this->log_context_list.at(i);
		if (str.mid(0, prefix.length()) != prefix) {
			result.push_back(str);
		}
	}
	this->log_context_list = result;
}
/*-------------------------------------------------
		数据 - 清理空格
*/
void P_TxtFastReader::prepare_trimAllRows() {
	this->m_trim = true;
}
/*-------------------------------------------------
		数据 - 替换字符
*/
void P_TxtFastReader::prepare_replaceInStrings(QString re_str, QString str) {
	this->m_replace_strFrom.push_back(re_str);
	this->m_replace_strTo.push_back(str);
}
void P_TxtFastReader::prepare_replaceInStrings(QRegExp re, QString str) {
	this->m_replace_expFrom.push_back(re);
	this->m_replace_expTo.push_back(str);
}
/*-------------------------------------------------
		数据 - 执行预备操作
*/
QString P_TxtFastReader::doPrepare(QString str) {
	if (this->m_trim){ str = str.trimmed();}
	for (int i = 0; i < this->m_replace_strFrom.count(); i++){
		str = str.replace(this->m_replace_strFrom.at(i), this->m_replace_strTo.at(i));
	}
	for (int i = 0; i < this->m_replace_expFrom.count(); i++){
		str = str.replace(this->m_replace_expFrom.at(i), this->m_replace_expTo.at(i));
	}
	return str;
}


/*-------------------------------------------------
		直接 - 第i行
*/
QString P_TxtFastReader::d_rowAt(int i) {
	if( i<0 || i>=this->d_rowCount() ){ return ""; }
	return this->doPrepare(log_context_list.at(i));
}
/*-------------------------------------------------
		直接 - 行数
*/
int P_TxtFastReader::d_rowCount() {
	return this->log_context_list.count();
}

/*-------------------------------------------------
		直接 - 获取包含行
*/
int P_TxtFastReader::d_indexOf(QString contains) {
	for (int i = 0; i < this->log_context_list.count(); i++) {
		QString result = this->log_context_list.at(i);
		if (result.contains(contains)) {
			return i;
		}
	}
	return -1;
}
int P_TxtFastReader::d_indexOf(QRegExp re_contains) {
	for (int i = 0; i < this->log_context_list.count(); i++) {
		QString result = this->log_context_list.at(i);
		if (result.contains(re_contains)) {
			return i;
		}
	}
	return -1;
}
/*-------------------------------------------------
		直接 - 获取包含行（从末尾找）
*/
int P_TxtFastReader::d_lastIndexOf(QString contains) {
	for (int i = this->log_context_list.count()-1; i >=0 ; i--) {
		QString result = this->log_context_list.at(i);
		if (result.contains(contains)) {
			return i;
		}
	}
	return -1;
}
int P_TxtFastReader::d_lastIndexOf(QRegExp re_contains) {
	for (int i = this->log_context_list.count() - 1; i >= 0; i--) {
		QString result = this->log_context_list.at(i);
		if (result.contains(re_contains)) {
			return i;
		}
	}
	return -1;
}

/*-------------------------------------------------
		直接 - 获取包含行（指定范围）
*/
int P_TxtFastReader::d_indexOf(QString contains, int n, int m) {
	if (n == -1) { n = 0; }
	if (m == -1) { m = this->log_context_list.count(); }
	if (n >= this->log_context_list.count() ) { return -1; }
	if (m >= this->log_context_list.count() ) { m = this->log_context_list.count(); }
	for (int i = n; i < m; i++) {
		QString result = this->log_context_list.at(i);
		if (result.contains(contains)) {
			return i;
		}
	}
	return -1;
}
int P_TxtFastReader::d_indexOf(QRegExp re_contains, int n, int m) {
	if (n == -1) { n = 0; }
	if (m == -1) { m = this->log_context_list.count(); }
	if (n >= this->log_context_list.count()) { return -1; }
	if (m >= this->log_context_list.count()) { m = this->log_context_list.count(); }
	for (int i = n; i < m; i++) {
		QString result = this->log_context_list.at(i);
		if (result.contains(re_contains)) {
			return i;
		}
	}
	return -1;
}

/*-------------------------------------------------
		直接 - 获取包含行（指定范围）
*/
QStringList P_TxtFastReader::d_getRows(int n, int row_count) {
	if (n == -1) { n = 0; }
	if (n >= this->log_context_list.count()) { return QStringList(); }
	int m = n + row_count;
	if (row_count == -1) { m = this->log_context_list.count(); }
	if (m >= this->log_context_list.count()) { m = this->log_context_list.count(); }
	QStringList result_list;
	for (int i = n; i < m; i++) {
		QString result = this->log_context_list.at(i);
		result = this->doPrepare(result);
		result_list.push_back(result);
	}
	return result_list;
}


/*-------------------------------------------------
		直接 - 获取包含的所有行
*/
QList<int> P_TxtFastReader::d_getAllRowIndexsContains(QString contains){
	QList<int> result_list;
	for (int i = 0; i < this->log_context_list.count(); i++) {
		QString result = this->log_context_list.at(i);
		if (result.contains(contains)) {
			result_list.push_back(i);
		}
	}
	return result_list;
}
QList<int> P_TxtFastReader::d_getAllRowIndexsContains(QRegExp re_contains){
	QList<int> result_list;
	for (int i = 0; i < this->log_context_list.count(); i++) {
		QString result = this->log_context_list.at(i);
		if (result.contains(re_contains)) {
			result_list.push_back(i);
		}
	}
	return result_list;
}
QStringList P_TxtFastReader::d_getAllRowsContains(QString contains) {
	QStringList result_list;
	for (int i = 0; i < this->log_context_list.count(); i++) {
		QString result = this->log_context_list.at(i);
		if (result.contains(contains)) {
			result = this->doPrepare(result);
			result_list.push_back(result);
		}
	}
	return result_list;
}
QStringList P_TxtFastReader::d_getAllRowsContains(QRegExp re_contains) {
	QStringList result_list;
	for (int i = 0; i < this->log_context_list.count(); i++) {
		QString result = this->log_context_list.at(i);
		if (result.contains(re_contains)) {
			result = this->doPrepare(result);
			result_list.push_back(result);
		}
	}
	return result_list;
}


/*-------------------------------------------------
		字符串 - 获取整数
*/
QList<int> P_TxtFastReader::s_getIntInRow(QString row) {
	QList<int> result_list;
	QStringList s_list = row.split(QRegExp("[ \t]+"));
	for (int i = 0; i < s_list.count(); i++) {
		QString result = s_list.at(i);
		if (TTool::_match_re_int_(result)) {
			result_list.push_back(result.toInt());
		}
	}
	return result_list;
}
QList<int> P_TxtFastReader::s_getIntForceInRow(QString row) {
	return TTool::_to_ints_(row);
}

/*-------------------------------------------------
		字符串 - 获取浮点数
*/
QList<double> P_TxtFastReader::s_getDoubleInRow(QString row) {
	QList<double> result_list;
	QStringList s_list = row.split(QRegExp("[ \t]+"));
	for (int i = 0; i < s_list.count(); i++) {
		QString result = s_list.at(i);
		if (TTool::_match_re_double_e_(result)) {
			result_list.push_back(result.toDouble());
		}
	}
	return result_list;
}
QList<double> P_TxtFastReader::s_getDoubleForceInRow(QString row) {
	return TTool::_to_doubles_e_(row);
}