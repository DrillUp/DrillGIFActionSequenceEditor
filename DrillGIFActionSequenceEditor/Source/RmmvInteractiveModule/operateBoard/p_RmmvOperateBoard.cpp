#include "stdafx.h"
#include "p_RmmvOperateBoard.h"

#include "../base/s_RmmvCaller_ActionSeq.h"

/*
-----==========================================================-----
		�ࣺ		�������.cpp
		����ģ�飺	����ģ��
		���ܣ�		�ò����ṩrmmv�Ľ�����ť���ܡ�
		
		Ŀ�꣺		1.

		ʹ�÷�����
				>��ʼ��

-----==========================================================-----
*/
P_RmmvOperateBoard::P_RmmvOperateBoard(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������

	//-----------------------------------
	//----��ʼ��ui


	//-----------------------------------
	//----�¼���
	connect(ui.toolButton_open, &QPushButton::clicked, this, &P_RmmvOperateBoard::openRmmv);
	connect(ui.toolButton_import, &QPushButton::clicked, this, &P_RmmvOperateBoard::importRmmv);
	connect(ui.toolButton_save, &QPushButton::clicked, this, &P_RmmvOperateBoard::saveToRmmv);
	connect(ui.toolButton_run, &QPushButton::clicked, this, &P_RmmvOperateBoard::runRmmv);

}

P_RmmvOperateBoard::~P_RmmvOperateBoard(){
}



/*-------------------------------------------------
		�ؼ� - ��rmmv
*/
void P_RmmvOperateBoard::openRmmv(){
	this->local_rmmvData = S_RmmvCaller_ActionSeq::getInstance()->callRmmvOpen();
	ui.lineEdit->setText(this->local_rmmvData.name);
}
/*-------------------------------------------------
		�ؼ� - ����rmmv����
*/
void P_RmmvOperateBoard::importRmmv(){
	if (this->local_rmmvData.isNull()){
		QMessageBox::warning(this, "��ʾ", "����Ҫ�ȴ�rmmv���̡�", QMessageBox::Yes);
		return;
	}
	switch (QMessageBox::information(this, "��ʾ", "�˲������rmmv�����ж�ȡ���ݣ������ǵ���ǰ���ݣ��Ƿ����?", "����", "ȡ��", 0, 1)){
		case 0:
			break;
		case 1:
		default:
			return;
			break;
	}

	S_RmmvCaller_ActionSeq::getInstance()->loadAllRmmvFile(this->local_rmmvData);
}
/*-------------------------------------------------
		�ؼ� - ���浽rmmv
*/
void P_RmmvOperateBoard::saveToRmmv(){
	if (this->local_rmmvData.isNull()){
		QMessageBox::warning(this, "��ʾ", "����Ҫ�ȴ�rmmv���̡�", QMessageBox::Yes);
		return;
	}

	// > Ҫ��رձ༭�����ܴ�
	S_RmmvCaller_ActionSeq::getInstance()->callRmmvSave_RequestingClose(this->local_rmmvData);

}
/*-------------------------------------------------
		�ؼ� - ����rmmv
*/
void P_RmmvOperateBoard::runRmmv(){
	if (this->local_rmmvData.isNull()){
		QMessageBox::warning(this, "��ʾ", "����Ҫ�ȴ�rmmv���̡�", QMessageBox::Yes);
		return;
	}

	S_RmmvCaller_ActionSeq::getInstance()->callRmmvRun(this->local_rmmvData);

}


/*-------------------------------------------------
		���� - ��������
*/
void P_RmmvOperateBoard::setData(C_RmmvProjectData actionData) {
	this->local_rmmvData = actionData;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ȡ������
*/
C_RmmvProjectData P_RmmvOperateBoard::getData(){
	this->putUiToData();
	return this->local_rmmvData;
}
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void P_RmmvOperateBoard::putDataToUi() {
	
	ui.lineEdit->setText(this->local_rmmvData.name);
}
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_RmmvOperateBoard::putUiToData() {
	
	//���ޣ�
}
