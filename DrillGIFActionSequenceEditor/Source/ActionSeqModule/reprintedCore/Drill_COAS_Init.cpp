#include "stdafx.h"
#include "Drill_COAS_Init.h"

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

		ʹ�÷�����
				>��ʼ��

-----==========================================================-----
*/
Drill_COAS_Init::Drill_COAS_Init(QObject *parent = 0)
	: QObject(parent)
{
	this->_drill_data = data;			//�������
	this->drill_initData();				//��ʼ������
}
Drill_COAS_Init::~Drill_COAS_Init(){
}

/*-------------------------------------------------
		���� - ��ʼ��
*/
void Drill_COAS_Init::drill_initData(){
	QJsonObject data = this->_drill_data;
	
	QString state_default_randomSeq_str = data.value("state_default_randomSeq").toString();
	if (){

	}
	QStringList


	this->_drill_time = 0;											//����ʱ��
	this->_drill_arrayCheck = true;									//�������Ԫ��
	this->_drill_bitmapName = "";									//��ǰ��bitmap������
	this->_drill_bitmapPath = "";									//��ǰ��bitmap·��
	this->_drill_bitmapTint = 0;										//��ǰ��bitmapɫ��
	this->_drill_bitmapSmooth = false;								//��ǰ��bitmapģ��
	this->_drill_state_curCom = "";									//״̬Ԫ - ��ǰ״̬
	this->_drill_state_curTime = 0;									//״̬Ԫ - ��ǰʱ��
	this->_drill_state_curSeq = data['state_default_randomSeq'];		//״̬Ԫ - ��ǰ����
	this->_drill_act_curCom = "";									//����Ԫ - ��ǰ����
	this->_drill_act_curTime = 0;									//����Ԫ - ��ǰʱ��
}
/*-------------------------------------------------
		���� - ֡ˢ�£���Ҫ�����ֶ�ִ�У�
*/
void Drill_COAS_Init::update(){
	



}