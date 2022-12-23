#include "stdafx.h"
#include "W_PicturePartitioner.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		图片分割器 窗口.cpp
		所属模块：	图形模块
		功能：		可以将图片分割成多个图片。
-----==========================================================-----
*/

W_PicturePartitioner::W_PicturePartitioner(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	//-----------------------------------
	//----最大化/最小化
	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowCloseButtonHint;
	flags |= Qt::WindowMaximizeButtonHint;
	flags |= Qt::WindowMinimizeButtonHint;
	this->setWindowFlags(flags);

	//-----------------------------------
	//----初始化参数
	this->m_slotBlock = false;

	//-----------------------------------
	//----控件初始化

	// > 切割预览图
	this->m_p_SinglePictureViewer = new P_SinglePictureViewer(ui.widget_preview);
	this->m_p_SinglePictureViewer->rebuildUI();

	// > 帧选择图
	this->m_p_PPaViewer = new P_PPaViewer(ui.widget_selectArea);
	this->m_p_PPaViewer->rebuildUI();

	// > 缩放按钮块1
	this->m_p_MouseResizeButtonPart1 = new P_MRe_ButtonPart();
	QVBoxLayout* latout = new QVBoxLayout(ui.widget_zoom_btn);
	latout->setMargin(0);
	latout->addWidget(this->m_p_MouseResizeButtonPart1);
	this->m_p_MouseResizeButtonPart1->initWidget(this->m_p_SinglePictureViewer->getResizeController());

	// > 缩放按钮块2
	this->m_p_MouseResizeButtonPart2 = new P_MRe_ButtonPart();
	QVBoxLayout* latout2 = new QVBoxLayout(ui.widget_zoom_btn_2);
	latout2->setMargin(0);
	latout2->addWidget(this->m_p_MouseResizeButtonPart2);
	this->m_p_MouseResizeButtonPart2->initWidget(this->m_p_PPaViewer->getResizeController());

	//-----------------------------------
	//----事件绑定

	// > 行列
	connect(ui.spinBox_column, SIGNAL(valueChanged(int)), this, SLOT(columnChanged(int)));
	connect(ui.spinBox_row, SIGNAL(valueChanged(int)), this, SLOT(rowChanged(int)));

	// > 流程
	connect(ui.pushButton_next, &QPushButton::clicked, this, &W_PicturePartitioner::toFlow_2);
	connect(ui.pushButton_last, &QPushButton::clicked, this, &W_PicturePartitioner::toFlow_1);
	connect(ui.pushButton_finish, &QPushButton::clicked, this, &W_PicturePartitioner::acceptData);

	//-----------------------------------
	//----初始化ui
	TTool::_chinese_(ui.buttonBox);

}
W_PicturePartitioner::~W_PicturePartitioner(){
}

/*-----------------------------------
		行列 - 输入框变化
*/
void W_PicturePartitioner::rowChanged(int value){
	this->refreshGridLine();
}
void W_PicturePartitioner::columnChanged(int value){
	this->refreshGridLine();
}
/*-----------------------------------
		行列 - 刷新网格线
*/
void W_PicturePartitioner::refreshGridLine(){

	int width = this->local_bitmap.width();
	int height = this->local_bitmap.height();
	int row = ui.spinBox_row->value();
	int column = ui.spinBox_column->value();
	int f_width = qFloor( width / column );
	int f_height = qFloor( height / row );

	this->m_p_SinglePictureViewer->setGridLine(column, row);	//（刷新网格，只流程页1中刷新）
	
	if (f_width * column != width || f_height * row != height){
		ui.label_cutTip->setText("◆ 由于值不能被整除，所以切割的帧会丢失一些边缘像素。");
	}else{
		ui.label_cutTip->setText("◆ 值能被整除。");
	}
	QString text = QString::number(f_width) + "x" + QString::number(f_height) + "  (" + QString::number(row*column) + "个)";
	ui.label_framePic_wh->setText(text);

}


/*-------------------------------------------------
		流程 - 进入流程1
*/
void W_PicturePartitioner::toFlow_1(){
	ui.stackedWidget->setCurrentIndex(0);
}
/*-------------------------------------------------
		流程 - 进入流程2
*/
void W_PicturePartitioner::toFlow_2(){
	
	int row = ui.spinBox_row->value();
	int column = ui.spinBox_column->value();
	this->m_p_PPaViewer->setGridLineAndBlockMatrix(column, row);

	ui.stackedWidget->setCurrentIndex(1);
}

/*-------------------------------------------------
		窗口 - 设置数据
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
		窗口 - 本地数据 -> ui数据
*/
void W_PicturePartitioner::putDataToUi(){

	this->m_p_SinglePictureViewer->setSource(this->local_bitmap);
	this->m_p_PPaViewer->setSource(this->local_bitmap);
	ui.label_totalPic_wh->setText(QString::number(this->local_bitmap.width()) + "x" + QString::number(this->local_bitmap.height()));

	this->refreshGridLine();		//（刷新流程页1，流程2在下一步时才刷）
}
/*-----------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_PicturePartitioner::putUiToData(){
	this->local_bitmapTank = this->m_p_PPaViewer->getCutBitmap();
}

/*-------------------------------------------------
		窗口 - 取出数据
*/
QList<QPixmap> W_PicturePartitioner::getData(){
	return this->local_bitmapTank;
};
/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_PicturePartitioner::acceptData(){
	this->putUiToData();

	this->accept();
};