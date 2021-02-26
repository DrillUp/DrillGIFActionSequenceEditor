#pragma once

#include <QWidget>

/*
-----==========================================================-----
		类：		快速类定义.h
		版本：		v1.13
		作者：		drill_up
		所属模块：	工具模块
		功能：		定义一个表单填写用的类定义，快速表单、快速窗口通用。
					【这里类似于定义model，所有字段都是字符串】
					【类名、参数名，用中文】
					（详细见cpp）
-----==========================================================-----
*/
class C_FastClassParam;
class C_FastClass{

	public:
		C_FastClass();
		C_FastClass(QString className);
		~C_FastClass();
		
	//-----------------------------------
	//----一设（确定数据类型）
	public:
							//一设 - 类名
		void setClassName(QString classNameChinese);
		void setClassNameEnglish(QString classNameEnglish);
		void setClassNameShowing(QString classNameShowing);
		QString getClassNameChinese();
		QString getClassNameEnglish();
		QString getClassNameShowing();
							//一设 - Double
							//（默认ui：QLineEdit）
							//（默认值示意：字符串"0.36","0.1M"）
		void addDouble(QString paramNameChinese);
		void addDouble(QString paramNameChinese, QString defaultValue);
							//一设 - Int
							//（默认ui：QLineEdit）
							//（默认值示意：字符串"100"）
		void addInt(QString paramNameChinese);
		void addInt(QString paramNameChinese, QString defaultValue);
							//一设 - QString
							//（默认ui：QLineEdit）
							//（默认值示意："字符串"）
		void addQString(QString paramNameChinese);
		void addQString(QString paramNameChinese, QString defaultValue);
							//一设 - Int多选项
							//（默认ui：QComboBox）
							//（默认值示意：0表示索引编号）
							//（indexList为真实下拉索引值，indexShowingList为显示的下拉选项）
		void addIntWithChoose(QString paramNameChinese, int defaultIndex, QList<int> indexList);
		void addIntWithChoose(QString paramNameChinese, int defaultIndex, QList<int> indexList, QStringList indexShowingList);
							//一设 - QString多选项
							//（默认ui：QComboBox）
							//（默认值示意："真实选项值"）
							//（chooseList为真实下拉选项，chooseShowingList为显示的下拉选项）
		void addQStringWithChoose(QString paramNameChinese, QString defaultValue, QStringList chooseList);
		void addQStringWithChoose(QString paramNameChinese, QString defaultValue, QStringList chooseList, QStringList chooseShowingList);
							//一设 - Boolean
							//（默认ui：QCheckBox）
							//（默认值示意：false）
		void addBoolean(QString paramNameChinese);
		void addBoolean(QString paramNameChinese, QString defaultValue);
							//一设（嵌套） - 类嵌套
		void addFastClassQWidget(C_FastClass fastClass);
		void addFastClassOneLine(C_FastClass fastClass);
		void addFastClassQGroupBox(C_FastClass fastClass);
		C_FastClass getChildClass(QString classNameChinese);

	//-----------------------------------
	//----二设（确定数据组成）
	public:
							//二设 - 英文名
		void setParamEnglishName(QString paramNameChinese, QString paramNameEnglish);
							//二设 - 显示名
		void setParamShowingName(QString paramNameChinese, QString showingName);
							//二设 - 必填项
		void setParamNotNull(QString paramNameChinese, bool notNull);
							//二设 - 后缀
		void setParamSuffix(QString paramNameChinese, QString suffix);
							//二设 - SI写法（注意，只对QLineEdit控件有效）
		void setParamSI(QString paramNameChinese, bool isSI);
		
	//-----------------------------------
	//----三设（确定UI配置）
	public:
							//三设 - 不可编辑
		void setParamEditable(QString paramNameChinese, bool isEditable);
		void setAllParamEditable(bool isEditable);
							//三设 - 设置Int类型为数字控件
		void setInt_UI_ToQSpinBox(QString paramNameChinese, long valueMin, long valueMax);
		void setInt_UI_ToQSpinBox(QString paramNameChinese, long valueMin, long valueMax, long singleStep);
		void setInt_UI_ToQSpinBox(QString paramNameChinese, long valueMin, long valueMax, long singleStep, QString prefix, QString suffix);
							//三设 - 设置Double类型为数字控件
		void setDouble_UI_ToQDoubleSpinBox(QString paramNameChinese, double valueMin, double valueMax);
		void setDouble_UI_ToQDoubleSpinBox(QString paramNameChinese, double valueMin, double valueMax, double singleStep, int decimals);
		void setDouble_UI_ToQDoubleSpinBox(QString paramNameChinese, double valueMin, double valueMax, double singleStep, int decimals, QString prefix, QString suffix);
							//三设 - 设置Boolean类型为下拉框
		void setBoolean_UI_ToQCombox(QString paramNameChinese, QString trueText, QString falseText);
							//三设 - 设置QString类型为文本域
		void setQString_UI_ToQPlainTextEdit(QString paramNameChinese, int plainTextHeight);
		
	//-----------------------------------
	//----四设（临时修改某配置）
	public:
							//四设 - 默认值修改
		void setParamDefaultValue(QString paramNameChinese, QString defaultValue);
		QString getParamDefaultValue(QString paramNameChinese);
							//四设 - 下拉框的可选项修改
		void setParamChooseList(QString paramNameChinese, QStringList chooseList);
		void setParamChooseShowingList(QString paramNameChinese, QStringList chooseShowingList);
		QStringList getParamChooseList(QString paramNameChinese);
		QStringList getParamChooseShowingList(QString paramNameChinese);


	//-----------------------------------
	//----高级
	public:
							//高级 - 所在列数
		void setParamColumn(QString paramNameChinese, int col);
							//高级 - 主键（只能设置一个，不能与其它含主键的子类嵌套）
		void setClassPrimaryKeyId(QString classNameChinese);
		void setClassPrimaryKeyName(QString classNameChinese);
		QString getClassPrimaryKeyId();
		QString getClassPrimaryKeyName();

	//-----------------------------------
	//----获取
	public:
							//获取 - 快速类参数（含FastClass）
		QList<C_FastClassParam> getParamList();
							//获取 - 快速类参数（FastClass全部被摊开）
		QList<C_FastClassParam> getConvertedParamList();
							//获取 - 所有参数中文名（包括类套类的参数）
		QStringList getParamChineseNameList();
							//获取 - 所有参数英文名（包括类套类的参数）
		QStringList getParamEnglishNameList();
							//获取 - 判断空
		bool isNull();
							//获取 - 样例数据（建议在一设、二设、三设执行完之后获取）
		QJsonObject getExampleDataInChinese();
		QJsonObject getExampleDataInEnglish();
		QJsonObject getExampleDataInShowing();

		
	//-----------------------------------
	//----特殊功能
	public:
							//特殊功能 - 将QJsonObject的数据，根据该类的格式，填充到table中（中文）
		void fillDataInTableWidget(QTableWidget* tableWidget, QList<QJsonObject> data_list);
							//特殊功能 - 将QJsonObject的数据，根据该类的格式，填充到table中（英文）
		void fillDataInTableWidgetEnglish(QTableWidget* tableWidget, QList<QJsonObject> data_list);
							//特殊功能 - jsonobj 英文 -> 中文
		QJsonObject convertEnglishToChinese(QJsonObject data);
							//特殊功能 - jsonobj 英文 -> 中文（转不了的扔掉）
		QJsonObject convertEnglishToChineseExcludely(QJsonObject data);
							//特殊功能 - jsonobj 中文 -> 英文
		QJsonObject convertChineseToEnglish(QJsonObject data);
							//特殊功能 - jsonobj 中文 -> 英文（转不了的扔掉）
		QJsonObject convertChineseToEnglishExcludely(QJsonObject data);
							//特殊功能 - 去掉中文名的标识格式
		QString removeSpecialChineseCharacter(QString paramNameChinese);

	//-----------------------------------
	//----私有参数
	protected:
		QString classNameChinese;			//类中文名
		QString classNameEnglish;			//类英文名
		QString classNameShowing;			//类显示名
		QList<C_FastClassParam> paramList;	//类参数集
		QStringList nameChinese_list;		//全部中文名
		QStringList nameEnglish_list;		//全部英文名
		QString classPrimaryKeyId;			//主键id
		QString classPrimaryKeyName;		//主键name
	protected:
		C_FastClassParam getParamByName(QString nameChinese);
		int getIndexByName(QString nameChinese);
		void checkDuplicatedChineseValue(QString nameChinese);
		void checkDuplicatedEnglishValue(QString nameEnglish);
};
/*
-----==========================================================-----
		类：		快速类参数.h
-----==========================================================-----
*/
class C_FastClassParam{

	public:
		C_FastClassParam();
		~C_FastClassParam();

	public:
		// > 一设
		QString nameChinese;				//中文名
		QString type;						//数据类型
		QString widgetType;					//表单类型
		QString defaultValue;				//默认值
		QStringList chooseList;				//选项列表

		// > 二设
		QString nameEnglish;				//英文名
		QString nameShowing;				//实际显示名
		QStringList chooseShowingList;		//实际显示选项
		bool notNull;						//必填项
		QString suffix;						//后缀（单位）
		bool isSI;							//SI写法

		// > 三设
		bool isEditable;					//可编辑
		int comboxHeight;					//下拉框 - 选项高度
		double valueMin;					//数字控件 - 最小值
		double valueMax;					//数字控件 - 最大值
		double singleStep;					//数字控件 - 上下阶数
		int decimals;						//数字控件 - 位数
		QString num_prefix;					//数字控件 - 前缀
		QString num_suffix;					//数字控件 - 后缀
		int plainTextHeight;				//文本域 - 高度

		// > 高级
		int columnPos;						//所在列
		C_FastClass fastClass;				//嵌套体

		// > EX用数据
		QString tempA;						//缓冲数据A
		QString tempB;						//缓冲数据B
		QString tempC;						//缓冲数据C
		QString tempD;						//缓冲数据D
		QString tempE;						//缓冲数据E
		QString tempF;						//缓冲数据F

	//-----------------------------------
	//----获取
	public:
									//运算符重载
		const bool operator== (const C_FastClassParam& a)const;
									//判断空
		bool isNull();
									//获取 - 显示文本名
		QString getShowingName();
	public:
									//获取 - 去掉中文名的标识用字符（静态函数）
		static QString removeSpecialChineseCharacter(QString nameChinese);

};