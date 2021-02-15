#include "stdafx.h"
#include "p_ActionSeqPart.h"

#include "Source/PluginModule/lengthEditor/s_LEAnnotationReader.h"
#include "Source/Utils/manager/GIFManager/s_GIFManager.h"

#include "../actionSeqData/s_ActionSeqDataContainer.h"

/*
-----==========================================================-----
		�ࣺ		�������п�.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ò����ṩ�������з�ӳ�����ܣ���Ƕ��״̬Ԫ������Ԫ�Ŀ顣
		
		Ŀ�꣺		1.

		ʹ�÷�����
				>��ʼ��

-----==========================================================-----
*/
P_ActionSeqPart::P_ActionSeqPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������

	//-----------------------------------
	//----��ʼ��ui
	this->m_actionPart = new P_ActionPart(parent);
	ui.verticalLayout_action->addWidget(this->m_actionPart);

	this->m_statePart = new P_StatePart(parent);
	ui.verticalLayout_state->addWidget(this->m_statePart);
	

	//-----------------------------------
	//----�¼���
	connect(ui.toolButton, &QPushButton::clicked, this, &P_ActionSeqPart::startPlay);
	connect(ui.toolButton_2, &QPushButton::clicked, this, &P_ActionSeqPart::endPlay);

}

P_ActionSeqPart::~P_ActionSeqPart(){
}


/*-------------------------------------------------
		�ؼ� - ��ʼ����
*/
void P_ActionSeqPart::startPlay() {
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
		�ؼ� - ��������
*/
void P_ActionSeqPart::endPlay() {
	S_GIFManager::getInstance()->dismantlingGIF(QFileInfo("F:/gif��/6HDFVPQ(DV~UCB)2L1D4[(R.gif"),QDir("F:/�½��ļ���/"),"png");
}

/*-------------------------------------------------
		���� - ��������
*/
void P_ActionSeqPart::setData(QJsonObject actionSeq) {
	this->local_actionSeq = actionSeq;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ȡ������
*/
QJsonObject P_ActionSeqPart::getData(){
	this->putUiToData();

	// > У��
	//...
	
	return this->local_actionSeq;
}
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void P_ActionSeqPart::putDataToUi() {
	
	// > ������������
	QFileInfo plugin_file = S_ActionSeqDataContainer::getInstance()->getActionSeqPluginFile();
	C_LEAnnotation* c_le = S_LEAnnotationReader::getInstance()->readPlugin(plugin_file);
	int actionSeq_len = c_le->getParamByKey("��������-%d").getRealLen();


	// > ���붯����������
	qDebug() << this->local_actionSeq;

}
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_ActionSeqPart::putUiToData() {
	

}
