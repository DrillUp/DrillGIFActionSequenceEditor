#include "stdafx.h"
#include "C_ALE_DataSet.h"

/*
-----==========================================================-----
		类：		动画帧编辑块 数据类.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		动画帧编辑块 的数据类。
					【该数据类只用于传值，所以不需要 ID或名称 的标识】
-----==========================================================-----
*/

C_ALE_DataSet::C_ALE_DataSet()
	: C_PLE_DataSet()
{

	// > 动画帧单位（全局数据）
	this->m_unit = DATA_UNIT::FrameUnit;

	// > 默认帧间隔（全局数据）
	this->m_gif_interval = 4;

	// > 帧间隔明细表
	this->m_gif_intervalTank.clear();
}
C_ALE_DataSet::~C_ALE_DataSet(){
}



/*-------------------------------------------------
		主流程 - 设置资源
*/
void C_ALE_DataSet::setSource(QString pic_file, QList<QString> pic_name_list){
	//（目前该函数与 父类函数 一模一样）
	pic_file = QDir(pic_file).absolutePath();	//（去掉末尾的"/"）

	// > 设置父路径
	this->setData_ParentDir(pic_file);

	// > 设置资源
	this->picPath_list.clear();
	for (int i = 0; i < pic_name_list.count(); i++){
		QString path;
		path.append(pic_file);
		path.append("/");
		path.append(pic_name_list.at(i));
		path.append(".png");
		this->picPath_list.append(path);
	}
}
/*-------------------------------------------------
		主流程 - 设置帧间隔数据
*/
void C_ALE_DataSet::setInterval(int gif_interval, QList<int> gif_intervalTank){
	this->setData_IntervalDefault( gif_interval );
	this->setData_IntervalTank( gif_intervalTank );
}


/*-------------------------------------------------
		动画帧单位 - 设置
*/
void C_ALE_DataSet::setData_Unit(C_ALE_DataSet::DATA_UNIT unit){
	this->m_unit = unit;
}
/*-------------------------------------------------
		动画帧单位 - 获取
*/
C_ALE_DataSet::DATA_UNIT C_ALE_DataSet::getData_Unit(){
	return this->m_unit;
}


/*-------------------------------------------------
		默认帧间隔 - 设置
*/
void C_ALE_DataSet::setData_IntervalDefault(int gif_interval){
	if (gif_interval == 0){ return; }		//（不允许赋值零）
	this->m_gif_interval = gif_interval;
}
/*-------------------------------------------------
		默认帧间隔 - 获取
*/
int C_ALE_DataSet::getData_IntervalDefault(){
	return this->m_gif_interval;
}
/*-------------------------------------------------
		默认帧间隔 - 修改默认帧间隔
*/
void C_ALE_DataSet::setIntervalDefaultInAll(int gif_interval){
	if (gif_interval == 0){ return; }		//（不允许赋值零）
	int old_interval = this->m_gif_interval;
	this->m_gif_interval = gif_interval;
	for (int i = 0; i < this->m_gif_intervalTank.count(); i++){	//（替换掉所有默认的旧帧间隔）
		if (this->m_gif_intervalTank.at(i) == old_interval){
			this->m_gif_intervalTank.replace(i, gif_interval);
		}
	}
}


/*-------------------------------------------------
		帧间隔明细表 - 获取（原数据）
*/
void C_ALE_DataSet::setData_IntervalTank(QList<int> gif_intervalTank){
	this->m_gif_intervalTank = gif_intervalTank;
}
/*-------------------------------------------------
		帧间隔明细表 - 获取（原数据）
*/
QList<int> C_ALE_DataSet::getData_IntervalTank(){
	return this->m_gif_intervalTank;
}

/*-------------------------------------------------
		帧间隔明细表 - 自适应 - 获取（含自适应）
*/
QList<int> C_ALE_DataSet::getData_IntervalTank_WithFit(){
	return C_ALE_DataSet::converterFit_getIntervalList(this->m_gif_intervalTank, this->m_gif_interval, this->getData_PicListCount());
}
/*-------------------------------------------------
		帧间隔明细表 - 自适应 - 执行自适应转换
*/
QList<int> C_ALE_DataSet::converterFit_getIntervalList(QList<int> intervalValueList, int default_interval, int pic_count){

	// > 多出时，填充默认帧间隔
	for (int i = intervalValueList.count(); i < pic_count; i++){
		intervalValueList.append(default_interval);
	}

	// > 缺少时，移除帧间隔
	if (intervalValueList.count() > pic_count){
		int out_count = intervalValueList.count() - pic_count;
		for (int i = 0; i < out_count; i++){
			intervalValueList.removeLast();
		}
	}

	return intervalValueList;
}

/*-------------------------------------------------
		帧间隔明细表 - 单位 - 获取（含单位转换）
*/
double C_ALE_DataSet::getData_IntervalTankOne_WithUnit(int index){

	// > 若越界，则返回默认帧间隔
	if (index < 0 || index >= this->m_gif_intervalTank.count()){
		return C_ALE_DataSet::converterUnit_getInterval(this->m_gif_interval, this->m_unit); 
	}
	return C_ALE_DataSet::converterUnit_getInterval(this->m_gif_intervalTank.at(index), this->m_unit);
}
QList<double> C_ALE_DataSet::getData_IntervalTankList_WithUnit(QList<int> index_list){
	QList<double> result_list;
	for (int i = 0; i < index_list.count(); i++){
		double interval = this->getData_IntervalTankOne_WithUnit(index_list.at(i));
		result_list.append(interval);
	}
	return result_list;
}
QList<double> C_ALE_DataSet::getData_IntervalTankAll_WithUnit(){
	QList<double> result_list;
	for (int i = 0; i < this->m_gif_intervalTank.count(); i++){
		double interval = C_ALE_DataSet::converterUnit_getInterval(this->m_gif_intervalTank.at(i), this->m_unit);
		result_list.append( interval );
	}
	return result_list;
}
/*-------------------------------------------------
		帧间隔明细表 - 单位 - 执行单位转换
*/
double C_ALE_DataSet::converterUnit_getInterval(int intervalValue, C_ALE_DataSet::DATA_UNIT unit){
	
	// > 秒单位：0.0100秒
	if (unit == DATA_UNIT::SecondUnit){
		return intervalValue*1.000;
	}
	// > 帧单位：0.0166秒
	if (unit == DATA_UNIT::FrameUnit){
		return intervalValue*1.666;
	}

	return intervalValue;
}
QList<double> C_ALE_DataSet::converterUnit_getIntervalList(QList<int> intervalValueList, C_ALE_DataSet::DATA_UNIT unit){
	QList<double> result_list;
	for (int i = 0; i < intervalValueList.count(); i++){
		int intervalValue = intervalValueList.at(i);
		result_list.append(C_ALE_DataSet::converterUnit_getInterval(intervalValue, unit));
	}
	return result_list;
}


/*-------------------------------------------------
		帧间隔明细表 - 单位 - 获取描述文本
*/
QString C_ALE_DataSet::getDescriptionString(int intervalValue){
	return C_ALE_DataSet::converterUnit_getDescriptionString(intervalValue, this->m_unit);
}
/*-------------------------------------------------
		帧间隔明细表 - 单位 - 执行描述文本转换
*/
QString C_ALE_DataSet::converterUnit_getDescriptionString(int intervalValue, C_ALE_DataSet::DATA_UNIT unit){
	
	// > 秒单位：0.0100秒
	if (unit == DATA_UNIT::SecondUnit){
		return QString::number(intervalValue*0.01) + "秒";
	}

	// > 帧单位：0.0166秒
	return QString::number(intervalValue) + "帧";
}


/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_ALE_DataSet::getJsonObject(){
	QJsonObject obj = C_PLE_DataSet::getJsonObject();

	// > 动画帧单位（全局数据）
	obj.insert("m_unit", this->m_unit);

	// > 默认帧间隔（全局数据）
	obj.insert("m_gif_interval", this->m_gif_interval);

	// > 帧间隔明细表
	QJsonArray arr;
	for (int i = 0; i < this->m_gif_intervalTank.count(); i++){
		arr.append(this->m_gif_intervalTank.at(i));
	}
	obj.insert("m_gif_intervalTank", arr);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_ALE_DataSet::setJsonObject(QJsonObject obj){
	C_PLE_DataSet::setJsonObject(obj);

	// > 动画帧单位（全局数据）
	this->m_unit = (C_ALE_DataSet::DATA_UNIT)obj.value("m_unit").toInt();

	// > 默认帧间隔（全局数据）
	this->m_gif_interval = obj.value("m_gif_interval").toInt(4);	//（不允许赋值零）

	// > 帧间隔明细表
	if (obj.value("m_gif_intervalTank").isUndefined() == false){
		this->m_gif_intervalTank.clear();
		QJsonArray arr = obj.value("m_gif_intervalTank").toArray();
		for (int i = 0; i < arr.count(); i++){
			this->m_gif_intervalTank.append(arr.at(i).toInt());
		}
	}

}