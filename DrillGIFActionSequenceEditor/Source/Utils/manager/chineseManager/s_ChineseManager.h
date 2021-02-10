#pragma once


/*
-----==========================================================-----
		类：		中文管理器.h
		版本：		v1.01
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
										//		【说明】：如果存在中文字符，则返回true
		static bool hasAnyChineseCharacter(QString test_str);

	//-----------------------------------
	//----中文拼音
	private:
		QTextCodec* m_textCodeGBK;
	public:
										//中文拼音 - 获取首字母
										//		【说明】：返回小写字母；空字符时，返回""；生僻字时，返回"@"。
		QString getChineseFirstSpell(QString chinese_string);
										//中文拼音 - 获取中文的拼音
		QString getChinesePinYin(QString chinese_string);
	private:
										//中文拼音 - 私有方法
		bool codeIn(wchar_t start, wchar_t end, wchar_t code);
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
