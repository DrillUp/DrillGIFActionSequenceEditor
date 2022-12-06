#ifndef W_PROJECT_CREATE_H
#define W_PROJECT_CREATE_H
#include "stdafx.h"

#include "ui_W_ProjectCreate.h"
#include "P_ProjectCreate.h"

/*
-----==========================================================-----
		类：		新建项目窗口.cpp
		所属模块：	项目管理模块
		功能：		经过该流程后，将新建项目文件夹。
					比如新建项目、另存为项目。
-----==========================================================-----
*/
class W_ProjectCreate : public QDialog
{
	Q_OBJECT

	public:
		W_ProjectCreate(QWidget* parent = 0);
		~W_ProjectCreate();
		
	//-----------------------------------
	//----控件
	public:
		P_ProjectCreate* m_P_ProjectCreate;
				
	//-----------------------------------
	//----位置
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
										//窗口 - 设置数据
		void setDataInModifyMode(C_ProjectData p);
										//窗口 - 取出数据
		C_ProjectData getData();
	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_ProjectCreate ui;
};

#endif 