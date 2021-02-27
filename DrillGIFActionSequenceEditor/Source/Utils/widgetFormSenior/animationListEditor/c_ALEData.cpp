#include "stdafx.h"
#include "c_ALEData.h"

/*
-----==========================================================-----
		类：		动画帧编辑块 数据类.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		动画帧编辑块 的数据类。
					注意，只有五项。如果有其他数据合并，想办法另起一个类。
-----==========================================================-----
*/
C_ALEData::C_ALEData() {
	this->id = -1;
	this->gif_src = QList<QString>();			//资源文件名
	this->gif_src_file = "";					//资源文件夹
	this->gif_intervalTank = QList<int>();		//帧间隔列表
	this->gif_interval = 4;						//帧间隔
}
C_ALEData::~C_ALEData(){
}

/*-------------------------------------------------
		设置 - 标识
*/
void C_ALEData::setId(int id){
	this->id = id;
}
/*-------------------------------------------------
		设置 - 资源
*/
void C_ALEData::setSource(QString gif_src_file, QList<QString> gif_src){
	this->gif_src_file = gif_src_file;
	this->gif_src = gif_src;
	this->checkInterval();
}
void C_ALEData::setSourceParentFile(QString gif_src_file){
	this->gif_src_file = gif_src_file;
}
/*-------------------------------------------------
		设置 - 帧间隔
*/
void C_ALEData::setInterval(int gif_interval, QList<int> gif_intervalTank){
	this->gif_interval = gif_interval;
	this->gif_intervalTank = gif_intervalTank;
	this->checkInterval();
}
void C_ALEData::setIntervalDefault(int gif_interval){
	this->gif_interval = gif_interval;
	this->checkIntervalValue();
}
void C_ALEData::setIntervalDefaultAndChange(int gif_interval){
	int old_interval = this->gif_interval;
	this->gif_interval = gif_interval;
	this->checkIntervalValue();
	for (int i = 0; i < this->gif_intervalTank.count(); i++){	//（替换掉所有默认的旧帧间隔）
		if (this->gif_intervalTank.at(i) == old_interval){
			this->gif_intervalTank.replace(i, gif_interval);
		}
	}
}

/*-------------------------------------------------
		设置 - 检查帧间隔（私有）
*/
void C_ALEData::checkInterval(){
	this->checkIntervalValue();

	for (int i = this->gif_intervalTank.count(); i < this->gif_src.count(); i++){
		this->gif_intervalTank.append(this->gif_interval);	//（自动填充 默认帧间隔）
	}

	if (this->gif_intervalTank.count() > this->gif_src.count()){
		int out_count = this->gif_intervalTank.count() - this->gif_src.count();
		for (int i = 0; i < out_count; i++){
			this->gif_intervalTank.removeLast();
		}
	}
}
/*-------------------------------------------------
		设置 - 校验帧间隔（私有）
*/
void C_ALEData::checkIntervalValue(){

	// > 默认值不能为零
	if (this->gif_interval == 0){ this->gif_interval = 4; }
}


/*-------------------------------------------------
		获取 - 获取文件数量
*/
int C_ALEData::getFileCount(){
	return this->gif_src.count();
}
/*-------------------------------------------------
		获取 - 获取文件
*/
QFileInfo C_ALEData::getFile(int index){
	if (index < 0){ return QFileInfo(); }
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
		获取 - 获取文件路径（F:/aaa/vvv ）
*/
QString C_ALEData::getFileRoot(){
	QString path = this->gif_src_file;
	if (path.at(path.length() - 1) == '/'){ path = path.mid(0, path.count() - 1); }
	return path;
}
/*-------------------------------------------------
		获取 - 获取帧间隔
*/
int C_ALEData::getIntervalDefault(){
	return this->gif_interval;
}
QList<int> C_ALEData::getAllInterval(){
	return this->gif_intervalTank;
}
/*-------------------------------------------------
		获取 - 获取帧间隔文本（"0.01"，实际帧为 0.01666 * n ）
*/
QString C_ALEData::getIntervalString(int index){
	int interval = this->gif_interval;
	if (index >= 0 && index < this->getFileCount()){
		interval = this->gif_intervalTank.at(index);
	}
	return QString::number(interval*0.01);
}
/*-------------------------------------------------
		获取 - 获取标识
*/
int C_ALEData::getId(){
	return this->id;
}
/*-------------------------------------------------
		获取 - 检查文件（不带文件后缀）
*/
bool C_ALEData::hasFileName(QString file_name){
	return this->gif_src.contains(file_name);
}


/*-------------------------------------------------
		操作 - 添加
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

		// > 资源
		this->gif_src.insert(index + i, gif_src_list.at(i));
		
		// > 帧间隔
		int interval = gif_interval;
		if (i < interval_list.count()){ interval = interval_list.at(i); }
		this->gif_intervalTank.insert(index + i, interval);
	}
}
/*-------------------------------------------------
		操作 - 替换
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
		操作 - 移除
*/
void C_ALEData::op_remove(int index){
	this->gif_src.removeAt(index);
	this->gif_intervalTank.removeAt(index);
}
/*-------------------------------------------------
		操作 - 交换位置
*/
void C_ALEData::op_swap(int index_a, int index_b){
	this->gif_src.swap(index_a, index_b);
	this->gif_intervalTank.swap(index_a, index_b);
}


/*-------------------------------------------------
		操作 - 复制文件（取自 tempFile 代码片段）
*/
bool C_ALEData::copyFile(QString filePath_from, QString filePath_to) {
	QFileInfo info_from(filePath_from);
	QFileInfo info_to(filePath_to);
	QFile file_from(info_from.absoluteFilePath());
	QFile file_to(info_to.absoluteFilePath());
	if (info_from.absoluteFilePath() == info_to.absoluteFilePath()){ return true; }		//（相同路径跳过）

	if (!file_from.open(QIODevice::ReadOnly)) { return false; }
	if (!file_to.open(QIODevice::WriteOnly | QIODevice::Truncate)) { return false; }

	QByteArray ba = file_from.readAll();
	file_to.write(ba);
	file_from.close();
	file_to.close();
	return true;
}


/*-------------------------------------------------
		空判断
*/
bool C_ALEData::isNull(){
	return this->id == -1;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
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
	obj.insert("gif_src_file", this->gif_src_file);//（temp文件夹中的配置，实际用不上）
	obj.insert("gif_interval", this->gif_interval);
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
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
	if (obj.value("gif_src_file").isUndefined() == false){ this->gif_src_file = obj.value("gif_src_file").toString(); }
	if (obj.value("gif_interval").isUndefined() == false){ this->gif_interval = obj.value("gif_interval").toInt(); }
}