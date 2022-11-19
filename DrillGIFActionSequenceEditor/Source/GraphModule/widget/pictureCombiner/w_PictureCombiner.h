#ifndef W_PictureCombiner_H
#define W_PictureCombiner_H
#include "stdafx.h"

#include "ui_W_PictureCombiner.h"
#include "P_PictureCombiner.h"

/*
-----==========================================================-----
		类：		图片合成器 窗口.cpp
		所属模块：	图形模块
		功能：		可以将多张图片以矩阵的形式合并到一起。
-----==========================================================-----
*/
class W_PictureCombiner : public QDialog
{
	Q_OBJECT

	public:
		W_PictureCombiner(QWidget* parent = 0);
		~W_PictureCombiner();
		
	//-----------------------------------
	//----控件
	public:
		P_PictureCombiner* m_P_PictureCombiner;

	//-----------------------------------
	//----窗口
	public:
										//窗口 - 设置数据
		void setData(QList<QFileInfo> file_list);
		void setData(QList<QPixmap> bitmap_list);
										//窗口 - 取出数据
		QPixmap getData();
	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_PictureCombiner ui;
};

#endif 