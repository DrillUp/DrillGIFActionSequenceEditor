#pragma once


/*
-----==========================================================-----
		�ࣺ		���Ĺ�����.h
		�汾��		v1.01
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ṩ����ת��������ȹ��ܡ�
					����ϸ��.cpp��
-----==========================================================-----
*/

class S_ChineseManager : public QObject
{
	Q_OBJECT

	public:
		S_ChineseManager();
		~S_ChineseManager();
		static S_ChineseManager* cur_manager;		//����
		static S_ChineseManager* getInstance();		//��������ȡ�Լ�
		
	//-----------------------------------
	//----����
	public:
										//���� - �ж������ַ�
										//		��˵������������������ַ����򷵻�true
		static bool hasAnyChineseCharacter(QString test_str);

	//-----------------------------------
	//----����ƴ��
	private:
		QTextCodec* m_textCodeGBK;
	public:
										//����ƴ�� - ��ȡ����ĸ
										//		��˵����������Сд��ĸ�����ַ�ʱ������""����Ƨ��ʱ������"@"��
		QString getChineseFirstSpell(QString chinese_string);
										//����ƴ�� - ��ȡ���ĵ�ƴ��
		QString getChinesePinYin(QString chinese_string);
	private:
										//����ƴ�� - ˽�з���
		bool codeIn(wchar_t start, wchar_t end, wchar_t code);
		char convert(int n);

		
	//-----------------------------------
	//----����ת����
	public:
										//����ת���� - תСд
		QString toChineseNumLower(int num);
		QString toChineseNumLower(long num);
										//����ת���� - ת��д
		QString toChineseNumUpper(int num);
		QString toChineseNumUpper(long num);
	private:
		QString toChineseNum(double value, QStringList numerical_unit, QStringList numerical_value);

};
