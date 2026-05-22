#pragma once


/*
-----==========================================================-----
		类：		中文管理器.h
		版本：		v1.03
		作者：		drill_up
		所属模块：	工具模块
		功能：		提供中文转换、排序等功能。
					（详细见.cpp）
-----==========================================================-----
*/

class S_ChineseManager : public QObject
{
	Q_OBJECT

	public:
		S_ChineseManager();
		~S_ChineseManager();
		static S_ChineseManager* cur_manager;		//单例
		static S_ChineseManager* getInstance();		//单例，获取自己
		

	//-----------------------------------
	//----基本
	public:
										//基本 - 判断中文字符
										//		【说明】：如果存在中文字符，则返回true。
		static bool hasAnyChineseCharacter(QString test_str);


	//-----------------------------------
	//----首字母
	private:
		QTextCodec* m_textCodeGBK;
	public:
										//首字母 - 获取
										//		【参数】：中文字符串
										//		【返回】：返回小写字母字符串；生僻字时，字符为"@"；空字符时，返回""。
		QString getChineseFirstSpell(QString chinese_string);
										//首字母 - 获取（排除指定字符）
										//		【参数1】：中文字符串
										//		【参数2】：排除的指定字符列表
										//		【返回】：返回 排除指定字符 的小写字母字符串；生僻字时，字符为"@"；空字符时，返回""。
		QString getChineseFirstSpell_Exclude(QString chinese_string, QList<QChar> exclude_char_list);
										//首字母 - 获取（排除常规符号）
										//		【参数】：中文字符串
										//		【返回】：返回 排除常规符号 的小写字母字符串；生僻字时，字符为"@"；空字符时，返回""。
		QString getChineseFirstSpell_ExcludeBracket(QString chinese_string);
	public:
										//首字母 - 获取常规符号
		QList<QChar> getBracketList();


	//-----------------------------------
	//----中文拼音
	public:
										//中文拼音 - 获取
		QString getChinesePinYin(QString chinese_string);
	private:
										//中文拼音 - 单个字符判断（私有）
		bool codeIn(wchar_t start, wchar_t end, wchar_t code);
										//中文拼音 - 单个字符转换（私有）
		char convert(int n);

		
	//-----------------------------------
	//----数字转中文
	public:
										//数字转中文 - 转小写
		QString toChineseNumLower(int num);
		QString toChineseNumLower(long num);
										//数字转中文 - 转大写
		QString toChineseNumUpper(int num);
		QString toChineseNumUpper(long num);
	private:
		QString toChineseNum(double value, QStringList numerical_unit, QStringList numerical_value);

};
