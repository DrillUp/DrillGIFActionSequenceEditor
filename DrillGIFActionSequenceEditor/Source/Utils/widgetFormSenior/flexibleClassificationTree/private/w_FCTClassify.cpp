#include "stdafx.h"
#include "w_FCTClassify.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		���� ���ô���.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���͵����������ڸô������á�
-----==========================================================-----
*/

W_FCTClassify::W_FCTClassify(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������
	this->isAddMode = true;

	//-----------------------------------
	//----�¼���
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(acceptData()));

	//-----------------------------------
	//----��ʼ��ui
	TTool::_chinese_(ui.buttonBox);

}
W_FCTClassify::~W_FCTClassify(){
}




/*-------------------------------------------------
		���� - �������ݣ���ӣ�
*/
void W_FCTClassify::setDataInAddMode(QList<C_FCTClassify>* checkFrom) {
	this->isAddMode = true;
	this->local_FCTClassify = C_FCTClassify();
	this->local_FCTClassify.name = "�µķ���";
	this->local_FCTClassify.description = "û��������";
	this->m_checkFrom = checkFrom;
	this->setWindowTitle("��ӷ���");
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - �������ݣ��޸ģ�
*/
void W_FCTClassify::setDataInModifyMode(C_FCTClassify p, QList<C_FCTClassify>* checkFrom) {
	this->isAddMode = false;
	this->local_FCTClassify = p;
	this->m_checkFrom = checkFrom;
	this->setWindowTitle("�༭����");
	this->putDataToUi();
}

/*-------------------------------------------------
		���� - ȡ������
*/
C_FCTClassify W_FCTClassify::getData(){
	return this->local_FCTClassify;
};
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void W_FCTClassify::putDataToUi() {
	TTool::_QString_(ui.lineEdit_tagName, &local_FCTClassify.name);
	TTool::_QString_(ui.plainTextEdit, &local_FCTClassify.description);
};
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void W_FCTClassify::putUiToData() {
	TTool::_QString_(&local_FCTClassify.name, ui.lineEdit_tagName);
	TTool::_QString_(&local_FCTClassify.description, ui.plainTextEdit);
};
/*-------------------------------------------------
		���� - �ύ���ݣ�У�飩
*/
void W_FCTClassify::acceptData(){
	QString old_name = local_FCTClassify.name;
	this->putUiToData();

	// > ��У��
	if (ui.lineEdit_tagName->text() == "") {
		QMessageBox::warning(this,"��ʾ","�������Ʋ���Ϊ�ա�");
		return;
	}
	if (ui.lineEdit_tagName->text() == "δ����") {
		QMessageBox::warning(this, "��ʾ", "�������Ʋ���Ϊ\"δ����\"��");
		return;
	}

	// > ����У��
	if (this->isAddMode == true && this->m_checkFrom->contains(local_FCTClassify)){
		QMessageBox::warning(this, "��ʾ", "��������\"" + local_FCTClassify.name + "\"�Ѵ��ڡ�");
		return;
	}
	// > �༭У��
	if (this->isAddMode == false &&
		old_name != local_FCTClassify.name &&
		this->m_checkFrom->contains(local_FCTClassify) ){
		QMessageBox::warning(this, "��ʾ", "��������\"" + local_FCTClassify.name + "\"�Ѵ��ڡ�");
		return;
	}

	this->accept();
};