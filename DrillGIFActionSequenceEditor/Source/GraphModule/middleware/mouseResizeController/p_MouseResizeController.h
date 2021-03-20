#ifndef P_MouseResizeController_H
#define P_MouseResizeController_H

#include <QtGui/QMouseEvent>
#include <QGraphicsView>


/*
-----==========================================================-----
		类：		鼠标缩放控制器.h
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		该控制器用于装饰 QGraphicsView ，使其能够支持 鼠标控制缩放+鼠标拖拽 功能。
					（详细见.cpp）
-----==========================================================-----
*/
class P_MouseResizeController : public QObject
{
	Q_OBJECT

	public:
		P_MouseResizeController();				//构造函数
		~P_MouseResizeController();				//析构函数

	//-----------------------------------
	//----初始化
	private:
		QGraphicsView* m_view;
	public:
										//初始化 - 设置视图
		void initGraphicsView(QGraphicsView* view);
		
	//-----------------------------------
	//----缩放
	private:
		double m_scale;					//缩放比例
		double m_lastScale;				//
		QString m_scaleWheelModifier;	//控制用修饰符
	public:
										//缩放 - 缩小
		void zoomIn();
										//缩放 - 放大
		void zoomOut();
										//缩放 - 大小重置
		void zoomReset();
										//缩放 - 获取缩放值
		double getScale();
										//缩放 - 设置滚轮缩放修饰符
										//		【参数】：填"ctrl"、"alt"、"只滚轮"，空字符串表示 关闭 滚轮缩放。
		void setScaleWheelModifier(QString charModifier);
	signals:
										//缩放 - 缩放值改变
		void scaleChanged(double scale);
	private:
										//缩放 - 大小重置
		void zoomResetPrivate();

	//-----------------------------------
	//----鼠标拖拽
	private:
		QPoint m_mousePos;				//鼠标移动点
		QPoint m_mousePosLast;			//鼠标点击位置点

	//-----------------------------------
	//----监听事件
	private:
		bool m_mousePressed;			//鼠标按下标记
	public:
										//监听事件 - 鼠标按下（接口）
		void event_mousePressEvent(QMouseEvent *event);
										//监听事件 - 鼠标抬起（接口）
		void event_mouseReleaseEvent(QMouseEvent *event);
										//监听事件 - 鼠标移动（接口）
		void event_mouseMoveEvent(QMouseEvent *event);
										//监听事件 - 鼠标滚轮（接口）
										//			【返回】：返回值决定该事件是否阻塞。
		bool event_wheelEvent(QWheelEvent *event);

};

#endif // P_MouseResizeController_H