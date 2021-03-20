#include "stdafx.h"
#include "p_PictureCombiner.h"

#include <QSpinBox>
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ�ϳ���.cpp
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	ͼ��ģ��
		���ܣ�		���Խ�����ͼƬ�Ծ������ʽ�ϲ���һ��
		
		�ӹ��ܣ�	->�ؼ�
						->�����
						->����ļ�
						->�ļ�����ѡ��
					->�ؼ�Լ��
						->����ļ�ֻ���ڸ��� ����� ��д
						->�������ֵʱ��������ļ�ѡ��ָ���ļ�
						->�����༭���Զ�ѡ��������
						->������༭���Զ����׺
					->Դ�ļ�Լ��
						->Դ���ͱ仯 �������ˢ�£������ȫ���ӿؼ������ܵ���Ӧ����������ѭ����
						->Դ�ļ����������Զ��� ����ļ�

		ʹ�÷�����
				>��ʼ���������
					this->m_p_PictureCombiner = new P_PictureCombiner();
					QVBoxLayout* latout = new QVBoxLayout(ui.widget);
					latout->setMargin(0);
					latout->addWidget(this->m_p_PictureCombiner);

					this->m_p_PictureCombiner->addConvert("facet", "stl", "������", "������");
					this->m_p_PictureCombiner->addConvert("facet", "nas", "������", "�����ļ�");
					this->m_p_PictureCombiner->addConvert("stl", "facet", "������", "������");

-----==========================================================-----
*/
P_PictureCombiner::P_PictureCombiner(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������
	this->m_slotBlock = false;

	//-----------------------------------
	//----��ʼ��ui
	this->m_p_SinglePictureViewer = new P_SinglePictureViewer(ui.widget_preview);
	this->m_p_SinglePictureViewer->rebuildUI();

	ui.label_sizeTip->hide();
	ui.toolButton_zoom_in->setIcon(QIcon(QRC_IconSrcPath + "/view/Zoom_In.png"));
	ui.toolButton_zoom_out->setIcon(QIcon(QRC_IconSrcPath + "/view/Zoom_Out.png"));
	ui.toolButton_zoom_regular->setIcon(QIcon(QRC_IconSrcPath + "/view/Zoom_Regular.png"));

	//-----------------------------------
	//----�¼���

	// > ����
	connect(ui.spinBox_column, SIGNAL(valueChanged(int)), this, SLOT(columnChanged(int)));
	connect(ui.spinBox_row, SIGNAL(valueChanged(int)), this, SLOT(rowChanged(int)));
	connect(ui.checkBox_column, &QCheckBox::toggled, this, &P_PictureCombiner::columnToggled);
	connect(ui.checkBox_row, &QCheckBox::toggled, this, &P_PictureCombiner::rowToggled);
	connect(ui.comboBox, &QComboBox::currentTextChanged, this, &P_PictureCombiner::comboxChanged);

	// > ��ͼ - ͼƬ�鿴�� - ����
	connect(ui.toolButton_zoom_in, &QPushButton::clicked, this->m_p_SinglePictureViewer, &P_SinglePictureViewer::zoomIn);
	connect(ui.toolButton_zoom_out, &QPushButton::clicked, this->m_p_SinglePictureViewer, &P_SinglePictureViewer::zoomOut);
	connect(ui.toolButton_zoom_regular, &QPushButton::clicked, this->m_p_SinglePictureViewer, &P_SinglePictureViewer::zoomReset);
	connect(this->m_p_SinglePictureViewer, &P_SinglePictureViewer::scaleChanged, this, &P_PictureCombiner::zoomValueChanged);
	connect(ui.checkBox_gridLine, &QCheckBox::toggled, this, &P_PictureCombiner::gridLineToggled);

	// > ֡����
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


/*-------------------------------------------------
		��ͼ - ���ű����л�
*/
void P_PictureCombiner::zoomValueChanged(double value){
	ui.label_zoomValue->setText( QString::number(value * 100)+"%" );
}
/*-----------------------------------
		��ͼ - ����ѡ�仯
*/
void P_PictureCombiner::gridLineToggled(bool checked){
	this->redrawBitmap();
}

/*-----------------------------------
		���� - �����仯
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
		���� - ��ѡ�仯������Ӧ��
*/
void P_PictureCombiner::rowToggled(bool checked){
	if (checked == true){
		ui.spinBox_row->setEnabled(false);

		// > �����Զ�����ͬʱ��ѡ
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

		// > �����Զ�����ͬʱ��ѡ
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
		���� - ������仯
*/
void P_PictureCombiner::comboxChanged(QString text){
	this->redrawBitmap();
}
/*-----------------------------------
		���� - ��������
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
		֡���� - �����仯
*/
void P_PictureCombiner::frameWidthChanged(int value){
	this->redrawBitmap();
}
void P_PictureCombiner::frameHeightChanged(int value){
	this->redrawBitmap();
}
/*-----------------------------------
		֡���� - ��ѡ�仯��Ĭ��ֵ��
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
		���� - �ػ�ͼƬ
*/
void P_PictureCombiner::redrawBitmap(){
	int row = ui.spinBox_row->value();
	int column = ui.spinBox_column->value();
	int ww = this->getMaxWidth();
	int hh = this->getMaxHeight();
	if (ui.checkBox_frameWidthDefault->isChecked() == false ){ ww = ui.spinBox_frameWidth->value(); }
	if (ui.checkBox_frameHeightDefault->isChecked() == false){ hh = ui.spinBox_frameHeight->value(); }

	// > ���ɵ�ͼƬ
	QPixmap bitmap = this->getDrawedBitmap(ww,hh);
	this->m_p_SinglePictureViewer->setSource(bitmap);

	// > ������
	if (ui.checkBox_gridLine->isChecked()){
		this->m_p_SinglePictureViewer->setGridLine(column, row);
	}else{
		this->m_p_SinglePictureViewer->clearGridLine();
	}
}
/*-----------------------------------
		���� - ��ȡ���Ƶ�ͼƬ
*/
QPixmap P_PictureCombiner::getDrawedBitmap(int p_width, int p_height){

	// > �߿����
	int ww = p_width;
	int hh = p_height;
	int row = ui.spinBox_row->value();
	int column = ui.spinBox_column->value();
	int total_width = column * ww;
	int total_height = row * hh;
	ui.label_totalPic_wh->setText(QString::number(total_width) + "x" + QString::number(total_height));

	// > ��ʼ����
	QPixmap bitmap = QPixmap(total_width, total_height);
	bitmap.setMask(QBitmap(bitmap));				//��alpha��
	bitmap.fill(Qt::transparent);					//͸������

	// > ����ͼƬ
	QPainter painter(&bitmap);						//����
	if (ui.comboBox->currentText() == "��������"){
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
		���� - ��ָ���ķ�Χ�ڻ���ͼ���Զ����У��Զ�����
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

	int wwww = bw;		//����ʵ�и�߿�
	int hhhh = bh;
	
	if (bw > ww){		//��ͼ��� �� ���ÿ� ��ʱ���и
		bx = ((bw - ww) >> 1);
		wwww = ww;
	}else{				//�����ÿ� �� ͼ��� Сʱ���ӿհף�
		xx = xx + ((ww - bw) >> 1);
	}
	if (bh > hh){
		by = ((bh - hh) >> 1);
		hhhh = hh;
	}
	else{
		yy = yy + ((hh - bh) >> 1);
	}

	QRect r1 = QRect(xx, yy, wwww, hhhh);		//���ȸ߿��и
	QRect r2 = QRect(bx, by, wwww, hhhh);
	painter->drawPixmap(r1, bitmap, r2);
}
/*-------------------------------------------------
		���� - ��ȡ�����
*/
int P_PictureCombiner::getMaxWidth(){
	if (this->local_bitmapTank.count() == 0){ return 150; }	//��Ĭ�ϣ�
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
		���� - ��ȡ���߶�
*/
int P_PictureCombiner::getMaxHeight(){
	if (this->local_bitmapTank.count() == 0){ return 100; }	//��Ĭ�ϣ�
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
		���� - �жϸ߿�һ��
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
		�� - ��������
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
		�� - ȡ������
*/
QPixmap P_PictureCombiner::getData(){
	this->putUiToData();
	return this->local_draw;
}
/*-----------------------------------
		�� - �������� -> ui����
*/
void P_PictureCombiner::putDataToUi(){
	ui.label_sizeTip->setVisible( !this->isSizeFit() );
	ui.spinBox_frameWidth->setValue( this->getMaxWidth());
	ui.spinBox_frameHeight->setValue( this->getMaxHeight());
	this->autoFillNum();
	this->redrawBitmap();
}
/*-----------------------------------
		�� - ui���� -> ��������
*/
void P_PictureCombiner::putUiToData(){
	int ww = this->getMaxWidth();
	int hh = this->getMaxHeight();
	if (ui.checkBox_frameWidthDefault->isChecked() == false){ ww = ui.spinBox_frameWidth->value(); }
	if (ui.checkBox_frameHeightDefault->isChecked() == false){ hh = ui.spinBox_frameHeight->value(); }
	this->local_draw = this->getDrawedBitmap(ww,hh);
}