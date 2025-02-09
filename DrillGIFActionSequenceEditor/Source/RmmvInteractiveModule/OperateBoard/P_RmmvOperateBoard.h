#ifndef P_OperateBoard_H
#define P_OperateBoard_H

#include <QtWidgets>
#include "ui_P_RmmvOperateBoard.h"

#include "Source/RmmvUtilsProjectModule/ProjectData/C_RmmvProjectData.h"

/*
-----==========================================================-----
		类：		操作面板.h
		作者：		drill_up
		所属模块：	交互模块
		功能：		该部分提供rmmv的交互按钮功能。
					（详细见cpp）
-----==========================================================-----
*/
class P_RmmvOperateBoard : public QWidget
{
	Q_OBJECT

	public:
		P_RmmvOperateBoard(QWidget *parent = 0);
		~P_RmmvOperateBoard();
		
	//-----------------------------------
	//----控件
	public:
										//控件 - 选择rmmv
		void selectRmmv();
										//控件 - 刷新rmmv操作可用
		void refreshRmmvEnable();
										//控件 - 打开rmmv编辑器
		void openRmmvSoftware();
										//控件 - 导入rmmv数据
		void importRmmv();
										//控件 - 保存到rmmv
		void saveToRmmv();
										//控件 - 运行rmmv
		void runRmmv();
		
										//控件 - 备份设置变化时
		void bakChanged();

	//-----------------------------------
	//----窗口
	public:
		C_RmmvProjectData local_rmmvData;
	public:
										//窗口 - 设置数据
		void setData(C_RmmvProjectData rmmvData);
										//窗口 - 取出数据
		C_RmmvProjectData getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();

	private:
		Ui::P_RmmvOperateBoard ui;

};

#endif // P_OperateBoard_H
