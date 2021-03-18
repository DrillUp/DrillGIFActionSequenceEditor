#ifndef P_AnimPictureViewer_H
#define P_AnimPictureViewer_H

#include <QFileInfo>
#include <QtGui/QMouseEvent>
#include "private/i_APVView.h"

/*
-----==========================================================-----
		类：		图片查看块-动图 组装体.h
		版本：		v1.02
		作者：		drill_up
		所属模块：	工具模块
		功能：		通过new，自动将一个QWidget，开辟成 图片查看块 的QGraphView。
					（详细见.cpp）
-----==========================================================-----
*/
class P_AnimPictureViewer : public QObject
{
	Q_OBJECT

	public:
		P_AnimPictureViewer(QWidget* _parent);		//构造函数
		~P_AnimPictureViewer();						//析构函数
		
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
		I_APVView* m_GraphView;			//视图
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
	//----动画帧
	public slots:
										//动画帧 - 设置 图片资源
										//		【说明1】：由于底层已固化，所以只能输入资源文件，QPixmap不支持。
										//		【说明2】：如果你用索引切换帧，资源可以重复；如果用资源名称切换帧，不建议重复。
		void setSource(QList<QFileInfo> file_list);
										//动画帧 - 获取 图片资源
		QList<QFileInfo> getSource();
										//动画帧 - 清除资源
		void clearSource();
										//动画帧 - 切换帧（根据索引）
										//		【说明】：索引如果越界，将不显示任何图像。
		void setAnimFrame(int index);
										//动画帧 - 切换帧（根据资源名称）
										//		【说明】：没有资源会自动添加。添加时会去重。
		void setAnimFile(QFileInfo file);
		

	//-----------------------------------
	//----辅助
	public slots:
										//辅助 - 设置网格线
		void setGridLine(int column, int row);
										//辅助 - 清空网格线
		void clearGridLine();
};

#endif // P_AnimPictureViewer_H