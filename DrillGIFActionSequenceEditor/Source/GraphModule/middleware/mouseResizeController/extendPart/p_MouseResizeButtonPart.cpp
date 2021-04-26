#include "stdafx.h"
#include "p_MouseResizeButtonPart.h"

#include <QSpinBox>
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		������� ��ť��.cpp
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	ͼ��ģ��
		���ܣ�		�ṩ������ŵ� ��ť�ؼ��顣

		ʹ�÷�����
				> ��ʼ�� ��ť��
					this->m_p_MouseResizeButtonPart = new P_MouseResizeButtonPart();
					QVBoxLayout* latout = new QVBoxLayout(ui.widget);
					latout->setMargin(0);
					latout->addWidget(this->m_p_MouseResizeButtonPart);
					this->m_p_MouseResizeButtonPart->initWidget(this->m_p_SinglePictureViewer->getController());	//����Ҫ�������ſ�������

-----==========================================================-----
*/
P_MouseResizeButtonPart::P_MouseResizeButtonPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������
	this->m_p_MouseResizeController = nullptr;

	//-----------------------------------
	//----��ʼ��ui
	ui.toolButton_zoom_in->setIcon(QIcon(QRC_IconSrcPath + "/view/Zoom_In.png"));
	ui.toolButton_zoom_out->setIcon(QIcon(QRC_IconSrcPath + "/view/Zoom_Out.png"));
	ui.toolButton_zoom_regular->setIcon(QIcon(QRC_IconSrcPath + "/view/Zoom_Regular.png"));


}

P_MouseResizeButtonPart::~P_MouseResizeButtonPart(){
}


/*-------------------------------------------------
		������ - ��ʼ��
*/
void P_MouseResizeButtonPart::initWidget(P_MouseResizeController* controller){

	// > �¼���
	connect(ui.toolButton_zoom_in, &QPushButton::clicked, controller, &P_MouseResizeController::zoomIn);
	connect(ui.toolButton_zoom_out, &QPushButton::clicked, controller, &P_MouseResizeController::zoomOut);
	connect(ui.toolButton_zoom_regular, &QPushButton::clicked, controller, &P_MouseResizeController::zoomReset);
	connect(controller, &P_MouseResizeController::scaleChanged, this, &P_MouseResizeButtonPart::zoomValueChanged);
}
/*-------------------------------------------------
		������ - ��ʼ��
*/
void P_MouseResizeButtonPart::zoomValueChanged(double value){
	ui.label_zoomValue->setText( QString::number(value * 100)+"%" );
}
