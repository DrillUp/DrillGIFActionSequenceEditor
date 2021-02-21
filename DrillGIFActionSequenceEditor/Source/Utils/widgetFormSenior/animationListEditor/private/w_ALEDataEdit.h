#ifndef W_ALEDataEdit_H
#define W_ALEDataEdit_H

#include <QDialog>
#include "../c_ALEData.h"
#include "ui_W_ALEDataEdit.h"

/*
-----==========================================================-----
		类：		动画帧编辑 窗口.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		编辑动画帧的一些配置。
					（详细见cpp）
-----==========================================================-----
*/
class W_ALEDataEdit : public QDialog
{
	Q_OBJECT

	public:
		W_ALEDataEdit(QWidget *parent = 0 );
		~W_ALEDataEdit();
		
	//-----------------------------------
	//----控件		
	public:
		bool m_slotBlock;
	public:
										//回车过滤
		void keyPressEvent(QKeyEvent *event);
	public slots:
										//选择文件（单张）
		void chooseFile();
										//控件 - 选择数字
		void timeSelected(QString text);
										//控件 - 编辑数字
		void timeEdited(double value);

	//-----------------------------------
	//----窗口
	protected:
		C_ALEData local_data;
		int local_index;
		QList<int> local_index_list;
	public:
										//窗口 - 设置数据（修改）
		void setDataInModifyMode_One(C_ALEData data, int index);
		void setDataInModifyMode_Multi(C_ALEData data, QList<int> index_list);
										//窗口 - 取出数据
		C_ALEData getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();

	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_ALEDataEdit ui;
	//-----------------------------------
};

#endif // W_ALEDataEdit_H
