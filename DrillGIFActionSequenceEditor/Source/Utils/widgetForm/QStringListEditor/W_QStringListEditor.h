#ifndef W_QStringListEditor_H
#define W_QStringListEditor_H
#include "stdafx.h"

#include "ui_W_QStringListEditor.h"

/*
-----==========================================================-----
		类：		字符串编辑列表 窗口.h
		版本：		v1.02
		作者：		drill_up
		所属模块：	工具模块
		功能：		数据编辑的字符串列表控制。
					（详细见cpp）
-----==========================================================-----
*/

class W_QStringListEditor : public QDialog
{
	Q_OBJECT

	public:
		W_QStringListEditor(QWidget *parent = 0);
		~W_QStringListEditor();


	//-----------------------------------
	//----控件
	protected:
		QString m_paramShowingName;			//参数显示名
		QString m_paramDescription;			//参数编辑的描述
		QString m_paramListDescription;		//列表编辑的描述
		bool m_notNull;						//非空
		bool m_noRepeat;					//不可重复
	public:
										//控件 - 设置参数显示名
		void setParamShowingName(QString name);
										//控件 - 设置参数编辑的描述
		void setParamDescription(QString description);
										//控件 - 设置列表编辑的描述
		void setParamListDescription(QString description);
										//控件 - 设置非空
		void setConditionNotNull(bool b);
										//控件 - 设置不可重复
		void setConditionNoRepeat(bool b);

	public slots:
										//控件 - 添加
		virtual void addOneRow();
										//控件 - 编辑
		virtual int modifyOneRow();
										//控件 - 删除
		virtual int deleteOneRow();
										//控件 - 上移
		virtual int moveUpOneRow();
										//控件 - 下移
		virtual int moveDownOneRow();
		
										//控件 - 刷新表格
		void refreshTable();	


	//-----------------------------------
	//----窗口
	protected:
		//bool m_isAddMode;
		QStringList local_dataList;
	public:
										//窗口 - 设置数据
		void setDataInModifyMode(QStringList data);
										//窗口 - 取出数据
		QStringList getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();			

	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	
	signals:
		void acceptDataSignal(QStringList data);

	private:
		Ui::W_QStringListEditor ui;
	//-----------------------------------
};

#endif // W_QStringListEditor_H
