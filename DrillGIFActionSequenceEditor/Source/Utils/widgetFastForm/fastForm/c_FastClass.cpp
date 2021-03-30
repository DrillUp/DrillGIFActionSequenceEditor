#include "stdafx.h"

#include "c_FastClass.h"
#include "../../common/TTool.h"


/*
-----==========================================================-----
		�ࣺ		�����ඨ��.cpp
		�汾��		v1.14
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		����һ������д�õ��ඨ�壬���ٱ������ٴ���ͨ�á�
					�����������ڶ���model�������ֶζ����ַ�����
					���������������������ġ�
					
		���ܣ�		1.һ�裺	������Double��Int��QString��Int��ѡ�QString��ѡ�Boolean
					2.���裺	Ӣ��������ʾ�����������׺
					3.���裺	SIд�������ɱ༭��Ĭ��ֵ�޸�
					4.�߼���	������������Ƕ��
					���⣺		QJsonObject��䵽table�С�QJsonObject��Ӣ��ת��

		����Լ����	1.��������Ӣ���������ظ�����������ظ���������������������
					2.Ƕ���� ��������Ҫ��һ����������Ȼ������鱨��
					3.����ֻ������һ����������ͬ���������࣬����Ƕ����һ��

		�Զ�����	1.���Ĳ�����ֵȫ��ΪQString��
					  Ӣ�Ĳ�������������Զ�תdouble��int��
					2.��׺Ϊ"Hz"ʱ���Զ�SIд����
					3.����Ϊdoubleʱ����ʾtablewidgetʱ�Զ���Ӻ�׺��
					4.��ʶ��ʽ��"<-��ʶ�ַ�->������"��
					  ���Ĳ������иø�ʽ���ַ������ᱻ��ʾ��������Ҳ������showingName���ã�
					
		ʹ�÷�����
				>����
					c_class = C_FastClass("Ŀ��ģ��");						//�����൱��ֱ����дһ�Ѳ������������ڿ��ٱ������ٴ���
					c_class.addDouble				("λ��", "0.0");		//��ϸ�� ���ٱ������ٴ��� ��ʹ�÷���
					c_class.addDouble				("����", "0.0"); 
					c_class.addQString				("˵��", "0.0");
					c_class.addQString				("����", "0.0");
				>�����޸�
					c_class.addQString				("������", "0");		//������Ĳ����󣬿���������Ӹ�������
					c_class.setParamEnglishName		("������", "ModPulMaxInterfPulseWidth");
					c_class.setParamShowingName		("������", "������������");
					c_class.setParamSuffix			("������", "��s");
				>��Ƕ��
					c_class = C_FastClass("����");							//Ƕ�׺�����Ȼ��Ҫȷ�����Ĳ��������ظ�
						C_FastClass class_6 = C_FastClass("�źŷ�Χ");
						class_6.addQString						("�ź�����", "0");
						class_6.setParamEnglishName				("�ź�����", "ModPulSignalCarrierFreqStart");
						class_6.setParamSuffix					("�ź�����", "");
						class_6.addQString						("�ź�����", "0");
						class_6.setParamEnglishName				("�ź�����", "ModPulSignalCarrierFreqEnd");
						class_6.setParamSuffix					("�ź�����", "Hz");
					c_class.addFastClassOneLine				(class_6);
				>����
					this->m_target_class.addQString("<-SPAN->xxxxx", "");	//����ָ��ǰ׺�ı����������Ϊ������Ŀ�ռλ����

-----==========================================================-----
*/
C_FastClass::C_FastClass(){
	this->classNameChinese = "";
	this->classNameEnglish = "";
	this->classNameShowing = "";
	this->classPrimaryKeyId = "";
	this->classPrimaryKeyName = "";
	this->paramList = QList<C_FastClassParam>();
	this->nameChinese_list = QStringList() << this->classNameChinese;
	this->nameEnglish_list = QStringList() << this->classNameEnglish;
}
C_FastClass::C_FastClass(QString className)
{
	this->classNameChinese = className;
	this->classNameEnglish = "";
	this->classNameShowing = "";
	this->classPrimaryKeyId = "";
	this->classPrimaryKeyName = "";
	this->paramList = QList<C_FastClassParam>();
	this->nameChinese_list = QStringList() << this->classNameChinese;
	this->nameEnglish_list = QStringList() << this->classNameEnglish;
}

C_FastClass::~C_FastClass(){
}

/*-------------------------------------------------
		˽�� - ����������ȡ����
*/
C_FastClassParam C_FastClass::getParamByName(QString nameChinese){
	for (int i = 0; i < this->paramList.count(); i++){
		C_FastClassParam c_fp = this->paramList.at(i);
		if (c_fp.nameChinese == nameChinese){
			return c_fp;
		}
	}
	return C_FastClassParam();
}
int C_FastClass::getIndexByName(QString nameChinese){
	for (int i = 0; i < this->paramList.count(); i++){
		C_FastClassParam c_fp = this->paramList.at(i);
		if (c_fp.nameChinese == nameChinese){
			return i;
		}
	}
	return -1;
}

/*-------------------------------------------------
		˽�� - �ж�����
*/
void C_FastClass::checkDuplicatedChineseValue(QString nameChinese){
	if (nameChinese == ""){ return; }
	#ifdef DEBUG_OPEN_PRO
		if (nameChinese_list.indexOf(nameChinese) != -1){
			qDebug() << "[" + this->classNameChinese + "]�����������Ĳ�����" + nameChinese;
			Q_ASSERT(false);
		}
	#else
	
	#endif
	nameChinese_list.push_back(nameChinese);
}
void C_FastClass::checkDuplicatedEnglishValue(QString nameEnglish){
	if (nameEnglish == ""){ return; }
	#ifdef DEBUG_OPEN_PRO
		if (nameEnglish_list.indexOf(nameEnglish) != -1){
			qDebug() << "[" + this->classNameChinese + "]��������Ӣ�Ĳ�����" + nameEnglish;
			Q_ASSERT(false);
		}
	#else
	
	#endif
	nameEnglish_list.push_back(nameEnglish);
}

/*-------------------------------------------------
		һ�� - ����
*/
void C_FastClass::setClassName(QString className){
	this->nameChinese_list.removeOne(this->classNameChinese);
	this->classNameChinese = className;
	this->checkDuplicatedChineseValue(className);
}
void C_FastClass::setClassNameEnglish(QString className){
	this->nameEnglish_list.removeOne(this->classNameEnglish);
	this->classNameEnglish = className;
	this->checkDuplicatedEnglishValue(className);
}
void C_FastClass::setClassNameShowing(QString className){
	this->classNameShowing = className;
}
QString C_FastClass::getClassNameChinese(){
	return classNameChinese;
}
QString C_FastClass::getClassNameEnglish(){
	return classNameEnglish;
}
QString C_FastClass::getClassNameShowing(){
	if (this->classNameShowing != ""){ return classNameShowing; }
	return this->classNameChinese.replace(QRegExp("<-[^>]*->"), "");
}
/*-------------------------------------------------
		һ�� - Double
*/
void C_FastClass::addDouble(QString paramName){ this->addDouble(paramName, ""); }	//(���غ���)
void C_FastClass::addDouble(QString paramName, QString defaultValue){
	C_FastClassParam c_f = C_FastClassParam();
	c_f.nameChinese = paramName;			//������
	c_f.type = "Double";					//��������
	c_f.widgetType = "QLineEdit";			//������
	c_f.defaultValue = defaultValue;		//Ĭ��ֵ
	c_f.chooseList = QStringList();			//ѡ���б�
	this->checkDuplicatedChineseValue(paramName);
	this->paramList.push_back(c_f);
}
/*-------------------------------------------------
		һ�� - Int
*/
void C_FastClass::addInt(QString paramName){ this->addInt(paramName, ""); }		//(���غ���)
void C_FastClass::addInt(QString paramName, QString defaultValue){
	C_FastClassParam c_f = C_FastClassParam();
	c_f.nameChinese = paramName;			//������
	c_f.type = "Int";						//��������
	c_f.widgetType = "QLineEdit";			//������
	c_f.defaultValue = defaultValue;		//Ĭ��ֵ
	c_f.chooseList = QStringList();			//ѡ���б�
	this->checkDuplicatedChineseValue(paramName);
	this->paramList.push_back(c_f);
}
/*-------------------------------------------------
		һ�� - QString
*/
void C_FastClass::addQString(QString paramName){ this->addQString(paramName, ""); }	//(���غ���)
void C_FastClass::addQString(QString paramName, QString defaultValue){
	C_FastClassParam c_f = C_FastClassParam();
	c_f.nameChinese = paramName;			//������
	c_f.type = "QString";					//��������
	c_f.widgetType = "QLineEdit";			//������
	c_f.defaultValue = defaultValue;		//Ĭ��ֵ
	c_f.chooseList = QStringList();			//ѡ���б�
	this->checkDuplicatedChineseValue(paramName);
	this->paramList.push_back(c_f);
}
/*-------------------------------------------------
		һ�� - Int��ѡ��
*/
void C_FastClass::addIntWithChoose(QString paramName, int defaultIndex, QList<int> indexList){
	// > intת�ַ���
	QStringList chooseList = QStringList();
	for (int i = 0; i < indexList.count(); i++){ chooseList.append(QString::number(indexList.at(i))); }
	// > ������ֵ
	this->addIntWithChoose(paramName, defaultIndex, indexList, chooseList);		//(���غ���)
}
void C_FastClass::addIntWithChoose(QString paramName, int defaultIndex, QList<int> indexList, QStringList chooseShowingList){
	// > intת�ַ���
	QStringList chooseList = QStringList();
	for (int i = 0; i < indexList.count(); i++){ chooseList.append(QString::number(indexList.at(i))); }
	// > ������ֵ
	C_FastClassParam c_f = C_FastClassParam();
	c_f.nameChinese = paramName;						//������
	c_f.type = "Int";									//��������
	c_f.widgetType = "QComboBox";						//������
	c_f.defaultValue = QString::number(defaultIndex);	//Ĭ��ֵ
	c_f.chooseList = chooseList;						//ѡ���б�
	c_f.chooseShowingList = chooseShowingList;			//ʵ����ʾѡ����裩
	this->checkDuplicatedChineseValue(paramName);
	this->paramList.push_back(c_f);
}
/*-------------------------------------------------
		һ�� - QString��ѡ��
*/
void C_FastClass::addQStringWithChoose(QString paramName, QString defaultValue, QStringList chooseList){
	this->addQStringWithChoose(paramName, defaultValue, chooseList, chooseList);	//(���غ���)
}
void C_FastClass::addQStringWithChoose(QString paramName, QString defaultValue, QStringList chooseList, QStringList chooseShowingList){
	C_FastClassParam c_f = C_FastClassParam();
	c_f.nameChinese = paramName;						//������
	c_f.type = "QString";								//��������
	c_f.widgetType = "QComboBox";						//������
	c_f.defaultValue = defaultValue;					//Ĭ��ֵ
	c_f.chooseList = chooseList;						//ѡ���б�
	c_f.chooseShowingList = chooseShowingList;			//ʵ����ʾѡ����裩
	this->checkDuplicatedChineseValue(paramName);
	this->paramList.push_back(c_f);
}
/*-------------------------------------------------
		һ�� - Boolean
*/
void C_FastClass::addBoolean(QString paramName){ this->addBoolean(paramName, false); }	//(���غ���)
void C_FastClass::addBoolean(QString paramName, QString defaultValue){
	C_FastClassParam c_f = C_FastClassParam();
	c_f.nameChinese = paramName;						//������
	c_f.type = "Boolean";								//��������
	c_f.widgetType = "QCheckBox";						//������
	c_f.defaultValue = (defaultValue == "true" ? "true" : "false");	//Ĭ��ֵ
	c_f.chooseList = QStringList();						//ѡ���б�
	this->checkDuplicatedChineseValue(paramName);
	this->paramList.push_back(c_f);
}

/*-------------------------------------------------
		һ�裨Ƕ�ף� - ������Ƕ��
*/
void C_FastClass::addFastClassQWidget(C_FastClass fastClass){
	C_FastClassParam c_f = C_FastClassParam();
	c_f.nameChinese = fastClass.getClassNameChinese();
	c_f.nameEnglish = fastClass.getClassNameEnglish();
	c_f.type = "FastClass";
	c_f.widgetType = "QWidget";
	c_f.fastClass = fastClass;

	// > ������������
	QStringList name_list = fastClass.getParamChineseNameList();
	Q_ASSERT(name_list.count() > 0);
	for (int i = 0; i < name_list.count(); i++){
		this->checkDuplicatedChineseValue(name_list.at(i));
	}
	QStringList eng_name_list = fastClass.getParamEnglishNameList();
	for (int i = 0; i < eng_name_list.count(); i++){
		this->checkDuplicatedEnglishValue(eng_name_list.at(i));
	}

	// > ������ֵ
	this->setClassPrimaryKeyId(fastClass.getClassPrimaryKeyId());
	this->setClassPrimaryKeyName(fastClass.getClassPrimaryKeyName());

	this->paramList.push_back(c_f);
}
void C_FastClass::addFastClassOneLine(C_FastClass fastClass){
	C_FastClassParam c_f = C_FastClassParam();
	c_f.nameChinese = fastClass.getClassNameChinese();
	c_f.nameEnglish = fastClass.getClassNameEnglish();
	c_f.type = "FastClass";
	c_f.widgetType = "OneLine";
	c_f.fastClass = fastClass;

	// > ������������
	QStringList name_list = fastClass.getParamChineseNameList();
	Q_ASSERT(name_list.count() > 0);
	for (int i = 0; i < name_list.count(); i++){
		this->checkDuplicatedChineseValue(name_list.at(i));
	}
	QStringList eng_name_list = fastClass.getParamEnglishNameList();
	for (int i = 0; i < eng_name_list.count(); i++){
		this->checkDuplicatedEnglishValue(eng_name_list.at(i));
	}

	// > ������ֵ
	this->setClassPrimaryKeyId(fastClass.getClassPrimaryKeyId());
	this->setClassPrimaryKeyName(fastClass.getClassPrimaryKeyName());

	this->paramList.push_back(c_f);
}
void C_FastClass::addFastClassQGroupBox(C_FastClass fastClass){
	C_FastClassParam c_f = C_FastClassParam();
	c_f.nameChinese = fastClass.getClassNameChinese();
	c_f.nameEnglish = fastClass.getClassNameEnglish();
	c_f.type = "FastClass";
	c_f.widgetType = "QGroupBox";
	c_f.fastClass = fastClass;

	// > ������������
	QStringList name_list = fastClass.getParamChineseNameList();
	Q_ASSERT(name_list.count() > 0);
	for (int i = 0; i < name_list.count(); i++){
		this->checkDuplicatedChineseValue(name_list.at(i));
	}
	QStringList eng_name_list = fastClass.getParamEnglishNameList();
	for (int i = 0; i < eng_name_list.count(); i++){
		this->checkDuplicatedEnglishValue(eng_name_list.at(i));
	}

	// > ������ֵ
	this->setClassPrimaryKeyId(fastClass.getClassPrimaryKeyId());
	this->setClassPrimaryKeyName(fastClass.getClassPrimaryKeyName());

	this->paramList.push_back(c_f);
}
/*-------------------------------------------------
		һ�裨Ƕ�ף� - ��ȡ�ӿ�����
*/
C_FastClass C_FastClass::getChildClass(QString classNameChinese){
	C_FastClassParam c_fp = this->getParamByName(classNameChinese);
	if (c_fp.type == "FastClass"){
		return c_fp.fastClass;
	}
	return C_FastClass();
}

/*-------------------------------------------------
		���� - Ӣ����
*/
void C_FastClass::setParamEnglishName(QString paramNameChinese, QString paramNameEnglish){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	c_fp.nameEnglish = paramNameEnglish;
	this->checkDuplicatedEnglishValue(paramNameEnglish);		//���Ӣ�����ظ�
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}
/*-------------------------------------------------
		���� - ��ʾ��
*/
void C_FastClass::setParamShowingName(QString paramNameChinese, QString showingName){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	c_fp.nameShowing = showingName;
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}
/*-------------------------------------------------
		���� - ������
*/
void C_FastClass::setParamNotNull(QString paramNameChinese, bool notNull){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	c_fp.notNull = notNull;
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}
/*-------------------------------------------------
		���� - ��׺
*/
void C_FastClass::setParamSuffix(QString paramNameChinese, QString suffix){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	c_fp.suffix = suffix;
	if (suffix == "Hz"){ c_fp.isSI = true; }	//�Զ���Ϊsiд��
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}
/*-------------------------------------------------
		���� - SIд��
*/
void C_FastClass::setParamSI(QString paramNameChinese, bool isSI){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	if (c_fp.isNull()){ return; }
	c_fp.isSI = isSI;
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}

/*-------------------------------------------------
		���� - ���ɱ༭
*/
void C_FastClass::setParamEditable(QString paramNameChinese, bool isEditable){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	if (c_fp.isNull()){ return; }
	c_fp.isEditable = isEditable;
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}
void C_FastClass::setAllParamEditable(bool isEditable){
	for (int i = 0; i < this->paramList.count(); i++){
		C_FastClassParam c_fp = this->paramList.at(i);
		c_fp.isEditable = isEditable;
		this->paramList.replace(i, c_fp);
	}
}
/*-------------------------------------------------
		���� - ����Int����Ϊ���ֿؼ�
*/
void C_FastClass::setInt_UI_ToQSpinBox(QString paramNameChinese, long valueMin, long valueMax){
	this->setInt_UI_ToQSpinBox(paramNameChinese, valueMin, valueMax, 1);	//�����غ�����
}
void C_FastClass::setInt_UI_ToQSpinBox(QString paramNameChinese, long valueMin, long valueMax, long singleStep){
	this->setInt_UI_ToQSpinBox(paramNameChinese, valueMin, valueMax, 1, "", "");	//�����غ�����
}
void C_FastClass::setInt_UI_ToQSpinBox(QString paramNameChinese, long valueMin, long valueMax, long singleStep, QString prefix, QString suffix){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	if (c_fp.isNull()){ return; }
	c_fp.widgetType = "QSpinBox";
	if (valueMax < valueMin){
		c_fp.valueMin = valueMax;
		c_fp.valueMax = valueMin;
	}
	else{
		c_fp.valueMin = valueMin;
		c_fp.valueMax = valueMax;
	}
	c_fp.singleStep = singleStep;
	c_fp.num_prefix = prefix;
	c_fp.num_suffix = suffix;
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}
/*-------------------------------------------------
		���� - ����Double����Ϊ���ֿؼ�
*/
void C_FastClass::setDouble_UI_ToQDoubleSpinBox(QString paramNameChinese, double valueMin, double valueMax){
	this->setDouble_UI_ToQDoubleSpinBox(paramNameChinese, valueMin, valueMax, 1, 4);	//�����غ�����
}
void C_FastClass::setDouble_UI_ToQDoubleSpinBox(QString paramNameChinese, double valueMin, double valueMax, double singleStep, int decimals){
	this->setDouble_UI_ToQDoubleSpinBox(paramNameChinese, valueMin, valueMax, 1, 4, "", "");	//�����غ�����
}
void C_FastClass::setDouble_UI_ToQDoubleSpinBox(QString paramNameChinese, double valueMin, double valueMax, double singleStep, int decimals, QString prefix, QString suffix){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	if (c_fp.isNull()){ return; }
	c_fp.widgetType = "QDoubleSpinBox";
	if (valueMax < valueMin){
		c_fp.valueMin = valueMax;
		c_fp.valueMax = valueMin;
	}
	else{
		c_fp.valueMin = valueMin;
		c_fp.valueMax = valueMax;
	}
	c_fp.singleStep = singleStep;
	c_fp.decimals = decimals;
	c_fp.num_prefix = prefix;
	c_fp.num_suffix = suffix;
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}
/*-------------------------------------------------
		���� - ����Boolean����Ϊ������
*/
void C_FastClass::setBoolean_UI_ToQCombox(QString paramNameChinese, QString trueText, QString falseText){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	if (c_fp.isNull()){ return; }
	c_fp.widgetType = "QComboBox";
	c_fp.chooseList = QStringList() << "true" << "false";
	c_fp.chooseShowingList = QStringList() << trueText << falseText;
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}
/*-------------------------------------------------
		���� - ����QString����Ϊ�ı���
*/
void C_FastClass::setQString_UI_ToQPlainTextEdit(QString paramNameChinese, int plainTextHeight){
	this->setQString_UI_ToQPlainTextEdit(paramNameChinese, plainTextHeight, false);
}
void C_FastClass::setQString_UI_ToQPlainTextEdit(QString paramNameChinese, int plainTextHeight, bool plainTextEscape){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	if (c_fp.isNull()){ return; }
	c_fp.widgetType = "QPlainTextEdit";
	c_fp.plainTextHeight = plainTextHeight;
	c_fp.plainTextEscape = plainTextEscape;
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}

/*-------------------------------------------------
		���� - Ĭ��ֵ�޸�
*/
void C_FastClass::setParamDefaultValue(QString paramNameChinese, QString defaultValue){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	if (c_fp.isNull()){ return; }
	c_fp.defaultValue = defaultValue;
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}
QString C_FastClass::getParamDefaultValue(QString paramNameChinese){
	return this->getParamByName(paramNameChinese).defaultValue;
}
/*-------------------------------------------------
		���� - ������Ŀ�ѡ���޸�
*/
void C_FastClass::setParamChooseList(QString paramNameChinese, QStringList chooseList){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	if (c_fp.isNull()){ return; }
	c_fp.chooseList = chooseList;
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}
void C_FastClass::setParamChooseShowingList(QString paramNameChinese, QStringList chooseShowingList){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	if (c_fp.isNull()){ return; }
	c_fp.chooseShowingList = chooseShowingList;
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}
QStringList C_FastClass::getParamChooseList(QString paramNameChinese){
	return this->getParamByName(paramNameChinese).chooseList;
}
QStringList C_FastClass::getParamChooseShowingList(QString paramNameChinese){
	return this->getParamByName(paramNameChinese).chooseShowingList;
}

/*-------------------------------------------------
		�߼� - ��������
*/
void C_FastClass::setParamColumn(QString paramNameChinese, int col){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	c_fp.columnPos = col;
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}
/*-------------------------------------------------
		�߼� - ������ֻ������һ��������Ƕ�׵�ȫ�����ֻ࣬��һ����
*/
void C_FastClass::setClassPrimaryKeyId(QString paramNameChinese){
	if (paramNameChinese == ""){ return; }
	#ifdef DEBUG_OPEN_PRO
		if (this->classPrimaryKeyId != "" &&
			this->classPrimaryKeyId != paramNameChinese){
			qDebug() << "[" + this->classNameChinese + "]�������е����� " + this->classPrimaryKeyId + " �������� " + paramNameChinese + " ��ͻ";
			Q_ASSERT(false);
		}
	#else

	#endif
	this->classPrimaryKeyId = paramNameChinese;
}
void C_FastClass::setClassPrimaryKeyName(QString paramNameChinese){
	if (paramNameChinese == ""){ return; }
	#ifdef DEBUG_OPEN_PRO
		if (this->classPrimaryKeyName != "" &&
			this->classPrimaryKeyName != paramNameChinese){
			qDebug() << "[" + this->classNameChinese + "]�������е����� " + this->classPrimaryKeyName + " �������� " + paramNameChinese + " ��ͻ";
			Q_ASSERT(false);
		}
	#else

	#endif
	this->classPrimaryKeyName = paramNameChinese;
}
QString C_FastClass::getClassPrimaryKeyId(){
	return this->classPrimaryKeyId;
}
QString C_FastClass::getClassPrimaryKeyName(){
	return this->classPrimaryKeyName;
}


/*-------------------------------------------------
		��ȡ - ȫ��������/Ӣ����
*/
QStringList C_FastClass::getParamChineseNameList(){
	return nameChinese_list;
}
QStringList C_FastClass::getParamEnglishNameList(){
	return nameEnglish_list;
}
/*-------------------------------------------------
		��ȡ - �������������FastClass��
*/
QList<C_FastClassParam> C_FastClass::getParamList(){
	return paramList;
}
/*-------------------------------------------------
		��ȡ - �����������FastClassȫ����̯����
*/
QList<C_FastClassParam> C_FastClass::getConvertedParamList(){
	QList<C_FastClassParam> result_list = QList<C_FastClassParam>();
	for (int i = 0; i < this->paramList.count(); i++){
		C_FastClassParam c_fp = this->paramList.at(i);
		if (c_fp.type == "FastClass"){
			result_list.append(c_fp.fastClass.getConvertedParamList());
		}else{
			result_list.push_back(c_fp);
		}
	}
	return result_list;
}
/*-------------------------------------------------
		��ȡ - �жϿ�
*/
bool C_FastClass::isNull(){
	if (this->classNameChinese == ""){ return true; }
	return false;
}
/*-------------------------------------------------
		��ȡ - ��������
*/
QJsonObject C_FastClass::getExampleDataInChinese(){
	QJsonObject result_obj = QJsonObject();
	QList<C_FastClassParam> p_list = this->getConvertedParamList();
	for (int i = 0; i < p_list.count(); i++){
		C_FastClassParam c_fp = p_list.at(i);
		result_obj.insert(c_fp.nameChinese,c_fp.defaultValue);
	}
	return result_obj;
}
QJsonObject C_FastClass::getExampleDataInEnglish(){
	QJsonObject result_obj = QJsonObject();
	QList<C_FastClassParam> p_list = this->getConvertedParamList();
	for (int i = 0; i < p_list.count(); i++){
		C_FastClassParam c_fp = p_list.at(i);
		if (c_fp.nameEnglish.isEmpty()){
			result_obj.insert("�ղ�������" + c_fp.nameChinese+"��", c_fp.defaultValue);
		}else{
			result_obj.insert(c_fp.nameEnglish, c_fp.defaultValue);
		}
	}
	return result_obj;
}
QJsonObject C_FastClass::getExampleDataInShowing(){
	QJsonObject result_obj = QJsonObject();
	QList<C_FastClassParam> p_list = this->getConvertedParamList();
	for (int i = 0; i < p_list.count(); i++){
		C_FastClassParam c_fp = p_list.at(i);
		if (c_fp.getShowingName().isEmpty()){
			result_obj.insert("�ղ�������" + c_fp.nameChinese+"��", c_fp.defaultValue);
		}else{
			result_obj.insert(c_fp.getShowingName(), c_fp.defaultValue);
		}
	}
	return result_obj;
}



/*-------------------------------------------------
		���⹦�� - ��QJsonObject�����ݣ����ݸ���ĸ�ʽ����䵽table��
*/
void C_FastClass::fillDataInTableWidget(QTableWidget* tableWidget, QList<QJsonObject> data_list){
	QList<C_FastClassParam> temp_cols = this->getConvertedParamList();
	QList<QJsonObject> temp_rows = data_list;
	tableWidget->setColumnCount(temp_cols.size());
	tableWidget->setRowCount(temp_rows.size());

	// > ��ͷ
	QStringList col_names = QStringList();
	for (int i = 0; i < temp_cols.size(); i++) {
		C_FastClassParam c_fp = temp_cols.at(i);
		col_names.append(c_fp.getShowingName());
	}
	tableWidget->setHorizontalHeaderLabels(col_names);

	// > ������
	for (int i = 0; i < temp_rows.size(); i++) {
		QJsonObject obj = temp_rows.at(i);
		for (int j = 0; j < temp_cols.size(); j++) {
			C_FastClassParam c_fp = temp_cols.at(j);
			QString data = "";		// > ���Ĳ�����ֱ����ʾ 
			data = obj.value(c_fp.nameChinese).toString();
			if (c_fp.isSI){			// > SIת��
				data = TTool::_to_QString_bySI_(data);
			}
			if (c_fp.type == "Double"){	// > double�Զ�����׺����λ��
				data += c_fp.suffix;
			}
			tableWidget->setItem(i, j, new QTableWidgetItem(data));
		}
	}
}
/*-------------------------------------------------
		���⹦�� - ��QJsonObject�����ݣ����ݸ���ĸ�ʽ����䵽table��
*/
void C_FastClass::fillDataInTableWidgetEnglish(QTableWidget* tableWidget, QList<QJsonObject> data_list){
	QList<C_FastClassParam> temp_cols = this->getConvertedParamList();
	QList<QJsonObject> temp_rows = data_list;
	tableWidget->setColumnCount(temp_cols.size());
	tableWidget->setRowCount(temp_rows.size());

	// > ��ͷ
	QStringList col_names = QStringList();
	for (int i = 0; i < temp_cols.size(); i++) {
		C_FastClassParam c_fp = temp_cols.at(i);
		col_names.append(c_fp.getShowingName());
	}
	tableWidget->setHorizontalHeaderLabels(col_names);

	// > ������
	for (int i = 0; i < temp_rows.size(); i++) {
		QJsonObject obj = temp_rows.at(i);
		for (int j = 0; j < temp_cols.size(); j++) {
			C_FastClassParam c_fp = temp_cols.at(j);
			QString data = "";		// > Ӣ�Ĳ�����double -> QString ��ʾ 
			if (obj.value(c_fp.nameEnglish).isDouble()){
				data =  QString::number(obj.value(c_fp.nameEnglish).toDouble());
			}else{
				data = obj.value(c_fp.nameEnglish).toString();
			}
			if (c_fp.isSI){			// > SIת��
				data = TTool::_to_QString_bySI_(data);
			}
			if (c_fp.type == "Double"){	// > double�Զ�����׺����λ��
				data += c_fp.suffix;
			}
			tableWidget->setItem(i, j, new QTableWidgetItem(data));
		}
	}
}
/*-------------------------------------------------
		���⹦�� - jsonobj Ӣ�� -> ����
*/
QJsonObject C_FastClass::convertEnglishToChinese(QJsonObject data){
	QList<C_FastClassParam> param_list = this->getConvertedParamList();
	QJsonObject result_obj = QJsonObject();
	QStringList keys = data.keys();
	for (int i = 0; i < keys.length(); i++){
		QString key = keys.at(i);
		QJsonValue value = data.value(key);

		bool inserted = false;
		for (int j = 0; j < param_list.length(); j++){
			C_FastClassParam c_p = param_list.at(j);
			if (c_p.nameEnglish == key){

				// > ���Ĳ�����double -> QString������ȫ��QString�� 
				if (value.isDouble()){
					result_obj.insert(c_p.nameChinese, QString::number(value.toDouble()));
				}
				else{
					result_obj.insert(c_p.nameChinese, value);
				}
				inserted = true;
			}
		}
		if (inserted == false){
			result_obj.insert(key, value);
		}
	}
	return result_obj;
}
/*-------------------------------------------------
		���⹦�� - jsonobj Ӣ�� -> ���ģ�ת���˵��ӵ���
*/
QJsonObject C_FastClass::convertEnglishToChineseExcludely(QJsonObject data){
	QList<C_FastClassParam> param_list = this->getConvertedParamList();
	QJsonObject result_obj = QJsonObject();
	QStringList keys = data.keys();
	for (int i = 0; i < keys.length(); i++){
		QString key = keys.at(i);
		QJsonValue value = data.value(key);

		for (int j = 0; j < param_list.length(); j++){
			C_FastClassParam c_p = param_list.at(j);
			if (c_p.nameEnglish == key){

				// > ���Ĳ�����double -> QString������ȫ��QString�� 
				if (value.isDouble()){
					result_obj.insert(c_p.nameChinese, QString::number(value.toDouble()));
				}
				else{
					result_obj.insert(c_p.nameChinese, value);
				}
			}
		}
	}
	return result_obj;
}
/*-------------------------------------------------
		���⹦�� - jsonobj ���� -> Ӣ��
*/
QJsonObject C_FastClass::convertChineseToEnglish(QJsonObject data){
	QList<C_FastClassParam> param_list = this->getConvertedParamList();
	QJsonObject result_obj = QJsonObject();
	QStringList keys = data.keys();
	for (int i = 0; i < keys.length(); i++){
		QString key = keys.at(i);
		QJsonValue value = data.value(key);
		if (key.contains("<-SPAN->")){ continue; }		//��ռλ������

		bool inserted = false;
		for (int j = 0; j < param_list.length(); j++){
			C_FastClassParam c_p = param_list.at(j);
			if (c_p.nameChinese == key){

				// > Ӣ�Ĳ����� QString -> double
				if (value.isString() && c_p.type == "Double"){
					result_obj.insert(c_p.nameEnglish, value.toString().toDouble());
				}
				else if (value.isString() && c_p.type == "Int"){
					result_obj.insert(c_p.nameEnglish, value.toString().toLong());
				}
				else{
					result_obj.insert(c_p.nameEnglish, value);
				}
				inserted = true;
			}
		}
		if (inserted == false){
			result_obj.insert(key, value);
		}
	}
	return result_obj;
}
/*-------------------------------------------------
		���⹦�� - jsonobj ���� -> Ӣ�ģ�ת���˵��ӵ���
*/
QJsonObject C_FastClass::convertChineseToEnglishExcludely(QJsonObject data){
	QList<C_FastClassParam> param_list = this->getConvertedParamList();
	QJsonObject result_obj = QJsonObject();
	QStringList keys = data.keys();
	for (int i = 0; i < keys.length(); i++){
		QString key = keys.at(i);
		QJsonValue value = data.value(key);
		if (key.contains("<-SPAN->")){ continue; }		//��ռλ������

		for (int j = 0; j < param_list.length(); j++){
			C_FastClassParam c_p = param_list.at(j);
			if (c_p.nameChinese == key){

				// > Ӣ�Ĳ����� QString -> double
				if (value.isString() && c_p.type == "Double"){
					result_obj.insert(c_p.nameEnglish, value.toString().toDouble());
				}
				else if (value.isString() && c_p.type == "Int"){
					result_obj.insert(c_p.nameEnglish, value.toString().toInt());
				}
				else{
					result_obj.insert(c_p.nameEnglish, value);
				}
			}
		}
	}
	return result_obj;
}



/*
-----==========================================================-----
		�ࣺ		���������.h
-----==========================================================-----
*/
C_FastClassParam::C_FastClassParam(){

	// > һ��
	this->nameChinese = "";						//������
	this->type = "";							//��������
	this->widgetType = "";						//������
	this->defaultValue = "";					//Ĭ��ֵ
	this->chooseList = QStringList();			//ѡ���б�

	// > ����
	this->nameEnglish = "";						//Ӣ����
	this->nameShowing = "";						//ʵ����ʾ��
	this->chooseShowingList = QStringList();	//ʵ����ʾѡ��
	this->notNull = false;						//������
	this->suffix = "";							//��׺����λ��
	this->isSI = false;							//SIд��

	// > ����
	this->isEditable = true;					//�ɱ༭
	this->comboxHeight = 24;					//������ - ѡ��߶�
	this->valueMin = 0;							//���ֿؼ� - ��Сֵ
	this->valueMax = 99;						//���ֿؼ� - ���ֵ
	this->singleStep = 1;						//���ֿؼ� - ���½���
	this->decimals = 8;							//���ֿؼ� - λ��
	this->num_prefix = "";						//���ֿؼ� - ǰ׺
	this->num_suffix = "";						//���ֿؼ� - ��׺
	this->plainTextHeight = 80;					//�ı��� - �߶�
	this->plainTextEscape = false;				//�ı��� - �洢ת��

	// > �߼�
	this->columnPos = 0;
	//this->fastClass;

	// > EX������
	this->tempA = "";
	this->tempB = "";
	this->tempC = "";
	this->tempD = "";
	this->tempE = "";
	this->tempF = "";
}
C_FastClassParam::~C_FastClassParam(){
}

/*-------------------------------------------------
		���������
*/
const bool C_FastClassParam::operator== (const C_FastClassParam& a)const {
	return this->nameChinese == a.nameChinese;
}
/*-------------------------------------------------
		�жϿ�
*/
bool C_FastClassParam::isNull(){
	if (this->nameChinese == ""){ return true; }
	if (this->type == ""){ return true; }
	return false;
}
/*-------------------------------------------------
		��ȡ - ��ʾ�ı���
*/
QString C_FastClassParam::getShowingName(){
	if (this->nameShowing != ""){ return nameShowing; }
	return removeSpecialChineseCharacter(this->nameChinese);
}
/*-------------------------------------------------
		��ȡ - ȥ���������ı�ʶ���ַ�
*/
QString C_FastClassParam::removeSpecialChineseCharacter(QString nameChinese){
	return nameChinese.replace(QRegExp("<-[^>]*->"), "");
}