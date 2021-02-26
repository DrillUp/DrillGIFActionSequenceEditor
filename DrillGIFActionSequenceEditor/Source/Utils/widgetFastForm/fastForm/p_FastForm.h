#pragma once

#include <QWidget>
#include "c_FastClass.h"

/*
-----==========================================================-----
		类：		快速表单.h
		版本：		v1.13
		作者：		drill_up
		所属模块：	工具模块
		功能：		快速绘制出需要填写的表单列。
					并且能够快速获取表单的值。
					（详细见cpp）
-----==========================================================-----
*/
class C_FastForm;
class P_FastForm : public QObject
{
	Q_OBJECT

	public:
		P_FastForm(QWidget* _parent);		//构造函数
		~P_FastForm();						//析构函数
		
	//-----------------------------------
	//----父控件
	protected:
		QWidget* m_parent;				//父块
		QGridLayout* m_layout;			//父布局
	private:
										//父控件初始化
		void initWidgetAndLayout(QWidget* _parent);


	//-----------------------------------
	//----准备
	protected:
		QList<C_FastClassParam> m_param_list;	//准备属性
	public:
										//简单准备 - 单个弹簧
		void prepareQSpacerItem();
										//简单准备 - 单个标签
		void prepareQLabel(QString name);
										//简单准备 - 标签 + 输入框
		void prepareQLineEdit(QString name, QString default_value);
										//简单准备 - 标签 + 下拉框
		void prepareQComboBox(QString name, QStringList choose_list, QString default_value);

										//准备 - 快速类
		void prepareFastClass(C_FastClass c_f);
										//准备 - 清理全部参数
		void clearPrepare();
										//准备 - 是否含准备参数
		bool hasPreparedSequence();
		
	//-----------------------------------
	//----控件
	protected:
		QList<C_FastForm*> m_form_list;
	public:
										//控件 - 重建控件
		void rebuildUI();
										//控件 - 清理UI
		virtual void clearUI();
										//控件 - 锁定后缀宽度
		void lockSuffixWidth(int width);

	//-----------------------------------
	//----获取
	public:
										//获取 - 编辑框（没有则返回nullptr）
		QLineEdit* getQLineEditByName(QString name);
		QList<QLineEdit*> getAllQLineEdit();
										//获取 - 下拉框（没有则返回nullptr）
		QComboBox* getQComboBoxByName(QString name);
		QList<QComboBox*> getAllQComboBox();
										//获取 - 勾选框（没有则返回nullptr）
		QCheckBox* getQCheckBoxByName(QString name);
		QList<QCheckBox*> getAllQCheckBox();
										//获取 - 数字控件（没有则返回nullptr）
		QSpinBox* getQSpinBoxByName(QString name);
		QList<QSpinBox*> getAllQSpinBox();
										//获取 - 数字控件double（没有则返回nullptr）
		QDoubleSpinBox* getQDoubleSpinBoxByName(QString name);
		QList<QDoubleSpinBox*> getAllQDoubleSpinBox();

										//获取 - 群组-并列控件（没有则返回nullptr）
		QWidget* getOneLineByName(QString class_name);
		QList<QWidget*> getOneLineByDepth(int depth);
		QList<QWidget*> getAllOneLine();
										//获取 - 群组-群组控件（没有则返回nullptr）
		QGroupBox* getQGroupBoxByName(QString class_name);
		QList<QGroupBox*> getQGroupBoxByDepth(int depth);
		QList<QGroupBox*> getAllQGroupBox();
										//获取 - 群组-块组控件（没有则返回nullptr）
		QWidget* getQWidgetGroupByName(QString class_name);
		QList<QWidget*> getQWidgetGroupByDepth(int depth);
		QList<QWidget*> getAllQWidgetGroup();
		
	//-----------------------------------
	//----参数
	public:
										//参数 - 获取名称列表
		QStringList getAllNameChinese();
		QStringList getAllNameEnglish();
		QStringList getAllNameShowing();
										//参数 - 获取参数值
		QString getValueByName(QString name);
										//参数 - 设置某个控件的值
		void setValueByName(QString name,QString value);

										//参数 - 获取参数集（ { 名字:值 } ）
		virtual QJsonObject getJsonObject();
										//参数 - 设置参数集（ { 名字:值 } ）
		virtual void setJsonObject(QJsonObject p);
										//参数 - 设置参数集（如果空则填默认值）
		virtual void setJsonObjectAutoDefault(QJsonObject p);
										//参数 - 检查必填项是否非空
		virtual bool isAllNotNullConform();
		
	//-----------------------------------
	//----构建（私有）
	protected:
		QList<QWidget*> m_frames;		//构建框架
		int m_depth;					//深度（建立ui时使用）
	protected:
										//构建 - 变量容器
		virtual C_FastForm* _createForm();
										//构建 - 初始化
		virtual void _initForm(C_FastForm* c_f);
										//构建 - 单个控件
		virtual void _addSingle(C_FastClassParam c_fp, QGridLayout* layout, int curRow, int curCol);
										//布局 - 并列控件
		virtual void _addOneLine(C_FastClass c_fc, QGridLayout* layout, int curRow, int curCol, bool editable = true);
										//布局 - 群组控件
		virtual void _addQGroupBox(C_FastClass c_fc, QGridLayout* layout, int curRow, int curCol, bool editable = true);
										//布局 - 块组控件
		virtual void _addQWidgetGroup(C_FastClass c_fc, QGridLayout* layout, int curRow, int curCol, bool editable = true);
	protected:
										//构建 - 表单组（没有则返回nullptr）
		C_FastForm* getFastFormByName(QString name);
		QList<C_FastForm*> getAllFastFormByType(QString type);
										//构建 - 获取深度
		int getDepth(QString statusTipData);

};
/*
-----==========================================================-----
		类：		表单组.h
		所属模块：	工具模块
		功能：		多个表单的组合结构。
-----==========================================================-----
*/
class C_FastForm{
	public:
		C_FastForm();		
		~C_FastForm();		
	public:
		C_FastClassParam params;					//参数

		QLabel* label;								//标签（显示名称）
		QLineEdit* lineEdit;						//文本编辑框
		QComboBox* comboBox;						//下拉框
		QStyledItemDelegate* comboBoxDelegate;		//下拉框选项设置
		QCheckBox* checkBox;						//勾选框
		QSpinBox* spinBox;							//数字控件
		QDoubleSpinBox* doubleSpinBox;				//数字控件（double）
		QPlainTextEdit* plainTextEdit;				//文本域

		QLabel* label_suffix;						//后缀
		QSpacerItem* spacerItem;					//弹簧

	public:
		virtual void xxx();		//强制让vs识别，子类转父类用
};