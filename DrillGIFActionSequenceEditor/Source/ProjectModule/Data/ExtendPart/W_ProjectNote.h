#ifndef W_ProjectNote_H
#define W_ProjectNote_H
#include "stdafx.h"

#include "ui_W_ProjectNote.h"

/*
-----==========================================================-----
		类：		项目备注 编辑窗口.h
		作者：		drill_up
		所属模块：	项目管理模块
		功能：		项目备注文本的编辑窗口。
-----==========================================================-----
*/
class W_ProjectNote : public QDialog
{
	Q_OBJECT

	public:
		W_ProjectNote(QWidget *parent = 0);
		~W_ProjectNote();
		
	//-----------------------------------
	//----控件
	public slots:
										//控件 - 刷新标签
		void refreshLabel();
		
	//-----------------------------------
	//----窗口
	public:
		//bool isAddMode;
		QString local_ProjectAuthor;
		QString local_ProjectNote;
	public:
										//窗口 - 设置数据
		void setData(QString author, QString note);
										//窗口 - 取出数据
		QString getData_note();
		QString getData_author();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();
										//窗口 - 按钮设置
		void clickedBtn(QAbstractButton * btn);
	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	
	private:
		Ui::W_ProjectNote ui;

};
#endif // W_ProjectNote_H
