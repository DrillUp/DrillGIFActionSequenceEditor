#include "stdafx.h"
#include "p_PlayingPart.h"

#include "Source/Utils/manager/GIFManager/s_GIFManager.h"

/*
-----==========================================================-----
		类：		放映区.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		该部分提供动作元编辑功能。
		
		目标：		1.

		使用方法：
				>初始化

-----==========================================================-----
*/
P_PlayingPart::P_PlayingPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数

	//-----------------------------------
	//----初始化ui


	//-----------------------------------
	//----事件绑定
	connect(ui.toolButton, &QPushButton::clicked, this, &P_PlayingPart::startPlay);
	connect(ui.toolButton_2, &QPushButton::clicked, this, &P_PlayingPart::endPlay);

}

P_PlayingPart::~P_PlayingPart(){
}

/*-------------------------------------------------
		放映区 - 开始播放
*/
void P_PlayingPart::startPlay() {
	//S_GIFManager::getInstance()->dismantlingGIF_testApi();
	QList<QFileInfo> info_list = QList<QFileInfo>();
	info_list.push_back(QFileInfo("F:/新建文件夹/0.png"));
	info_list.push_back(QFileInfo("F:/新建文件夹/1.png"));
	info_list.push_back(QFileInfo("F:/新建文件夹/2.png"));
	info_list.push_back(QFileInfo("F:/新建文件夹/3.png"));
	info_list.push_back(QFileInfo("F:/新建文件夹/4.png"));
	info_list.push_back(QFileInfo("F:/新建文件夹/5.png"));
	info_list.push_back(QFileInfo("F:/新建文件夹/6.png"));
	info_list.push_back(QFileInfo("F:/新建文件夹/7.png"));
	info_list.push_back(QFileInfo("F:/新建文件夹/8.png"));
	info_list.push_back(QFileInfo("F:/新建文件夹/9.png"));
	info_list.push_back(QFileInfo("F:/新建文件夹/10.png"));
	info_list.push_back(QFileInfo("F:/新建文件夹/11.png"));
	info_list.push_back(QFileInfo("F:/新建文件夹/12.png"));
	info_list.push_back(QFileInfo("F:/新建文件夹/13.png"));
	info_list.push_back(QFileInfo("F:/新建文件夹/14.png"));
	S_GIFManager::getInstance()->generateGIF(info_list, QFileInfo("F:/aaa.gif"), 4);
}
/*-------------------------------------------------
		放映区 - 结束播放
*/
void P_PlayingPart::endPlay() {
	S_GIFManager::getInstance()->dismantlingGIF(QFileInfo("F:/gif箱/6HDFVPQ(DV~UCB)2L1D4[(R.gif"),QDir("F:/新建文件夹/"),"png");
}

/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_PlayingPart::setData(QJsonObject actionData) {
	this->local_actionData = actionData;
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
QJsonObject P_PlayingPart::getData(){
	this->putUiToData();

	// > 校验
	//...
	
	return this->local_actionData;
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_PlayingPart::putDataToUi() {
	

}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_PlayingPart::putUiToData() {
	

}
