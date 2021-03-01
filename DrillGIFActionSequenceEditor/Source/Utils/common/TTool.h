#pragma once
#include <QtWidgets>
#include <QDebug>
#include <QString>
#include <QTextCodec>
#include <QDialogButtonBox>
#include <QRegExpValidator>
#include <vector>
#pragma execution_character_set("utf-8")

/*
-----==========================================================-----
		类：		Drill_up工具包.h
		版本：		v1.16
		作者：		drill_up
		编码：		UTF-8
		所属模块：	工具模块
		功能：		一些基本的工具函数，以及代码。（用于防坑）
					（详细见cpp）
-----==========================================================-----
*/


class TTool
{
	public:
		TTool();		//构造函数
		~TTool();		//析构函数

	//-----------------------------------------------------
	//----http操作
	public:
		/*http操作
			【说明】：post的内容转QByteArray。*/
		static QByteArray encodeUrlData(QJsonObject json_obj);
		/*http操作
			【说明】：post的内容转数组。*/
		static QString encodeUrlDataString(QJsonObject json_obj);

	//-----------------------------------------------------
	//----html操作
	public:
		/*html操作
			【说明】：QString的内容转html纯文本。*/
		static QString _to_HtmlText_(QString text);
		/*html操作
			【说明】：QColor转html颜色文本。*/
		static QString _to_HtmlColor_(QColor color);
		static QString _to_HtmlColor_rgb_(QColor color);
		static QString _to_HtmlColor_rgba_(QColor color);

	//-----------------------------------------------------
	//----文件名操作
	public:
		/*文件名-获取后缀
			【说明】：获取字符串文件名的后缀。*/
		static QString getFileSuffix(QString file_name);
		/*文件名-换后缀
			【说明】：获取换掉后缀的新文件名。*/
		static QString getFileNameWithNewSuffix(QString file_name, QString new_suffix);
		/*文件名-替换非法字符
			【说明】：将文件名中的非法字符替换成别的字符。*/
		static QString getFixedFileName(QString file_name, QString replace_str = "");
		/*文件名-获取文件夹下的所有文件名
			【参数】："F:/name" "F:\\name"（文件夹）（要含根目录 如F:）
			【返回】："name.xxx"（文件）（不含路径）
			【说明】：如果路径格式不正确，则返回空。 */
		static QStringList getChildFileName(QString dir_name);
		/*文件名-获取文件夹下的所有文件夹名
			【参数】："F:/name" "F:\\name"（文件夹）（要含根目录 如F:）
			【返回】："name"（文件夹）（不含路径）
			【说明】：如果路径格式不正确，则返回空。 */
		static QStringList getChildDirName(QString dir_name);
		
	//-----------------------------------------------------
	//----buttonBox中文化

	public:
		/*buttonBox中文化
			【说明】：一次性全部按钮设置默认的中文。*/
		static void _chinese_(QDialogButtonBox *buttonBox);
		/*buttonBox中文化
			【说明】：对指定的按钮设置中文，如果按钮不存在，不会报错。*/
		void TTool::_chinese_(QDialogButtonBox *buttonBox, QDialogButtonBox::StandardButton type, QString name);


	//-----------------------------------------------------
	//----ui快速赋值 b给a

	public:
		/*b的值赋给a
			【说明1】：如果b的值为空字符串，则赋default_的值。
			【说明2】：如果QObject对象为nullptr，则不操作赋值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都获取到QString。*/
		static void _QString_(QString *a, QComboBox *b, QString default_ = "");
		/*b的值赋给a
			【说明1】：如果b的值为空字符串，则赋default_的值。
			【说明2】：如果QObject对象为nullptr，则不操作赋值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都获取到QString。*/
		static void _QString_(QString *a, QTableWidgetItem *b, QString default_ = "");
		/*b的值赋给a
			【说明1】：如果b的值为空字符串，则赋default_的值。
			【说明2】：如果QObject对象为nullptr，则不操作赋值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都获取到QString。*/
		static void _QString_(QString *a, QTextEdit *b, QString default_ = "");
		/*b的值赋给a
			【说明1】：如果b的值为空字符串，则赋default_的值。
			【说明2】：如果QObject对象为nullptr，则不操作赋值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都获取到QString。*/
		static void _QString_(QString *a, QPlainTextEdit *b, QString default_ = "");
		/*b的值赋给a
			【说明1】：如果b的值为空字符串，则赋default_的值。
			【说明2】：如果QObject对象为nullptr，则不操作赋值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都获取到QString。*/
		static void _QString_(QString *a, QLineEdit *b, QString default_ = "");
		/*b的值赋给a
			【说明1】：如果b的值为空字符串，则赋default_的值。
			【说明2】：如果QObject对象为nullptr，则不操作赋值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都获取到QString。*/
		static void _QString_(QString *a, QDoubleSpinBox *b );
		/*b的值赋给a
			【说明1】：如果b的值为空字符串，则赋default_的值。
			【说明2】：如果QObject对象为nullptr，则不操作赋值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都获取到QString。*/
		static void _QString_(QString *a, QSpinBox *b );

		/*b的值赋给a（返回值型）
			【返回】：指定的QObject中的字符串
			【说明1】：如果b的值为空字符串，则返回default_的值。
			【说明2】：如果QObject对象为nullptr，则返回default_的值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都获取到QString。*/
		static QString _QString_( QComboBox *b, QString default_ = "");
		/*b的值赋给a（返回值型）
			【返回】：指定的QObject中的字符串
			【说明1】：如果b的值为空字符串，则返回default_的值。
			【说明2】：如果QObject对象为nullptr，则返回default_的值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都获取到QString。*/
		static QString _QString_( QTableWidgetItem *b, QString default_ = "");
		/*b的值赋给a（返回值型）
			【返回】：指定的QObject中的字符串
			【说明1】：如果b的值为空字符串，则返回default_的值。
			【说明2】：如果QObject对象为nullptr，则返回default_的值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都获取到QString。*/
		static QString _QString_( QTextEdit *b, QString default_ = "");
		/*b的值赋给a（返回值型）
			【返回】：指定的QObject中的字符串
			【说明1】：如果b的值为空字符串，则返回default_的值。
			【说明2】：如果QObject对象为nullptr，则返回default_的值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都获取到QString。*/
		static QString _QString_(QPlainTextEdit *b, QString default_ = "");
		/*b的值赋给a（返回值型）
			【返回】：指定的QObject中的字符串
			【说明1】：如果b的值为空字符串，则返回default_的值。
			【说明2】：如果QObject对象为nullptr，则返回default_的值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都获取到QString。*/
		static QString _QString_(QLineEdit *b, QString default_ = "");
		/*b的值赋给a（返回值型）
			【返回】：指定的QObject中的字符串
			【说明1】：如果b的值为空字符串，则返回default_的值。
			【说明2】：如果QObject对象为nullptr，则返回default_的值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都获取到QString。*/
		static QString _QString_(QDoubleSpinBox *b );
		/*b的值赋给a（返回值型）
			【返回】：指定的QObject中的字符串
			【说明1】：如果b的值为空字符串，则返回default_的值。
			【说明2】：如果QObject对象为nullptr，则返回default_的值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都获取到QString。*/
		static QString _QString_(QSpinBox *b );

		/*b的值赋给a
			【说明1】：如果b的值为空字符串，则赋default_的值。
			【说明2】：如果QObject对象为nullptr，则不操作赋值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都赋值QString。*/
		static void _QString_(QComboBox *a, QString *b, QString default_ = "");
		/*b的值赋给a
			【说明1】：如果b的值为空字符串，则赋default_的值。
			【说明2】：如果QObject对象为nullptr，则不操作赋值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都赋值QString。*/
		static void _QString_(QTableWidgetItem *a, QString *b, QString default_ = "");
		/*b的值赋给a
			【说明1】：如果b的值为空字符串，则赋default_的值。QTextEdit使用的setText()，标签<xx>会被包含进去。
			【说明2】：如果QObject对象为nullptr，则不操作赋值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都赋值QString。*/
		static void _QString_(QTextEdit *a, QString *b, QString default_ = "");
		/*b的值赋给a
			【说明1】：如果b的值为空字符串，则赋default_的值。
			【说明2】：如果QObject对象为nullptr，则不操作赋值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都赋值QString。*/
		static void _QString_(QPlainTextEdit *a, QString *b, QString default_ = "");
		/*b的值赋给a
			【说明1】：如果b的值为空字符串，则赋default_的值。
			【说明2】：如果QObject对象为nullptr，则不操作赋值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都赋值QString。*/
		static void _QString_(QLineEdit *a, QString *b, QString default_ = "");
		/*b的值赋给a
			【说明1】：如果b的值为空字符串，则赋default_的值。
			【说明2】：如果QObject对象为nullptr，则不操作赋值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都赋值QString。*/
		static void _QString_(QDoubleSpinBox *a, QString *b, QString default_ = "");
		/*b的值赋给a
			【说明1】：如果b的值为空字符串，则赋default_的值。
			【说明2】：如果QObject对象为nullptr，则不操作赋值。
			【说明3】：注意看该方法名，_QString_表示无论什么ui，都赋值QString。*/
		static void _QString_(QSpinBox *a, QString *b, QString default_ = "");


		/*b的值赋给a
			【说明】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _double_(double *a, QComboBox *b);
		/*b的值赋给a
			【说明】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _double_(double *a, QTableWidgetItem *b);
		/*b的值赋给a
			【说明】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _double_(double *a, QLineEdit *b);
		/*b的值赋给a
			【说明】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _double_(double *a, QDoubleSpinBox *b);
		/*b的值赋给a
			【说明】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _double_(QComboBox *a, double *b);
		/*b的值赋给a
			【说明】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _double_(QComboBox *a, double b);
		/*b的值赋给a
			【说明】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _double_(QTableWidgetItem *a, double *b);
		/*b的值赋给a
			【说明】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _double_(QTableWidgetItem *a, double b);
		/*b的值赋给a
			【说明】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _double_(QLineEdit *a, double *b);
		/*b的值赋给a
			【说明】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _double_(QLineEdit *a, double b);
		/*b的值赋给a
			【说明】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _double_(QDoubleSpinBox *a, double *b);
		/*b的值赋给a
			【说明】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _double_(QDoubleSpinBox *a, double b);
		
		/*b的值赋给a
			【说明1】：这里赋的是index的值。
			【说明2】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _int_(int *a, QComboBox *b);
		/*b的值赋给a
			【说明】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _int_(int *a, QTableWidgetItem *b);
		/*b的值赋给a
			【说明】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _int_(int *a, QLineEdit *b);
		/*b的值赋给a
			【说明】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _int_(int *a, QSpinBox *b);
		
		/*b的值赋给a
			【说明1】：这里赋的是index的值。
			【说明2】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _int_(QComboBox *a, int *b);
		/*b的值赋给a
			【说明】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _int_(QTableWidgetItem *a, int *b);
		/*b的值赋给a
			【说明】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _int_(QLineEdit *a, int *b);
		/*b的值赋给a
			【说明】：如果QObject对象为nullptr，则不操作赋值。*/
		static void _int_(QSpinBox *a, int *b);

	//-----------------------------------------------------
	//----数字

	public:
		/*榨取int
			【说明】：榨取QString中的一个int值。*/
		static int _to_int_(QString a);
		/*榨取int
			【说明】：榨取QString中所有int值。*/
		static QList<int> _to_ints_(QString a);
		/*榨取int（SI）
			【说明1】：榨取QString中的一个int值。
			【说明2】：只识别kMG三个单位，1k = 1000，另外，小数、1T太大int装不下。*/
		static int _to_int_bySI_(QString a);
		/*榨取int（SI）
			【说明1】：榨取QString中所有int值。
			【说明2】：只识别kMG三个单位，1k = 1000，另外，小数、1T太大int装不下。*/
		static QList<int> _to_ints_bySI_(QString a);

		/*榨取double
			【说明】：榨取QString中的一个double值。*/
		static double _to_double_(QString a);
		/*榨取double
			【说明】：榨取QString中所有double值列表。*/
		static QList<double> _to_doubles_(QString a);
		/*榨取double
			【说明】：榨取QString中的一个double值，识别科学计数法，比如1.02e3。*/
		static double _to_double_e_(QString a);
		/*榨取double
			【说明】：榨取QString中所有double值列表，识别科学计数法。
			【举例】："1.02e3das1e3d"中有两个double："1.02e3"和"1e3"。*/
		static QList<double> _to_doubles_e_(QString a);
		/*榨取double（SI）
			【说明】：榨取QString中的一个double值。
			【举例】：识别munkMGT单位，1.02k = 1020。*/
		static double _to_double_bySI_(QString a);
		/*榨取double（SI）
			【说明】：榨取QString中所有double值列表。
			【举例】：识别munkMGT单位，1.02k = 1020。*/
		static QList<double> _to_doubles_bySI_(QString a);

		/*字符零填充
			【说明】：给指定的数字值填充零，也可以是其它字符。*/
		static QString _zeroFill_(int num, int digit);
		static QString _zeroFill_(long num, int digit);
		static QString _zeroFill_(int num, int digit, QLatin1Char ch);
		static QString _zeroFill_(long num, int digit, QLatin1Char ch);
		
	//-----------------------------------------------------
	//----单位与SI

	public:
		/*SI值转换
			【说明】：将int值转换为SI写法的字符串。*/
		static QString _to_QString_bySI_(int a);
		/*SI值转换
			【说明】：将double值转换为SI写法的字符串。*/
		static QString _to_QString_bySI_(double a);
		/*SI值转换
			【说明】：将QString的double值转换为SI写法的字符串。*/
		static QString _to_QString_bySI_(QString a);

		/*double值添加单位
			【说明】：给double值添加单位。*/
		static QString _to_Unit_(double d, QString unit);
		/*SI值添加单位
			【说明】：给字符串添加单位。*/
		static QString _to_Unit_(QString si_qstring, QString unit);
		/*double值添加单位
			【说明】：给double值添加单位，如果值为0，则输出指定字符串。*/
		static QString _to_Unit_includeEmptyCheck(double d, QString unit, QString default_ = "-");
		/*SI值添加单位
			【说明】：给SI字符串添加单位，如果值为0，则输出指定字符串。*/
		static QString _to_Unit_includeEmptyCheck(QString si_qstring, QString unit, QString default_ = "-");

	//-----------------------------------------------------
	//----正则
	public:
		/*获取整数正则
			【说明】：获取只识别int的正则公式。*/
		static QString _get_re_int_();
		/*获取整数正则（SI）
			【说明】：获取只识别int的正则公式，包含SI写法的识别。*/
		static QString _get_re_int_SI_();
		/*获取小数正则（不含e）
			【说明】：获取识别double的正则公式，不含科学计数法。*/
		static QString _get_re_double_();
		/*获取小数正则（含e）
			【说明】：获取识别double的正则公式，包含科学计数法识别。*/
		static QString _get_re_double_e_();
		/*获取小数正则（SI）
			【说明】：获取识别double的正则公式，包含SI写法的识别。*/
		static QString _get_re_double_SI_();
		/*获取整数正则校验器
			【说明】：获取只识别int的正则公式。*/
		static QRegExpValidator* _getValidator_re_int_();
		/*获取整数正则校验器（SI）
			【说明】：获取只识别int的正则公式，包含SI写法的识别。*/
		static QRegExpValidator* _getValidator_re_int_SI_();
		/*获取小数正则校验器（不含e）
			【说明】：获取识别double的正则公式，不含科学计数法。*/
		static QRegExpValidator* _getValidator_re_double_();
		/*获取小数正则校验器（含e）
			【说明】：获取识别double的正则公式，包含科学计数法识别。*/
		static QRegExpValidator* _getValidator_re_double_e_();
		/*获取小数正则校验器（SI）
			【说明】：获取识别double的正则公式，包含SI写法的识别。*/
		static QRegExpValidator* _getValidator_re_double_SI_();
		/*判断整数
			【说明】：只识别int。*/
		static bool _match_re_int_(QString test_str);
		/*判断整数（SI）
			【说明】：只识别int，包含SI写法的识别。*/
		static bool _match_re_int_SI_(QString test_str);
		/*判断小数（不含e）
			【说明】：不含科学计数法。*/
		static bool _match_re_double_(QString test_str);
		/*判断小数（含e）
			【说明】：包含科学计数法。*/
		static bool _match_re_double_e_(QString test_str);
		/*判断小数（SI）
			【说明】：包含SI写法和科学计数法。*/
		static bool _match_re_double_SI_(QString test_str);
		/*判断中文
			【说明】：如果字符串中存在中文字符则返回true。*/
		static bool _has_any_chineseCharacter_(QString test_str);
		
	//-----------------------------------------------------
	//----快速操作
	public:
		/*double转字符串
			【返回】：字符串
			【说明】：没什么技术含量的转换，但是函数进行了统一绑定，且精度有16位。*/
		static QString _auto_(double d);
		/*int转字符串
			【返回】：字符串
			【说明】：没什么技术含量的转换，但是函数进行了统一绑定。*/
		static QString _auto_(int i);
		/*uint转字符串
			【返回】：字符串
			【说明】：没什么技术含量的转换，但是函数进行了统一绑定。*/
		static QString _auto_(uint i);
		/*唯一ID字符串
			【返回】：唯一标识的字符串*/
		static QString _unique_id_();
		
	//-----------------------------------------------------
	//----快速操作 - 列表转换
	public:
		/*字符串列表 --> 布尔列表
			【说明】："True"和"TRUE"也能转true。*/
		static QList<bool> _QList_QStringToBool_(QList<QString> list);
		/*字符串列表 --> 数字列表*/
		static QList<int> _QList_QStringToInt_(QList<QString> list);
		/*字符串列表 --> 小数列表*/
		static QList<double> _QList_QStringToDouble_(QList<QString> list);
		/*字符串列表 --> obj列表*/
		static QList<QJsonObject> _QList_QStringToQJsonObject_(QList<QString> list);
		/*布尔列表 --> 字符串列表*/
		static QList<QString> _QList_BoolToQString_(QList<bool> list);
		/*数字列表 --> 字符串列表。*/
		static QList<QString> _QList_IntToQString_(QList<int> list);
		/*小数列表 --> 字符串列表。*/
		static QList<QString> _QList_DoubleToQString_(QList<double> list);
		/*obj列表 --> 字符串列表*/
		static QList<QString> _QList_QJsonObjectToQString_(QList<QJsonObject> list);

		/*数字列表 --> 小数列表*/
		static QList<double> _QList_IntToDouble_(QList<int> list);
		/*小数列表 --> 数字列表（取整）*/
		static QList<int> _QList_DoubleToInt_floor_(QList<double> list);
		/*小数列表 --> 数字列表（取顶）*/
		static QList<int> _QList_DoubleToInt_ceil_(QList<double> list);
		/*小数列表 --> 数字列表（四舍五入）*/
		static QList<int> _QList_DoubleToInt_round_(QList<double> list);

	//-----------------------------------------------------
	//----QJson
	public:
		/*b的值赋给a
			【说明】：QJsonObject赋值给另一个，相同的key会被覆盖。*/
		static void _QJsonObject_put_(QJsonObject *a, QJsonObject b);

		/*Json列表的字符串 --> 列表
			【说明】：冗余操作的简化，注意是utf8的字符串。*/
		static QList<QJsonObject> _QJsonArrayString_To_QListObj_(QString context);
		static QList<bool> _QJsonArrayString_To_QListBool_(QString context);
		static QList<int> _QJsonArrayString_To_QListInt_(QString context);
		static QList<double> _QJsonArrayString_To_QListDouble_(QString context);
		static QList<QString> _QJsonArrayString_To_QListQString_(QString context);
		
		/*列表 --> Json列表的字符串 */
		static QString _QListObj_To_QJsonArrayString_(QList<QJsonObject> data);
		static QString _QListBool_To_QJsonArrayString_(QList<bool> data);
		static QString _QListInt_To_QJsonArrayString_(QList<int> data);
		static QString _QListDouble_To_QJsonArrayString_(QList<double> data);
		static QString _QListQString_To_QJsonArrayString_(QList<QString> data);

};


// > getter和setter宏定义
#define TTOOL_DATA_PROPERTY(type, var) \
	protected: \
	type _##var; \
	public: \
	type get##var() { return _##var; }\
	void set##var(type val) { _##var = val; }

