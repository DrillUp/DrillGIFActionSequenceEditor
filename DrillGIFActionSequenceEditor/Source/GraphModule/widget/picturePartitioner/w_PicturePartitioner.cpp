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
	this->m_p_SinglePictureViewer = new P_SinglePictureViewer(ui.widget_preview);
	this->m_p_SinglePictureViewer->rebuildUI();

	this->m_p_PPaViewer = new P_PPaViewer(ui.widget_selectArea);
	this->m_p_PPaViewer->rebuildUI();

	//-----------------------------------
	//----�¼���

	// > ����
	connect(ui.spinBox_column, SIGNAL(valueChanged(int)), this, SLOT(columnChanged(int)));
	connect(ui.spinBox_row, SIGNAL(valueChanged(int)), this, SLOT(rowChanged(int)));

	// > ��ͼ - ͼƬ�鿴�飨����1�� - ����
	connect(ui.toolButton_zoom_in, &QPushButton::clicked, this->m_p_SinglePictureViewer, &P_SinglePictureViewer::zoomIn);
	connect(ui.toolButton_zoom_out, &QPushButton::clicked, this->m_p_SinglePictureViewer, &P_SinglePictureViewer::zoomOut);
	connect(ui.toolButton_zoom_regular, &QPushButton::clicked, this->m_p_SinglePictureViewer, &P_SinglePictureViewer::zoomReset);
	connect(this->m_p_SinglePictureViewer, &P_SinglePictureViewer::scaleChanged, this, &W_PicturePartitioner::zoomValueChanged_1);

	// > ��ͼ - ͼƬ�鿴�飨����2�� - ����
	connect(ui.toolButton_zoom_in_2, &QPushButton::clicked, this->m_p_PPaViewer, &P_PPaViewer::zoomIn);
	connect(ui.toolButton_zoom_out_2, &QPushButton::clicked, this->m_p_PPaViewer, &P_PPaViewer::zoomOut);
	connect(ui.toolButton_zoom_regular_2, &QPushButton::clicked, this->m_p_PPaViewer, &P_PPaViewer::zoomReset);
	connect(this->m_p_PPaViewer, &P_PPaViewer::scaleChanged, this, &W_PicturePartitioner::zoomValueChanged_2);

	// > ����
	connect(ui.pushButton_next, &QPushButton::clicked, this, &W_PicturePartitioner::toFlow_2);
	connect(ui.pushButton_last, &QPushButton::clicked, this, &W_PicturePartitioner::toFlow_1);
	connect(ui.pushButton_finish, &QPushButton::clicked, this, &W_PicturePartitioner::acceptData);

	//-----------------------------------
	//----��ʼ��ui
	ui.toolButton_zoom_in->setIcon(QIcon(QRC_IconSrcPath + "/view/Zoom_In.png"));
	ui.toolButton_zoom_out->setIcon(QIcon(QRC_IconSrcPath + "/view/Zoom_Out.png"));
	ui.toolButton_zoom_regular->setIcon(QIcon(QRC_IconSrcPath + "/view/Zoom_Regular.png"));
	ui.toolButton_zoom_in_2->setIcon(QIcon(QRC_IconSrcPath + "/view/Zoom_In.png"));
	ui.toolButton_zoom_out_2->setIcon(QIcon(QRC_IconSrcPath + "/view/Zoom_Out.png"));
	ui.toolButton_zoom_regular_2->setIcon(QIcon(QRC_IconSrcPath + "/view/Zoom_Regular.png"));

	TTool::_chinese_(ui.buttonBox);

}
W_PicturePartitioner::~W_PicturePartitioner(){
}

/*-------------------------------------------------
		��ͼ - ���ű����л�
*/
void W_PicturePartitioner::zoomValueChanged_1(double value){
	ui.label_zoomValue->setText( QString::number(value * 100)+"%" );
}
void W_PicturePartitioner::zoomValueChanged_2(double value){
	ui.label_zoomValue_2->setText( QString::number(value * 100)+"%" );
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