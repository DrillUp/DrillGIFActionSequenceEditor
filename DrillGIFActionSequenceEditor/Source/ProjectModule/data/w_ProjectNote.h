#ifndef W_ProjectNote_H
#define W_ProjectNote_H


#include "ui_w_ProjectNote.h"


class W_ProjectNote : public QDialog
{
	Q_OBJECT

	public:
		W_ProjectNote(QWidget *parent = 0);
		~W_ProjectNote();

	public slots:
		void clickedBtn(QAbstractButton * btn);
		
	//-----------------------------------
	//----窗口
	public:
		//bool isAddMode;
		QString local_ProjectNote;
	public:
										//窗口 - 设置数据
		void setData(QString p);
										//窗口 - 取出数据
		QString getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();

	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_ProjectNote ui;
	//-----------------------------------
};

#endif // W_ProjectNote_H
