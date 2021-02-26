#pragma once

#include <QWidget>

/*
-----==========================================================-----
		�ࣺ		�����ඨ��.h
		�汾��		v1.13
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		����һ������д�õ��ඨ�壬���ٱ������ٴ���ͨ�á�
					�����������ڶ���model�������ֶζ����ַ�����
					���������������������ġ�
					����ϸ��cpp��
-----==========================================================-----
*/
class C_FastClassParam;
class C_FastClass{

	public:
		C_FastClass();
		C_FastClass(QString className);
		~C_FastClass();
		
	//-----------------------------------
	//----һ�裨ȷ���������ͣ�
	public:
							//һ�� - ����
		void setClassName(QString classNameChinese);
		void setClassNameEnglish(QString classNameEnglish);
		void setClassNameShowing(QString classNameShowing);
		QString getClassNameChinese();
		QString getClassNameEnglish();
		QString getClassNameShowing();
							//һ�� - Double
							//��Ĭ��ui��QLineEdit��
							//��Ĭ��ֵʾ�⣺�ַ���"0.36","0.1M"��
		void addDouble(QString paramNameChinese);
		void addDouble(QString paramNameChinese, QString defaultValue);
							//һ�� - Int
							//��Ĭ��ui��QLineEdit��
							//��Ĭ��ֵʾ�⣺�ַ���"100"��
		void addInt(QString paramNameChinese);
		void addInt(QString paramNameChinese, QString defaultValue);
							//һ�� - QString
							//��Ĭ��ui��QLineEdit��
							//��Ĭ��ֵʾ�⣺"�ַ���"��
		void addQString(QString paramNameChinese);
		void addQString(QString paramNameChinese, QString defaultValue);
							//һ�� - Int��ѡ��
							//��Ĭ��ui��QComboBox��
							//��Ĭ��ֵʾ�⣺0��ʾ������ţ�
							//��indexListΪ��ʵ��������ֵ��indexShowingListΪ��ʾ������ѡ�
		void addIntWithChoose(QString paramNameChinese, int defaultIndex, QList<int> indexList);
		void addIntWithChoose(QString paramNameChinese, int defaultIndex, QList<int> indexList, QStringList indexShowingList);
							//һ�� - QString��ѡ��
							//��Ĭ��ui��QComboBox��
							//��Ĭ��ֵʾ�⣺"��ʵѡ��ֵ"��
							//��chooseListΪ��ʵ����ѡ�chooseShowingListΪ��ʾ������ѡ�
		void addQStringWithChoose(QString paramNameChinese, QString defaultValue, QStringList chooseList);
		void addQStringWithChoose(QString paramNameChinese, QString defaultValue, QStringList chooseList, QStringList chooseShowingList);
							//һ�� - Boolean
							//��Ĭ��ui��QCheckBox��
							//��Ĭ��ֵʾ�⣺false��
		void addBoolean(QString paramNameChinese);
		void addBoolean(QString paramNameChinese, QString defaultValue);
							//һ�裨Ƕ�ף� - ��Ƕ��
		void addFastClassQWidget(C_FastClass fastClass);
		void addFastClassOneLine(C_FastClass fastClass);
		void addFastClassQGroupBox(C_FastClass fastClass);
		C_FastClass getChildClass(QString classNameChinese);

	//-----------------------------------
	//----���裨ȷ��������ɣ�
	public:
							//���� - Ӣ����
		void setParamEnglishName(QString paramNameChinese, QString paramNameEnglish);
							//���� - ��ʾ��
		void setParamShowingName(QString paramNameChinese, QString showingName);
							//���� - ������
		void setParamNotNull(QString paramNameChinese, bool notNull);
							//���� - ��׺
		void setParamSuffix(QString paramNameChinese, QString suffix);
							//���� - SIд����ע�⣬ֻ��QLineEdit�ؼ���Ч��
		void setParamSI(QString paramNameChinese, bool isSI);
		
	//-----------------------------------
	//----���裨ȷ��UI���ã�
	public:
							//���� - ���ɱ༭
		void setParamEditable(QString paramNameChinese, bool isEditable);
		void setAllParamEditable(bool isEditable);
							//���� - ����Int����Ϊ���ֿؼ�
		void setInt_UI_ToQSpinBox(QString paramNameChinese, long valueMin, long valueMax);
		void setInt_UI_ToQSpinBox(QString paramNameChinese, long valueMin, long valueMax, long singleStep);
		void setInt_UI_ToQSpinBox(QString paramNameChinese, long valueMin, long valueMax, long singleStep, QString prefix, QString suffix);
							//���� - ����Double����Ϊ���ֿؼ�
		void setDouble_UI_ToQDoubleSpinBox(QString paramNameChinese, double valueMin, double valueMax);
		void setDouble_UI_ToQDoubleSpinBox(QString paramNameChinese, double valueMin, double valueMax, double singleStep, int decimals);
		void setDouble_UI_ToQDoubleSpinBox(QString paramNameChinese, double valueMin, double valueMax, double singleStep, int decimals, QString prefix, QString suffix);
							//���� - ����Boolean����Ϊ������
		void setBoolean_UI_ToQCombox(QString paramNameChinese, QString trueText, QString falseText);
							//���� - ����QString����Ϊ�ı���
		void setQString_UI_ToQPlainTextEdit(QString paramNameChinese, int plainTextHeight);
		
	//-----------------------------------
	//----���裨��ʱ�޸�ĳ���ã�
	public:
							//���� - Ĭ��ֵ�޸�
		void setParamDefaultValue(QString paramNameChinese, QString defaultValue);
		QString getParamDefaultValue(QString paramNameChinese);
							//���� - ������Ŀ�ѡ���޸�
		void setParamChooseList(QString paramNameChinese, QStringList chooseList);
		void setParamChooseShowingList(QString paramNameChinese, QStringList chooseShowingList);
		QStringList getParamChooseList(QString paramNameChinese);
		QStringList getParamChooseShowingList(QString paramNameChinese);


	//-----------------------------------
	//----�߼�
	public:
							//�߼� - ��������
		void setParamColumn(QString paramNameChinese, int col);
							//�߼� - ������ֻ������һ��������������������������Ƕ�ף�
		void setClassPrimaryKeyId(QString classNameChinese);
		void setClassPrimaryKeyName(QString classNameChinese);
		QString getClassPrimaryKeyId();
		QString getClassPrimaryKeyName();

	//-----------------------------------
	//----��ȡ
	public:
							//��ȡ - �������������FastClass��
		QList<C_FastClassParam> getParamList();
							//��ȡ - �����������FastClassȫ����̯����
		QList<C_FastClassParam> getConvertedParamList();
							//��ȡ - ���в���������������������Ĳ�����
		QStringList getParamChineseNameList();
							//��ȡ - ���в���Ӣ����������������Ĳ�����
		QStringList getParamEnglishNameList();
							//��ȡ - �жϿ�
		bool isNull();
							//��ȡ - �������ݣ�������һ�衢���衢����ִ����֮���ȡ��
		QJsonObject getExampleDataInChinese();
		QJsonObject getExampleDataInEnglish();
		QJsonObject getExampleDataInShowing();

		
	//-----------------------------------
	//----���⹦��
	public:
							//���⹦�� - ��QJsonObject�����ݣ����ݸ���ĸ�ʽ����䵽table�У����ģ�
		void fillDataInTableWidget(QTableWidget* tableWidget, QList<QJsonObject> data_list);
							//���⹦�� - ��QJsonObject�����ݣ����ݸ���ĸ�ʽ����䵽table�У�Ӣ�ģ�
		void fillDataInTableWidgetEnglish(QTableWidget* tableWidget, QList<QJsonObject> data_list);
							//���⹦�� - jsonobj Ӣ�� -> ����
		QJsonObject convertEnglishToChinese(QJsonObject data);
							//���⹦�� - jsonobj Ӣ�� -> ���ģ�ת���˵��ӵ���
		QJsonObject convertEnglishToChineseExcludely(QJsonObject data);
							//���⹦�� - jsonobj ���� -> Ӣ��
		QJsonObject convertChineseToEnglish(QJsonObject data);
							//���⹦�� - jsonobj ���� -> Ӣ�ģ�ת���˵��ӵ���
		QJsonObject convertChineseToEnglishExcludely(QJsonObject data);
							//���⹦�� - ȥ���������ı�ʶ��ʽ
		QString removeSpecialChineseCharacter(QString paramNameChinese);

	//-----------------------------------
	//----˽�в���
	protected:
		QString classNameChinese;			//��������
		QString classNameEnglish;			//��Ӣ����
		QString classNameShowing;			//����ʾ��
		QList<C_FastClassParam> paramList;	//�������
		QStringList nameChinese_list;		//ȫ��������
		QStringList nameEnglish_list;		//ȫ��Ӣ����
		QString classPrimaryKeyId;			//����id
		QString classPrimaryKeyName;		//����name
	protected:
		C_FastClassParam getParamByName(QString nameChinese);
		int getIndexByName(QString nameChinese);
		void checkDuplicatedChineseValue(QString nameChinese);
		void checkDuplicatedEnglishValue(QString nameEnglish);
};
/*
-----==========================================================-----
		�ࣺ		���������.h
-----==========================================================-----
*/
class C_FastClassParam{

	public:
		C_FastClassParam();
		~C_FastClassParam();

	public:
		// > һ��
		QString nameChinese;				//������
		QString type;						//��������
		QString widgetType;					//������
		QString defaultValue;				//Ĭ��ֵ
		QStringList chooseList;				//ѡ���б�

		// > ����
		QString nameEnglish;				//Ӣ����
		QString nameShowing;				//ʵ����ʾ��
		QStringList chooseShowingList;		//ʵ����ʾѡ��
		bool notNull;						//������
		QString suffix;						//��׺����λ��
		bool isSI;							//SIд��

		// > ����
		bool isEditable;					//�ɱ༭
		int comboxHeight;					//������ - ѡ��߶�
		double valueMin;					//���ֿؼ� - ��Сֵ
		double valueMax;					//���ֿؼ� - ���ֵ
		double singleStep;					//���ֿؼ� - ���½���
		int decimals;						//���ֿؼ� - λ��
		QString num_prefix;					//���ֿؼ� - ǰ׺
		QString num_suffix;					//���ֿؼ� - ��׺
		int plainTextHeight;				//�ı��� - �߶�

		// > �߼�
		int columnPos;						//������
		C_FastClass fastClass;				//Ƕ����

		// > EX������
		QString tempA;						//��������A
		QString tempB;						//��������B
		QString tempC;						//��������C
		QString tempD;						//��������D
		QString tempE;						//��������E
		QString tempF;						//��������F

	//-----------------------------------
	//----��ȡ
	public:
									//���������
		const bool operator== (const C_FastClassParam& a)const;
									//�жϿ�
		bool isNull();
									//��ȡ - ��ʾ�ı���
		QString getShowingName();
	public:
									//��ȡ - ȥ���������ı�ʶ���ַ�����̬������
		static QString removeSpecialChineseCharacter(QString nameChinese);

};