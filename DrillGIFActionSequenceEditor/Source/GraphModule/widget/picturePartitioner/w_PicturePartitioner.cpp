#include "stdafx.h"
#include "w_PicturePartitioner.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ�ָ��� ����.cpp
		����ģ�飺	ͼ��ģ��
		���ܣ�		���Խ�ͼƬ�ָ�ɶ��ͼƬ��
-----==========================================================-----
*/

W_PicturePartitioner::W_PicturePartitioner(QWidget* parent)
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
	//----��ʼ������
	this->m_slotBlock = false;

	//-----------------------------------
	//----�ؼ���ʼ��

	// > �и�Ԥ��ͼ
	this->m_p_SinglePictureViewer = new P_SinglePictureViewer(ui.widget_preview);
	this->m_p_SinglePictureViewer->rebuildUI();

	// > ֡ѡ��ͼ
	this->m_p_PPaViewer = new P_PPaViewer(ui.widget_selectArea);
	this->m_p_PPaViewer->rebuildUI();

	// > ���Ű�ť��1
	this->m_p_MouseResizeButtonPart1 = new P_MouseResizeButtonPart();
	QVBoxLayout* latout = new QVBoxLayout(ui.widget_zoom_btn);
	latout->setMargin(0);
	latout->addWidget(this->m_p_MouseResizeButtonPart1);
	this->m_p_MouseResizeButtonPart1->initWidget(this->m_p_SinglePictureViewer->getResizeController());

	// > ���Ű�ť��2
	this->m_p_MouseResizeButtonPart2 = new P_MouseResizeButtonPart();
	QVBoxLayout* latout2 = new QVBoxLayout(ui.widget_zoom_btn_2);
	latout2->setMargin(0);
	latout2->addWidget(this->m_p_MouseResizeButtonPart2);
	this->m_p_MouseResizeButtonPart2->initWidget(this->m_p_PPaViewer->getResizeController());

	//-----------------------------------
	//----�¼���

	// > ����
	connect(ui.spinBox_column, SIGNAL(valueChanged(int)), this, SLOT(columnChanged(int)));
	connect(ui.spinBox_row, SIGNAL(valueChanged(int)), this, SLOT(rowChanged(int)));

	// > ����
	connect(ui.pushButton_next, &QPushButton::clicked, this, &W_PicturePartitioner::toFlow_2);
	connect(ui.pushButton_last, &QPushButton::clicked, this, &W_PicturePartitioner::toFlow_1);
	connect(ui.pushButton_finish, &QPushButton::clicked, this, &W_PicturePartitioner::acceptData);

	//-----------------------------------
	//----��ʼ��ui
	TTool::_chinese_(ui.buttonBox);

}
W_PicturePartitioner::~W_PicturePartitioner(){
}

/*-----------------------------------
		���� - �����仯
*/
void W_PicturePartitioner::rowChanged(int value){
	this->refreshGridLine();
}
void W_PicturePartitioner::columnChanged(int value){
	this->refreshGridLine();
}
/*-----------------------------------
		���� - ˢ��������
*/
void W_PicturePartitioner::refreshGridLine(){

	int width = this->local_bitmap.width();
	int height = this->local_bitmap.height();
	int row = ui.spinBox_row->value();
	int column = ui.spinBox_column->value();
	int f_width = qFloor( width / column );
	int f_height = qFloor( height / row );

	this->m_p_SinglePictureViewer->setGridLine(column, row);	//��ˢ������ֻ����ҳ1��ˢ�£�

	if (f_width * column != width || f_height * row != height){
		ui.label_cutTip->setVisible(true);
	}else{
		ui.label_cutTip->setVisible(false);
	}
	QString text = QString::number(f_width) + "x" + QString::number(f_height) + "  (" + QString::number(row*column) + "��)";
	ui.label_framePic_wh->setText(text);

}


/*-------------------------------------------------
		���� - ��������1
*/
void W_PicturePartitioner::toFlow_1(){
	ui.stackedWidget->setCurrentIndex(0);
}
/*-------------------------------------------------
		���� - ��������2
*/
void W_PicturePartitioner::toFlow_2(){
	
	int row = ui.spinBox_row->value();
	int column = ui.spinBox_column->value();
	this->m_p_PPaViewer->setGridLineAndBlockMatrix(column, row);

	ui.stackedWidget->setCurrentIndex(1);
}

/*-------------------------------------------------
		���� - ��������
*/
void W_PicturePartitioner::setData(QFileInfo file) {
	QImage image = QImage(file.absoluteFilePath());
	QPixmap pixmap = QPixmap::fromImage(image);
	this->local_bitmap = pixmap;
	this->putDataToUi();
}
void W_PicturePartitioner::setData(QPixmap bitmap) {
	this->local_bitmap = bitmap;
	this->putDataToUi();
}
/*-----------------------------------
		���� - �������� -> ui����
*/
void W_PicturePartitioner::putDataToUi(){

	this->m_p_SinglePictureViewer->setSource(this->local_bitmap);
	this->m_p_PPaViewer->setSource(this->local_bitmap);
	ui.label_totalPic_wh->setText(QString::number(this->local_bitmap.width()) + "x" + QString::number(this->local_bitmap.height()));

	this->refreshGridLine();		//��ˢ������ҳ1������2����һ��ʱ��ˢ��
}
/*-----------------------------------
		���� - ui���� -> ��������
*/
void W_PicturePartitioner::putUiToData(){
	this->local_bitmapTank = this->m_p_PPaViewer->getCutBitmap();
}

/*-------------------------------------------------
		���� - ȡ������
*/
QList<QPixmap> W_PicturePartitioner::getData(){
	return this->local_bitmapTank;
};
/*-------------------------------------------------
		���� - �ύ���ݣ�У�飩
*/
void W_PicturePartitioner::acceptData(){
	this->putUiToData();

	this->accept();
};