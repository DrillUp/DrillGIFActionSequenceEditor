#include "stdafx.h"
#include "w_PictureCombiner.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ�ϳ��� ����.cpp
		����ģ�飺	ͼ��ģ��
		���ܣ�		���Խ�����ͼƬ�Ծ������ʽ�ϲ���һ��
-----==========================================================-----
*/

W_PictureCombiner::W_PictureCombiner(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	//-----------------------------------
	//----���/��С��
	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowCloseButtonHint;
	flags |= Qt::WindowMaximizeButtonHint;
	flags |= Qt::WindowMinimizeButtonHint;
	this->setWindowFlags(flags);

	//-----------------------------------
	//----�ؼ���ʼ��
	this->m_P_PictureCombiner = new P_PictureCombiner();
	QVBoxLayout* l = new QVBoxLayout(ui.widget);
	ui.widget->setLayout(l);
	l->setMargin(0);
	l->addWidget(this->m_P_PictureCombiner);

	//-----------------------------------
	//----�¼���
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(acceptData()));

	//-----------------------------------
	//----ui��ʼ��
	ui.buttonBox->button(QDialogButtonBox::Ok)->setText( ("ȷ��"));
	ui.buttonBox->button(QDialogButtonBox::Cancel)->setText( ("ȡ��"));
}
W_PictureCombiner::~W_PictureCombiner(){
}


/*-------------------------------------------------
		���� - ��������
*/
void W_PictureCombiner::setData(QList<QFileInfo> file_list) {
	this->m_P_PictureCombiner->setData(file_list);
}
void W_PictureCombiner::setData(QList<QPixmap> bitmap_list) {
	this->m_P_PictureCombiner->setData(bitmap_list);
}

/*-------------------------------------------------
		���� - ȡ������
*/
QPixmap W_PictureCombiner::getData(){
	return this->m_P_PictureCombiner->getData();
};
/*-------------------------------------------------
		���� - �ύ���ݣ�У�飩
*/
void W_PictureCombiner::acceptData(){
	this->accept();
};