#ifndef I_SPVView_H
#define I_SPVView_H

#include <QGraphicsView>
#include "i_SPVScene.h"

/*
-----==========================================================-----
		类：		图片查看块-单图 视图.h
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		连接控件与场景的中间类，控制滚动条与缩放功能。
					（详细见.cpp）
-----==========================================================-----
*/
class I_SPVView : public QGraphicsView
{
    Q_OBJECT
	
	public:
		I_SPVView(QWidget *parent = 0);			//构造函数
		~I_SPVView();							//析构函数
		void init();							//初始化
		
		
	//-----------------------------------
	//----部件
	private:
		I_SPVScene* m_scene;
	public:
									//部件 - 获取场景
		I_SPVScene* getScene();


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
										//		【说明】：填"ctrl"、"alt"、"只滚轮"，空字符串表示 关闭 滚轮缩放。
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
		bool m_mousePressed;
		QPoint m_mousePos;
		QPoint m_mousePosLast;


	//-----------------------------------
	//----滚动条
	private slots:
										//滚动条 - 滚动条滚动
		void scrollValueChanged();

	//-----------------------------------
	//----监听事件
	private:
										//监听事件 - 鼠标按下（暂未使用）
		void I_SPVView::mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//监听事件 - 鼠标抬起（暂未使用）
		void I_SPVView::mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//监听事件 - 鼠标移动（暂未使用）
		void I_SPVView::mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//监听事件 - 鼠标滚轮
		void I_SPVView::wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

};

#endif // I_SPVView_H
