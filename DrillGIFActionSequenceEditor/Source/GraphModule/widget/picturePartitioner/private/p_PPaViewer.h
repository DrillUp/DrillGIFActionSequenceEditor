#ifndef P_PPaViewer_H
#define P_PPaViewer_H
#include "stdafx.h"

#include <QtGui/QMouseEvent>
#include "I_PPaView.h"

/*
-----==========================================================-----
		类：		切割帧选择器 组装体.h
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		通过new，自动将一个QWidget，开辟成 切割帧选择器 的QGraphView。
					（详细见.cpp）
-----==========================================================-----
*/
class P_MRe_Controller;
class P_PPaViewer : public QObject
{
	Q_OBJECT

	public:
		P_PPaViewer(QWidget* _parent);		//构造函数
		~P_PPaViewer();						//析构函数
		
	//-----------------------------------
	//----父控件
	private:
		QWidget* m_parent;				//父块
		QVBoxLayout* m_layout;			//父布局
	private:
										//父控件初始化
		void initWidgetAndLayout(QWidget* _parent);

	//-----------------------------------
	//----控件
	private:
		I_PPaView* m_GraphView;			//视图
	public:
										//控件 - 重建控件
		void rebuildUI();
										//控件 - 清理UI
		void clearUI();

	//-----------------------------------
	//----图片
	public slots:
										//图片 - 设置 图片资源
		void setSource(QFileInfo file);
		void setSource(QPixmap pixmap);


	//-----------------------------------
	//----辅助
	public slots:
										//辅助 - 设置网格线和方块划分
		void setGridLineAndBlockMatrix(int column, int row);
										//辅助 - 清空网格线
		void clearGridLine();
										//辅助 - 获取切割的图片
		QList<QPixmap> getCutBitmap();
		
	//-----------------------------------
	//----缩放【来自 鼠标缩放控制器 功能】
	public slots:
										//缩放 - 获取控制器
		P_MRe_Controller* getResizeController();
										//缩放 - 缩小
		void zoomIn();
										//缩放 - 放大
		void zoomOut();
										//缩放 - 大小重置
		void zoomReset();
										//缩放 - 获取缩放值
		double getScale();
										//缩放 - 设置滚轮缩放修饰符
										//		【说明】：填"ctrl"、"alt"，空字符串表示鼠标滚轮直接缩放。
		void setScaleWheelModifier(QString charModifier);
	signals:
										//缩放 - 缩放值改变
		void scaleChanged(double scale);
	private slots:
										//缩放 - 缩放值改变（与view交互）
		void scaleChanged_view(double scale);

};

#endif // P_PPaViewer_H