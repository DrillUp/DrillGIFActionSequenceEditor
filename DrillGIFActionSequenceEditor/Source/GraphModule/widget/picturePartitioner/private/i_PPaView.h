#ifndef I_PPaView_H
#define I_PPaView_H

#include <QGraphicsView>
#include "i_PPaScene.h"

#include "Source/GraphModule/middleware/mouseResizeController/p_MouseResizeController.h"
#include "Source/GraphModule/middleware/matrixBlockSelector/p_MatrixBlockSelector.h"

/*
-----==========================================================-----
		类：		切割帧选择器 视图.h
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		连接控件与场景的中间类，控制滚动条与缩放功能。
					（详细见.cpp）
-----==========================================================-----
*/
class I_PPaView : public QGraphicsView
{
    Q_OBJECT
	
	public:
		I_PPaView(QWidget *parent = 0);			//构造函数
		~I_PPaView();							//析构函数
		void init();							//初始化
		
		
	//-----------------------------------
	//----部件
	private:
		I_PPaScene* m_scene;
		P_MouseResizeController* m_p_MouseResizeController;
		P_MatrixBlockSelector* m_p_MatrixBlockSelector;		//方块阵列点选器
	public:
										//部件 - 获取场景
		I_PPaScene* getScene();
										//部件 - 获取鼠标缩放控制器
		P_MouseResizeController* getMouseResizeController();
										//部件 - 方块阵列点选器
		P_MatrixBlockSelector* getMatrixBlockSelector();
		
	//-----------------------------------
	//----点选器
	public:
		int m_block_width;	//（父控件赋值）
		int m_block_height;
	public:
										//点选器 - 获取切割都图片
		QList<QPixmap> getCutBitmap();
	private slots:
										//点选器 - 方块点选器添加
		void blockAdded(I_MBSBlock* block, QGraphicsSvgItem* item);
										//点选器 - 方块点选器清除
		void blockClear(QList<I_MBSBlock*> block_list, QList<QGraphicsSvgItem*> item_list);


	//-----------------------------------
	//----滚动条
	private slots:
										//滚动条 - 滚动条滚动
		void scrollValueChanged();

	//-----------------------------------
	//----监听事件
	private:
										//监听事件 - 鼠标按下
		void I_PPaView::mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//监听事件 - 鼠标抬起
		void I_PPaView::mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//监听事件 - 鼠标移动
		void I_PPaView::mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//监听事件 - 鼠标滚轮
		void I_PPaView::wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

};

#endif // I_PPaView_H
