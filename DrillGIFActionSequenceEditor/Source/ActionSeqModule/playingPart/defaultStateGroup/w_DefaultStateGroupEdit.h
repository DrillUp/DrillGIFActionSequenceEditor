#ifndef W_DefaultStateGroupEdit_H
#define W_DefaultStateGroupEdit_H

#include <QtWidgets/QDialog>
#include "ui_W_DefaultStateGroupEdit.h"
#include "Source/Utils/widgetForm/radioTable/p_RadioTable.h"

/*
-----==========================================================-----
		类：		默认状态元集合 编辑.h
		所属模块：	动画序列模块
		功能：		默认状态元集合的窗口。
					（详细见cpp）
-----==========================================================-----
*/
class W_DefaultStateGroupEdit : public QDialog
{
	Q_OBJECT

	public:
		W_DefaultStateGroupEdit(QWidget *parent = 0);	//构造函数
		~W_DefaultStateGroupEdit();						//析构函数
		
	//-----------------------------------
	//----控件
	public:
		P_RadioTable* m_p_RadioTable;
	public:
									//控件 - 选中变化
		void selectedChanged(QList<int> index_list);
		
	//-----------------------------------
	//----窗口
	public slots:
									//窗口 - 设置数据
		void setData(QStringList data,QStringList selected);
									//窗口 - 获取数据
		QStringList getData();
									//窗口 - 本地数据 -> ui数据
									//窗口 - ui数据 -> 本地数据
									//窗口 - 提交数据（校验）
	private:
		Ui::W_DefaultStateGroupEdit ui;
		
};

#endif // W_QExpressionTest_H
