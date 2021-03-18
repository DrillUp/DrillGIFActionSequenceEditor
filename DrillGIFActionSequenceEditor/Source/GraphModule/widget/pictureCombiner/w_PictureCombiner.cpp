#include "stdafx.h"
#include "w_PictureCombiner.h"

/*
-----==========================================================-----
		类：		图片合成器 窗口.cpp
		所属模块：	图形模块
		功能：		可以将多张图片以矩阵的形式合并到一起。
-----==========================================================-----
*/

W_PictureCombiner::W_PictureCombiner(QWidget* parent)
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
	//----控件初始化
	this->m_P_PictureCombiner = new P_PictureCombiner();
	QVBoxLayout* l = new QVBoxLayout(ui.widget);
	ui.widget->setLayout(l);
	l->setMargin(0);
	l->addWidget(this->m_P_PictureCombiner);

	//-----------------------------------
	//----事件绑定
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(acceptData()));

	//-----------------------------------
	//----ui初始化
	ui.buttonBox->button(QDialogButtonBox::Ok)->setText( ("确定"));
	ui.buttonBox->button(QDialogButtonBox::Cancel)->setText( ("取消"));
}
W_PictureCombiner::~W_PictureCombiner(){
}


/*-------------------------------------------------
		窗口 - 设置数据
*/
void W_PictureCombiner::setData(QList<QFileInfo> file_list) {
	this->m_P_PictureCombiner->setData(file_list);
}
void W_PictureCombiner::setData(QList<QPixmap> bitmap_list) {
	this->m_P_PictureCombiner->setData(bitmap_list);
}

/*-------------------------------------------------
		窗口 - 取出数据
*/
QPixmap W_PictureCombiner::getData(){
	return this->m_P_PictureCombiner->getData();
};
/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_PictureCombiner::acceptData(){
	this->accept();
};