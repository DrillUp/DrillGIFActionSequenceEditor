#include "stdafx.h"
#include "P_TxtFastReaderWithRecord.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		文本快速读取器（记录行）.cpp
		版本：		v1.10
		作者：		drill_up
		所属模块：	工具模块
		功能：		读取 文本文件的内容，批量操作。
					【可对多篇文本解析，速度快（优先考虑性能）】
					与S_TxtReadingManager功能相似，但是不做额外多余操作。
					以优化性能读取为主。

		注意事项：	1.该管理器只读文本，并返回规范好的文本。

		使用方法：
				>规范文本
					P_TxtFastReaderWithRecord reader = P_TxtFastReaderWithRecord(context);
					reader.prepare_trimAllRows();

-----==========================================================-----
*/
P_TxtFastReaderWithRecord::P_TxtFastReaderWithRecord(QString context) : P_TxtFastReader(context){
	this->m_recorded_list = QSet<int>();
}
P_TxtFastReaderWithRecord::~P_TxtFastReaderWithRecord() {
}


/*-------------------------------------------------
		直接 - 第i行
*/
QString P_TxtFastReaderWithRecord::d_rowAt(int i) {
	QString text = P_TxtFastReader::d_rowAt(i);
	if (text != ""){
		this->m_recorded_list.insert(i);
	}
	return text;
}
/*-------------------------------------------------
		直接 - 行数
*/
int P_TxtFastReaderWithRecord::d_rowCount() {
	return P_TxtFastReader::d_rowCount();
}

/*-------------------------------------------------
		直接 - 获取包含行
*/
int P_TxtFastReaderWithRecord::d_indexOf(QString contains) {
	int row_index = P_TxtFastReader::d_indexOf(contains);
	if (row_index != -1){
		this->m_recorded_list.insert(row_index);
	}
	return row_index;
}
int P_TxtFastReaderWithRecord::d_indexOf(QRegExp re_contains) {
	int row_index = P_TxtFastReader::d_indexOf(re_contains);
	if (row_index != -1){
		this->m_recorded_list.insert(row_index);
	}
	return row_index;
}
/*-------------------------------------------------
		直接 - 获取包含行（从末尾找）
*/
int P_TxtFastReaderWithRecord::d_lastIndexOf(QString contains) {
	int row_index = P_TxtFastReader::d_lastIndexOf(contains);
	if (row_index != -1){
		this->m_recorded_list.insert(row_index);
	}
	return row_index;
}
int P_TxtFastReaderWithRecord::d_lastIndexOf(QRegExp re_contains) {
	int row_index = P_TxtFastReader::d_lastIndexOf(re_contains);
	if (row_index != -1){
		this->m_recorded_list.insert(row_index);
	}
	return row_index;
}

/*-------------------------------------------------
		直接 - 获取包含行（指定范围）
*/
int P_TxtFastReaderWithRecord::d_indexOf(QString contains, int n, int m) {
	int row_index = P_TxtFastReader::d_indexOf(contains,n,m);
	if (row_index != -1){
		this->m_recorded_list.insert(row_index);
	}
	return row_index;
}
int P_TxtFastReaderWithRecord::d_indexOf(QRegExp re_contains, int n, int m) {
	int row_index = P_TxtFastReader::d_indexOf(re_contains, n, m);
	if (row_index != -1){
		this->m_recorded_list.insert(row_index);
	}
	return row_index;
}

/*-------------------------------------------------
		直接 - 获取包含行（指定范围）
*/
QStringList P_TxtFastReaderWithRecord::d_getRows(int n, int row_count) {
	QStringList rows = P_TxtFastReader::d_getRows(n, row_count);
	if (rows.count() != 0){
		for (int i = 0; i < rows.count(); i++){
			this->m_recorded_list.insert(n+i);
		}
	}
	return rows;
}


/*-------------------------------------------------
		直接 - 获取包含的所有行
*/
QList<int> P_TxtFastReaderWithRecord::d_getAllRowIndexsContains(QString contains){
	QList<int> index_list = P_TxtFastReader::d_getAllRowIndexsContains(contains);
	for (int i = 0; i < index_list.count(); i++) {
		this->m_recorded_list.insert(index_list.at(i));
	}
	return index_list;
}
QList<int> P_TxtFastReaderWithRecord::d_getAllRowIndexsContains(QRegExp re_contains){
	QList<int> index_list = P_TxtFastReader::d_getAllRowIndexsContains(re_contains);
	for (int i = 0; i < index_list.count(); i++) {
		this->m_recorded_list.insert(index_list.at(i));
	}
	return index_list;
}
QStringList P_TxtFastReaderWithRecord::d_getAllRowsContains(QString contains) {
	// （覆写）

	QStringList result_list = QStringList();
	for (int i = 0; i < this->log_context_list.count(); i++) {
		QString result = this->log_context_list.at(i);
		if (result.contains(contains)) {
			result = this->doPrepare(result);
			result_list.push_back(result);
			this->m_recorded_list.insert(i);
		}
	}
	return result_list;
}
QStringList P_TxtFastReaderWithRecord::d_getAllRowsContains(QRegExp re_contains) {
	// （覆写）

	QStringList result_list = QStringList();
	for (int i = 0; i < this->log_context_list.count(); i++) {
		QString result = this->log_context_list.at(i);
		if (result.contains(re_contains)) {
			result = this->doPrepare(result);
			result_list.push_back(result);
			this->m_recorded_list.insert(i);
		}
	}
	return result_list;
}


/*-------------------------------------------------
		数据 - 获取全部行（含颜色标记）
*/
QStringList P_TxtFastReaderWithRecord::getAllRows_FillUsedColor(){
	QStringList result_list = QStringList();
	for (int i = 0; i < this->log_context_list.count(); i++) {
		QString str = this->log_context_list.at(i);
		if (m_recorded_list.contains(i) == true) {
			result_list.push_back("<span style=\" color:#bbbbbb; \">" + str + "</span>");	//彩色显示
		}else{
			result_list.push_back(str);
		}
	}
	return result_list;
}

