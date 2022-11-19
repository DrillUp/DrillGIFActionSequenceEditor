#include "stdafx.h"
#include "P_PictureCombiner.h"

#include <QSpinBox>
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		图片合成器.cpp
		版本：		v1.00
		作者：		drill_up
		所属模块：	图形模块
		功能：		可以将多张图片以矩阵的形式合并到一起。
		
		子功能：	->控件
						->输入框
						->浏览文件
						->文件类型选择
					->控件约束
						->浏览文件只用于辅助 输入框 填写
						->输入框有值时，点浏览文件选到指定文件
						->输入框编辑，自动选择下拉框
						->下拉框编辑，自动填后缀
					->源文件约束
						->源类型变化 输出类型刷新（输出的全部子控件都会受到反应，但不会死循环）
						->源文件浏览填入后，自动填 输出文件

		使用方法：
				>初始化添加类型
					this->m_p_PictureCombiner = new P_PictureCombiner();
					QVBoxLayout* latout = new QVBoxLayout(ui.widget);
					latout->setMargin(0);
					latout->addWidget(this->m_p_PictureCombiner);

					this->m_p_PictureCombiner->addConvert("facet", "stl", "面网格", "三角面");
					this->m_p_PictureCombiner->addConvert("facet", "nas", "面网格", "网格文件");
					this->m_p_PictureCombiner->addConvert("stl", "facet", "三角面", "面网格");

-----==========================================================-----
*/
P_PictureCombiner::P_PictureCombiner(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->m_slotBlock = false;

	//-----------------------------------
	//----初始化ui

	// > 图片查看块-单图
	this->m_p_SinglePictureViewer = new P_SinglePictureViewer(ui.widget_preview);
	this->m_p_SinglePictureViewer->rebuildUI();

	// > 缩放按钮块
	this->m_p_MouseResizeButtonPart = new P_MRe_ButtonPart();
	QVBoxLayout* latout = new QVBoxLayout(ui.widget_zoom_btn);
	latout->setMargin(0);
	latout->addWidget(this->m_p_MouseResizeButtonPart);
	this->m_p_MouseResizeButtonPart->initWidget(this->m_p_SinglePictureViewer->getResizeController());

	ui.label_sizeTip->hide();

	//-----------------------------------
	//----事件绑定

	// > 行列
	connect(ui.spinBox_column, SIGNAL(valueChanged(int)), this, SLOT(columnChanged(int)));
	connect(ui.spinBox_row, SIGNAL(valueChanged(int)), this, SLOT(rowChanged(int)));
	connect(ui.checkBox_column, &QCheckBox::toggled, this, &P_PictureCombiner::columnToggled);
	connect(ui.checkBox_row, &QCheckBox::toggled, this, &P_PictureCombiner::rowToggled);
	connect(ui.comboBox, &QComboBox::currentTextChanged, this, &P_PictureCombiner::comboxChanged);

	// > 网格
	connect(ui.checkBox_gridLine, &QCheckBox::toggled, this, &P_PictureCombiner::gridLineToggled);

	// > 帧属性
	connect(ui.checkBox_frameHeightDefault, &QCheckBox::toggled, this, &P_PictureCombiner::frameHeightDefaultToggled);
	connect(ui.checkBox_frameWidthDefault, &QCheckBox::toggled, this, &P_PictureCombiner::frameWidthDefaultToggled);
	connect(ui.spinBox_frameHeight, SIGNAL(valueChanged(int)), this, SLOT(frameHeightChanged(int)));
	connect(ui.spinBox_frameWidth, SIGNAL(valueChanged(int)), this, SLOT(frameWidthChanged(int)));


	ui.checkBox_row->setChecked(true);
	ui.checkBox_frameHeightDefault->setChecked(true);
	ui.checkBox_frameWidthDefault->setChecked(true);
}

P_PictureCombiner::~P_PictureCombiner(){
}


/*-----------------------------------
		视图 - 网格勾选变化
*/
void P_PictureCombiner::gridLineToggled(bool checked){
	this->redrawBitmap();
}

/*-----------------------------------
		行列 - 输入框变化
*/
void P_PictureCombiner::rowChanged(int value){
	if (this->m_slotBlock == true){ return; }
	this->autoFillNum();
	this->redrawBitmap();
}
void P_PictureCombiner::columnChanged(int value){
	if (this->m_slotBlock == true){ return; }
	this->autoFillNum();
	this->redrawBitmap();
}

/*-----------------------------------
		行列 - 勾选变化（自适应）
*/
void P_PictureCombiner::rowToggled(bool checked){
	if (checked == true){
		ui.spinBox_row->setEnabled(false);

		// > 两个自动不能同时勾选
		if (ui.checkBox_column->isChecked()){
			ui.checkBox_column->setChecked(false);
		}
	}else{
		ui.spinBox_row->setEnabled(true);
	}
	this->autoFillNum();
	this->redrawBitmap();
}
void P_PictureCombiner::columnToggled(bool checked){
	if (checked == true){
		ui.spinBox_column->setEnabled(false);

		// > 两个自动不能同时勾选
		if (ui.checkBox_row->isChecked()){
			ui.checkBox_row->setChecked(false);
		}
	}else{
		ui.spinBox_column->setEnabled(true);
	}
	this->autoFillNum();
	this->redrawBitmap();
}
/*-----------------------------------
		行列 - 下拉框变化
*/
void P_PictureCombiner::comboxChanged(QString text){
	this->redrawBitmap();
}
/*-----------------------------------
		行列 - 计算行列
*/
void P_PictureCombiner::autoFillNum(){
	this->m_slotBlock = true;
	if (ui.checkBox_row->isChecked()){
		int col = ui.spinBox_column->value();
		int row = qCeil((double)this->local_bitmapTank.count() / col);
		ui.spinBox_row->setValue(row);
	}
	if (ui.checkBox_column->isChecked()){
		int row = ui.spinBox_row->value();
		int col = qCeil((double)this->local_bitmapTank.count() / row);
		ui.spinBox_column->setValue(col);
	}
	this->m_slotBlock = false;
}

/*-----------------------------------
		帧属性 - 输入框变化
*/
void P_PictureCombiner::frameWidthChanged(int value){
	this->redrawBitmap();
}
void P_PictureCombiner::frameHeightChanged(int value){
	this->redrawBitmap();
}
/*-----------------------------------
		帧属性 - 勾选变化（默认值）
*/
void P_PictureCombiner::frameWidthDefaultToggled(bool checked){
	ui.spinBox_frameWidth->setDisabled(checked);
	if (checked == true){
		ui.spinBox_frameWidth->setValue(this->getMaxWidth());
	}
}
void P_PictureCombiner::frameHeightDefaultToggled(bool checked){
	ui.spinBox_frameHeight->setDisabled(checked);
	if (checked == true){
		ui.spinBox_frameHeight->setValue(this->getMaxHeight());
	}
}


/*-----------------------------------
		绘制 - 重绘图片
*/
void P_PictureCombiner::redrawBitmap(){
	int row = ui.spinBox_row->value();
	int column = ui.spinBox_column->value();
	int ww = this->getMaxWidth();
	int hh = this->getMaxHeight();
	if (ui.checkBox_frameWidthDefault->isChecked() == false ){ ww = ui.spinBox_frameWidth->value(); }
	if (ui.checkBox_frameHeightDefault->isChecked() == false){ hh = ui.spinBox_frameHeight->value(); }

	// > 生成的图片
	QPixmap bitmap = this->getDrawedBitmap(ww,hh);
	this->m_p_SinglePictureViewer->setSource(bitmap);

	// > 辅助线
	if (ui.checkBox_gridLine->isChecked()){
		this->m_p_SinglePictureViewer->setGridLine(column, row);
	}else{
		this->m_p_SinglePictureViewer->clearGridLine();
	}
}
/*-----------------------------------
		绘制 - 获取绘制的图片
*/
QPixmap P_PictureCombiner::getDrawedBitmap(int p_width, int p_height){

	// > 高宽更新
	int ww = p_width;
	int hh = p_height;
	int row = ui.spinBox_row->value();
	int column = ui.spinBox_column->value();
	int total_width = column * ww;
	int total_height = row * hh;
	ui.label_totalPic_wh->setText(QString::number(total_width) + "x" + QString::number(total_height));

	// > 开始绘制
	QPixmap bitmap = QPixmap(total_width, total_height);
	bitmap.setMask(QBitmap(bitmap));				//打开alpha层
	bitmap.fill(Qt::transparent);					//透明背景

	// > 绘制图片
	QPainter painter(&bitmap);						//画家
	if (ui.comboBox->currentText() == "从上往下"){
		for (int i = 0; i < column; i++){
			for (int j = 0; j < row; j++){
				int index = j + i*row;
				int x = i * ww;
				int y = j * hh;
				if (index < this->local_bitmapTank.count()){
					QPixmap temp_bitmap = this->local_bitmapTank.at(index);
					this->drawBitmap_CenterAndCut(&painter, temp_bitmap, QRect(x, y, ww, hh));
				}
			}
		}
	}
	else{
		for (int j = 0; j < row; j++){
			for (int i = 0; i < column; i++){
				int index = i + j*column;
				int x = i * ww;
				int y = j * hh;
				if (index < this->local_bitmapTank.count()){
					QPixmap temp_bitmap = this->local_bitmapTank.at(index);
					this->drawBitmap_CenterAndCut(&painter, temp_bitmap, QRect(x,y,ww,hh) );
				}
			}
		}
	}
	painter.end();

	return bitmap;
}
/*-------------------------------------------------
		绘制 - 在指定的范围内绘制图像，自动居中，自动剪切
*/
void P_PictureCombiner::drawBitmap_CenterAndCut(QPainter* painter, QPixmap bitmap, QRect frame){
	int xx = frame.x();
	int yy = frame.y();
	int ww = frame.width();
	int hh = frame.height();
	int bx = 0;
	int by = 0;
	int bw = bitmap.width();
	int bh = bitmap.height();

	int wwww = bw;		//（真实切割高宽）
	int hhhh = bh;
	
	if (bw > ww){		//（图像宽 比 设置宽 大时，切割）
		bx = ((bw - ww) >> 1);
		wwww = ww;
	}else{				//（设置宽 比 图像宽 小时，加空白）
		xx = xx + ((ww - bw) >> 1);
	}
	if (bh > hh){
		by = ((bh - hh) >> 1);
		hhhh = hh;
	}
	else{
		yy = yy + ((hh - bh) >> 1);
	}

	QRect r1 = QRect(xx, yy, wwww, hhhh);		//（等高宽切割）
	QRect r2 = QRect(bx, by, wwww, hhhh);
	painter->drawPixmap(r1, bitmap, r2);
}
/*-------------------------------------------------
		绘制 - 获取最大宽度
*/
int P_PictureCombiner::getMaxWidth(){
	if (this->local_bitmapTank.count() == 0){ return 150; }	//（默认）
	int result = 0;
	for (int i = 0; i < this->local_bitmapTank.count(); i++){
		int ww = this->local_bitmapTank.at(i).width();
		if (result < ww){
			result = ww;
		}
	}
	return result;
}
/*-------------------------------------------------
		绘制 - 获取最大高度
*/
int P_PictureCombiner::getMaxHeight(){
	if (this->local_bitmapTank.count() == 0){ return 100; }	//（默认）
	int result = 0;
	for (int i = 0; i < this->local_bitmapTank.count(); i++){
		int hh = this->local_bitmapTank.at(i).height();
		if (result < hh){
			result = hh;
		}
	}
	return result;
}
/*-----------------------------------
		绘制 - 判断高宽一致
*/
bool P_PictureCombiner::isSizeFit(){
	if (this->local_bitmapTank.count() == 0){ return true; }
	if (this->local_bitmapTank.count() == 1){ return true; }

	int ww = this->local_bitmapTank.at(0).width();
	int hh = this->local_bitmapTank.at(0).height();
	for (int i = 1; i < this->local_bitmapTank.count(); i++){
		QPixmap pixmap = this->local_bitmapTank.at(i);
		if (ww != pixmap.width()){ return false; }
		if (hh != pixmap.height()){ return false; }
	}
	return true;
}



/*-----------------------------------
		块 - 设置数据
*/
void P_PictureCombiner::setData(QList<QFileInfo> file_tank){
	this->local_bitmapTank = QList<QPixmap>();
	for (int i = 0; i < file_tank.count(); i++){
		QFileInfo info = file_tank.at(i);
		QImage image = QImage(info.absoluteFilePath());
		QPixmap pixmap = QPixmap::fromImage(image);
		this->local_bitmapTank.append(pixmap);
	}
	this->putDataToUi();
}
void P_PictureCombiner::setData(QList<QPixmap> bitmap_tank){
	this->local_bitmapTank = bitmap_tank;
	this->putDataToUi();
}
/*-----------------------------------
		块 - 取出数据
*/
QPixmap P_PictureCombiner::getData(){
	this->putUiToData();
	return this->local_draw;
}
/*-----------------------------------
		块 - 本地数据 -> ui数据
*/
void P_PictureCombiner::putDataToUi(){
	ui.label_sizeTip->setVisible( !this->isSizeFit() );
	ui.spinBox_frameWidth->setValue( this->getMaxWidth());
	ui.spinBox_frameHeight->setValue( this->getMaxHeight());
	this->autoFillNum();
	this->redrawBitmap();
}
/*-----------------------------------
		块 - ui数据 -> 本地数据
*/
void P_PictureCombiner::putUiToData(){
	int ww = this->getMaxWidth();
	int hh = this->getMaxHeight();
	if (ui.checkBox_frameWidthDefault->isChecked() == false){ ww = ui.spinBox_frameWidth->value(); }
	if (ui.checkBox_frameHeightDefault->isChecked() == false){ hh = ui.spinBox_frameHeight->value(); }
	this->local_draw = this->getDrawedBitmap(ww,hh);
}