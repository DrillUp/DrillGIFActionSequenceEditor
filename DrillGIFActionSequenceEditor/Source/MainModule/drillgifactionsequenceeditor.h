#ifndef DRILLGIFACTIONSEQUENCEEDITOR_H
#define DRILLGIFACTIONSEQUENCEEDITOR_H

#include <QtWidgets/QDialog>
#include "ui_drillgifactionsequenceeditor.h"

/*
-----==========================================================-----
		类：		主窗体.h
		作者：		drill_up
		所属模块：	主窗体模块
		功能：		程序进入后的主控制窗口。
					（详细见cpp）
-----==========================================================-----
*/
class P_ActionSeqPart;
class W_RmmvOperateBoard;
class DrillGIFActionSequenceEditor : public QDialog
{
	Q_OBJECT

	public:
		DrillGIFActionSequenceEditor(QWidget *parent = 0);
		~DrillGIFActionSequenceEditor();
		static DrillGIFActionSequenceEditor* cur_instance;		//单例
		static DrillGIFActionSequenceEditor* getInstance();		//单例，获取自己
		void _init();											//初始化
		
	//-----------------------------------
	//----控件（rmmv交互数据）
	public:
		W_RmmvOperateBoard* m_w_RmmvOperateBoard;
	public:
								//控件 - 打开rmmv交互窗口
		void openWindowRmmvInteractive();
	public slots:
								//控件 - rmmv交互数据已读取
		void rmmvInteractiveDataLoaded();

	//-----------------------------------
	//----控件（动作序列交互数据）
	public:
		P_ActionSeqPart* m_p_ActionSeqPart;
	public slots:
								//控件 - 动作序列已读取
		void actionSeqDataLoaded();
								//控件 - 动作序列数据重建
		void rebuildActionSeqData();
								//控件 - 新建项目
		void newProject();
								//控件 - 打开项目
		void openProject();
								//控件 - 保存项目
		void saveProject();
								//控件 - 另存为项目
		void saveAsProject();
								//控件 - 修改文件名称
		void changeWindowTitle(QString title);
								//控件 - 用户手册
		void openUserManual();
								//控件 - 关于...
		void openAbout();

	//-----------------------------------
	//----事件
	private:
		QSize m_last_uiSize;
	public:
								//事件 - 窗口切换大小事件
		void resizeEvent(QResizeEvent *event);
								//事件 - 窗口关闭事件（点击关闭按钮）
		void closeEvent(QCloseEvent *event);
								//事件 - 拖入文件判断
		void dragEnterEvent(QDragEnterEvent *event);
								//事件 - 放置文件动作
		void dropEvent(QDropEvent *event);

	//-----------------------------------
	//----窗口
	public:
								//窗口 - 用户自定义UI读取
		void ui_loadConfig();
								//窗口 - 用户自定义UI存储
		void ui_saveConfig();
	private:
		Ui::DrillGIFActionSequenceEditorClass ui;
};

#endif // DRILLGIFACTIONSEQUENCEEDITOR_H
