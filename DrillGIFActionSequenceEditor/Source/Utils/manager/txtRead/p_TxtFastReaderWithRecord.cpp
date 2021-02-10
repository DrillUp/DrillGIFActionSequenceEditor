#include "stdafx.h"
#include "P_TxtFastReaderWithRecord.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		�ı����ٶ�ȡ������¼�У�.cpp
		�汾��		v1.10
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��ȡ �ı��ļ������ݣ�����������
					���ɶԶ�ƪ�ı��������ٶȿ죨���ȿ������ܣ���
					��S_TxtReadingManager�������ƣ����ǲ���������������
					���Ż����ܶ�ȡΪ����

		ע�����	1.�ù�����ֻ���ı��������ع淶�õ��ı���

		ʹ�÷�����
				>�淶�ı�
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
		ֱ�� - ��i��
*/
QString P_TxtFastReaderWithRecord::d_rowAt(int i) {
	QString text = P_TxtFastReader::d_rowAt(i);
	if (text != ""){
		this->m_recorded_list.insert(i);
	}
	return text;
}
/*-------------------------------------------------
		ֱ�� - ����
*/
int P_TxtFastReaderWithRecord::d_rowCount() {
	return P_TxtFastReader::d_rowCount();
}

/*-------------------------------------------------
		ֱ�� - ��ȡ������
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
		ֱ�� - ��ȡ�����У���ĩβ�ң�
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
		ֱ�� - ��ȡ�����У�ָ����Χ��
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
		ֱ�� - ��ȡ�����У�ָ����Χ��
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
		ֱ�� - ��ȡ������������
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
	// ����д��

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
	// ����д��

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
		���� - ��ȡȫ���У�����ɫ��ǣ�
*/
QStringList P_TxtFastReaderWithRecord::getAllRows_FillUsedColor(){
	QStringList result_list = QStringList();
	for (int i = 0; i < this->log_context_list.count(); i++) {
		QString str = this->log_context_list.at(i);
		if (m_recorded_list.contains(i) == true) {
			result_list.push_back("<span style=\" color:#bbbbbb; \">" + str + "</span>");	//��ɫ��ʾ
		}else{
			result_list.push_back(str);
		}
	}
	return result_list;
}

