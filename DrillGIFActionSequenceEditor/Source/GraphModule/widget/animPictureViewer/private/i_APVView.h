#ifndef I_APVView_H
#define I_APVView_H
#include "stdafx.h"

#include <QGraphicsView>
#include "I_APVScene.h"

#include "Source/GraphModule/Middleware/MouseResizeController/P_MRe_Controller.h"

/*
-----==========================================================-----
		类：		图片查看块-动图 视图.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		连接控件与场景的中间类，控制滚动条与缩放功能。
					（详细见.cpp）
-----==========================================================-----
*/
class I_APVView : public QGraphicsView
{
    Q_OBJECT
	
	public:
		I_APVView(QWidget *parent = 0);			//构造函数
		~I_APVView();							//析构函数
		void init();							//初始化
		
		
	//-----------------------------------
	//----部件
	private:
		I_APVScene* m_scene;
		P_MRe_Controller* m_p_MouseResizeController;
	public:
										//部件 - 获取场景
		I_APVScene* getScene();
										//部件 - 获取鼠标缩放控制器
		P_MRe_Controller* getMouseResizeController();


	//-----------------------------------
	//----滚动条
	private slots:
										//滚动条 - 滚动条滚动
		void scrollValueChanged();

	//-----------------------------------
	//----监听事件
	private:
										//监听事件 - 鼠标按下（暂未使用）
		void I_APVView::mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//监听事件 - 鼠标抬起（暂未使用）
		void I_APVView::mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//监听事件 - 鼠标移动（暂未使用）
		void I_APVView::mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//监听事件 - 鼠标滚轮
		void I_APVView::wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

};

#endif // I_APVView_H
