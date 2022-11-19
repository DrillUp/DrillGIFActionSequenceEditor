#ifndef I_APVScene_H
#define I_APVScene_H

#include <QGraphicsScene>
#include "Source/GraphModule/Item/MaskBackgroundItem/I_MaskBackgroundItem.h"
#include "Source/GraphModule/Item/GridLineItem/P_GridLineItem.h"

/*
-----==========================================================-----
		类：		图片查看块-动图 场景.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		查看变化动图的画布。
					（详细见.cpp）
-----==========================================================-----
*/
class I_APVScene : public QGraphicsScene
{
    Q_OBJECT
	
	public:
		I_APVScene(QWidget *parent = 0);		//构造函数
		~I_APVScene();							//析构函数
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
	//----着色器
	protected:
		int m_curTint;
	public:
									//着色器 - 修改色调
		void setTint(int rotate_offset);
									//着色器 - 获取色调
		int getTint();
	protected:
									//着色器 - 旋转色彩
		QPixmap rotateColor(QPixmap pixmap,int rotate_offset);
									//着色器 - rgb模式转hsl模式
		void rgbToHsl(int r, int g, int b, double* result_arr);
									//着色器 - hsl模式转rgb模式
		void hslToRgb(double h, double s, double l, double* result_arr);
		
	//-----------------------------------
	//----资源
	private:
		bool m_rebuildBlock;
		QList<QFileInfo> m_fileList;
		QList<QPixmap> m_bitmapList;
	public:
									//资源 - 设置资源
		void setSource(QList<QFileInfo> file_list);
									//资源 - 获取资源
		QList<QFileInfo> getSource();
									//资源 - 清除资源
		void clearSource();

	protected:
									//资源 - 重建场景
		void rebuildScene();
									//资源 - 添加资源（私有）
		void addSource(QFileInfo file);

	//-----------------------------------
	//----动画帧
	private:
		QList<QGraphicsPixmapItem*> m_animList;		//动画帧列表
		int m_curFrame;
	public:
									//动画帧 - 切换帧（根据索引）
		void setAnimFrame(int index);
									//动画帧 - 切换帧（根据资源名称）
		void setAnimName(QFileInfo file);
									//动画帧 - 刷新帧显示
		void refreshFrame();
									//动画帧 - 最大高度
		int getMaxHeight();
									//动画帧 - 最大宽度
		int getMaxWidth();

};

#endif // I_APVScene_H
