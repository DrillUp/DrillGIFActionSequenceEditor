#ifndef I_PPaScene_H
#define I_PPaScene_H

#include <QGraphicsScene>
#include "Source/GraphModule/item/maskBackgroundItem/i_MaskBackgroundItem.h"
#include "Source/GraphModule/item/gridLineItem/p_GridLineItem.h"

/*
-----==========================================================-----
		类：		切割帧选择器 场景.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		使得可以在场景中点击勾选切割帧。
					（详细见.cpp）
-----==========================================================-----
*/
class I_PPaScene : public QGraphicsScene
{
    Q_OBJECT
	
	public:
		I_PPaScene(QWidget *parent = 0);		//构造函数
		~I_PPaScene();							//析构函数
		void init();							//初始化
		
		
	//-----------------------------------
	//----鼠标事件
	private:
										//监听事件 - 鼠标按下
		void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
										//监听事件 - 鼠标抬起
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
										//监听事件 - 鼠标移动
		void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
										//监听事件 - 鼠标双击
		void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;


	//-----------------------------------
	//----属性
	private:
		int m_canvasWidth;			//画布宽度
		int m_canvasHeight;			//画布高度
		int m_canvasThickness;		//画布出血线厚度
		int m_pixelWidth;			//图块宽度
		int m_pixelHeight;			//图块高度
		
	//-----------------------------------
	//----辅助对象
	private:
		QColor m_gridLineColor;						//网格线底色
		QColor m_backgroundColor;					//背景底色
		P_GridLineItem* m_P_GridLineItem;			//网格线
		I_MaskBackgroundItem* m_maskBackground;		//背景
	public:
									//辅助 - 设置网格线
		void setGridLine(int column, int row);
									//辅助 - 清空网格线
		void clearGridLine();
	private:
									//辅助 - 刷新背景
		void refreshBackground();
		
	//-----------------------------------
	//----资源
	public:
		QPixmap m_bitmap;
	public:
									//资源 - 设置资源
		void setSource(QPixmap bitmap);
									//资源 - 清除资源
		void clearSource();

	protected:
									//资源 - 重建场景
		void rebuildScene();

	//-----------------------------------
	//----图片
	private:
		QGraphicsPixmapItem* m_bitmapItem;
	public:
									//图片 - 最大高度
		int getMaxHeight();
									//图片 - 最大宽度
		int getMaxWidth();

};

#endif // I_PPaScene_H
