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
		�ࣺ		Drill_up���߰�.h
		�汾��		v1.20
		���ߣ�		drill_up
		���룺		UTF-8
		����ģ�飺	����ģ��
		���ܣ�		һЩ�����Ĺ��ߺ������Լ����롣�����ڷ��ӣ�
					����ϸ��cpp��
-----==========================================================-----
*/


class TTool
{
	public:
		TTool();		//���캯��
		~TTool();		//��������

	//-----------------------------------------------------
	//----http����
	public:
		/*http����
			��˵������post������תQByteArray��*/
		static QByteArray encodeUrlData(QJsonObject json_obj);
		/*http����
			��˵������post������ת���顣*/
		static QString encodeUrlDataString(QJsonObject json_obj);

	//-----------------------------------------------------
	//----html����
	public:
		/*html����
			��˵������QString������תhtml���ı���*/
		static QString _to_HtmlText_(QString text);
		/*html����
			��˵������QColorתhtml��ɫ�ı���*/
		static QString _to_HtmlColor_(QColor color);
		static QString _to_HtmlColor_rgb_(QColor color);
		static QString _to_HtmlColor_rgba_(QColor color);

	//-----------------------------------------------------
	//----�ļ�������
	public:
		/*�ļ���-��ȡ��׺
			��˵��������ȡ�ַ����ļ����ĺ�׺��*/
		static QString getFileSuffix(QString file_name);
		/*�ļ���-����׺
			��˵��������ȡ������׺�����ļ�����*/
		static QString getFileNameWithNewSuffix(QString file_name, QString new_suffix);
		/*�ļ���-�滻�Ƿ��ַ�
			��˵���������ļ����еķǷ��ַ��滻�ɱ���ַ���*/
		static QString getFixedFileName(QString file_name, QString replace_str = "");
		/*�ļ���-��ȡ�ļ����µ������ļ���
			����������"F:/name" "F:\\name"���ļ��У���Ҫ����Ŀ¼ ��F:��
			�����ء���"name.xxx"���ļ���������·����
			��˵���������·����ʽ����ȷ���򷵻ؿա� */
		static QStringList getChildFileName(QString dir_name);
		/*�ļ���-��ȡ�ļ����µ������ļ�����
			����������"F:/name" "F:\\name"���ļ��У���Ҫ����Ŀ¼ ��F:��
			�����ء���"name"���ļ��У�������·����
			��˵���������·����ʽ����ȷ���򷵻ؿա� */
		static QStringList getChildDirName(QString dir_name);
		
	//-----------------------------------------------------
	//----buttonBox���Ļ�

	public:
		/*buttonBox���Ļ�
			��˵������һ����ȫ����ť����Ĭ�ϵ����ġ�*/
		static void _chinese_(QDialogButtonBox *buttonBox);
		/*buttonBox���Ļ�
			��˵��������ָ���İ�ť�������ģ������ť�����ڣ����ᱨ��*/
		void TTool::_chinese_(QDialogButtonBox *buttonBox, QDialogButtonBox::StandardButton type, QString name);


	//-----------------------------------------------------
	//----ui���ٸ�ֵ b��a

	public:
		/*b��ֵ����a
			��˵��1�������b��ֵΪ���ַ�������default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򲻲�����ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ȡ��QString��*/
		static void _QString_(QString *a, QComboBox *b, QString default_ = "");
		/*b��ֵ����a
			��˵��1�������b��ֵΪ���ַ�������default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򲻲�����ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ȡ��QString��*/
		static void _QString_(QString *a, QTableWidgetItem *b, QString default_ = "");
		/*b��ֵ����a
			��˵��1�������b��ֵΪ���ַ�������default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򲻲�����ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ȡ��QString��*/
		static void _QString_(QString *a, QTextEdit *b, QString default_ = "");
		/*b��ֵ����a
			��˵��1�������b��ֵΪ���ַ�������default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򲻲�����ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ȡ��QString��*/
		static void _QString_(QString *a, QPlainTextEdit *b, QString default_ = "");
		/*b��ֵ����a
			��˵��1�������b��ֵΪ���ַ�������default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򲻲�����ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ȡ��QString��*/
		static void _QString_(QString *a, QLineEdit *b, QString default_ = "");
		/*b��ֵ����a
			��˵��1�������b��ֵΪ���ַ�������default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򲻲�����ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ȡ��QString��*/
		static void _QString_(QString *a, QDoubleSpinBox *b );
		/*b��ֵ����a
			��˵��1�������b��ֵΪ���ַ�������default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򲻲�����ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ȡ��QString��*/
		static void _QString_(QString *a, QSpinBox *b );

		/*b��ֵ����a������ֵ�ͣ�
			�����ء���ָ����QObject�е��ַ���
			��˵��1�������b��ֵΪ���ַ������򷵻�default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򷵻�default_��ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ȡ��QString��*/
		static QString _QString_( QComboBox *b, QString default_ = "");
		/*b��ֵ����a������ֵ�ͣ�
			�����ء���ָ����QObject�е��ַ���
			��˵��1�������b��ֵΪ���ַ������򷵻�default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򷵻�default_��ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ȡ��QString��*/
		static QString _QString_( QTableWidgetItem *b, QString default_ = "");
		/*b��ֵ����a������ֵ�ͣ�
			�����ء���ָ����QObject�е��ַ���
			��˵��1�������b��ֵΪ���ַ������򷵻�default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򷵻�default_��ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ȡ��QString��*/
		static QString _QString_( QTextEdit *b, QString default_ = "");
		/*b��ֵ����a������ֵ�ͣ�
			�����ء���ָ����QObject�е��ַ���
			��˵��1�������b��ֵΪ���ַ������򷵻�default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򷵻�default_��ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ȡ��QString��*/
		static QString _QString_(QPlainTextEdit *b, QString default_ = "");
		/*b��ֵ����a������ֵ�ͣ�
			�����ء���ָ����QObject�е��ַ���
			��˵��1�������b��ֵΪ���ַ������򷵻�default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򷵻�default_��ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ȡ��QString��*/
		static QString _QString_(QLineEdit *b, QString default_ = "");
		/*b��ֵ����a������ֵ�ͣ�
			�����ء���ָ����QObject�е��ַ���
			��˵��1�������b��ֵΪ���ַ������򷵻�default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򷵻�default_��ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ȡ��QString��*/
		static QString _QString_(QDoubleSpinBox *b );
		/*b��ֵ����a������ֵ�ͣ�
			�����ء���ָ����QObject�е��ַ���
			��˵��1�������b��ֵΪ���ַ������򷵻�default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򷵻�default_��ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ȡ��QString��*/
		static QString _QString_(QSpinBox *b );

		/*b��ֵ����a
			��˵��1�������b��ֵΪ���ַ�������default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򲻲�����ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ֵQString��*/
		static void _QString_(QComboBox *a, QString *b, QString default_ = "");
		/*b��ֵ����a
			��˵��1�������b��ֵΪ���ַ�������default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򲻲�����ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ֵQString��*/
		static void _QString_(QTableWidgetItem *a, QString *b, QString default_ = "");
		/*b��ֵ����a
			��˵��1�������b��ֵΪ���ַ�������default_��ֵ��QTextEditʹ�õ�setText()����ǩ<xx>�ᱻ������ȥ��
			��˵��2�������QObject����Ϊnullptr���򲻲�����ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ֵQString��*/
		static void _QString_(QTextEdit *a, QString *b, QString default_ = "");
		/*b��ֵ����a
			��˵��1�������b��ֵΪ���ַ�������default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򲻲�����ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ֵQString��*/
		static void _QString_(QPlainTextEdit *a, QString *b, QString default_ = "");
		/*b��ֵ����a
			��˵��1�������b��ֵΪ���ַ�������default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򲻲�����ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ֵQString��*/
		static void _QString_(QLineEdit *a, QString *b, QString default_ = "");
		/*b��ֵ����a
			��˵��1�������b��ֵΪ���ַ�������default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򲻲�����ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ֵQString��*/
		static void _QString_(QDoubleSpinBox *a, QString *b, QString default_ = "");
		/*b��ֵ����a
			��˵��1�������b��ֵΪ���ַ�������default_��ֵ��
			��˵��2�������QObject����Ϊnullptr���򲻲�����ֵ��
			��˵��3����ע�⿴�÷�������_QString_��ʾ����ʲôui������ֵQString��*/
		static void _QString_(QSpinBox *a, QString *b, QString default_ = "");

		/*�ַ��� - ת��ɵ����ַ�
			��˵��������\n\r\tת��������ַ����ʺ�����Ĳ����ַ�����*/
		static QString _QString_escapeToOneLine_(QString data);
		/*�ַ��� - �ָ��ɶ����ַ�
			��˵��������\\n�ָ���\n��*/
		static QString _QString_escapeToRows_(QString data);

		/*b��ֵ����a
			��˵���������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _double_(double *a, QComboBox *b);
		/*b��ֵ����a
			��˵���������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _double_(double *a, QTableWidgetItem *b);
		/*b��ֵ����a
			��˵���������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _double_(double *a, QLineEdit *b);
		/*b��ֵ����a
			��˵���������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _double_(double *a, QDoubleSpinBox *b);
		/*b��ֵ����a
			��˵���������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _double_(QComboBox *a, double *b);
		/*b��ֵ����a
			��˵���������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _double_(QComboBox *a, double b);
		/*b��ֵ����a
			��˵���������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _double_(QTableWidgetItem *a, double *b);
		/*b��ֵ����a
			��˵���������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _double_(QTableWidgetItem *a, double b);
		/*b��ֵ����a
			��˵���������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _double_(QLineEdit *a, double *b);
		/*b��ֵ����a
			��˵���������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _double_(QLineEdit *a, double b);
		/*b��ֵ����a
			��˵���������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _double_(QDoubleSpinBox *a, double *b);
		/*b��ֵ����a
			��˵���������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _double_(QDoubleSpinBox *a, double b);
		
		/*b��ֵ����a
			��˵��1�������︳����index��ֵ��
			��˵��2�������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _int_(int *a, QComboBox *b);
		/*b��ֵ����a
			��˵���������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _int_(int *a, QTableWidgetItem *b);
		/*b��ֵ����a
			��˵���������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _int_(int *a, QLineEdit *b);
		/*b��ֵ����a
			��˵���������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _int_(int *a, QSpinBox *b);
		
		/*b��ֵ����a
			��˵��1�������︳����index��ֵ��
			��˵��2�������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _int_(QComboBox *a, int *b);
		/*b��ֵ����a
			��˵���������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _int_(QTableWidgetItem *a, int *b);
		/*b��ֵ����a
			��˵���������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _int_(QLineEdit *a, int *b);
		/*b��ֵ����a
			��˵���������QObject����Ϊnullptr���򲻲�����ֵ��*/
		static void _int_(QSpinBox *a, int *b);

	//-----------------------------------------------------
	//----����

	public:
		/*եȡint
			��˵������եȡQString�е�һ��intֵ��*/
		static int _to_int_(QString a);
		/*եȡint
			��˵������եȡQString������intֵ��*/
		static QList<int> _to_ints_(QString a);
		/*եȡint��SI��
			��˵��1����եȡQString�е�һ��intֵ��
			��˵��2����ֻʶ��kMG������λ��1k = 1000�����⣬С����1T̫��intװ���¡�*/
		static int _to_int_bySI_(QString a);
		/*եȡint��SI��
			��˵��1����եȡQString������intֵ��
			��˵��2����ֻʶ��kMG������λ��1k = 1000�����⣬С����1T̫��intװ���¡�*/
		static QList<int> _to_ints_bySI_(QString a);

		/*եȡdouble
			��˵������եȡQString�е�һ��doubleֵ��*/
		static double _to_double_(QString a);
		/*եȡdouble
			��˵������եȡQString������doubleֵ�б�*/
		static QList<double> _to_doubles_(QString a);
		/*եȡdouble
			��˵������եȡQString�е�һ��doubleֵ��ʶ���ѧ������������1.02e3��*/
		static double _to_double_e_(QString a);
		/*եȡdouble
			��˵������եȡQString������doubleֵ�б�ʶ���ѧ��������
			����������"1.02e3das1e3d"��������double��"1.02e3"��"1e3"��*/
		static QList<double> _to_doubles_e_(QString a);
		/*եȡdouble��SI��
			��˵������եȡQString�е�һ��doubleֵ��
			����������ʶ��munkMGT��λ��1.02k = 1020��*/
		static double _to_double_bySI_(QString a);
		/*եȡdouble��SI��
			��˵������եȡQString������doubleֵ�б�
			����������ʶ��munkMGT��λ��1.02k = 1020��*/
		static QList<double> _to_doubles_bySI_(QString a);

		/*�ַ������
			��˵��������ָ��������ֵ����㣬Ҳ�����������ַ���*/
		static QString _zeroFill_(int num, int digit);
		static QString _zeroFill_(long num, int digit);
		static QString _zeroFill_(int num, int digit, QLatin1Char ch);
		static QString _zeroFill_(long num, int digit, QLatin1Char ch);
		
	//-----------------------------------------------------
	//----��λ��SI

	public:
		/*SIֵת��
			��˵��������intֵת��ΪSIд�����ַ�����*/
		static QString _to_QString_bySI_(int a);
		/*SIֵת��
			��˵��������doubleֵת��ΪSIд�����ַ�����*/
		static QString _to_QString_bySI_(double a);
		/*SIֵת��
			��˵��������QString��doubleֵת��ΪSIд�����ַ�����*/
		static QString _to_QString_bySI_(QString a);

		/*doubleֵ��ӵ�λ
			��˵��������doubleֵ��ӵ�λ��*/
		static QString _to_Unit_(double d, QString unit);
		/*SIֵ��ӵ�λ
			��˵���������ַ�����ӵ�λ��*/
		static QString _to_Unit_(QString si_qstring, QString unit);
		/*doubleֵ��ӵ�λ
			��˵��������doubleֵ��ӵ�λ�����ֵΪ0�������ָ���ַ�����*/
		static QString _to_Unit_includeEmptyCheck(double d, QString unit, QString default_ = "-");
		/*SIֵ��ӵ�λ
			��˵��������SI�ַ�����ӵ�λ�����ֵΪ0�������ָ���ַ�����*/
		static QString _to_Unit_includeEmptyCheck(QString si_qstring, QString unit, QString default_ = "-");

	//-----------------------------------------------------
	//----����
	public:
		/*��ȡ��������
			��˵��������ȡֻʶ��int������ʽ��*/
		static QString _get_re_int_();
		/*��ȡ��������SI��
			��˵��������ȡֻʶ��int������ʽ������SIд����ʶ��*/
		static QString _get_re_int_SI_();
		/*��ȡС�����򣨲���e��
			��˵��������ȡʶ��double������ʽ��������ѧ��������*/
		static QString _get_re_double_();
		/*��ȡС�����򣨺�e��
			��˵��������ȡʶ��double������ʽ��������ѧ������ʶ��*/
		static QString _get_re_double_e_();
		/*��ȡС������SI��
			��˵��������ȡʶ��double������ʽ������SIд����ʶ��*/
		static QString _get_re_double_SI_();
		/*��ȡ��������У����
			��˵��������ȡֻʶ��int������ʽ��*/
		static QRegExpValidator* _getValidator_re_int_(QWidget* parent = nullptr);
		/*��ȡ��������У������SI��
			��˵��������ȡֻʶ��int������ʽ������SIд����ʶ��*/
		static QRegExpValidator* _getValidator_re_int_SI_(QWidget* parent = nullptr);
		/*��ȡС������У����������e��
			��˵��������ȡʶ��double������ʽ��������ѧ��������*/
		static QRegExpValidator* _getValidator_re_double_(QWidget* parent = nullptr);
		/*��ȡС������У��������e��
			��˵��������ȡʶ��double������ʽ��������ѧ������ʶ��*/
		static QRegExpValidator* _getValidator_re_double_e_(QWidget* parent = nullptr);
		/*��ȡС������У������SI��
			��˵��������ȡʶ��double������ʽ������SIд����ʶ��*/
		static QRegExpValidator* _getValidator_re_double_SI_(QWidget* parent = nullptr);
		/*�ж�����
			��˵������ֻʶ��int��*/
		static bool _match_re_int_(QString test_str);
		/*�ж�������SI��
			��˵������ֻʶ��int������SIд����ʶ��*/
		static bool _match_re_int_SI_(QString test_str);
		/*�ж�С��������e��
			��˵������������ѧ��������*/
		static bool _match_re_double_(QString test_str);
		/*�ж�С������e��
			��˵������������ѧ��������*/
		static bool _match_re_double_e_(QString test_str);
		/*�ж�С����SI��
			��˵����������SIд���Ϳ�ѧ��������*/
		static bool _match_re_double_SI_(QString test_str);
		/*�ж�����
			��˵����������ַ����д��������ַ��򷵻�true��*/
		static bool _has_any_chineseCharacter_(QString test_str);
		/*�ַ�����׺����+1
			��˵���������ַ���ĩβ������+1�����أ�û�������򷵻�"�ַ���2"��*/
		static QString _QString_suffix_addOne_(QString modify_str);
		
	//-----------------------------------------------------
	//----���ٲ���
	public:
		/*doubleת�ַ���
			�����ء����ַ���
			��˵������ûʲô����������ת�������Ǻ���������ͳһ�󶨣��Ҿ�����16λ��*/
		static QString _auto_(double d);
		/*intת�ַ���
			�����ء����ַ���
			��˵������ûʲô����������ת�������Ǻ���������ͳһ�󶨡�*/
		static QString _auto_(int i);
		/*uintת�ַ���
			�����ء����ַ���
			��˵������ûʲô����������ת�������Ǻ���������ͳһ�󶨡�*/
		static QString _auto_(uint i);
		/*ΨһID�ַ���
			�����ء���Ψһ��ʶ���ַ���*/
		static QString _unique_id_();
		
	//-----------------------------------------------------
	//----���ٲ��� - �б�ת��
	public:
		/*�ַ����б� --> �����б�
			��˵������"True"��"TRUE"Ҳ��תtrue��*/
		static QList<bool> _QList_QStringToBool_(QList<QString> list);
		/*�ַ����б� --> �����б�*/
		static QList<int> _QList_QStringToInt_(QList<QString> list);
		/*�ַ����б� --> С���б�*/
		static QList<double> _QList_QStringToDouble_(QList<QString> list);
		/*�ַ����б� --> obj�б�*/
		static QList<QJsonObject> _QList_QStringToQJsonObject_(QList<QString> list);
		/*�����б� --> �ַ����б�*/
		static QList<QString> _QList_BoolToQString_(QList<bool> list);
		/*�����б� --> �ַ����б�*/
		static QList<QString> _QList_IntToQString_(QList<int> list);
		/*С���б� --> �ַ����б�*/
		static QList<QString> _QList_DoubleToQString_(QList<double> list);
		/*obj�б� --> �ַ����б�*/
		static QList<QString> _QList_QJsonObjectToQString_(QList<QJsonObject> list);

		/*�����б� --> С���б�*/
		static QList<double> _QList_IntToDouble_(QList<int> list);
		/*С���б� --> �����б�ȡ����*/
		static QList<int> _QList_DoubleToInt_floor_(QList<double> list);
		/*С���б� --> �����б�ȡ����*/
		static QList<int> _QList_DoubleToInt_ceil_(QList<double> list);
		/*С���б� --> �����б��������룩*/
		static QList<int> _QList_DoubleToInt_round_(QList<double> list);

	//-----------------------------------------------------
	//----QJson
	public:
		/*b��ֵ����a
			��˵������QJsonObject��ֵ����һ������ͬ��key�ᱻ���ǡ�*/
		static void _QJsonObject_put_(QJsonObject *a, QJsonObject b);

		/*�����б� --> ����A�б� */
		static QJsonArray _QJsonArray_BoolToA_(QList<bool> list);
		/*�����б� --> ����A�б� */
		static QJsonArray _QJsonArray_IntToA_(QList<int> list);
		/*С���б� --> С��A�б� */
		static QJsonArray _QJsonArray_DoubleToA_(QList<double> list);
		/*�ַ����б� --> �ַ���A�б� */
		static QJsonArray _QJsonArray_QStringToA_(QList<QString> list);
		/*obj�б� --> objA�б� */
		static QJsonArray _QJsonArray_QJsonObjectToA_(QList<QJsonObject> list);

		/*����A�б� --> �����б� */
		static QList<bool> _QJsonArray_AToBool_(QJsonArray arr);
		/*����A�б� --> �����б� */
		static QList<int> _QJsonArray_AToInt_(QJsonArray arr);
		/*С��A�б� --> С���б� */
		static QList<double> _QJsonArray_AToDouble_(QJsonArray arr);
		/*�ַ���A�б� --> �ַ����б� */
		static QList<QString> _QJsonArray_AToQString_(QJsonArray arr);
		/*objA�б� --> obj�б� */
		static QList<QJsonObject> _QJsonArray_AToQJsonObject_(QJsonArray arr);


	//-----------------------------------------------------
	//----JSON parse��stringify
	public:
		/*Json�ַ��� --> �б�
			��˵��1������������ļ򻯣�ע����utf8���ַ�����
			��˵��2����ע�⣬����\\"��б��ʱ����Ҫһ�������ס�*/
		static QList<bool> _JSON_parse_To_QListBool_(QString context);
		/*Json�ַ��� --> �б�
			��˵��1������������ļ򻯣�ע����utf8���ַ�����
			��˵��2����ע�⣬����\\"��б��ʱ����Ҫһ�������ס�*/
		static QList<int> _JSON_parse_To_QListInt_(QString context);
		/*Json�ַ��� --> �б�
			��˵��1������������ļ򻯣�ע����utf8���ַ�����
			��˵��2����ע�⣬����\\"��б��ʱ����Ҫһ�������ס�*/
		static QList<double> _JSON_parse_To_QListDouble_(QString context);
		/*Json�ַ��� --> �б�
			��˵��1������������ļ򻯣�ע����utf8���ַ�����
			��˵��2����ע�⣬����\\"��б��ʱ����Ҫһ�������ס�*/
		static QList<QString> _JSON_parse_To_QListQString_(QString context);
		/*Json�ַ��� --> �б�
			��˵��1������������ļ򻯣�ע����utf8���ַ�����
			��˵��2����ע�⣬����\\"��б��ʱ����Ҫһ�������ס�*/
		static QJsonObject _JSON_parse_To_Obj_(QString context);
		/*Json�ַ��� --> �б�
			��˵��1������������ļ򻯣�ע����utf8���ַ�����
			��˵��2����ע�⣬����\\"��б��ʱ����Ҫһ�������ס�*/
		static QList<QJsonObject> _JSON_parse_To_QListObj_(QString context);
		
		/*�б� --> Json�б���ַ���
			��˵��1������������ļ򻯣�ע����utf8���ַ�����
			��˵��2����ʹ�õ�QJsonDocumentת�壬���Ƕ�׻����\\"��б�ܡ� */
		static QString _JSON_stringify_(QList<bool> data);
		/*�б� --> Json�б���ַ���
			��˵��1������������ļ򻯣�ע����utf8���ַ�����
			��˵��2����ʹ�õ�QJsonDocumentת�壬���Ƕ�׻����\\"��б�ܡ� */
		static QString _JSON_stringify_(QList<int> data);
		/*�б� --> Json�б���ַ���
			��˵��1������������ļ򻯣�ע����utf8���ַ�����
			��˵��2����ʹ�õ�QJsonDocumentת�壬���Ƕ�׻����\\"��б�ܡ� */
		static QString _JSON_stringify_(QList<double> data);
		/*�б� --> Json�б���ַ���
			��˵��1������������ļ򻯣�ע����utf8���ַ�����
			��˵��2����ʹ�õ�QJsonDocumentת�壬���Ƕ�׻����\\"��б�ܡ� */
		static QString _JSON_stringify_(QList<QString> data);
		/*�б� --> Json�б���ַ���
			��˵��1������������ļ򻯣�ע����utf8���ַ�����
			��˵��2����ʹ�õ�QJsonDocumentת�壬���Ƕ�׻����\\"��б�ܡ� */
		static QString _JSON_stringify_(QJsonObject data);
		/*�б� --> Json�б���ַ���
			��˵��1������������ļ򻯣�ע����utf8���ַ�����
			��˵��2����ʹ�õ�QJsonDocumentת�壬���Ƕ�׻����\\"��б�ܡ� */
		static QString _JSON_stringify_(QList<QJsonObject> data);
		
	//-----------------------------------------------------
	//----QStringList
	public:
		/*�ַ����б� - ȥ������
			��˵���������ײ���������и����Ӳ�����ȥ��p_TxtFastReader�� */
		static void _QStringList_clearEmptyRows_(QStringList* data_list);
		/*�ַ����б� - ȥ��ע���У�����ǰ׺��
			��˵���������ײ���������и����Ӳ�����ȥ��p_TxtFastReader�� */
		static void _QStringList_clearComment_(QStringList* data_list, QString prefix);
		/*�ַ����б� - ����С��λ
			��˵���������ײ����� */
		static QStringList _QStringList_decimalChange_(QStringList data, int decimal);
		/*�ַ����б� - ȫ�������ǰ׺
			��˵���������ײ����� */
		static QStringList _QStringList_addPrefix_(QStringList data_list, QString prefix);
		/*�ַ����б� - ȫ������Ӻ�׺
			��˵���������ײ����� */
		static QStringList _QStringList_addSuffix_(QStringList data_list, QString suffix);
		
	//-----------------------------------------------------
	//----QWidget
	public:
		/*�ؼ��� - �����Ӵ������
			��˵���������ٴ���һ���Ӵֵ������ı�����Ҫ�����ؼ������ؼ��ã��� */
		static QLabel* _CreateQWidget_headerLabel_(QString text, int fontsize, int padding, QWidget* parent = nullptr);
		/*�ؼ��� - �����ⲿ������
			��˵���������ٴ��������飬��Ŀ��QWidget���������� */
		static QWidget* _CreateQWidget_containsTarget_(QWidget* target, int padding);
		/*�ؼ��� - �޸���������߶�
			��˵�����������޸�����������ĸ߶ȡ� */
		static QStyledItemDelegate* _ChangeCombox_itemHeight_(QComboBox* target, int item_height);

};


// > getter��setter�궨��
#define TTOOL_DATA_PROPERTY(type, var) \
	protected: \
	type _##var; \
	public: \
	type get##var() { return _##var; }\
	void set##var(type val) { _##var = val; }

