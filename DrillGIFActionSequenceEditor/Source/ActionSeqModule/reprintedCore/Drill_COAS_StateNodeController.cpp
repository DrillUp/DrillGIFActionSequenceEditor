#include "stdafx.h"
#include "Drill_COAS_StateNodeController.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		动画序列数据【复刻类】.cpp
		作者：		drill_up
		所属模块：	动画序列数据
		功能：		动画序列核心插件的复刻类。
					
		说明：		当前复刻版本：[v1.3]
-----==========================================================-----
*/
Drill_COAS_StateNodeController::Drill_COAS_StateNodeController(){
	this->_drill_data = QJsonObject();	//深拷贝数据
	this->drill_initData();				//初始化数据
}
Drill_COAS_StateNodeController::Drill_COAS_StateNodeController(QJsonObject data){
	this->_drill_data = data;			//深拷贝数据
	this->drill_initData();				//初始化数据
}
Drill_COAS_StateNodeController::~Drill_COAS_StateNodeController(){
}

/*-------------------------------------------------
		数据 - 空判断
*/
bool Drill_COAS_StateNodeController::isNull(){
	return this->_drill_data.isEmpty();
}
/*-------------------------------------------------
		创建 - 初始化
*/
void Drill_COAS_StateNodeController::drill_initData(){
	QJsonObject data = this->_drill_data;
	
	// > 默认值
	if (data["waitForPreload"].isUndefined() == true){ data["waitForPreload"] = false; }							//预加载等待（子插件用参数）
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
	this->_drill_state_lastAnnotation = "";							//状态元 - 上一个注解名
	this->_drill_act_curCom = "";									//动作元 - 当前动作
	this->_drill_act_curTime = 0;									//动作元 - 当前时间
}
/*-------------------------------------------------
		数据 - 帧刷新（需要父类手动执行）
*/
void Drill_COAS_StateNodeController::update(){
	if (this->isNull()){ return; }
	this->_drill_time += 1;				//时间+1
	this->drill_COAS_updateState();		//刷新状态元
	this->drill_COAS_updateAct();		//刷新动作元
}
/*-------------------------------------------------
		帧刷新 - 刷新状态元
*/
void Drill_COAS_StateNodeController::drill_COAS_updateState(){
	if (this->drill_COAS_isPlayingAct() == true){ return; }		//动作播放时，不操作状态元
	if (this->_drill_state_curSeq.count() == 0){ return; }		//状态元集合 为空时，不操作

	// > 随机抽取
	if (this->_drill_state_curCom == ""){
		this->drill_COAS_rollCurrentState();
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
	if (gif_src.count() != 0){
		
		int index = 0;
		int inter_time = this->_drill_state_curTime;
		if( data_state["gif_back_run"].toBool() == false){

			// > 正向播放
			QJsonArray gif_intervalRealTank = data_state["gif_intervalRealTank"].toArray();
			for (int i = 0; i < gif_intervalRealTank.count(); i++){
				int i_time = gif_intervalRealTank[i].toInt();
				if( inter_time < i_time ){
					index = i;
					break;
				}
				inter_time -= i_time;
			}
		}else{

			// > 倒放
			QJsonArray gif_intervalRealTank = data_state["gif_intervalRealTank"].toArray();
			for (int i = gif_intervalRealTank.count() - 1; i >= 0; i--){
				int i_time = gif_intervalRealTank[i].toInt();
				if( inter_time < i_time ){
					index = i;
					break;
				}
				inter_time -= i_time;
			}
		}
		this->_drill_bitmapName = gif_src[index].toString();
		this->_drill_bitmapPath = data_state["gif_src_file"].toString();
		this->_drill_bitmapTint = data_state["gif_tint"].toInt();
		this->_drill_bitmapSmooth = data_state["gif_smooth"].toBool();
	}

	// > 时间+1
	this->_drill_state_curTime += 1;
	if (this->_drill_state_curTime >= data_state["gif_intervalRealTank_total"].toInt()){
		this->drill_COAS_rollCurrentState();
		this->_drill_state_curTime = 0;
	}
}
/*-------------------------------------------------
		操作 - 抽取新的状态元
*/
void Drill_COAS_StateNodeController::drill_COAS_rollCurrentState(){
	if (this->_drill_state_curSeq.count() == 0){ return; }		//状态元集合 为空时，不操作

	// > 只有一个就不变
	if (this->_drill_state_curSeq.count() == 1){
		this->_drill_state_curCom = this->_drill_state_curSeq[0];
	}

	// > 随机抽取
	int index = rand() % this->_drill_state_curSeq.count();
	this->_drill_state_curCom = this->_drill_state_curSeq[index];
}

/*-------------------------------------------------
		帧刷新 - 刷新动作元
*/
void Drill_COAS_StateNodeController::drill_COAS_updateAct(){
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
	if (gif_src.count() != 0){
		
		int index = 0;
		int inter_time = this->_drill_state_curTime;
		if (data_act["gif_back_run"].toBool() == false){

			// > 正向播放
			QJsonArray gif_intervalRealTank = data_act["gif_intervalRealTank"].toArray();
			for (int i = 0; i < gif_intervalRealTank.count(); i++){
				int i_time = gif_intervalRealTank[i].toInt();
				if( inter_time < i_time ){
					index = i;
					break;
				}
				inter_time -= i_time;
			}
		}else{

			// > 倒放
			QJsonArray gif_intervalRealTank = data_act["gif_intervalRealTank"].toArray();
			for (int i = gif_intervalRealTank.count() - 1; i >= 0; i--){
				int i_time = gif_intervalRealTank[i].toInt();
				if( inter_time < i_time ){
					index = i;
					break;
				}
				inter_time -= i_time;
			}
		}
		this->_drill_bitmapName = gif_src[index].toString();
		this->_drill_bitmapPath = data_act["gif_src_file"].toString();
		this->_drill_bitmapTint = data_act["gif_tint"].toInt();
		this->_drill_bitmapSmooth = data_act["gif_smooth"].toBool();
	}

	// > 时间+1
	this->_drill_act_curTime += 1;
	if (this->_drill_act_curTime > data_act["gif_intervalRealTank_total"].toInt()){
		this->_drill_act_curCom = "";
		this->_drill_act_curTime = 0;
	}

}


/*-------------------------------------------------
		数据 - 检查数组元素

		（c++中已自动检查）
*/

/*-------------------------------------------------
		数据 - 状态元 - 设置序列（接口）

				说明：	序列中存放 状态名称 的数组，状态元会跳过不识别的对象。
*/
void Drill_COAS_StateNodeController::drill_COAS_setSequence(QStringList seq){
	this->_drill_state_curSeq = seq;
}
/*-------------------------------------------------
		数据 - 状态元 - 设置序列，立刻改变（接口）
*/
void Drill_COAS_StateNodeController::drill_COAS_setSequenceImmediate(QStringList seq){
	this->drill_COAS_setSequence( seq );
	this->drill_COAS_rollCurrentState();
	this->_drill_state_curTime = 0;
}
/*-------------------------------------------------
		数据 - 状态元 - 设置状态元序列[注解模式]（接口）

				说明：	状态元名称中含有特定注解的，会被捕获。如果都没捕获到，返回失败（false）。
*/
bool Drill_COAS_StateNodeController::drill_COAS_setSequenceByAnnotation(QString annotation){
	if (this->_drill_state_lastAnnotation == annotation){ return true; }	//（重复注解时跳过）
	this->_drill_state_lastAnnotation = annotation;

	// > 获取注解列表
	QStringList annotation_list = QStringList();
	QStringList temp_list = annotation.split("@");
	for (int i = 0; i < temp_list.count(); i++){
		QString temp = temp_list[i];
		if (temp == ""){ continue; }
		annotation_list.push_back("@" + temp);
	}

	// > 找到符合注解数量最多的状态元名
	QStringList name_list = this->drill_COAS_getAllStateName();
	int max_fit_count = 0;									//（最大符合数量）
	QList<QJsonObject> tag_seq = QList<QJsonObject>();		//（最大符合的索引列表）
	for (int i = 0; i < name_list.count(); i++){
		QString name = name_list[i];

		// > 获取@符号数量
		int char_count = 0;
		for (int j = 0; j < name.count(); j++){
			if (name[j] == '@'){
				char_count += 1;
			}
		}
		if (char_count == 0){ continue; }

		// > 记录注解符合数量
		int fit_count = 0;
		for (int j = 0; j < annotation_list.count(); j++){
			QString annotation = annotation_list[j];
			if (name.contains(annotation) == true){
				fit_count += 1;
			}
		}

		// > 含有不匹配的@符号时，跳过
		if (char_count > fit_count){
			continue;
		}

		// > 符合数量更大时，清空序列，重新添加
		if (fit_count > max_fit_count){
			tag_seq = QList<QJsonObject>();
			max_fit_count = fit_count;

			QJsonObject tag = QJsonObject();
			tag["index"] = i;
			tag["count"] = fit_count;
			tag["name"] = name;
			tag_seq.push_back(tag);

		// > 符合数量相等，累计
		}else if (fit_count == max_fit_count){
			QJsonObject tag = QJsonObject();
			tag["index"] = i;
			tag["count"] = fit_count;
			tag["name"] = name;
			tag_seq.push_back(tag);

		// > 符合数量少了，跳过
		}else{
			continue;
		}
	}
	if (tag_seq.count() == 0){ return false; }

	// > 根据最大值的下标取出符合的名称
	QStringList seq = QStringList();
	for (int i = 0; i < tag_seq.count(); i++){
		seq.push_back(tag_seq[i]["name"].toString());
	}

	this->_drill_state_curSeq = seq;
	return true;
}

/*-------------------------------------------------
		数据 - 状态元 - 设置状态元序列[注解模式]-立刻改变（接口）
*/
bool Drill_COAS_StateNodeController::drill_COAS_setSequenceImmediateByAnnotation(QString annotation){
	if (this->_drill_state_lastAnnotation == annotation){ return true; }	//（重复注解时跳过）

	bool success = this->drill_COAS_setSequenceByAnnotation(annotation);
	if (success){
		this->drill_COAS_rollCurrentState();
		this->_drill_state_curTime = 0;
	}
	return success;
}
/*-------------------------------------------------
		数据 - 状态元 - 获取默认状态元集合（接口）
*/
QStringList Drill_COAS_StateNodeController::drill_COAS_getDefaultStateGroup(){
	return TTool::_QJsonArray_AToQString_(this->_drill_data["state_default_randomSeq"].toArray());
}
/*-------------------------------------------------
		数据 - 状态元 - 获取当前状态元名称（接口）
*/
QString Drill_COAS_StateNodeController::drill_COAS_getCurrentStateName(){
	return this->_drill_state_curCom;
}
/*-------------------------------------------------
		数据 - 状态元 - 获取当前状态元集合名称（接口）
*/
QStringList Drill_COAS_StateNodeController::drill_COAS_getCurrentStateSeqName(){
	return this->_drill_state_curSeq;
}
/*-------------------------------------------------
		数据 - 状态元 - 获取全部状态元名称（接口）
*/
QStringList Drill_COAS_StateNodeController::drill_COAS_getAllStateName(){
	QJsonObject data = this->_drill_data;
	QStringList result_list = QStringList();
	QJsonArray state_tank = data["state_tank"].toArray();
	for (int i = 0; i < state_tank.count(); i++){
		QJsonObject data_state = state_tank[i].toObject();
		if (data_state.isEmpty() == false && data_state["name"].toString() != ""){
			result_list.push_back(data_state["name"].toString());
		}
	}
	return result_list;
}
/*-------------------------------------------------
		数据 - 状态元 - 获取数据 根据名称
*/
QJsonObject Drill_COAS_StateNodeController::drill_COAS_getDataState(QString state_name){
	QJsonObject data = this->_drill_data;
	QJsonArray state_tank = data["state_tank"].toArray();
	for (int i = 0; i < state_tank.count(); i++){
		QJsonObject data_state = state_tank[i].toObject();
		if (data_state.isEmpty() == false && data_state["name"].toString() == state_name){
			return data_state;
		}
	}
	return QJsonObject();
}

/*-------------------------------------------------
		数据 - 动作元 - 判断播放
*/
bool Drill_COAS_StateNodeController::drill_COAS_isPlayingAct(){
	return this->_drill_act_curCom != "";
}
/*-------------------------------------------------
		数据 - 动作元 - 添加动作（接口）
*/
void Drill_COAS_StateNodeController::drill_COAS_setAct(QString act_name){
	if( this->_drill_act_curCom == act_name ){ return; }
	
	// > 检查高优先级状态元
	if (this->drill_COAS_isPlayingAct() == false){
		QJsonObject data_act = this->drill_COAS_getDataAct(act_name);
		QJsonObject cur_state = this->drill_COAS_getDataState(this->_drill_state_curCom);

		//qDebug() << this->_drill_state_curCom;
		//qDebug() << data_act;
		//qDebug() << cur_state;
		//qDebug() << cur_state["priority"].toInt();
		//qDebug() << data_act["priority"].toInt();

		if (cur_state["priority"].toInt() > data_act["priority"].toInt()){	//（同级的动作元可以覆盖状态元）
			return;
		}
	}
		
	// > 动作正在播放时
	if( this->drill_COAS_isPlayingAct() ){
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
void Drill_COAS_StateNodeController::drill_COAS_stopAct(){
	this->_drill_act_curCom = "";
	this->_drill_act_curTime = 0;
}
/*-------------------------------------------------
		数据 - 动作元 - 获取当前动作元名称（接口）
*/
QString Drill_COAS_StateNodeController::drill_COAS_getCurrentActName(){
	return this->_drill_act_curCom;
}
/*-------------------------------------------------
		数据 - 动作元 - 获取全部动作元名称（接口）
*/
QStringList Drill_COAS_StateNodeController::drill_COAS_getAllActName(){
	QJsonObject data = this->_drill_data;
	QStringList result_list = QStringList();
	QJsonArray act_tank = data["act_tank"].toArray();
	for (int i = 0; i < act_tank.count(); i++){
		QJsonObject data_act = act_tank[i].toObject();
		if (data_act.isEmpty() == false && data_act["name"].toString() != ""){
			result_list.push_back(data_act["name"].toString());
		}
	}
	return result_list;
}
/*-------------------------------------------------
		数据 - 动作元 - 获取数据 根据名称
*/
QJsonObject Drill_COAS_StateNodeController::drill_COAS_getDataAct(QString act_name){
	QJsonObject data = this->_drill_data;
	QJsonArray act_tank = data["act_tank"].toArray();
	for (int i = 0; i < act_tank.count(); i++){
		QJsonObject data_act = act_tank[i].toObject();
		if (data_act.isEmpty() == false && data_act["name"].toString() == act_name){
			return data_act;
		}
	}
	return QJsonObject();
}