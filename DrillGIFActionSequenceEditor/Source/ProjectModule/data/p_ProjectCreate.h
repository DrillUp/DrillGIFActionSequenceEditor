#ifndef P_PROJECT_CREATE_H
#define P_PROJECT_CREATE_H
#include "stdafx.h"

#include "ui_P_ProjectCreate.h"
#include "C_ProjectData.h"

/*
-----==========================================================-----
		类：		新建项目块.h
		所属模块：	项目管理模块
		功能：		经过该流程后，将新建项目文件夹。
					比如新建项目、另存为项目。
					（详细见cpp）
-----==========================================================-----
*/
class P_ProjectCreate : public QWidget
{
	Q_OBJECT

	public:
		P_ProjectCreate(QWidget* parent = 0);
		~P_ProjectCreate();
		
	//-----------------------------------
	//----操作
	public slots:
										//操作 - 名称修改
		void nameChanged(QString name);
										//操作 - 位置修改
		void pathChanged(QString path);
										//操作 - 选择文件夹
		void chooseFiles();		
										//操作 - 建立文件夹修改
		void radioChanged(bool toggled);
										//操作 - 刷新完整路径
		void refreshOutput();
		
	//-----------------------------------
	//----位置（这里将ui封装了一下，防止路径混乱调用）
	public:
		QStringList m_pathList;
	public:
										//位置 - 添加位置
		void insertPath(QString path);
										//位置 - 选择位置
		void selectPath(QString path);
										//位置 - 获取选择的位置
		QString getCurrentPath();

	//-----------------------------------
	//----窗口
	public:
		C_ProjectData local_ProjectData;
	public:
										//窗口 - 设置数据
		void setDataInModifyMode(C_ProjectData p);
										//窗口 - 取出数据
		C_ProjectData getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();
	private:
		Ui::P_ProjectCreate ui;
};

#endif 