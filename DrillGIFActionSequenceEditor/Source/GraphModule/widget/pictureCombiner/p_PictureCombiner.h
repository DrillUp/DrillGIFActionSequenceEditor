#ifndef P_PictureCombiner_H
#define P_PictureCombiner_H
#include "stdafx.h"

#include <QtWidgets>
#include "ui_P_PictureCombiner.h"
#include "Source/GraphModule/Widget/SinglePictureViewer/P_SinglePictureViewer.h"
#include "Source/GraphModule/Middleware/MouseResizeController/ExtendPart/P_MRe_ButtonPart.h"

/*
-----==========================================================-----
		类：		图片合成器.h
		版本：		v1.00
		作者：		drill_up
		所属模块：	图形模块
		功能：		可以将多张图片以矩阵的形式合并到一起。
					（详细见cpp）
-----==========================================================-----
*/
class P_PictureCombiner : public QWidget
{
	Q_OBJECT

	public:
		P_PictureCombiner(QWidget *parent = 0);
		~P_PictureCombiner();
		
	//-----------------------------------
	//----视图
	protected:
		P_SinglePictureViewer* m_p_SinglePictureViewer;			//图片查看块-单图
		P_MRe_ButtonPart* m_p_MouseResizeButtonPart;		//缩放按钮块
	public slots:
										//视图 - 网格勾选变化
		void gridLineToggled(bool checked);

	//-----------------------------------
	//----行列
	public slots:
										//行列 - 输入框变化
		void rowChanged(int value);
		void columnChanged(int value);
										//行列 - 勾选变化（自适应）
		void rowToggled(bool checked);
		void columnToggled(bool checked);
										//行列 - 下拉框变化
		void comboxChanged(QString text);
	protected:
										//控件 - 计算行列
		void autoFillNum();
	
	//-----------------------------------
	//----帧属性
	public slots:
										//帧属性 - 输入框变化
		void frameWidthChanged(int value);
		void frameHeightChanged(int value);
										//帧属性 - 勾选变化（默认值）
		void frameWidthDefaultToggled(bool checked);
		void frameHeightDefaultToggled(bool checked);

		
	//-----------------------------------
	//----绘制
	public:
										//绘制 - 重绘图片
		void redrawBitmap();
										//绘制 - 获取最大宽度
		int getMaxWidth();
										//绘制 - 获取最大高度
		int getMaxHeight();
										//绘制 - 判断高宽一致
		bool isSizeFit();
	protected:
										//绘制 - 在指定的范围内绘制图像，自动居中，自动剪切
		void drawBitmap_CenterAndCut(QPainter* painter, QPixmap bitmap, QRect frame);
										//绘制 - 获取绘制的图片
		QPixmap getDrawedBitmap(int p_width,int p_height);
		
	//-----------------------------------
	//----块
	protected:
		bool m_slotBlock;
		QPixmap local_draw;
		QList<QPixmap> local_bitmapTank;
	public:
										//块 - 设置数据
		void setData(QList<QFileInfo> file_tank);
		void setData(QList<QPixmap> bitmap_tank);
										//块 - 取出数据
		QPixmap getData();
										//块 - 本地数据 -> ui数据
		void putDataToUi();							
										//块 - ui数据 -> 本地数据
		void putUiToData();

	private:
		Ui::P_PictureCombiner ui;

};

#endif // W_ImportMeshDialog_H
