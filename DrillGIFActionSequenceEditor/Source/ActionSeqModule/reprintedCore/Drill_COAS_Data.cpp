#include "stdafx.h"
#include "Drill_COAS_Data.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		动画序列数据【复刻类】.cpp
		作者：		drill_up
		所属模块：	动画序列数据
		功能：		动作序列核心插件的复刻类。
		
		目标：		->控件
						->单选表格
						->快速表单

		使用方法：
				>初始化

-----==========================================================-----
*/
Drill_COAS_Data::Drill_COAS_Data(QJsonObject data, QObject *parent)
	: QObject(parent)
{
	this->_drill_data = data;			//深拷贝数据
	this->drill_initData();				//初始化数据
}
Drill_COAS_Data::~Drill_COAS_Data(){
}

/*-------------------------------------------------
		创建 - 初始化
*/
void Drill_COAS_Data::drill_initData(){
	QJsonObject data = this->_drill_data;
	
	// > 默认值
	if (data["state_default_randomSeq"].isUndefined() == true){ data["state_default_randomSeq"] = QJsonArray(); }	//默认状态元集合
	if (data["state_tank"].isUndefined() == true){ data["state_tank"] = QJsonArray(); }								//状态元 容器
	if (data["act_tank"].isUndefined() == true){ data["act_tank"] = QJsonArray(); }									//动作元 容器
	
	// > 私有变量初始化
	this->_drill_time = 0;											//持续时间
	this->_drill_arrayCheck = true;									//检查数组元素
	this->_drill_bitmapName = "";									//当前的bitmap对象名
	this->_drill_bitmapPath = "";									//当前的bitmap路径
	this->_drill_bitmapTint = 0;									//当前的bitmap色调
	this->_drill_bitmapSmooth = false;								//当前的bitmap模糊
	this->_drill_state_curCom = "";									//状态元 - 当前状态
	this->_drill_state_curTime = 0;									//状态元 - 当前时间
	this->_drill_state_curSeq = TTool::_QJsonArray_AToQString_(data["state_default_randomSeq"].toArray());		//状态元 - 当前序列
	this->_drill_act_curCom = "";									//动作元 - 当前动作
	this->_drill_act_curTime = 0;									//动作元 - 当前时间
}
/*-------------------------------------------------
		数据 - 帧刷新（需要父类手动执行）
*/
void Drill_COAS_Data::update(){
	this->_drill_time += 1;				//时间+1
	this->drill_COAS_updateState();		//刷新状态元
	this->drill_COAS_updateAct();		//刷新动作元
}
/*-------------------------------------------------
		帧刷新 - 刷新状态元
*/
void Drill_COAS_Data::drill_COAS_updateState(){
	if (this->drill_COAS_isPlayingAct() == true){ return; }		//动作播放时，不操作状态元
	if (this->_drill_state_curSeq.count() == 0){ return; }		//状态元集合 为空时，不操作

	// > 随机抽取
	if (this->_drill_state_curCom == ""){
		if (this->_drill_state_curSeq.count() == 1){
			this->_drill_state_curCom = this->_drill_state_curSeq[0];
		}
		int index = rand() % this->_drill_state_curSeq.count();
		this->_drill_state_curCom = this->_drill_state_curSeq[index];
	}

	QJsonObject data = this->_drill_data;
	QJsonObject data_state = this->drill_COAS_getDataState(this->_drill_state_curCom);

	// > 没有该状态元时
	if (data_state["gif_src"].isUndefined() == true){
		this->_drill_state_curCom = "";
		return;
	}

	// > 状态声音
	//if (this->_drill_state_curTime == 0){
	//	QJsonObject se = {};
	//	se["name"] = data_state["se"];
	//	se["pitch"] = 100;
	//	se["volume"] = 100;
	//	AudioManager.playSe(se);
	//}

	// > gif播放
	QJsonArray gif_src = data_state["gif_src"].toArray();
	//if (gif_src.count() != 0){
	//	int inter = this->_drill_time;
	//	inter = inter / data_state["gif_interval"].toInt();
	//	inter = inter % gif_src.count();
	//	if (data_state['gif_back_run']){
	//		inter = data_state['gif_src'].length - 1 - inter;
	//	}
	//	inter = Math.floor(inter);
	//	this._drill_bitmapName = data_state['gif_src'][inter];
	//	this._drill_bitmapPath = data_state['gif_src_file'];
	//}

	// > 时间+1
	this->_drill_state_curTime += 1;
	if (this->_drill_state_curTime >= gif_src.count() * data_state["gif_interval"].toInt()){
		this->_drill_state_curCom = "";
		this->_drill_state_curTime = 0;
	}
}
/*-------------------------------------------------
		帧刷新 - 刷新动作元
*/
void Drill_COAS_Data::drill_COAS_updateAct(){
	if (this->drill_COAS_isPlayingAct() == false){ return; }		//动作未播放时，不操作

	QJsonObject data = this->_drill_data;
	QJsonObject data_act = this->drill_COAS_getDataAct(this->_drill_act_curCom);

	// > 没有该动作元时
	if (data_act["gif_src"].isUndefined() == true){
		this->_drill_act_curCom = "";
		return;
	}

	// > 动作声音
	//if (this->_drill_state_curTime == 0){
	//	QJsonObject se = {};
	//	se["name"] = data_state["se"];
	//	se["pitch"] = 100;
	//	se["volume"] = 100;
	//	AudioManager.playSe(se);
	//}

	// > gif播放（一次只能播放一种行为）
	QJsonArray gif_src = data_act["gif_src"].toArray();
	

	// > 时间+1
	this->_drill_act_curTime += 1;
	if (this->_drill_act_curTime > gif_src.count() * data_act["gif_interval"].toInt()){
		this->_drill_act_curCom = "";
		this->_drill_act_curTime = 0;
	}

}


/*-------------------------------------------------
		数据 - 状态元 - 设置序列（接口）
*/
void Drill_COAS_Data::drill_COAS_setSequence(QStringList seq){
	this->_drill_state_curSeq = seq;
}
/*-------------------------------------------------
		数据 - 状态元 - 设置序列，立刻改变（接口）
*/
void Drill_COAS_Data::drill_COAS_setSequenceImmediate(QStringList seq){
	this->_drill_state_curSeq = seq;
	this->_drill_state_curCom = "";
	this->_drill_state_curTime = 0;
}
/*-------------------------------------------------
		数据 - 状态元 - 获取数据 根据名称
*/
QJsonObject Drill_COAS_Data::drill_COAS_getDataState(QString state_name){
	QJsonObject data = this->_drill_data;
	QJsonArray arr = data["state_tank"].toArray();
	for (int i = 0; i < arr.count(); i++){
		QJsonObject data_state = arr[i].toObject();
		if (data_state.isEmpty() == false && data_state["name"].toString() == state_name){
			return data_state;
		}
	}
	return QJsonObject();
}
/*-------------------------------------------------
		数据 - 动作元 - 判断播放
*/
bool Drill_COAS_Data::drill_COAS_isPlayingAct(){
	return this->_drill_act_curCom != "";
}
/*-------------------------------------------------
		数据 - 动作元 - 添加动作（接口）
*/
void Drill_COAS_Data::drill_COAS_setAct(QString act_name){
	if( this->_drill_act_curCom == act_name ){ return; }
	
	// > 检查高优先级状态元
	if( this->_drill_act_curCom == "" ){
		QJsonObject data_act = this->drill_COAS_getDataAct(act_name);
		QJsonObject cur_state = this->drill_COAS_getDataState(this->_drill_state_curCom);
		
		if (cur_state["priority"].toInt() >= data_act["priority"].toInt()){	//（只能覆盖小的优先级，不包括同级）
			return;
		}
	}
		
	// > 动作正在播放时
	if( this->_drill_act_curCom != "" ){
		QJsonObject data_act = this->drill_COAS_getDataAct(act_name);
		QJsonObject cur_act = this->drill_COAS_getDataAct(this->_drill_act_curCom);
		
		if (cur_act["priority"].toInt() >= data_act["priority"].toInt()){	//（只能覆盖小的优先级，不包括同级）
			return;
		}
	}
	
	this->_drill_act_curCom = act_name;
}
/*-------------------------------------------------
		数据 - 动作元 - 立刻终止动作（接口）
*/
void Drill_COAS_Data::drill_COAS_stopAct(){
	this->_drill_act_curCom = "";
	this->_drill_act_curTime = 0;
}
/*-------------------------------------------------
		数据 - 动作元 - 获取数据 根据名称
*/
QJsonObject Drill_COAS_Data::drill_COAS_getDataAct(QString act_name){
	QJsonObject data = this->_drill_data;
	QJsonArray arr = data["act_tank"].toArray();
	for (int i = 0; i < arr.count(); i++){
		QJsonObject data_act = arr[i].toObject();
		if (data_act.isEmpty() == false && data_act["name"].toString() == act_name){
			return data_act;
		}
	}
	return QJsonObject();
}