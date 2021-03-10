#include "stdafx.h"
#include "w_ALEGIFWriterType.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		ѡ��GIF��ȡ�� ����.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		����ѡ����Ĵ��ڡ�

		ʹ�÷�����
				>�򿪴���
					W_ALELoadGIFType d;
					if (d.exec() == QDialog::Accepted){
						this->setConfigParam( d.getData() );
					}
-----==========================================================-----
*/

W_ALEGIFWriterType::W_ALEGIFWriterType(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������
	this->local_index = 0;

	//-----------------------------------
	//----��ʼ��ui
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_ALEGIFWriterType::acceptData);
	TTool::_chinese_(ui.buttonBox);

}

W_ALEGIFWriterType::~W_ALEGIFWriterType(){
}


/*-------------------------------------------------
		���� - ��������
*/
void W_ALEGIFWriterType::setDataInModifyMode(int index){
	this->local_index = index;
	this->putDataToUi();
};
/*-------------------------------------------------
		���� - ȡ������
*/
int W_ALEGIFWriterType::getData(){
	this->putUiToData();
	return this->local_index;
};
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void W_ALEGIFWriterType::putDataToUi() {

	if (this->local_index == 0){ ui.radioButton->setChecked(true); }
	if (this->local_index == 1){ ui.radioButton_2->setChecked(true); }
};
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void W_ALEGIFWriterType::putUiToData() {
	
	if (ui.radioButton->isChecked()){ this->local_index = 0; }
	if (ui.radioButton_2->isChecked()){ this->local_index = 1; }
};
/*-------------------------------------------------
		���� - �ύ���ݣ�У�飩
*/
void W_ALEGIFWriterType::acceptData(){
	this->putUiToData();

	this->accept();
};