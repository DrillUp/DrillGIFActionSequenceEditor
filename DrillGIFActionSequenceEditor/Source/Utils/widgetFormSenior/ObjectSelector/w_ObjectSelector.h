#ifndef W_ObjectSelector_H
#define W_ObjectSelector_H

#include "ui_W_ObjectSelector.h"
#include "private/c_ObjectSelectorConfig.h"

/*
-----==========================================================-----
		类：		对象选择 窗口.h
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		从 QJsonObject列表 中选择一个对象的窗口。（要有id和name，id必须为数字，obj不能为空）
					也可以从 id_list和name_list 中选择一个id。（两字符串必须长度相等）
					（详细见cpp）
-----==========================================================-----
*/
class P_ObjectSortController;
class C_ObjectSortData;
class W_ObjectSelector : public QDialog
{
	Q_OBJECT

	public:
		W_ObjectSelector(QWidget *parent = 0);
		~W_ObjectSelector();

	//-----------------------------------
	//----控件
	protected:
		int cur_selectIndex_page;
		int cur_selectIndex_context;
	public slots:
										//控件 - 刷新表格
		void refreshTable();
										//控件 - 刷新表格
		void refreshTable_page();
		void refreshTable_context();
										//控件 - 页变化
		void currentPageChanged(QTableWidgetItem* cur, QTableWidgetItem* pre);
		
	//-----------------------------------
	//----选择器设置
	protected:
		C_ObjectSelectorConfig m_config;	//选择器设置
	public:
										//选择器设置 - 功能开关
		void setConfigBtnVisible(bool visible);
										//选择器设置 - 设置参数
		void setConfigParam(QJsonObject config);
										//选择器设置 - 取出参数
		QJsonObject getConfigParam();
	public slots:
										//选择器设置 - 编辑窗口
		void openConfigParamWindow();


	//-----------------------------------
	//----资源数据
	protected:
		P_ObjectSortController* m_source_ObjectSortController;		//数据排序管理器
		QList<QJsonObject>* m_source_list;							//原数据
	public:
										//资源数据 - 载入
		void loadSource(QList<QJsonObject>* obj_list);
		void loadSource(QList<QJsonObject>* obj_list, QString id_symbol, QString name_symbol, QString type_symbol);
										//资源数据 - 获取选中的数据
		int getSelectedSourceId();
		QString getSelectedSourceName();
		QString getSelectedSourceType();
	private:
										//资源数据 - 获取选中的对象（私有）
		C_ObjectSortData* getSelectedSource();
	
	//-----------------------------------
	//----窗口
	public:
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();
	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_ObjectSelector ui;
	//-----------------------------------
};

#endif // W_ObjectSelector_H
