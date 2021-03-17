#ifndef P_SinglePictureViewer_H
#define P_SinglePictureViewer_H

#include <QtGui/QMouseEvent>
#include "private/i_SPVView.h"

/*
-----==========================================================-----
		类：		图片查看块-单图 组装体.h
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		通过new，自动将一个QWidget，开辟成 图片查看块 的QGraphView。
					（详细见.cpp）
-----==========================================================-----
*/
class P_SinglePictureViewer : public QObject
{
	Q_OBJECT

	public:
		P_SinglePictureViewer(QWidget* _parent);		//构造函数
		~P_SinglePictureViewer();						//析构函数
		
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
		I_SPVView* m_GraphView;			//视图
	public:
										//控件 - 重建控件
		void rebuildUI();
										//控件 - 清理UI
		void clearUI();
		
	//-----------------------------------
	//----缩放
	public slots:
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

	//-----------------------------------
	//----图片
	public slots:
										//图片 - 设置 图片资源
		void setSource(QPixmap pixmap);

};

#endif // P_SinglePictureViewer_H