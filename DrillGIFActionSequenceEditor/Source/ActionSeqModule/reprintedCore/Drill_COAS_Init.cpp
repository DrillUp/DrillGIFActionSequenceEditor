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
		
		目标：		->变量获取
						->状态元（~struct~DrillCOASState）
						->动作元（~struct~DrillCOASAct）
						->动画序列（~struct~DrillCOASSequence）

		使用方法：	直接调用静态方法即可。
-----==========================================================-----
*/
Drill_COAS_Init::Drill_COAS_Init(){
}
Drill_COAS_Init::~Drill_COAS_Init(){
}

/*-------------------------------------------------
		变量获取 - 状态元（~struct~DrillCOASState）
*/
QJsonObject Drill_COAS_Init::drill_COAS_initState(QJsonObject dataFrom){
	QJsonObject data = QJsonObject();
	data["name"] = dataFrom["状态元名称"].toString("");
	data["priority"] = dataFrom["状态元优先级"].toInt(0);
	data["proportion"] = dataFrom["状态元权重"].toInt(40);

	QStringList gif_src = TTool::_JSON_parse_To_QListQString_(dataFrom["资源-状态元"].toString(""));
	data["gif_src"] = TTool::_QJsonArray_QStringToA_(gif_src);
	QStringList gif_intervalTank = TTool::_JSON_parse_To_QListQString_(dataFrom["帧间隔-明细表"].toString(""));
	QList<int> gif_intervalTank_int = TTool::_QList_QStringToInt_(gif_intervalTank);
	data["gif_intervalTank"] = TTool::_QJsonArray_IntToA_(gif_intervalTank_int);

	data["gif_src_file"] = "img/Special__actionSeq/";
	data["gif_interval"] = dataFrom["帧间隔"].toInt(4);
	data["gif_back_run"] = dataFrom["是否倒放"].toBool(false);
	data["gif_tint"] = dataFrom["图像-色调值"].toInt(0);
	data["gif_smooth"] = dataFrom["图像-模糊边缘"].toBool(false);

	// > 帧间隔计算
	QJsonArray gif_intervalRealTank = QJsonArray();
	int gif_intervalRealTank_total = 0;
	for (int i = 0; i < gif_src.count(); i++){
		int interval = data["gif_interval"].toInt();
		if (i < gif_intervalTank_int.count()){
			interval = gif_intervalTank_int[i];
		}
		gif_intervalRealTank.append(interval);
		gif_intervalRealTank_total += interval;
	}
	data["gif_intervalRealTank"] = gif_intervalRealTank;
	data["gif_intervalRealTank_total"] = gif_intervalRealTank_total;
	return data;
}
/*-------------------------------------------------
		变量获取 - 动作元（~struct~DrillCOASAct）
*/
QJsonObject Drill_COAS_Init::drill_COAS_initAct(QJsonObject dataFrom){
	QJsonObject data = QJsonObject();
	data["name"] = dataFrom["动作元名称"].toString("");
	data["priority"] = dataFrom["动作元优先级"].toInt(20);

	QStringList gif_src = TTool::_JSON_parse_To_QListQString_(dataFrom["资源-动作元"].toString(""));
	data["gif_src"] = TTool::_QJsonArray_QStringToA_(gif_src);
	QStringList gif_intervalTank = TTool::_JSON_parse_To_QListQString_(dataFrom["帧间隔-明细表"].toString(""));
	QList<int> gif_intervalTank_int = TTool::_QList_QStringToInt_(gif_intervalTank);
	data["gif_intervalTank"] = TTool::_QJsonArray_IntToA_(gif_intervalTank_int);

	data["gif_src_file"] = "img/Special__actionSeq/";
	data["gif_interval"] = dataFrom["帧间隔"].toInt(4);
	data["gif_back_run"] = dataFrom["是否倒放"].toBool(false);
	data["gif_tint"] = dataFrom["图像-色调值"].toInt(0);
	data["gif_smooth"] = dataFrom["图像-模糊边缘"].toBool(false);

	// > 帧间隔计算
	QJsonArray gif_intervalRealTank = QJsonArray();
	int gif_intervalRealTank_total = 0;
	for (int i = 0; i < gif_src.count(); i++){
		int interval = data["gif_interval"].toInt();
		if (i < gif_intervalTank_int.count()){
			interval = gif_intervalTank_int[i];
		}
		gif_intervalRealTank.append(interval);
		gif_intervalRealTank_total += interval;
	}
	data["gif_intervalRealTank"] = gif_intervalRealTank;
	data["gif_intervalRealTank_total"] = gif_intervalRealTank_total;
	return data;
}
/*-------------------------------------------------
		变量获取 - 动画序列（~struct~DrillCOASSequence）
*/
QJsonObject Drill_COAS_Init::drill_COAS_initSequence(QJsonObject dataFrom){
	QJsonObject data = QJsonObject();
	C_ActionSeqLength len = S_ActionSeqDataContainer::getInstance()->getActionSeqLength();

	QStringList state_default_randomSeq = TTool::_JSON_parse_To_QListQString_(dataFrom["默认的状态元集合"].toString(""));
	data["state_default_randomSeq"] = TTool::_QJsonArray_QStringToA_(state_default_randomSeq);

	QJsonArray state_tank = QJsonArray();
	for (int i = 0; i < len.realLen_state; i++){
		QJsonObject obj = TTool::_JSON_parse_To_Obj_(dataFrom["状态元-"+QString::number(i+1)].toString("{}"));
		state_tank.append(Drill_COAS_Init::drill_COAS_initState(obj));
	}
	data["state_tank"] = state_tank;

	QJsonArray act_tank = QJsonArray();
	for (int i = 0; i < len.realLen_action; i++){
		QJsonObject obj = TTool::_JSON_parse_To_Obj_(dataFrom["动作元-" + QString::number(i+1)].toString("{}"));
		act_tank.append(Drill_COAS_Init::drill_COAS_initAct(obj));
	}
	data["act_tank"] = act_tank;

	return data;
}