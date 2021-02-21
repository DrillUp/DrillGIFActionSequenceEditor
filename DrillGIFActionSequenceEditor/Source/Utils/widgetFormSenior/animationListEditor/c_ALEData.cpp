#include "stdafx.h"
#include "c_ALEData.h"

/*
-----==========================================================-----
		�ࣺ		����֡�༭�� ������.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		����֡�༭�� �������ࡣ
					
		˵����		��������̳���C_PiSConfig��
-----==========================================================-----
*/
C_ALEData::C_ALEData() {
	this->id = -1;
	this->gif_src = QList<QString>();			//��Դ�ļ���
	this->gif_src_file = "";					//��Դ�ļ���
	this->gif_intervalTank = QList<int>();		//֡����б�
	this->gif_interval = 4;						//֡���
	this->gif_back_run = false;					//�Ƿ񵹷�
	this->gif_tint = 0;							//ͼ��-ɫ��ֵ
	this->gif_smooth = false;					//ͼ��-ģ����Ե
}
C_ALEData::~C_ALEData(){
}

/*-------------------------------------------------
		���� - ��ʶ
*/
void C_ALEData::setId(int id){
	this->id = id;
}
/*-------------------------------------------------
		���� - ��Դ
*/
void C_ALEData::setSource(QString gif_src_file, QList<QString> gif_src){
	this->gif_src_file = gif_src_file;
	this->gif_src = gif_src;
	this->checkInterval();
}
/*-------------------------------------------------
		���� - ֡���
*/
void C_ALEData::setInterval(int gif_interval){
	int old_interval = this->gif_interval;
	this->gif_interval = gif_interval;
	for (int i = 0; i < this->gif_intervalTank.count(); i++){	//���滻������Ĭ�ϵľ�֡�����
		if (this->gif_intervalTank.at(i) == old_interval){
			this->gif_intervalTank.replace(i, gif_interval);
		}
	}
}
void C_ALEData::setInterval(int gif_interval, QList<int> gif_intervalTank){
	this->gif_interval = gif_interval;
	this->gif_intervalTank = gif_intervalTank;
	this->checkInterval();
}

/*-------------------------------------------------
		���� - ���֡�����˽�У�
*/
void C_ALEData::checkInterval(){
	for (int i = this->gif_intervalTank.count(); i < this->gif_src.count(); i++){
		this->gif_intervalTank.append(this->gif_interval);	//���Զ���� Ĭ��֡�����
	}

	if (this->gif_intervalTank.count() > this->gif_src.count()){
		int out_count = this->gif_intervalTank.count() - this->gif_src.count();
		for (int i = 0; i < out_count; i++){
			this->gif_intervalTank.removeLast();
		}
	}
}


/*-------------------------------------------------
		��ȡ - ��ȡ�ļ�����
*/
int C_ALEData::getFileCount(){
	return this->gif_src.count();
}
/*-------------------------------------------------
		��ȡ - ��ȡ�ļ�
*/
QFileInfo C_ALEData::getFile(int index){
	if (index >= this->getFileCount()){ return QFileInfo(); }

	QString path = this->gif_src_file;
	if (path.at(path.length() - 1) != '/'){ path += "/"; }
	path +=this->gif_src.at(index);
	return QFileInfo(path);
}
QList<QFileInfo> C_ALEData::getFileList(QList<int> index_list){
	QList<QFileInfo> result_list = QList<QFileInfo>();
	for (int i = 0; i < index_list.count(); i++){
		result_list.append(this->getFile(index_list.at(i)));
	}
	return result_list;
}
QList<QFileInfo> C_ALEData::getAllFile(){
	QList<QFileInfo> result_list = QList<QFileInfo>();
	for (int i = 0; i < this->gif_src.count(); i++){
		result_list.append(this->getFile(i));
	}
	return result_list;
}
/*-------------------------------------------------
		��ȡ - ��ȡ�ļ�·����F:/aaa/vvv ��
*/
QString C_ALEData::getFileRoot(){
	QString path = this->gif_src_file;
	if (path.at(path.length() - 1) == '/'){ path = path.mid(0, path.count() - 1); }
	return path;
}
/*-------------------------------------------------
		��ȡ - ��ȡ֡���
*/
QList<int> C_ALEData::getAllInterval(){
	return this->gif_intervalTank;
}
/*-------------------------------------------------
		��ȡ - ��ȡ֡����ı���"0.01"��ʵ��֡Ϊ 0.01666 * n ��
*/
QString C_ALEData::getIntervalString(int index){
	int interval = this->gif_interval;
	if (index < this->getFileCount()){ 
		interval = this->gif_intervalTank.at(index);
	}
	return QString::number(interval*0.01);
}
/*-------------------------------------------------
		��ȡ - ��ȡ��ʶ
*/
int C_ALEData::getId(){
	return this->id;
}
/*-------------------------------------------------
		��ȡ - ����ļ��������ļ���׺��
*/
bool C_ALEData::hasFileName(QString file_name){
	return this->gif_src.contains(file_name);
}


/*-------------------------------------------------
		���� - ����
*/
void C_ALEData::op_append(QString gif_src){
	this->gif_src.append(gif_src);
	this->gif_intervalTank.append(gif_interval);
}
void C_ALEData::op_insert(int index, QString gif_src){
	this->gif_src.insert(index, gif_src);
	this->gif_intervalTank.insert(index,gif_interval);
}
void C_ALEData::op_insert(int index, QStringList gif_src_list, QList<int> interval_list){
	for (int i = 0; i < gif_src_list.count(); i++){

		// > ��Դ
		this->gif_src.insert(index + i, gif_src_list.at(i));
		
		// > ֡���
		int interval = gif_interval;
		if (i < interval_list.count()){ interval = interval_list.at(i); }
		this->gif_intervalTank.insert(index + i, interval);
	}
}
/*-------------------------------------------------
		���� - �滻
*/
void C_ALEData::op_replace(int index, QString gif_src){
	this->gif_src.replace(index, gif_src);
}
void C_ALEData::op_replace_interval(int index, int interval){
	this->gif_intervalTank.replace(index, interval);
}
void C_ALEData::op_replace_interval(QList<int> index_list, int interval){
	for (int i = 0; i < index_list.count(); i++){
		this->gif_intervalTank.replace(index_list.at(i), interval);
	}
}
/*-------------------------------------------------
		���� - �Ƴ�
*/
void C_ALEData::op_remove(int index){
	this->gif_src.removeAt(index);
	this->gif_intervalTank.removeAt(index);
}
/*-------------------------------------------------
		���� - ����λ��
*/
void C_ALEData::op_swap(int index_a, int index_b){
	this->gif_src.swap(index_a, index_b);
	this->gif_intervalTank.swap(index_a, index_b);
}


/*-------------------------------------------------
		���� - �����ļ���ȡ�� tempFile ����Ƭ�Σ�
*/
bool C_ALEData::copyFile(QString filePath_from, QString filePath_to) {
	QFileInfo info_from(filePath_from);
	QFileInfo info_to(filePath_to);
	QFile file_from(info_from.absoluteFilePath());
	QFile file_to(info_to.absoluteFilePath());
	if (info_from.absoluteFilePath() == info_to.absoluteFilePath()){ return true; }		//����ͬ·��������

	if (!file_from.open(QIODevice::ReadOnly)) { return false; }
	if (!file_to.open(QIODevice::WriteOnly | QIODevice::Truncate)) { return false; }

	QByteArray ba = file_from.readAll();
	file_to.write(ba);
	file_from.close();
	file_to.close();
	return true;
}


/*-------------------------------------------------
		���ж�
*/
bool C_ALEData::isNull(){
	return this->id == -1;
}
/*-------------------------------------------------
		ʵ���� -> QJsonObject
*/
QJsonObject C_ALEData::getJsonObject(){
	QJsonObject obj = QJsonObject();

	QJsonArray arr_1 = QJsonArray();
	for (int i = 0; i < this->gif_src.count(); i++){ arr_1.append(this->gif_src.at(i)); }
	obj.insert("gif_src", arr_1);
	QJsonArray arr_2 = QJsonArray();
	for (int i = 0; i < this->gif_intervalTank.count(); i++){ arr_2.append(this->gif_intervalTank.at(i)); }
	obj.insert("gif_intervalTank", arr_2);

	obj.insert("id", this->id);
	//obj.insert("gif_src_file", this->gif_src_file);��temp�ļ����е����ã�ʵ�ʲ��棩
	obj.insert("gif_interval", this->gif_interval);
	obj.insert("gif_back_run", this->gif_back_run);
	obj.insert("gif_tint", this->gif_tint);
	obj.insert("gif_smooth", this->gif_smooth);
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_ALEData::setJsonObject(QJsonObject obj){
	if (obj.value("gif_src").isUndefined() == false){
		this->gif_src.clear();
		QJsonArray arr = obj.value("gif_src").toArray();
		for (int i = 0; i < arr.count(); i++){
			this->gif_src.append(arr.at(i).toString());
		}
	}
	if (obj.value("gif_intervalTank").isUndefined() == false){
		this->gif_intervalTank.clear();
		QJsonArray arr = obj.value("gif_intervalTank").toArray();
		for (int i = 0; i < arr.count(); i++){
			this->gif_intervalTank.append(arr.at(i).toInt());
		}
	}

	if (obj.value("id").isUndefined() == false){ this->id = obj.value("id").toInt(); }
	//if (obj.value("gif_src_file").isUndefined() == false){ this->gif_src_file = obj.value("gif_src_file").toString(); }
	if (obj.value("gif_interval").isUndefined() == false){ this->gif_interval = obj.value("gif_interval").toInt(); }
	if (obj.value("gif_back_run").isUndefined() == false){ this->gif_back_run = obj.value("gif_back_run").toBool(); }
	if (obj.value("gif_tint").isUndefined() == false){ this->gif_tint = obj.value("gif_tint").toInt(); }
	if (obj.value("gif_smooth").isUndefined() == false){ this->gif_smooth = obj.value("gif_smooth").toBool(); }
}