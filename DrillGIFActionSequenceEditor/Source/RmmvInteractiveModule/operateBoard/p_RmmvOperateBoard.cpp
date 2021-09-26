#include "stdafx.h"
#include "p_RmmvOperateBoard.h"

#include "../custom/s_RmmvCaller_ActionSeq.h"

/*
-----==========================================================-----
		�ࣺ		�������.cpp
		�汾��		v1.01
		����ģ�飺	����ģ��
		���ܣ�		�ò����ṩrmmv�Ľ�����ť���ܡ�
		
		Ŀ�꣺		> ��rmmv��ť
					> rmmv������ʾ������
					> ����rmmv��ť
					> ����rmmv��ť

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
	connect(ui.toolButton_select, &QPushButton::clicked, this, &P_RmmvOperateBoard::selectRmmv);
	connect(ui.toolButton_import, &QPushButton::clicked, this, &P_RmmvOperateBoard::importRmmv);
	connect(ui.toolButton_save, &QPushButton::clicked, this, &P_RmmvOperateBoard::saveToRmmv);
	connect(ui.toolButton_run, &QPushButton::clicked, this, &P_RmmvOperateBoard::runRmmv);
	connect(ui.toolButton_software, &QPushButton::clicked, this, &P_RmmvOperateBoard::openRmmvSoftware);
	connect(ui.lineEdit_name, &QLineEdit::textChanged, this, &P_RmmvOperateBoard::refreshRmmvEnable);

}

P_RmmvOperateBoard::~P_RmmvOperateBoard(){
}



/*-------------------------------------------------
		�ؼ� - ѡ��rmmv
*/
void P_RmmvOperateBoard::selectRmmv(){
	this->local_rmmvData = S_RmmvCaller_ActionSeq::getInstance()->callRmmvSelect();
	ui.lineEdit_path->setText(this->local_rmmvData.getRootPath());
	ui.lineEdit_name->setText(this->local_rmmvData.getName());
}
/*-------------------------------------------------
		�ؼ� - ˢ��rmmv��������
*/
void P_RmmvOperateBoard::refreshRmmvEnable(){
	if (ui.lineEdit_name->text() == ""){
		ui.groupBox->setEnabled(false);
	}else{
		ui.groupBox->setEnabled(true);
	}
}
/*-------------------------------------------------
		�ؼ� - ��rmmv�༭��
*/
void P_RmmvOperateBoard::openRmmvSoftware(){
	if (this->local_rmmvData.isNull()){ return; }

	S_RmmvCaller_ActionSeq::getInstance()->callRmmvSoftware(this->local_rmmvData);
}
/*-------------------------------------------------
		�ؼ� - ����rmmv����
*/
void P_RmmvOperateBoard::importRmmv(){
	if (this->local_rmmvData.isNull()){ return; }
	if (QMessageBox::information(this, "��ʾ", "�˲������rmmv�����ж�ȡ���ݣ������ǵ���ǰ���ݣ���ǰ�Ķ����������ݽ�ȫ����ʧ���Ƿ����?", "����", "ȡ��", 0, 1) == 1){
		return;
	}

	S_RmmvCaller_ActionSeq::getInstance()->loadAllRmmvFile(this->local_rmmvData);
	S_RmmvCaller_ActionSeq::getInstance()->coverSourceFileToTemp(this->local_rmmvData);
}
/*-------------------------------------------------
		�ؼ� - ���浽rmmv
*/
void P_RmmvOperateBoard::saveToRmmv(){
	if (this->local_rmmvData.isNull()){ return; }

	// > Ҫ��رձ༭�����ܴ�
	S_RmmvCaller_ActionSeq::getInstance()->callRmmvSave_RequestingClose(this->local_rmmvData);
}
/*-------------------------------------------------
		�ؼ� - ����rmmv
*/
void P_RmmvOperateBoard::runRmmv(){
	if (this->local_rmmvData.isNull()){ return; }

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

	ui.lineEdit_path->setText(this->local_rmmvData.getRootPath());
	ui.lineEdit_name->setText(this->local_rmmvData.getName());
	ui.checkBox->setChecked(this->local_rmmvData.optional_backup);
}
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_RmmvOperateBoard::putUiToData() {
	
	this->local_rmmvData.optional_backup = ui.checkBox->isChecked();
}
