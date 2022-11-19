#ifndef W_PicturePartitioner_H
#define W_PicturePartitioner_H
#include "stdafx.h"

#include "ui_W_PicturePartitioner.h"
#include "Private/P_PPaViewer.h"
#include "Source/GraphModule/Widget/SinglePictureViewer/P_SinglePictureViewer.h"
#include "Source/GraphModule/Middleware/MouseResizeController/ExtendPart/P_MRe_ButtonPart.h"

/*
-----==========================================================-----
		类：		图片分割器 窗口.cpp
		所属模块：	图形模块
		功能：		可以将图片分割成多个图片。
-----==========================================================-----
*/
class W_PicturePartitioner : public QDialog
{
	Q_OBJECT

	public:
		W_PicturePartitioner(QWidget* parent = 0);
		~W_PicturePartitioner();

	//-----------------------------------
	//----视图
	protected:
		P_SinglePictureViewer* m_p_SinglePictureViewer;				//（切割预览图）
		P_PPaViewer* m_p_PPaViewer;									//（帧选择图）
		P_MRe_ButtonPart* m_p_MouseResizeButtonPart1;		//缩放按钮块1
		P_MRe_ButtonPart* m_p_MouseResizeButtonPart2;		//缩放按钮块2
		
	//-----------------------------------
	//----行列
	public slots:
										//行列 - 输入框变化
		void rowChanged(int value);
		void columnChanged(int value);
										//行列 - 刷新网格线
		void refreshGridLine();

	//-----------------------------------
	//----流程
	public slots:
										//流程 - 进入流程1
		void toFlow_1();
										//流程 - 进入流程2
		void toFlow_2();

	//-----------------------------------
	//----窗口
	protected:
		bool m_slotBlock;
		QPixmap local_bitmap;				//当前图片
		QList<QPixmap> local_bitmapTank;	//切割的列表
	public:
										//窗口 - 设置数据
		void setData(QFileInfo file);
		void setData(QPixmap bitmap);
										//窗口 - 取出数据
		QList<QPixmap> getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();
	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_PicturePartitioner ui;
};

#endif 