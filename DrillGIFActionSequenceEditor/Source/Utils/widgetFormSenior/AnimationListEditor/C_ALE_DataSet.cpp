#include "stdafx.h"
#include "C_ALE_DataSet.h"

/*
-----==========================================================-----
		类：		动画帧编辑块 数据类.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		动画帧编辑块 的数据类。
					注意，只有五项。如果有其他数据合并，想办法另起一个类。
-----==========================================================-----
*/
C_ALE_DataSet::C_ALE_DataSet() {
	this->id = -1;								//标识
	this->m_unit = DATA_UNIT::FrameUnit;		//单位（默认1秒60帧）

	this->gif_src;								//资源文件名
	this->gif_src_file = "";					//资源文件夹
	this->gif_intervalTank;						//帧间隔列表
	this->gif_interval = 4;						//帧间隔
}
C_ALE_DataSet::~C_ALE_DataSet(){
}

/*-------------------------------------------------
		访问器 - 标识
*/
void C_ALE_DataSet::setData_Id(int id){
	this->id = id;
}
/*-------------------------------------------------
		访问器 - 设置文件父目录
*/
void C_ALE_DataSet::setData_ParentFile(QString gif_src_file){
	this->gif_src_file = gif_src_file;
}
/*-------------------------------------------------
		访问器 - 设置默认帧间隔
*/
void C_ALE_DataSet::setData_IntervalDefault(int gif_interval){
	this->gif_interval = gif_interval;
	this->checkIntervalValue();
}
/*-------------------------------------------------
		访问器 - 设置单位
*/
void C_ALE_DataSet::setData_Unit(C_ALE_DataSet::DATA_UNIT unit){
	this->m_unit = unit;
}
/*-------------------------------------------------
		接口 - 设置资源
*/
void C_ALE_DataSet::setSource(QString gif_src_file, QList<QString> gif_src){
	this->gif_src_file = gif_src_file;
	this->gif_src = gif_src;
	this->checkInterval();
}
/*-------------------------------------------------
		接口 - 设置帧间隔
*/
void C_ALE_DataSet::setInterval(int gif_interval, QList<int> gif_intervalTank){
	this->gif_interval = gif_interval;
	this->gif_intervalTank = gif_intervalTank;
	this->checkInterval();
}
/*-------------------------------------------------
		接口 - 设置默认帧间隔（统一改变）
*/
void C_ALE_DataSet::setIntervalDefaultWithFit(int gif_interval){
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
		私有 - 检查帧间隔（私有）
*/
void C_ALE_DataSet::checkInterval(){
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
		私有 - 校验帧间隔（私有）
*/
void C_ALE_DataSet::checkIntervalValue(){

	// > 默认值不能为零
	if (this->gif_interval == 0){ this->gif_interval = 4; }
}


/*-------------------------------------------------
		访问器 - 获取标识
*/
int C_ALE_DataSet::getData_Id(){
	return this->id;
}
/*-------------------------------------------------
		访问器 - 获取帧间隔
*/
int C_ALE_DataSet::getData_IntervalDefault(){
	return this->gif_interval;
}
/*-------------------------------------------------
		访问器 - 获取帧间隔明细表
*/
QList<int> C_ALE_DataSet::getData_IntervalTank(){
	return this->gif_intervalTank;
}
/*-------------------------------------------------
		访问器 - 单位
*/
C_ALE_DataSet::DATA_UNIT C_ALE_DataSet::getData_Unit(){
	return this->m_unit;
}

/*-------------------------------------------------
		接口 - 获取文件
*/
QFileInfo C_ALE_DataSet::getFile(int index){
	if (index < 0){ return QFileInfo(); }
	if (index >= this->getFileCount()){ return QFileInfo(); }

	QString path = this->gif_src_file;
	if (path.at(path.length() - 1) != '/'){ path += "/"; }
	path += this->gif_src.at(index);
	path += ".png";
	return QFileInfo(path);
}
QList<QFileInfo> C_ALE_DataSet::getFile_Multi(QList<int> index_list){
	QList<QFileInfo> result_list;
	for (int i = 0; i < index_list.count(); i++){
		result_list.append(this->getFile(index_list.at(i)));
	}
	return result_list;
}
QList<QFileInfo> C_ALE_DataSet::getAllFile(){
	QList<QFileInfo> result_list;
	for (int i = 0; i < this->gif_src.count(); i++){
		result_list.append(this->getFile(i));
	}
	return result_list;
}
int C_ALE_DataSet::getFileCount(){
	return this->gif_src.count();
}
/*-------------------------------------------------
		接口 - 获取文件路径（F:/aaa/vvv ）
*/
QString C_ALE_DataSet::getFileRoot(){
	QString path = this->gif_src_file;
	if (path.at(path.length() - 1) == '/'){ path = path.mid(0, path.count() - 1); }
	return path;
}
/*-------------------------------------------------
		接口 - 检查文件（不带文件后缀）
*/
bool C_ALE_DataSet::hasFileName(QString file_name){
	return this->gif_src.contains(file_name);
}

/*-------------------------------------------------
		接口 - 获取帧间隔（含单位转换）
*/
double C_ALE_DataSet::getIntervalDefaultWithUnit(){
	return this->intervalUnitTransform(this->gif_interval);
}
/*-------------------------------------------------
		访问器 - 获取帧间隔（含单位转换）
*/
double C_ALE_DataSet::getIntervalWithUnit(int index){
	if (index < 0){ return this->getIntervalDefaultWithUnit(); }
	if (index >= this->gif_intervalTank.count()){ return this->getIntervalDefaultWithUnit(); }
	return this->intervalUnitTransform(this->gif_intervalTank.at(index));
}
QList<double> C_ALE_DataSet::getIntervalWithUnit_Multi(QList<int> index_list){
	QList<double> result_list;
	for (int i = 0; i < index_list.count(); i++){
		double interval = this->getIntervalWithUnit(index_list.at(i));
		result_list.append(interval);
	}
	return result_list;
}
/*-------------------------------------------------
		接口 - 获取帧间隔明细表（含单位转换）
*/
QList<double> C_ALE_DataSet::getIntervalTankWithUnit(){
	QList<double> result_list;
	for (int i = 0; i < this->gif_intervalTank.count(); i++){
		double interval = this->intervalUnitTransform(this->gif_intervalTank.at(i));
		result_list.append( interval );
	}
	return result_list;
}
/*-------------------------------------------------
		接口 - 获取帧间隔文本
*/
QString C_ALE_DataSet::getIntervalString(int index){
	int interval = this->gif_interval;
	if (index >= 0 && index < this->getFileCount()){
		interval = this->gif_intervalTank.at(index);
	}

	// > 秒单位：0.0100秒
	if (this->m_unit == DATA_UNIT::SecondUnit){
		return QString::number(interval*0.01)+"秒";	
	}

	// > 帧单位：0.0166秒
	return QString::number(interval) + "帧";
}
/*-------------------------------------------------
		私有 - 单位转换
*/
double C_ALE_DataSet::intervalUnitTransform(int interval){

	// > 秒单位：0.0100秒
	if (this->m_unit == DATA_UNIT::SecondUnit){
		return interval*1.000;
	}
	// > 帧单位：0.0166秒
	if (this->m_unit == DATA_UNIT::FrameUnit){
		return interval*1.666;
	}

	return interval;
}


/*-------------------------------------------------
		操作 - 添加
*/
void C_ALE_DataSet::op_append(QString gif_src){
	this->gif_src.append(gif_src);
	this->gif_intervalTank.append(gif_interval);
}
void C_ALE_DataSet::op_insert(int index, QString gif_src){
	this->gif_src.insert(index, gif_src);
	this->gif_intervalTank.insert(index,gif_interval);
}
void C_ALE_DataSet::op_insert(int index, QStringList gif_src_list, QList<int> interval_list){
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
void C_ALE_DataSet::op_replace(int index, QString gif_src){
	this->gif_src.replace(index, gif_src);
}
void C_ALE_DataSet::op_replace_interval(int index, int interval){
	this->gif_intervalTank.replace(index, interval);
}
void C_ALE_DataSet::op_replace_interval(QList<int> index_list, int interval){
	for (int i = 0; i < index_list.count(); i++){
		this->gif_intervalTank.replace(index_list.at(i), interval);
	}
}
/*-------------------------------------------------
		操作 - 移除
*/
void C_ALE_DataSet::op_remove(int index){
	this->gif_src.removeAt(index);
	this->gif_intervalTank.removeAt(index);
}
/*-------------------------------------------------
		操作 - 交换位置
*/
void C_ALE_DataSet::op_swap(int index_a, int index_b){
	this->gif_src.swap(index_a, index_b);
	this->gif_intervalTank.swap(index_a, index_b);
}


/*-------------------------------------------------
		操作 - 复制文件（取自 tempFile 代码片段）
*/
bool C_ALE_DataSet::copyFile(QString filePath_from, QString filePath_to) {
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
bool C_ALE_DataSet::isNull(){
	return this->id == -1;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_ALE_DataSet::getJsonObject(){
	QJsonObject obj;

	QJsonArray arr_1;
	for (int i = 0; i < this->gif_src.count(); i++){ arr_1.append(this->gif_src.at(i)); }
	obj.insert("gif_src", arr_1);
	QJsonArray arr_2;
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
void C_ALE_DataSet::setJsonObject(QJsonObject obj){
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