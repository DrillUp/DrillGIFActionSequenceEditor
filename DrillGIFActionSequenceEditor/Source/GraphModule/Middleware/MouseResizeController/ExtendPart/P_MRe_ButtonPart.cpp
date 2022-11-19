#include "stdafx.h"
#include "P_MRe_ButtonPart.h"

#include <QSpinBox>
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		鼠标缩放 按钮块.cpp
		版本：		v1.00
		作者：		drill_up
		所属模块：	图形模块
		功能：		提供鼠标缩放的 按钮控件块。

		使用方法：
				> 初始化 按钮块
					this->m_p_MRe_ButtonPart = new P_MRe_ButtonPart();
					QVBoxLayout* latout = new QVBoxLayout(ui.widget);
					latout->setMargin(0);
					latout->addWidget(this->m_p_MRe_ButtonPart);
					this->m_p_MRe_ButtonPart->initWidget(this->m_p_SinglePictureViewer->getController());	//（需要关联缩放控制器）

-----==========================================================-----
*/
P_MRe_ButtonPart::P_MRe_ButtonPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->m_p_MouseResizeController = nullptr;

	//-----------------------------------
	//----初始化ui
	ui.toolButton_zoom_in->setIcon(QIcon(QRC_IconSrcPath + "/view/Zoom_In.png"));
	ui.toolButton_zoom_out->setIcon(QIcon(QRC_IconSrcPath + "/view/Zoom_Out.png"));
	ui.toolButton_zoom_regular->setIcon(QIcon(QRC_IconSrcPath + "/view/Zoom_Regular.png"));


}

P_MRe_ButtonPart::~P_MRe_ButtonPart(){
}


/*-------------------------------------------------
		控制器 - 初始化
*/
void P_MRe_ButtonPart::initWidget(P_MRe_Controller* controller){

	// > 事件绑定
	connect(ui.toolButton_zoom_in, &QPushButton::clicked, controller, &P_MRe_Controller::zoomIn);
	connect(ui.toolButton_zoom_out, &QPushButton::clicked, controller, &P_MRe_Controller::zoomOut);
	connect(ui.toolButton_zoom_regular, &QPushButton::clicked, controller, &P_MRe_Controller::zoomReset);
	connect(controller, &P_MRe_Controller::scaleChanged, this, &P_MRe_ButtonPart::zoomValueChanged);
}
/*-------------------------------------------------
		控制器 - 初始化
*/
void P_MRe_ButtonPart::zoomValueChanged(double value){
	ui.label_zoomValue->setText( QString::number(value * 100)+"%" );
}
