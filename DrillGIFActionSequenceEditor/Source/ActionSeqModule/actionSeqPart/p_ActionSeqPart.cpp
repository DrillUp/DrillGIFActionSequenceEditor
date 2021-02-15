#include "stdafx.h"
#include "p_ActionSeqPart.h"

#include "Source/PluginModule/lengthEditor/s_LEAnnotationReader.h"
#include "Source/Utils/manager/GIFManager/s_GIFManager.h"

#include "../actionSeqData/s_ActionSeqDataContainer.h"

/*
-----==========================================================-----
		类：		动作序列块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		该部分提供动作序列放映，功能，并嵌套状态元、动作元的块。
		
		目标：		1.

		使用方法：
				>初始化

-----==========================================================-----
*/
P_ActionSeqPart::P_ActionSeqPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数

	//-----------------------------------
	//----初始化ui
	this->m_actionPart = new P_ActionPart(parent);
	ui.verticalLayout_action->addWidget(this->m_actionPart);

	this->m_statePart = new P_StatePart(parent);
	ui.verticalLayout_state->addWidget(this->m_statePart);
	

	//-----------------------------------
	//----事件绑定
	connect(ui.toolButton, &QPushButton::clicked, this, &P_ActionSeqPart::startPlay);
	connect(ui.toolButton_2, &QPushButton::clicked, this, &P_ActionSeqPart::endPlay);

}

P_ActionSeqPart::~P_ActionSeqPart(){
}


/*-------------------------------------------------
		控件 - 开始播放
*/
void P_ActionSeqPart::startPlay() {
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
		控件 - 结束播放
*/
void P_ActionSeqPart::endPlay() {
	S_GIFManager::getInstance()->dismantlingGIF(QFileInfo("F:/gif箱/6HDFVPQ(DV~UCB)2L1D4[(R.gif"),QDir("F:/新建文件夹/"),"png");
}

/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_ActionSeqPart::setData(QJsonObject actionSeq) {
	this->local_actionSeq = actionSeq;
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
QJsonObject P_ActionSeqPart::getData(){
	this->putUiToData();

	// > 校验
	//...
	
	return this->local_actionSeq;
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_ActionSeqPart::putDataToUi() {
	
	// > 动作序列上限
	QFileInfo plugin_file = S_ActionSeqDataContainer::getInstance()->getActionSeqPluginFile();
	C_LEAnnotation* c_le = S_LEAnnotationReader::getInstance()->readPlugin(plugin_file);
	int actionSeq_len = c_le->getParamByKey("动作序列-%d").getRealLen();


	// > 载入动作序列数据
	qDebug() << this->local_actionSeq;

}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_ActionSeqPart::putUiToData() {
	

}
