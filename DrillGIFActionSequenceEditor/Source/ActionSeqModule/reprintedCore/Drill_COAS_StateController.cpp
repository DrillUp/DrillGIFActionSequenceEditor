#include "stdafx.h"
#include "Drill_COAS_StateController.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		״̬Ԫ�������������ࡿ.cpp
		���ߣ�		drill_up
		����ģ�飺	������������
		���ܣ�		> ����һ��ר�ſ��� ״̬Ԫ �������ࡣ
					> ����ɱ��浽�浵�С�
					
		˵����		��ǰ���̰汾��[v1.3]
-----==========================================================-----
*/
Drill_COAS_StateController::Drill_COAS_StateController(){
	this->_drill_data = QJsonObject();		//�������
	this->drill_initData_State();			//��ʼ������
	this->drill_initPrivateData_State();	//˽�����ݳ�ʼ��
}
Drill_COAS_StateController::Drill_COAS_StateController(QJsonObject data){
	this->_drill_data = data;				//�������
	this->drill_initData_State();			//��ʼ������
	this->drill_initPrivateData_State();	//˽�����ݳ�ʼ��
}
Drill_COAS_StateController::~Drill_COAS_StateController(){
}

/*-------------------------------------------------
		���� - ���ж�
*/
bool Drill_COAS_StateController::isNull(){
	return this->_drill_data.isEmpty();
}
/*-------------------------------------------------
		���� - ��ʼ������
*/
void Drill_COAS_StateController::drill_initData_State(){
	QJsonObject data = this->_drill_data;

	// > ����
	if (data["name"].isUndefined() == true){ data["name"] = ""; }					//״̬Ԫ����
	if (data["priority"].isUndefined() == true){ data["priority"] = 0; }			//״̬Ԫ���ȼ�
	if (data["proportion"].isUndefined() == true){ data["proportion"] = 40; }		//״̬ԪȨ��

	// > GIF
	if (data["gif_src"].isUndefined() == true){ data["gif_src"] = QJsonArray(); }							//GIF - ��Դ
	if (data["gif_src_file"].isUndefined() == true){ data["gif_src_file"] = "img/Special__actionSeq/"; }	//GIF - ��Դ
	if (data["gif_intervalTank"].isUndefined() == true){ data["gif_intervalTank"] = QJsonArray(); }			//GIF - ֡���-��ϸ��
	if (data["gif_interval"].isUndefined() == true){ data["gif_interval"] = 4; }							//GIF - ֡���
	if (data["gif_back_run"].isUndefined() == true){ data["gif_back_run"] = false; }						//GIF - �Ƿ񵹷�

	// > ͼ��
	if (data["tint"].isUndefined() == true){ data["tint"] = 0; }					//ͼ�� - ɫ��ֵ
	if (data["smooth"].isUndefined() == true){ data["smooth"] = false; }			//ͼ�� - ģ����Ե

	// > ����
	if (data["note"].isUndefined() == true){ data["note"] = ""; }					//���� - ��ע
}
/*-------------------------------------------------
		���� - ˽�����ݳ�ʼ��
*/
void Drill_COAS_StateController::drill_initPrivateData_State(){
	QJsonObject data = this->_drill_data;

	// > ����
	this->_drill_curTime = 0;										//���� - ��ǰʱ��
	this->_drill_needDestroy = false;								//���� - ����

	// > GIF - �������
	this->_drill_curBitmapName = "";								//������� - ��ǰ�Ķ�����
	this->_drill_curBitmapPath = data["gif_src_file"].toString();	//������� - ��ǰ��·��
	this->_drill_curBitmapTint = data["tint"].toInt();				//������� - ��ǰ��ɫ��
	this->_drill_curBitmapSmooth = data["smooth"].toBool();			//������� - ��ǰ��ģ��

	// > GIF - ����
	this->_drill_curTickTime = 0;									//���� - ��ǰ�ۼ�ʱ��
	this->_drill_curIndex = 0;										//���� - ��ǰ����
	this->_drill_tarIndex = 0;										//���� - ��������λ��

	// > GIF - ֡����б� ����
	this->_drill_curIntervalTank = QJsonArray();
	for (int i = 0; i < data["gif_src"].toArray().count(); i++){
		int interval = data["gif_interval"].toInt();
		QJsonArray gif_intervalTank = data["gif_intervalTank"].toArray();
		if (i < gif_intervalTank.count()){
			interval = gif_intervalTank[i].toInt();
		}
		this->_drill_curIntervalTank[i] = interval;
	}
}

/*-------------------------------------------------
		֡ˢ�£���Ҫ�����ֶ�ִ�У�
*/
void Drill_COAS_StateController::drill_COAS_update(){
	if (this->isNull()){ return; }
	this->_drill_curTime += 1;				//ʱ��+1
	this->drill_COAS_updateState();			//ˢ��״̬Ԫ
}
/*-------------------------------------------------
		֡ˢ�� - ˢ��״̬Ԫ
*/
void Drill_COAS_StateController::drill_COAS_updateState(){
	

}


/*-------------------------------------------------
		���� - ״̬Ԫ - �������У��ӿڣ�

				˵����	�����д�� ״̬���� �����飬״̬Ԫ��������ʶ��Ķ���
*/
void Drill_COAS_StateController::drill_COAS_setSequence(QStringList seq){
	
}