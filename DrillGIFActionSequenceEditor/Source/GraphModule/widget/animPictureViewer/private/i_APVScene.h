#ifndef I_APVScene_H
#define I_APVScene_H

#include <QGraphicsScene>

/*
-----==========================================================-----
		类：		图片查看块 场景.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		可以用鼠标进行点添加和线连接。
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
		QColor m_backgroundColor;					//底色
		QGraphicsPixmapItem* m_maskBackground;		//背景
	private:
									//属性 - 刷新背景
		void refreshBackground();
									//属性 - 获取马赛克贴图
		QPixmap getBitmapMaskBackground();
		
	//-----------------------------------
	//----资源
	private:
		QList<QPixmap> m_bitmapList;
	public:
									//资源 - 设置资源
		void setSource(QList<QPixmap> bitmap_list);
									//资源 - 重建场景
		void rebuildScene();

	//-----------------------------------
	//----部件
	private:
		QList<QGraphicsPixmapItem*> m_animList;		//动画帧列表
		int m_curFrame;
	public:
									//部件 - 切换帧
		void setAnimFrame(int index);
									//部件 - 刷新帧显示
		void refreshFrame();
									//部件 - 资源高度
		int getMaxHeight();
									//部件 - 资源宽度
		int getMaxWidth();

};

#endif // I_APVScene_H
