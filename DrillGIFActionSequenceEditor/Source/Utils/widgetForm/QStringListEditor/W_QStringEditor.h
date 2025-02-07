#ifndef W_QStringEditor_H
#define W_QStringEditor_H
#include "stdafx.h"

#include "ui_W_QStringEditor.h"

/*
-----==========================================================-----
		类：		字符串编辑 窗口.h
		版本：		v1.06
		作者：		drill_up
		所属模块：	工具模块
		功能：		编辑一条字符串的窗口。
					（详细见cpp）
-----==========================================================-----
*/
class W_QStringListEditor;
class W_QStringEditor : public QDialog
{
	Q_OBJECT

	public:
		W_QStringEditor(QWidget *parent = 0, W_QStringListEditor *windowParent = 0);
		~W_QStringEditor();
		
	public:
										//回车过滤
		void keyPressEvent(QKeyEvent *event);

	//-----------------------------------
	//----控件
	protected:
		W_QStringListEditor* m_windowParent;
		QString m_paramShowingName;		//参数显示名
		QString m_paramShowingSuffix;	//参数后缀名
		QString m_paramDescription;		//参数编辑的描述
		bool m_notNull;					//非空
		bool m_noRepeat;				//不可重复
	public:
										//控件 - 设置参数显示名
		void setParamShowingName(QString name);
										//控件 - 设置参数后缀名
		void setParamShowingSuffix(QString suffix);
										//控件 - 设置参数编辑的描述
		void setParamDescription(QString description);
										//控件 - 设置非空
		void setConditionNotNull(bool b);
										//控件 - 设置不可重复
		void setConditionNoRepeat(bool b);

	//-----------------------------------
	//----窗口
	protected:
		bool m_isAddMode;			
		QString m_modifyLastData;
		QString local_data;
	public:
										//窗口 - 设置数据（添加）
		void setDataInAddMode();
										//窗口 - 设置数据（修改）
		void setDataInModifyMode(QString data);
										//窗口 - 取出数据
		QString getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();

	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	
	signals:
		void acceptDataSignal(QString data);

	private:
		Ui::W_QStringEditor ui;
	//-----------------------------------
};

#endif // W_QStringEditor_H
