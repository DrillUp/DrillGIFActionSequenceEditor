#include "stdafx.h"
#include "p_RmmvOperateBoard.h"

#include "../custom/s_RmmvCaller_ActionSeq.h"

/*
-----==========================================================-----
		类：		操作面板.cpp
		版本：		v1.01
		所属模块：	交互模块
		功能：		该部分提供rmmv的交互按钮功能。
		
		目标：		> 打开rmmv按钮
					> rmmv交互显示的数据
					> 保存rmmv按钮
					> 运行rmmv按钮

		使用方法：
				>初始化

-----==========================================================-----
*/
P_RmmvOperateBoard::P_RmmvOperateBoard(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数

	//-----------------------------------
	//----初始化ui


	//-----------------------------------
	//----事件绑定
	connect(ui.toolButton_open, &QPushButton::clicked, this, &P_RmmvOperateBoard::openRmmv);
	connect(ui.toolButton_import, &QPushButton::clicked, this, &P_RmmvOperateBoard::importRmmv);
	connect(ui.toolButton_save, &QPushButton::clicked, this, &P_RmmvOperateBoard::saveToRmmv);
	connect(ui.toolButton_run, &QPushButton::clicked, this, &P_RmmvOperateBoard::runRmmv);

}

P_RmmvOperateBoard::~P_RmmvOperateBoard(){
}



/*-------------------------------------------------
		控件 - 打开rmmv
*/
void P_RmmvOperateBoard::openRmmv(){
	this->local_rmmvData = S_RmmvCaller_ActionSeq::getInstance()->callRmmvOpen();
	ui.lineEdit_path->setText(this->local_rmmvData.path);
	ui.lineEdit_name->setText(this->local_rmmvData.name);
}
/*-------------------------------------------------
		控件 - 导入rmmv数据
*/
void P_RmmvOperateBoard::importRmmv(){
	if (this->local_rmmvData.isNull()){
		QMessageBox::warning(this, "提示", "你需要先打开rmmv工程。", QMessageBox::Yes);
		return;
	}
	if (QMessageBox::information(this, "提示", "此操作会从rmmv工程中读取数据，并覆盖到当前数据，当前的动作序列数据将全部丢失，是否继续?", "继续", "取消", 0, 1) == 1){
		return;
	}

	S_RmmvCaller_ActionSeq::getInstance()->loadAllRmmvFile(this->local_rmmvData);
	S_RmmvCaller_ActionSeq::getInstance()->coverSourceFileToTemp(this->local_rmmvData);
}
/*-------------------------------------------------
		控件 - 保存到rmmv
*/
void P_RmmvOperateBoard::saveToRmmv(){
	if (this->local_rmmvData.isNull()){
		QMessageBox::warning(this, "提示", "你需要先打开rmmv工程。", QMessageBox::Yes);
		return;
	}

	// > 要求关闭编辑器才能存
	S_RmmvCaller_ActionSeq::getInstance()->callRmmvSave_RequestingClose(this->local_rmmvData);

}
/*-------------------------------------------------
		控件 - 运行rmmv
*/
void P_RmmvOperateBoard::runRmmv(){
	if (this->local_rmmvData.isNull()){
		QMessageBox::warning(this, "提示", "你需要先打开rmmv工程。", QMessageBox::Yes);
		return;
	}

	S_RmmvCaller_ActionSeq::getInstance()->callRmmvRun(this->local_rmmvData);

}


/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_RmmvOperateBoard::setData(C_RmmvProjectData actionData) {
	this->local_rmmvData = actionData;
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
C_RmmvProjectData P_RmmvOperateBoard::getData(){
	this->putUiToData();
	return this->local_rmmvData;
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_RmmvOperateBoard::putDataToUi() {

	ui.lineEdit_path->setText(this->local_rmmvData.path);
	ui.lineEdit_name->setText(this->local_rmmvData.name);
}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_RmmvOperateBoard::putUiToData() {
	
	//（无）
}
