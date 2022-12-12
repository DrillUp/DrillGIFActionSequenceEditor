#include "stdafx.h"
#include "Drill_COAS_Init.h"

#include "Source/ActionSeqModule/actionSeqData/s_ActionSeqDataContainer.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		变量获取【复刻类】.cpp
		作者：		drill_up
		所属模块：	动画序列数据
		功能：		动画序列核心插件的复刻类。

		说明：		当前复刻版本：[v1.3]
		目标：		->变量获取
						->状态元（~struct~DrillCOASState）
						->动作元（~struct~DrillCOASAct）
						->动画序列（~struct~DrillCOASSequence）

		使用方法：	直接调用静态方法即可。
-----==========================================================-----
*/
Drill_COAS_Init::Drill_COAS_Init(){
	Drill_COAS_Init::cur_instance = this;
	this->_init();
}
Drill_COAS_Init::~Drill_COAS_Init(){
}

/*-------------------------------------------------
		单例
*/
Drill_COAS_Init* Drill_COAS_Init::cur_instance = NULL;
Drill_COAS_Init* Drill_COAS_Init::getInstance() {
	if (cur_instance == NULL) {
		cur_instance = new Drill_COAS_Init();
	}
	return cur_instance;
}
/*-------------------------------------------------
		初始化
*/
void Drill_COAS_Init::_init() {
	this->g_COAS_list.clear();
}
/*-------------------------------------------------
		C++数据容器 - 设置数据
*/
void Drill_COAS_Init::setCOASDataByIndex(int index, QJsonObject data){
	if (index < 0){ return; }
	if (index < this->g_COAS_list.count()){
		this->g_COAS_list.replace(index, data);
		return;
	}
	for (int i = this->g_COAS_list.count(); i < index; i++){
		this->g_COAS_list.append(QJsonObject());
	}
	this->g_COAS_list.append(data);
}


/*-------------------------------------------------
		变量获取 - 状态元（~struct~DrillCOASState）
*/
QJsonObject Drill_COAS_Init::drill_COAS_initState(QJsonObject dataFrom){
	QJsonObject data = QJsonObject();

	// > 常规
	data["name"] = dataFrom["状态元名称"].toString("");
	QStringList tag_tank = TTool::_JSON_parse_To_QListQString_(dataFrom["状态元标签"].toString(""));
	data["tag_tank"] = TTool::_QJsonArray_QStringToA_(tag_tank);
	data["priority"] = dataFrom["状态元优先级"].toString("0").toInt();		//【注意大坑：变量获取一定要先toString，再转存为int】
	data["proportion"] = dataFrom["状态元权重"].toString("40").toInt();
	data["canBeInterrupted"] = dataFrom["可被动作元打断"].toString("false") == "true";

	// > GIF
	QStringList gif_src = TTool::_JSON_parse_To_QListQString_(dataFrom["资源-状态元"].toString(""));
	data["gif_src"] = TTool::_QJsonArray_QStringToA_(gif_src);
	QStringList gif_intervalTank = TTool::_JSON_parse_To_QListQString_(dataFrom["帧间隔-明细表"].toString(""));
	QList<int> gif_intervalTank_int = TTool::_QList_QStringToInt_(gif_intervalTank);
	data["gif_intervalTank"] = TTool::_QJsonArray_IntToA_(gif_intervalTank_int);
	data["gif_src_file"] = "img/Special__actionSeq/";
	data["gif_interval"] = dataFrom["帧间隔"].toString("4").toInt();
	data["gif_back_run"] = dataFrom["是否倒放"].toString("false") == "true";
	data["gif_preload"] = dataFrom["是否预加载"].toString("false") == "true";

	// > 图像
	data["tint"] = dataFrom["图像-色调值"].toString("0").toInt();
	data["smooth"] = dataFrom["图像-模糊边缘"].toString("false") == "true";

	// > 杂项
	QString temp = dataFrom["备注"].toString("");
	if (temp[0] == '\"'){
		temp = temp.mid(1, temp.count()-2);
		temp = temp.replace(QRegExp("\\\\"), "\\");
	}
	data["note"] = temp;

	return data;
}
/*-------------------------------------------------
		变量获取 - 状态节点（~struct~DrillCOASStateNode）
*/
QJsonObject Drill_COAS_Init::drill_COAS_initStateNode(QJsonObject dataFrom){
	QJsonObject data = QJsonObject();

	// > 常规
	data["name"] = dataFrom["节点名称"].toString("");
	QStringList tag_tank = TTool::_JSON_parse_To_QListQString_(dataFrom["节点标签"].toString(""));
	data["tag_tank"] = TTool::_QJsonArray_QStringToA_(tag_tank);
	data["priority"] = dataFrom["节点优先级"].toString("0").toInt();		//【注意大坑：变量获取一定要先toString，再转存为int】
	data["proportion"] = dataFrom["节点权重"].toString("40").toInt();
	data["canBeInterrupted"] = dataFrom["可被动作元打断"].toString("false") == "true";

	// > 播放列表
	data["play_type"] = dataFrom["播放方式"].toString("随机播放状态元");
	QStringList play_randomStateSeq = TTool::_JSON_parse_To_QListQString_(dataFrom["随机播放状态元"].toString(""));
	data["play_randomStateSeq"] = TTool::_QJsonArray_QStringToA_(play_randomStateSeq);
	QStringList play_plainStateSeq = TTool::_JSON_parse_To_QListQString_(dataFrom["顺序播放状态元"].toString(""));
	data["play_plainStateSeq"] = TTool::_QJsonArray_QStringToA_(play_plainStateSeq);
	QStringList play_randomNodeSeq = TTool::_JSON_parse_To_QListQString_(dataFrom["随机播放嵌套集合"].toString(""));
	data["play_randomNodeSeq"] = TTool::_QJsonArray_QStringToA_(play_randomNodeSeq);
	QStringList play_plainNodeSeq = TTool::_JSON_parse_To_QListQString_(dataFrom["顺序播放嵌套集合"].toString(""));
	data["play_plainNodeSeq"] = TTool::_QJsonArray_QStringToA_(play_plainNodeSeq);
	data["play_randomMax"] = dataFrom["随机播放的次数上限"].toString("5").toInt();
	qDebug() << play_randomNodeSeq;

	// > 杂项
	QString temp = dataFrom["备注"].toString("");
	if (temp[0] == '\"'){
		temp = temp.mid(1, temp.count() - 2);
		temp = temp.replace(QRegExp("\\\\"), "\\");
	}
	data["note"] = temp;

	return data;
}
/*-------------------------------------------------
		变量获取 - 动作元（~struct~DrillCOASAct）
*/
QJsonObject Drill_COAS_Init::drill_COAS_initAct(QJsonObject dataFrom){
	QJsonObject data = QJsonObject();

	// > 常规
	data["name"] = dataFrom["动作元名称"].toString("");
	QStringList tag_tank = TTool::_JSON_parse_To_QListQString_(dataFrom["动作元标签"].toString(""));
	data["tag_tank"] = TTool::_QJsonArray_QStringToA_(tag_tank);
	data["priority"] = dataFrom["动作元优先级"].toString("20").toInt();

	// > GIF
	QStringList gif_src = TTool::_JSON_parse_To_QListQString_(dataFrom["资源-动作元"].toString(""));
	data["gif_src"] = TTool::_QJsonArray_QStringToA_(gif_src);
	QStringList gif_intervalTank = TTool::_JSON_parse_To_QListQString_(dataFrom["帧间隔-明细表"].toString(""));
	QList<int> gif_intervalTank_int = TTool::_QList_QStringToInt_(gif_intervalTank);
	data["gif_intervalTank"] = TTool::_QJsonArray_IntToA_(gif_intervalTank_int);
	data["gif_src_file"] = "img/Special__actionSeq/";
	data["gif_interval"] = dataFrom["帧间隔"].toString("4").toInt();
	data["gif_back_run"] = dataFrom["是否倒放"].toString("false") == "true";
	data["gif_preload"] = dataFrom["是否预加载"].toString("false") == "true";

	// > 图像
	data["tint"] = dataFrom["图像-色调值"].toString("0").toInt();
	data["smooth"] = dataFrom["图像-模糊边缘"].toString("false") == "true";

	// > 杂项
	QString temp = dataFrom["备注"].toString("");
	if (temp[0] == '\"'){
		temp = temp.mid(1, temp.count() - 2);
		temp = temp.replace(QRegExp("\\\\"), "\\");
	}
	data["note"] = temp;

	return data;
}
/*-------------------------------------------------
		变量获取 - 动画序列（~struct~DrillCOASSequence）
*/
QJsonObject Drill_COAS_Init::drill_COAS_initSequence(QJsonObject dataFrom){
	QJsonObject data = QJsonObject();
	C_ActionSeqLength len = S_ActionSeqDataContainer::getInstance()->getActionSeqLength();

	// > 容器 - 默认的状态元集合
	QStringList state_default_randomSeq = TTool::_JSON_parse_To_QListQString_(dataFrom["默认的状态元集合"].toString(""));
	data["state_default_randomSeq"] = TTool::_QJsonArray_QStringToA_(state_default_randomSeq);

	// > 容器 - 状态元
	QJsonArray state_tank = QJsonArray();
	for (int i = 0; i < len.realLen_state; i++){
		QJsonObject obj = TTool::_JSON_parse_To_Obj_(dataFrom["状态元-"+QString::number(i+1)].toString("{}"));
		state_tank.append(Drill_COAS_Init::drill_COAS_initState(obj));
	}
	data["state_tank"] = state_tank;

	// > 容器 - 状态节点
	QJsonArray stateNode_tank = QJsonArray();
	for (int i = 0; i < len.realLen_stateNode; i++){
		QJsonObject obj = TTool::_JSON_parse_To_Obj_(dataFrom["状态节点-" + QString::number(i + 1)].toString("{}"));
		stateNode_tank.append(Drill_COAS_Init::drill_COAS_initStateNode(obj));
	}
	data["stateNode_tank"] = stateNode_tank;

	// > 容器 - 动作元
	QJsonArray act_tank = QJsonArray();
	for (int i = 0; i < len.realLen_action; i++){
		QJsonObject obj = TTool::_JSON_parse_To_Obj_(dataFrom["动作元-" + QString::number(i+1)].toString("{}"));
		act_tank.append(Drill_COAS_Init::drill_COAS_initAct(obj));
	}
	data["act_tank"] = act_tank;

	return data;
}


/*-------------------------------------------------
		数据访问器 - 获取 - 动画序列
*/
QJsonObject Drill_COAS_Init::drill_COAS_getSequenceData(int sequence_id){
	if (sequence_id < 0){ return QJsonObject(); }
	if (sequence_id >= this->g_COAS_list.count()){ return QJsonObject(); }
	qDebug() << "sequence_id:" << sequence_id;
	return this->g_COAS_list.at(sequence_id);
}
/*-------------------------------------------------
		数据访问器 - 获取 - 状态元
*/
QJsonObject Drill_COAS_Init::drill_COAS_getStateData(int sequence_id, QString state_name){
	if (state_name == ""){ return QJsonObject(); }
	QJsonObject sequence_data = this->drill_COAS_getSequenceData(sequence_id);
	if (sequence_data.isEmpty()){ return QJsonObject(); }
	QJsonArray state_tank = sequence_data["state_tank"].toArray();
	for (int i = 0; i < state_tank.count(); i++){
		QJsonObject data = state_tank[i].toObject();
		if (data["name"].toString() == state_name){
			return data;
		}
	}
	return QJsonObject();
}
/*-------------------------------------------------
		数据访问器 - 获取 - 状态节点
*/
QJsonObject Drill_COAS_Init::drill_COAS_getStateNodeData(int sequence_id, QString stateNode_name){
	if (stateNode_name == ""){ return QJsonObject(); }
	QJsonObject sequence_data = this->drill_COAS_getSequenceData(sequence_id);
	if (sequence_data.isEmpty()){ return QJsonObject(); }
	QJsonArray stateNode_tank = sequence_data["stateNode_tank"].toArray();
	for (int i = 0; i < stateNode_tank.count(); i++){
		QJsonObject data = stateNode_tank[i].toObject();
		if (data["name"].toString() == stateNode_name){
			return data;
		}
	}
	return QJsonObject();
}
/*-------------------------------------------------
		数据访问器 - 获取 - 动作元
*/
QJsonObject Drill_COAS_Init::drill_COAS_getActData(int sequence_id, QString act_name){
	if (act_name == ""){ return QJsonObject(); }
	QJsonObject sequence_data = this->drill_COAS_getSequenceData(sequence_id);
	if (sequence_data.isEmpty()){ return QJsonObject(); }
	QJsonArray act_tank = sequence_data["act_tank"].toArray();
	for (int i = 0; i < act_tank.count(); i++){
		QJsonObject data = act_tank[i].toObject();
		if (data["name"].toString() == act_name){
			return data;
		}
	}
	return QJsonObject();
}
/*-------------------------------------------------
		数据访问器 - 是否存在 状态元
*/
bool Drill_COAS_Init::drill_COAS_hasState(int sequence_id, QString state_name){
	QJsonObject data = this->drill_COAS_getStateData(sequence_id, state_name);
	if (data.isEmpty()){ return false; }
	return true;
}
/*-------------------------------------------------
		数据访问器 - 是否存在 状态节点
*/
bool Drill_COAS_Init::drill_COAS_hasStateNode(int sequence_id, QString stateNode_name){
	QJsonObject data = this->drill_COAS_getStateNodeData(sequence_id, stateNode_name);
	if (data.isEmpty()){ return false; }
	return true;
}
/*-------------------------------------------------
		数据访问器 - 是否存在 动作元
*/
bool Drill_COAS_Init::drill_COAS_hasAct(int sequence_id, QString act_name){
	QJsonObject data = this->drill_COAS_getActData(sequence_id, act_name);
	if (data.isEmpty()){ return false; }
	return true;
}



/*-------------------------------------------------
		数据校验器 - 检查 动画序列
*/
void Drill_COAS_Init::drill_COAS_checkSequenceData(QJsonObject* sequence_data){
	//...（暂不考虑）
}
/*-------------------------------------------------
		数据校验器 - 空检查 状态节点
*/
void Drill_COAS_Init::drill_COAS_checkStateNodeMiss(QJsonObject* sequence_data, QJsonObject* stateNode_data){
	//...（暂不考虑）
}
/*-------------------------------------------------
		数据校验器 - 嵌套检查 状态节点
*/
void Drill_COAS_Init::drill_COAS_checkStateNodeRecursion(QJsonObject* sequence_data, QJsonObject* stateNode_data, int layer){
	//...（暂不考虑）
}