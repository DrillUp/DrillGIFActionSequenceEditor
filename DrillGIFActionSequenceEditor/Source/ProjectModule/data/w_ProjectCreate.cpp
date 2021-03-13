#include "stdafx.h"
#include "w_ProjectCreate.h"
#include "../s_ProjectManager.h"

/*
-----==========================================================-----
		�ࣺ		�½���Ŀ����.cpp
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		���������̺󣬽��½���Ŀ�ļ��С�
					�����½���Ŀ�����Ϊ��Ŀ��
-----==========================================================-----
*/

W_ProjectCreate::W_ProjectCreate(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	//-----------------------------------
	//----�ؼ���ʼ��
	this->m_P_ProjectCreate = new P_ProjectCreate();
	QVBoxLayout* l = new QVBoxLayout(ui.widget);
	ui.widget->setLayout(l);
	l->setMargin(0);
	l->addWidget(this->m_P_ProjectCreate);

	//-----------------------------------
	//----�¼���
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(acceptData()));

	//-----------------------------------
	//----ui��ʼ��
	ui.buttonBox->button(QDialogButtonBox::Ok)->setText( ("ȷ��"));
	ui.buttonBox->button(QDialogButtonBox::Cancel)->setText( ("ȡ��"));
}
W_ProjectCreate::~W_ProjectCreate(){
}


/*-------------------------------------------------
		λ�� - ���λ��
*/
void W_ProjectCreate::insertPath(QString path){
	this->m_P_ProjectCreate->insertPath(path);
}
/*-------------------------------------------------
		λ�� - ѡ��λ��
*/
void W_ProjectCreate::selectPath(QString path){
	this->m_P_ProjectCreate->selectPath(path);
}
/*-------------------------------------------------
		λ�� - ��ȡѡ���λ��
*/
QString W_ProjectCreate::getCurrentPath(){
	return this->m_P_ProjectCreate->getCurrentPath();
}


/*-------------------------------------------------
		���� - ��������
*/
void W_ProjectCreate::setDataInModifyMode(C_ProjectData p) {
	this->m_P_ProjectCreate->setDataInModifyMode(p);
}

/*-------------------------------------------------
		���� - ȡ������
*/
C_ProjectData W_ProjectCreate::getData(){
	return this->m_P_ProjectCreate->getData();
};
/*-------------------------------------------------
		���� - �ύ���ݣ�У�飩
*/
void W_ProjectCreate::acceptData(){

	C_ProjectData data = this->getData();
	if (data.isNull()) { return; }

	this->accept();
};