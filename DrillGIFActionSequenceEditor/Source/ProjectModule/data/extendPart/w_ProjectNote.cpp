#include "stdafx.h"
#include "W_ProjectNote.h"

#include <QtWidgets>
#include "../../s_ProjectManager.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		��Ŀ��ע �༭����.cpp
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		��Ŀ��ע�ı��ı༭���ڡ�
-----==========================================================-----
*/
W_ProjectNote::W_ProjectNote(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----�¼���
	connect(ui.buttonBox, &QDialogButtonBox::clicked, this, &W_ProjectNote::clickedBtn);

	//-----------------------------------
	//----ui��ʼ��

	// > ���ڰ�ť��
	ui.buttonBox->button(QDialogButtonBox::Ok)->setText(("ȷ��"));
	ui.buttonBox->button(QDialogButtonBox::Retry)->setText(("���±༭"));
	ui.buttonBox->button(QDialogButtonBox::Cancel)->setText(("ȡ��"));

	// > ��Ϣ��ʾ
	C_ProjectData data = S_ProjectManager::getInstance()->data_ProjectData;
	QString name = data.getName();
	if (name == ""){ name = "δ����"; }
	ui.label->setText("�������ƣ� " + name);
	ui.label_2->setText("����ʱ�䣺 " + data.createDate.toString("yyyy/MM/dd hh:mm:ss"));

	// > �����ı���ʼ��
	this->putUiToData();
}
W_ProjectNote::~W_ProjectNote(){
}


/*-------------------------------------------------
		���� - ��������
*/
void W_ProjectNote::setData(QString p) {
	if (p == ""){ return; }
	this->local_ProjectNote = p;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ȡ������
*/
QString W_ProjectNote::getData(){
	return this->local_ProjectNote;
};
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void W_ProjectNote::putDataToUi() {

	TTool::_QString_(ui.plainTextEdit, &this->local_ProjectNote);
};
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void W_ProjectNote::putUiToData() {

	TTool::_QString_(&this->local_ProjectNote, ui.plainTextEdit);

};
/*-------------------------------------------------
		���� - �ύ���ݣ�У�飩
*/
void W_ProjectNote::acceptData(){
	this->putUiToData();

	//��У��

	this->accept();
};
/*-------------------------------------------------
		���� - ��ť����
*/
void W_ProjectNote::clickedBtn(QAbstractButton * btn){
	if (btn == ui.buttonBox->button(QDialogButtonBox::Ok)) {
		this->acceptData();
	}
	else if (btn == ui.buttonBox->button(QDialogButtonBox::Retry)) {
		this->putDataToUi();
	}
};