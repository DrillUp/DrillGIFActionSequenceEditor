#include "stdafx.h"
#include "p_PlayingPart.h"

#include "Source/Utils/manager/GIFManager/s_GIFManager.h"

/*
-----==========================================================-----
		�ࣺ		��ӳ��.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ò����ṩ����Ԫ�༭���ܡ�
		
		Ŀ�꣺		1.

		ʹ�÷�����
				>��ʼ��

-----==========================================================-----
*/
P_PlayingPart::P_PlayingPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������

	//-----------------------------------
	//----��ʼ��ui


	//-----------------------------------
	//----�¼���
	connect(ui.toolButton, &QPushButton::clicked, this, &P_PlayingPart::startPlay);
	connect(ui.toolButton_2, &QPushButton::clicked, this, &P_PlayingPart::endPlay);

}

P_PlayingPart::~P_PlayingPart(){
}

/*-------------------------------------------------
		��ӳ�� - ��ʼ����
*/
void P_PlayingPart::startPlay() {
	//S_GIFManager::getInstance()->dismantlingGIF_testApi();
	QList<QFileInfo> info_list = QList<QFileInfo>();
	info_list.push_back(QFileInfo("F:/�½��ļ���/0.png"));
	info_list.push_back(QFileInfo("F:/�½��ļ���/1.png"));
	info_list.push_back(QFileInfo("F:/�½��ļ���/2.png"));
	info_list.push_back(QFileInfo("F:/�½��ļ���/3.png"));
	info_list.push_back(QFileInfo("F:/�½��ļ���/4.png"));
	info_list.push_back(QFileInfo("F:/�½��ļ���/5.png"));
	info_list.push_back(QFileInfo("F:/�½��ļ���/6.png"));
	info_list.push_back(QFileInfo("F:/�½��ļ���/7.png"));
	info_list.push_back(QFileInfo("F:/�½��ļ���/8.png"));
	info_list.push_back(QFileInfo("F:/�½��ļ���/9.png"));
	info_list.push_back(QFileInfo("F:/�½��ļ���/10.png"));
	info_list.push_back(QFileInfo("F:/�½��ļ���/11.png"));
	info_list.push_back(QFileInfo("F:/�½��ļ���/12.png"));
	info_list.push_back(QFileInfo("F:/�½��ļ���/13.png"));
	info_list.push_back(QFileInfo("F:/�½��ļ���/14.png"));
	S_GIFManager::getInstance()->generateGIF(info_list, QFileInfo("F:/aaa.gif"), 4);
}
/*-------------------------------------------------
		��ӳ�� - ��������
*/
void P_PlayingPart::endPlay() {
	S_GIFManager::getInstance()->dismantlingGIF(QFileInfo("F:/gif��/6HDFVPQ(DV~UCB)2L1D4[(R.gif"),QDir("F:/�½��ļ���/"),"png");
}

/*-------------------------------------------------
		���� - ��������
*/
void P_PlayingPart::setData(QJsonObject actionData) {
	this->local_actionData = actionData;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ȡ������
*/
QJsonObject P_PlayingPart::getData(){
	this->putUiToData();

	// > У��
	//...
	
	return this->local_actionData;
}
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void P_PlayingPart::putDataToUi() {
	

}
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_PlayingPart::putUiToData() {
	

}
