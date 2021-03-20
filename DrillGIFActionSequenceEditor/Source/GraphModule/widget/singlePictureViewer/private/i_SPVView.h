#ifndef I_SPVView_H
#define I_SPVView_H

#include <QGraphicsView>
#include "i_SPVScene.h"

#include "Source/GraphModule/middleware/mouseResizeController/p_MouseResizeController.h"

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
		P_MouseResizeController* m_p_MouseResizeController;
	public:
										//部件 - 获取场景
		I_SPVScene* getScene();
										//部件 - 获取鼠标缩放控制器
		P_MouseResizeController* getMouseResizeController();


	//-----------------------------------
	//----滚动条
	private slots:
										//滚动条 - 滚动条滚动
		void scrollValueChanged();

	//-----------------------------------
	//----监听事件
	private:
										//监听事件 - 鼠标按下
		void I_SPVView::mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//监听事件 - 鼠标抬起
		void I_SPVView::mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//监听事件 - 鼠标移动
		void I_SPVView::mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//监听事件 - 鼠标滚轮
		void I_SPVView::wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

};

#endif // I_SPVView_H
