#include "stdafx.h"
#include "w_FCT_Classify.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		���� ���ô���.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��������������ڸô������á�
-----==========================================================-----
*/

W_FCT_Classify::W_FCT_Classify(P_FlexibleClassificationTree* p_obj)
	: QDialog(p_obj->getTree())
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������
	this->isAddMode = true;
	this->m_parentObj = p_obj;

	//-----------------------------------
	//----�¼���
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(acceptData()));

	//-----------------------------------
	//----��ʼ��ui
	TTool::_chinese_(ui.buttonBox);

}
W_FCT_Classify::~W_FCT_Classify(){
}




/*-------------------------------------------------
		���� - �������ݣ���ӣ�
*/
void W_FCT_Classify::setDataInAddMode() {
	this->isAddMode = true;
	this->m_dataPtr = this->m_parentObj->createClassifyData();
	this->m_dataPtr->setName("�µ�����");
	this->m_dataPtr->setDescription("û��������");
	this->setWindowTitle("�������");
	this->putDataToUi();
};
/*-------------------------------------------------
		���� - �������ݣ��޸ģ�
*/
void W_FCT_Classify::setDataInModifyMode(C_FCT_Classify* p) {
	this->isAddMode = false;
	this->m_dataPtr = p;
	this->setWindowTitle("�༭����");
	this->putDataToUi();
};
/*-------------------------------------------------
		���� - ȡ������
*/
C_FCT_Classify* W_FCT_Classify::getData(){
	return this->m_dataPtr;
};
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void W_FCT_Classify::putDataToUi() {
	ui.lineEdit_tagName->setText(m_dataPtr->getName());
	ui.plainTextEdit->setPlainText(m_dataPtr->getDescription());
};
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void W_FCT_Classify::putUiToData() {
	//��ע�⣬���ڴ���ָ�룬��ֵҪ�������˲�ִ�У�
	m_dataPtr->setName(ui.lineEdit_tagName->text());
	m_dataPtr->setDescription(ui.plainTextEdit->toPlainText());
};
/*-------------------------------------------------
		���� - У������
*/
bool W_FCT_Classify::checkData(){
	QString old_name = m_dataPtr->getName();
	QString tag_name = ui.lineEdit_tagName->text();
	QStringList all_name = this->m_parentObj->getAllClassifyName();

	// > ��У��
	if (tag_name == "") {
		QMessageBox::warning(this, "��ʾ", "�������Ʋ���Ϊ�ա�");
		return false;
	}
	if (tag_name == "δ����") {
		QMessageBox::warning(this, "��ʾ", "�������Ʋ���Ϊ\"δ����\"��");
		return false;
	}

	// > ����У��
	if (this->isAddMode == true && all_name.contains(tag_name)){
		QMessageBox::warning(this, "��ʾ", "��������\"" + tag_name + "\"�Ѵ��ڡ�");
		return false;
	}
	// > �༭У�飨�༭�������ƣ��������ظ������
	if (this->isAddMode == false &&
		old_name != tag_name &&
		all_name.contains(tag_name)){
		QMessageBox::warning(this, "��ʾ", "��������\"" + tag_name + "\"�Ѵ��ڡ�");
		return false;
	}
	return true;
}
/*-------------------------------------------------
		���� - �ύ����
*/
void W_FCT_Classify::acceptData(){

	// > У��
	bool pass = this->checkData();
	if ( pass == false){ return; }

	// > �ύ
	this->putUiToData();
	this->accept();
};