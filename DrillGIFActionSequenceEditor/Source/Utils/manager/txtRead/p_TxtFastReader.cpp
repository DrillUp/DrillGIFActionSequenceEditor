#include "stdafx.h"
#include "P_TxtFastReader.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		�ı����ٶ�ȡ��.cpp
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
					P_TxtFastReader reader = P_TxtFastReader(context);
					reader.prepare_trimAllRows();

-----==========================================================-----
*/
P_TxtFastReader::P_TxtFastReader(QString context){
	this->log_context_list = context.split(QRegExp("[\n\r]+"));
	this->m_trim = false;
}
P_TxtFastReader::~P_TxtFastReader() {
}

/*-------------------------------------------------
		���� - ��ȡȫ����
*/
QStringList P_TxtFastReader::getAllRows() {
	return this->log_context_list;
}

/*-------------------------------------------------
		ȫ�� - �������
*/
void P_TxtFastReader::clearEmptyRows() {
	QStringList result = QStringList();
	for (int i = 0; i < this->log_context_list.count(); i++) {
		QString str = this->log_context_list.at(i).trimmed();
		if (str != "") {
			result.push_back(str);
		}
	}
	this->log_context_list = result;
}
/*-------------------------------------------------
		ȫ�� - ȥ��ע���У��������ַ���
*/
void P_TxtFastReader::clearComment(QString prefix) {
	QStringList result = QStringList();
	for (int i = 0; i < this->log_context_list.count(); i++) {
		QString str = this->log_context_list.at(i);
		if (str.mid(0, prefix.length()) != prefix) {
			result.push_back(str);
		}
	}
	this->log_context_list = result;
}
/*-------------------------------------------------
		���� - ����ո�
*/
void P_TxtFastReader::prepare_trimAllRows() {
	this->m_trim = true;
}
/*-------------------------------------------------
		���� - �滻�ַ�
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
		���� - ִ��Ԥ������
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
		ֱ�� - ��i��
*/
QString P_TxtFastReader::d_rowAt(int i) {
	if( i<0 || i>=this->d_rowCount() ){ return ""; }
	return this->doPrepare(log_context_list.at(i));
}
/*-------------------------------------------------
		ֱ�� - ����
*/
int P_TxtFastReader::d_rowCount() {
	return this->log_context_list.count();
}

/*-------------------------------------------------
		ֱ�� - ��ȡ������
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
		ֱ�� - ��ȡ�����У���ĩβ�ң�
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
		ֱ�� - ��ȡ�����У�ָ����Χ��
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
		ֱ�� - ��ȡ�����У�ָ����Χ��
*/
QStringList P_TxtFastReader::d_getRows(int n, int row_count) {
	if (n == -1) { n = 0; }
	if (n >= this->log_context_list.count()) { return QStringList(); }
	int m = n + row_count;
	if (row_count == -1) { m = this->log_context_list.count(); }
	if (m >= this->log_context_list.count()) { m = this->log_context_list.count(); }
	QStringList result_list = QStringList();
	for (int i = n; i < m; i++) {
		QString result = this->log_context_list.at(i);
		result = this->doPrepare(result);
		result_list.push_back(result);
	}
	return result_list;
}


/*-------------------------------------------------
		ֱ�� - ��ȡ������������
*/
QList<int> P_TxtFastReader::d_getAllRowIndexsContains(QString contains){
	QList<int> result_list = QList<int>();
	for (int i = 0; i < this->log_context_list.count(); i++) {
		QString result = this->log_context_list.at(i);
		if (result.contains(contains)) {
			result_list.push_back(i);
		}
	}
	return result_list;
}
QList<int> P_TxtFastReader::d_getAllRowIndexsContains(QRegExp re_contains){
	QList<int> result_list = QList<int>();
	for (int i = 0; i < this->log_context_list.count(); i++) {
		QString result = this->log_context_list.at(i);
		if (result.contains(re_contains)) {
			result_list.push_back(i);
		}
	}
	return result_list;
}
QStringList P_TxtFastReader::d_getAllRowsContains(QString contains) {
	QStringList result_list = QStringList();
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
	QStringList result_list = QStringList();
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
		�ַ��� - ��ȡ����
*/
QList<int> P_TxtFastReader::s_getIntInRow(QString row) {
	QList<int> result_list = QList<int>();
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
		�ַ��� - ��ȡ������
*/
QList<double> P_TxtFastReader::s_getDoubleInRow(QString row) {
	QList<double> result_list = QList<double>();
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