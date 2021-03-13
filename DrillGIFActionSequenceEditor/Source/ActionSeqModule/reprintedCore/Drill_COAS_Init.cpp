#include "stdafx.h"
#include "Drill_COAS_Init.h"

#include "Source/ActionSeqModule/actionSeqData/s_ActionSeqDataContainer.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		������ȡ�������ࡿ.cpp
		���ߣ�		drill_up
		����ģ�飺	������������
		���ܣ�		�������к��Ĳ���ĸ����ࡣ
		
		Ŀ�꣺		->������ȡ
						->״̬Ԫ��~struct~DrillCOASState��
						->����Ԫ��~struct~DrillCOASAct��
						->�������У�~struct~DrillCOASSequence��

		ʹ�÷�����	ֱ�ӵ��þ�̬�������ɡ�
-----==========================================================-----
*/
Drill_COAS_Init::Drill_COAS_Init(){
}
Drill_COAS_Init::~Drill_COAS_Init(){
}

/*-------------------------------------------------
		������ȡ - ״̬Ԫ��~struct~DrillCOASState��
*/
QJsonObject Drill_COAS_Init::drill_COAS_initState(QJsonObject dataFrom){
	QJsonObject data = QJsonObject();
	data["name"] = dataFrom["״̬Ԫ����"].toString("");
	data["priority"] = dataFrom["״̬Ԫ���ȼ�"].toInt(0);
	data["proportion"] = dataFrom["״̬ԪȨ��"].toInt(40);

	QStringList gif_src = TTool::_JSON_parse_To_QListQString_(dataFrom["��Դ-״̬Ԫ"].toString(""));
	data["gif_src"] = TTool::_QJsonArray_QStringToA_(gif_src);
	QStringList gif_intervalTank = TTool::_JSON_parse_To_QListQString_(dataFrom["֡���-��ϸ��"].toString(""));
	QList<int> gif_intervalTank_int = TTool::_QList_QStringToInt_(gif_intervalTank);
	data["gif_intervalTank"] = TTool::_QJsonArray_IntToA_(gif_intervalTank_int);

	data["gif_src_file"] = "img/Special__actionSeq/";
	data["gif_interval"] = dataFrom["֡���"].toInt(4);
	data["gif_back_run"] = dataFrom["�Ƿ񵹷�"].toBool(false);
	data["gif_tint"] = dataFrom["ͼ��-ɫ��ֵ"].toInt(0);
	data["gif_smooth"] = dataFrom["ͼ��-ģ����Ե"].toBool(false);

	// > ֡�������
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
		������ȡ - ����Ԫ��~struct~DrillCOASAct��
*/
QJsonObject Drill_COAS_Init::drill_COAS_initAct(QJsonObject dataFrom){
	QJsonObject data = QJsonObject();
	data["name"] = dataFrom["����Ԫ����"].toString("");
	data["priority"] = dataFrom["����Ԫ���ȼ�"].toInt(20);

	QStringList gif_src = TTool::_JSON_parse_To_QListQString_(dataFrom["��Դ-����Ԫ"].toString(""));
	data["gif_src"] = TTool::_QJsonArray_QStringToA_(gif_src);
	QStringList gif_intervalTank = TTool::_JSON_parse_To_QListQString_(dataFrom["֡���-��ϸ��"].toString(""));
	QList<int> gif_intervalTank_int = TTool::_QList_QStringToInt_(gif_intervalTank);
	data["gif_intervalTank"] = TTool::_QJsonArray_IntToA_(gif_intervalTank_int);

	data["gif_src_file"] = "img/Special__actionSeq/";
	data["gif_interval"] = dataFrom["֡���"].toInt(4);
	data["gif_back_run"] = dataFrom["�Ƿ񵹷�"].toBool(false);
	data["gif_tint"] = dataFrom["ͼ��-ɫ��ֵ"].toInt(0);
	data["gif_smooth"] = dataFrom["ͼ��-ģ����Ե"].toBool(false);

	// > ֡�������
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
		������ȡ - �������У�~struct~DrillCOASSequence��
*/
QJsonObject Drill_COAS_Init::drill_COAS_initSequence(QJsonObject dataFrom){
	QJsonObject data = QJsonObject();
	C_ActionSeqLength len = S_ActionSeqDataContainer::getInstance()->getActionSeqLength();

	QStringList state_default_randomSeq = TTool::_JSON_parse_To_QListQString_(dataFrom["Ĭ�ϵ�״̬Ԫ����"].toString(""));
	data["state_default_randomSeq"] = TTool::_QJsonArray_QStringToA_(state_default_randomSeq);

	QJsonArray state_tank = QJsonArray();
	for (int i = 0; i < len.realLen_state; i++){
		QJsonObject obj = TTool::_JSON_parse_To_Obj_(dataFrom["״̬Ԫ-"+QString::number(i+1)].toString("{}"));
		state_tank.append(Drill_COAS_Init::drill_COAS_initState(obj));
	}
	data["state_tank"] = state_tank;

	QJsonArray act_tank = QJsonArray();
	for (int i = 0; i < len.realLen_action; i++){
		QJsonObject obj = TTool::_JSON_parse_To_Obj_(dataFrom["����Ԫ-" + QString::number(i+1)].toString("{}"));
		act_tank.append(Drill_COAS_Init::drill_COAS_initAct(obj));
	}
	data["act_tank"] = act_tank;

	return data;
}