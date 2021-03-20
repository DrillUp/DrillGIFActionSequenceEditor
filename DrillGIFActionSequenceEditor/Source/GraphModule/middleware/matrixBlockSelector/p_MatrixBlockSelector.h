#ifndef P_MatrixBlockSelector_H
#define P_MatrixBlockSelector_H

#include <QtGui/QMouseEvent>
#include <QGraphicsView>
#include <QSvgRenderer>
#include <QGraphicsSvgItem>
#include "i_MBSBlock.h"

/*
-----==========================================================-----
		类：		方块阵列点选器.h
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		该控制器用于装饰 QGraphicsView ，使其能够支持 方块阵列勾选切换 + 颜色块切换/矢量图切换 功能。
					（详细见.cpp）
-----==========================================================-----
*/
class P_MatrixBlockSelector : public QObject
{
	Q_OBJECT

	public:
		P_MatrixBlockSelector();				//构造函数
		~P_MatrixBlockSelector();				//析构函数

	//-----------------------------------
	//----初始化
	private:
		QGraphicsView* m_view;
	public:
										//初始化 - 设置视图
		void initGraphicsView(QGraphicsView* view);
										//初始化 - 设置方块颜色
										//		【说明】：列表的数量决定状态的数量。
		void initBlockColor(QList<QColor> color_list);
										//初始化 - 设置矢量图列表
										//		【说明】：要与方块颜色数量一致。
		void initBlockSvgIcon(QList<QFileInfo> svg_file_list);
										//初始化 - 设置方块大小
										//		【说明】：初始化后要赋值，矩阵将会以此大小为基准，放置方块。
		void initBlockSize(int width,int height);
										//重置 - 重置方块
										//		【说明】：该操作会清空方块，然后重新规划设置方块大小。
		void rebuildBlock(int width, int height);
		void rebuildBlock_double(double width, double height);
		
	//-----------------------------------
	//----方块
	private:
		double m_blockWidth;				//方块宽度（正常值为int，但不排除特殊double情况）
		double m_blockHeight;				//方块高度
		QList<QColor> m_blockColorTank;		//方块颜色
		QList<I_MBSBlock*> m_blockTank;		//方块容器
	public:
										//方块 - 获取全部方块
		QList<I_MBSBlock*> getAllBlock();
										//方块 - 获取方块矩阵（单位图块）
		QRect getBlockMatrix();
										//方块 - 清除全部方块、矢量标记
										//		【说明】：清除前，会发清除前信号。
		void clearAllBlock();
	signals:
										//方块 - 新方块被添加（信号）
		void newBlockAdded(I_MBSBlock* new_block,QGraphicsSvgItem* svgItem);
										//方块 - 全部方块被清除前（信号）
		void oldBlockClearBefore(QList<I_MBSBlock*> block_list, QList<QGraphicsSvgItem*> svgItem_list);
	private:
										//方块 - 添加方块（私有）
		void setBlock(QPoint matrix_pos, int state);
		
	//-----------------------------------
	//----矢量标记
	private:
		QList<QSvgRenderer*> m_rendererTank;		//渲染器列表
		QList<QGraphicsSvgItem*> m_svgTank;			//矢量图列表
	public:
										//矢量 - 获取全部方块
		QList<QGraphicsSvgItem*> getAllSvgItem();

	//-----------------------------------
	//----填涂
	private:
		QList<QPoint> m_coveredPointTank;	//鼠标经过的点（只增，最大值充满矩阵）
		int m_cur_mouseState;				//当前填涂状态
	public:
										//填涂 - 获取指定状态位的全部点
		QList<QPoint> getPointList(int state);
										//填涂 - 填涂全部状态位
		void fillAll(int state);


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


};

#endif // P_MatrixBlockSelector_H