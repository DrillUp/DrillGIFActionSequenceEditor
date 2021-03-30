#include "stdafx.h"

#include "c_FastClass.h"
#include "../../common/TTool.h"


/*
-----==========================================================-----
		类：		快速类定义.cpp
		版本：		v1.14
		作者：		drill_up
		所属模块：	工具模块
		功能：		定义一个表单填写用的类定义，快速表单、快速窗口通用。
					【这里类似于定义model，所有字段都是字符串】
					【类名、参数名，用中文】
					
		功能：		1.一设：	类名、Double、Int、QString、Int多选项、QString多选项、Boolean
					2.二设：	英文名、显示名、必填项、后缀
					3.三设：	SI写法、不可编辑、默认值修改
					4.高级：	所在列数、类嵌套
					特殊：		QJsonObject填充到table中、QJsonObject中英文转换

		内置约束：	1.中文名和英文名不能重复，程序会检查重复，并报错。（包括类名）
					2.嵌套类 里面至少要有一个参数，不然程序会检查报错。
					3.主键只能设置一个，两个不同主键的子类，不能嵌套在一起。

		自动规则：	1.中文参数键值全部为QString。
					  英文参数会根据类型自动转double和int。
					2.后缀为"Hz"时，自动SI写法。
					3.类型为double时，显示tablewidget时自动添加后缀。
					4.标识格式："<-标识字符->参数名"。
					  中文参数名中该格式的字符串不会被显示出来。（也可以用showingName设置）
					
		使用方法：
				>创建
					c_class = C_FastClass("目标模型");						//创建相当于直接填写一堆参数集，作用于快速表单、快速窗口
					c_class.addDouble				("位置", "0.0");		//详细见 快速表单、快速窗口 的使用方法
					c_class.addDouble				("方向", "0.0"); 
					c_class.addQString				("说明", "0.0");
					c_class.addQString				("描述", "0.0");
				>参数修改
					c_class.addQString				("脉冲宽度", "0");		//添加中文参数后，可以依次添加各个属性
					c_class.setParamEnglishName		("脉冲宽度", "ModPulMaxInterfPulseWidth");
					c_class.setParamShowingName		("脉冲宽度", "最大干扰脉冲宽度");
					c_class.setParamSuffix			("脉冲宽度", "μs");
				>类嵌套
					c_class = C_FastClass("脉冲");							//嵌套后，你仍然需要确保中文参数名不重复
						C_FastClass class_6 = C_FastClass("信号范围");
						class_6.addQString						("信号下限", "0");
						class_6.setParamEnglishName				("信号下限", "ModPulSignalCarrierFreqStart");
						class_6.setParamSuffix					("信号下限", "");
						class_6.addQString						("信号上限", "0");
						class_6.setParamEnglishName				("信号上限", "ModPulSignalCarrierFreqEnd");
						class_6.setParamSuffix					("信号上限", "Hz");
					c_class.addFastClassOneLine				(class_6);
				>特殊
					this->m_target_class.addQString("<-SPAN->xxxxx", "");	//含有指定前缀的变量，将会成为无意义的空占位变量

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
		私有 - 根据类名获取到类
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
		私有 - 判断重名
*/
void C_FastClass::checkDuplicatedChineseValue(QString nameChinese){
	if (nameChinese == ""){ return; }
	#ifdef DEBUG_OPEN_PRO
		if (nameChinese_list.indexOf(nameChinese) != -1){
			qDebug() << "[" + this->classNameChinese + "]发现重名中文参数：" + nameChinese;
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
			qDebug() << "[" + this->classNameChinese + "]发现重名英文参数：" + nameEnglish;
			Q_ASSERT(false);
		}
	#else
	
	#endif
	nameEnglish_list.push_back(nameEnglish);
}

/*-------------------------------------------------
		一设 - 类名
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
		一设 - Double
*/
void C_FastClass::addDouble(QString paramName){ this->addDouble(paramName, ""); }	//(重载函数)
void C_FastClass::addDouble(QString paramName, QString defaultValue){
	C_FastClassParam c_f = C_FastClassParam();
	c_f.nameChinese = paramName;			//中文名
	c_f.type = "Double";					//数据类型
	c_f.widgetType = "QLineEdit";			//表单类型
	c_f.defaultValue = defaultValue;		//默认值
	c_f.chooseList = QStringList();			//选项列表
	this->checkDuplicatedChineseValue(paramName);
	this->paramList.push_back(c_f);
}
/*-------------------------------------------------
		一设 - Int
*/
void C_FastClass::addInt(QString paramName){ this->addInt(paramName, ""); }		//(重载函数)
void C_FastClass::addInt(QString paramName, QString defaultValue){
	C_FastClassParam c_f = C_FastClassParam();
	c_f.nameChinese = paramName;			//中文名
	c_f.type = "Int";						//数据类型
	c_f.widgetType = "QLineEdit";			//表单类型
	c_f.defaultValue = defaultValue;		//默认值
	c_f.chooseList = QStringList();			//选项列表
	this->checkDuplicatedChineseValue(paramName);
	this->paramList.push_back(c_f);
}
/*-------------------------------------------------
		一设 - QString
*/
void C_FastClass::addQString(QString paramName){ this->addQString(paramName, ""); }	//(重载函数)
void C_FastClass::addQString(QString paramName, QString defaultValue){
	C_FastClassParam c_f = C_FastClassParam();
	c_f.nameChinese = paramName;			//中文名
	c_f.type = "QString";					//数据类型
	c_f.widgetType = "QLineEdit";			//表单类型
	c_f.defaultValue = defaultValue;		//默认值
	c_f.chooseList = QStringList();			//选项列表
	this->checkDuplicatedChineseValue(paramName);
	this->paramList.push_back(c_f);
}
/*-------------------------------------------------
		一设 - Int多选项
*/
void C_FastClass::addIntWithChoose(QString paramName, int defaultIndex, QList<int> indexList){
	// > int转字符串
	QStringList chooseList = QStringList();
	for (int i = 0; i < indexList.count(); i++){ chooseList.append(QString::number(indexList.at(i))); }
	// > 参数赋值
	this->addIntWithChoose(paramName, defaultIndex, indexList, chooseList);		//(重载函数)
}
void C_FastClass::addIntWithChoose(QString paramName, int defaultIndex, QList<int> indexList, QStringList chooseShowingList){
	// > int转字符串
	QStringList chooseList = QStringList();
	for (int i = 0; i < indexList.count(); i++){ chooseList.append(QString::number(indexList.at(i))); }
	// > 参数赋值
	C_FastClassParam c_f = C_FastClassParam();
	c_f.nameChinese = paramName;						//中文名
	c_f.type = "Int";									//数据类型
	c_f.widgetType = "QComboBox";						//表单类型
	c_f.defaultValue = QString::number(defaultIndex);	//默认值
	c_f.chooseList = chooseList;						//选项列表
	c_f.chooseShowingList = chooseShowingList;			//实际显示选项（二设）
	this->checkDuplicatedChineseValue(paramName);
	this->paramList.push_back(c_f);
}
/*-------------------------------------------------
		一设 - QString多选项
*/
void C_FastClass::addQStringWithChoose(QString paramName, QString defaultValue, QStringList chooseList){
	this->addQStringWithChoose(paramName, defaultValue, chooseList, chooseList);	//(重载函数)
}
void C_FastClass::addQStringWithChoose(QString paramName, QString defaultValue, QStringList chooseList, QStringList chooseShowingList){
	C_FastClassParam c_f = C_FastClassParam();
	c_f.nameChinese = paramName;						//中文名
	c_f.type = "QString";								//数据类型
	c_f.widgetType = "QComboBox";						//表单类型
	c_f.defaultValue = defaultValue;					//默认值
	c_f.chooseList = chooseList;						//选项列表
	c_f.chooseShowingList = chooseShowingList;			//实际显示选项（二设）
	this->checkDuplicatedChineseValue(paramName);
	this->paramList.push_back(c_f);
}
/*-------------------------------------------------
		一设 - Boolean
*/
void C_FastClass::addBoolean(QString paramName){ this->addBoolean(paramName, false); }	//(重载函数)
void C_FastClass::addBoolean(QString paramName, QString defaultValue){
	C_FastClassParam c_f = C_FastClassParam();
	c_f.nameChinese = paramName;						//中文名
	c_f.type = "Boolean";								//数据类型
	c_f.widgetType = "QCheckBox";						//表单类型
	c_f.defaultValue = (defaultValue == "true" ? "true" : "false");	//默认值
	c_f.chooseList = QStringList();						//选项列表
	this->checkDuplicatedChineseValue(paramName);
	this->paramList.push_back(c_f);
}

/*-------------------------------------------------
		一设（嵌套） - 快速类嵌套
*/
void C_FastClass::addFastClassQWidget(C_FastClass fastClass){
	C_FastClassParam c_f = C_FastClassParam();
	c_f.nameChinese = fastClass.getClassNameChinese();
	c_f.nameEnglish = fastClass.getClassNameEnglish();
	c_f.type = "FastClass";
	c_f.widgetType = "QWidget";
	c_f.fastClass = fastClass;

	// > 检查类参数重名
	QStringList name_list = fastClass.getParamChineseNameList();
	Q_ASSERT(name_list.count() > 0);
	for (int i = 0; i < name_list.count(); i++){
		this->checkDuplicatedChineseValue(name_list.at(i));
	}
	QStringList eng_name_list = fastClass.getParamEnglishNameList();
	for (int i = 0; i < eng_name_list.count(); i++){
		this->checkDuplicatedEnglishValue(eng_name_list.at(i));
	}

	// > 主键赋值
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

	// > 检查类参数重名
	QStringList name_list = fastClass.getParamChineseNameList();
	Q_ASSERT(name_list.count() > 0);
	for (int i = 0; i < name_list.count(); i++){
		this->checkDuplicatedChineseValue(name_list.at(i));
	}
	QStringList eng_name_list = fastClass.getParamEnglishNameList();
	for (int i = 0; i < eng_name_list.count(); i++){
		this->checkDuplicatedEnglishValue(eng_name_list.at(i));
	}

	// > 主键赋值
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

	// > 检查类参数重名
	QStringList name_list = fastClass.getParamChineseNameList();
	Q_ASSERT(name_list.count() > 0);
	for (int i = 0; i < name_list.count(); i++){
		this->checkDuplicatedChineseValue(name_list.at(i));
	}
	QStringList eng_name_list = fastClass.getParamEnglishNameList();
	for (int i = 0; i < eng_name_list.count(); i++){
		this->checkDuplicatedEnglishValue(eng_name_list.at(i));
	}

	// > 主键赋值
	this->setClassPrimaryKeyId(fastClass.getClassPrimaryKeyId());
	this->setClassPrimaryKeyName(fastClass.getClassPrimaryKeyName());

	this->paramList.push_back(c_f);
}
/*-------------------------------------------------
		一设（嵌套） - 获取子快速类
*/
C_FastClass C_FastClass::getChildClass(QString classNameChinese){
	C_FastClassParam c_fp = this->getParamByName(classNameChinese);
	if (c_fp.type == "FastClass"){
		return c_fp.fastClass;
	}
	return C_FastClass();
}

/*-------------------------------------------------
		二设 - 英文名
*/
void C_FastClass::setParamEnglishName(QString paramNameChinese, QString paramNameEnglish){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	c_fp.nameEnglish = paramNameEnglish;
	this->checkDuplicatedEnglishValue(paramNameEnglish);		//检查英文名重复
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}
/*-------------------------------------------------
		二设 - 显示名
*/
void C_FastClass::setParamShowingName(QString paramNameChinese, QString showingName){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	c_fp.nameShowing = showingName;
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}
/*-------------------------------------------------
		二设 - 必填项
*/
void C_FastClass::setParamNotNull(QString paramNameChinese, bool notNull){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	c_fp.notNull = notNull;
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}
/*-------------------------------------------------
		二设 - 后缀
*/
void C_FastClass::setParamSuffix(QString paramNameChinese, QString suffix){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	c_fp.suffix = suffix;
	if (suffix == "Hz"){ c_fp.isSI = true; }	//自动视为si写法
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}
/*-------------------------------------------------
		二设 - SI写法
*/
void C_FastClass::setParamSI(QString paramNameChinese, bool isSI){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	if (c_fp.isNull()){ return; }
	c_fp.isSI = isSI;
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}

/*-------------------------------------------------
		三设 - 不可编辑
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
		三设 - 设置Int类型为数字控件
*/
void C_FastClass::setInt_UI_ToQSpinBox(QString paramNameChinese, long valueMin, long valueMax){
	this->setInt_UI_ToQSpinBox(paramNameChinese, valueMin, valueMax, 1);	//（重载函数）
}
void C_FastClass::setInt_UI_ToQSpinBox(QString paramNameChinese, long valueMin, long valueMax, long singleStep){
	this->setInt_UI_ToQSpinBox(paramNameChinese, valueMin, valueMax, 1, "", "");	//（重载函数）
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
		三设 - 设置Double类型为数字控件
*/
void C_FastClass::setDouble_UI_ToQDoubleSpinBox(QString paramNameChinese, double valueMin, double valueMax){
	this->setDouble_UI_ToQDoubleSpinBox(paramNameChinese, valueMin, valueMax, 1, 4);	//（重载函数）
}
void C_FastClass::setDouble_UI_ToQDoubleSpinBox(QString paramNameChinese, double valueMin, double valueMax, double singleStep, int decimals){
	this->setDouble_UI_ToQDoubleSpinBox(paramNameChinese, valueMin, valueMax, 1, 4, "", "");	//（重载函数）
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
		三设 - 设置Boolean类型为下拉框
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
		三设 - 设置QString类型为文本域
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
		四设 - 默认值修改
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
		四设 - 下拉框的可选项修改
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
		高级 - 所在列数
*/
void C_FastClass::setParamColumn(QString paramNameChinese, int col){
	C_FastClassParam c_fp = this->getParamByName(paramNameChinese);
	c_fp.columnPos = col;
	this->paramList.replace(this->getIndexByName(paramNameChinese), c_fp);
}
/*-------------------------------------------------
		高级 - 主键（只能设置一个，包括嵌套的全部子类，只能一个）
*/
void C_FastClass::setClassPrimaryKeyId(QString paramNameChinese){
	if (paramNameChinese == ""){ return; }
	#ifdef DEBUG_OPEN_PRO
		if (this->classPrimaryKeyId != "" &&
			this->classPrimaryKeyId != paramNameChinese){
			qDebug() << "[" + this->classNameChinese + "]发现已有的主键 " + this->classPrimaryKeyId + " 与新主键 " + paramNameChinese + " 冲突";
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
			qDebug() << "[" + this->classNameChinese + "]发现已有的主键 " + this->classPrimaryKeyName + " 与新主键 " + paramNameChinese + " 冲突";
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
		获取 - 全部中文名/英文名
*/
QStringList C_FastClass::getParamChineseNameList(){
	return nameChinese_list;
}
QStringList C_FastClass::getParamEnglishNameList(){
	return nameEnglish_list;
}
/*-------------------------------------------------
		获取 - 快速类参数（含FastClass）
*/
QList<C_FastClassParam> C_FastClass::getParamList(){
	return paramList;
}
/*-------------------------------------------------
		获取 - 快速类参数（FastClass全部被摊开）
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
		获取 - 判断空
*/
bool C_FastClass::isNull(){
	if (this->classNameChinese == ""){ return true; }
	return false;
}
/*-------------------------------------------------
		获取 - 样例数据
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
			result_obj.insert("空参数名【" + c_fp.nameChinese+"】", c_fp.defaultValue);
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
			result_obj.insert("空参数名【" + c_fp.nameChinese+"】", c_fp.defaultValue);
		}else{
			result_obj.insert(c_fp.getShowingName(), c_fp.defaultValue);
		}
	}
	return result_obj;
}



/*-------------------------------------------------
		特殊功能 - 将QJsonObject的数据，根据该类的格式，填充到table中
*/
void C_FastClass::fillDataInTableWidget(QTableWidget* tableWidget, QList<QJsonObject> data_list){
	QList<C_FastClassParam> temp_cols = this->getConvertedParamList();
	QList<QJsonObject> temp_rows = data_list;
	tableWidget->setColumnCount(temp_cols.size());
	tableWidget->setRowCount(temp_rows.size());

	// > 表头
	QStringList col_names = QStringList();
	for (int i = 0; i < temp_cols.size(); i++) {
		C_FastClassParam c_fp = temp_cols.at(i);
		col_names.append(c_fp.getShowingName());
	}
	tableWidget->setHorizontalHeaderLabels(col_names);

	// > 表数据
	for (int i = 0; i < temp_rows.size(); i++) {
		QJsonObject obj = temp_rows.at(i);
		for (int j = 0; j < temp_cols.size(); j++) {
			C_FastClassParam c_fp = temp_cols.at(j);
			QString data = "";		// > 中文参数，直接显示 
			data = obj.value(c_fp.nameChinese).toString();
			if (c_fp.isSI){			// > SI转换
				data = TTool::_to_QString_bySI_(data);
			}
			if (c_fp.type == "Double"){	// > double自动带后缀（单位）
				data += c_fp.suffix;
			}
			tableWidget->setItem(i, j, new QTableWidgetItem(data));
		}
	}
}
/*-------------------------------------------------
		特殊功能 - 将QJsonObject的数据，根据该类的格式，填充到table中
*/
void C_FastClass::fillDataInTableWidgetEnglish(QTableWidget* tableWidget, QList<QJsonObject> data_list){
	QList<C_FastClassParam> temp_cols = this->getConvertedParamList();
	QList<QJsonObject> temp_rows = data_list;
	tableWidget->setColumnCount(temp_cols.size());
	tableWidget->setRowCount(temp_rows.size());

	// > 表头
	QStringList col_names = QStringList();
	for (int i = 0; i < temp_cols.size(); i++) {
		C_FastClassParam c_fp = temp_cols.at(i);
		col_names.append(c_fp.getShowingName());
	}
	tableWidget->setHorizontalHeaderLabels(col_names);

	// > 表数据
	for (int i = 0; i < temp_rows.size(); i++) {
		QJsonObject obj = temp_rows.at(i);
		for (int j = 0; j < temp_cols.size(); j++) {
			C_FastClassParam c_fp = temp_cols.at(j);
			QString data = "";		// > 英文参数，double -> QString 显示 
			if (obj.value(c_fp.nameEnglish).isDouble()){
				data =  QString::number(obj.value(c_fp.nameEnglish).toDouble());
			}else{
				data = obj.value(c_fp.nameEnglish).toString();
			}
			if (c_fp.isSI){			// > SI转换
				data = TTool::_to_QString_bySI_(data);
			}
			if (c_fp.type == "Double"){	// > double自动带后缀（单位）
				data += c_fp.suffix;
			}
			tableWidget->setItem(i, j, new QTableWidgetItem(data));
		}
	}
}
/*-------------------------------------------------
		特殊功能 - jsonobj 英文 -> 中文
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

				// > 中文参数，double -> QString（中文全部QString） 
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
		特殊功能 - jsonobj 英文 -> 中文（转不了的扔掉）
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

				// > 中文参数，double -> QString（中文全部QString） 
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
		特殊功能 - jsonobj 中文 -> 英文
*/
QJsonObject C_FastClass::convertChineseToEnglish(QJsonObject data){
	QList<C_FastClassParam> param_list = this->getConvertedParamList();
	QJsonObject result_obj = QJsonObject();
	QStringList keys = data.keys();
	for (int i = 0; i < keys.length(); i++){
		QString key = keys.at(i);
		QJsonValue value = data.value(key);
		if (key.contains("<-SPAN->")){ continue; }		//空占位符变量

		bool inserted = false;
		for (int j = 0; j < param_list.length(); j++){
			C_FastClassParam c_p = param_list.at(j);
			if (c_p.nameChinese == key){

				// > 英文参数， QString -> double
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
		特殊功能 - jsonobj 中文 -> 英文（转不了的扔掉）
*/
QJsonObject C_FastClass::convertChineseToEnglishExcludely(QJsonObject data){
	QList<C_FastClassParam> param_list = this->getConvertedParamList();
	QJsonObject result_obj = QJsonObject();
	QStringList keys = data.keys();
	for (int i = 0; i < keys.length(); i++){
		QString key = keys.at(i);
		QJsonValue value = data.value(key);
		if (key.contains("<-SPAN->")){ continue; }		//空占位符变量

		for (int j = 0; j < param_list.length(); j++){
			C_FastClassParam c_p = param_list.at(j);
			if (c_p.nameChinese == key){

				// > 英文参数， QString -> double
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
		类：		快速类参数.h
-----==========================================================-----
*/
C_FastClassParam::C_FastClassParam(){

	// > 一设
	this->nameChinese = "";						//中文名
	this->type = "";							//数据类型
	this->widgetType = "";						//表单类型
	this->defaultValue = "";					//默认值
	this->chooseList = QStringList();			//选项列表

	// > 二设
	this->nameEnglish = "";						//英文名
	this->nameShowing = "";						//实际显示名
	this->chooseShowingList = QStringList();	//实际显示选项
	this->notNull = false;						//必填项
	this->suffix = "";							//后缀（单位）
	this->isSI = false;							//SI写法

	// > 三设
	this->isEditable = true;					//可编辑
	this->comboxHeight = 24;					//下拉框 - 选项高度
	this->valueMin = 0;							//数字控件 - 最小值
	this->valueMax = 99;						//数字控件 - 最大值
	this->singleStep = 1;						//数字控件 - 上下阶数
	this->decimals = 8;							//数字控件 - 位数
	this->num_prefix = "";						//数字控件 - 前缀
	this->num_suffix = "";						//数字控件 - 后缀
	this->plainTextHeight = 80;					//文本域 - 高度
	this->plainTextEscape = false;				//文本域 - 存储转义

	// > 高级
	this->columnPos = 0;
	//this->fastClass;

	// > EX用数据
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
		运算符重载
*/
const bool C_FastClassParam::operator== (const C_FastClassParam& a)const {
	return this->nameChinese == a.nameChinese;
}
/*-------------------------------------------------
		判断空
*/
bool C_FastClassParam::isNull(){
	if (this->nameChinese == ""){ return true; }
	if (this->type == ""){ return true; }
	return false;
}
/*-------------------------------------------------
		获取 - 显示文本名
*/
QString C_FastClassParam::getShowingName(){
	if (this->nameShowing != ""){ return nameShowing; }
	return removeSpecialChineseCharacter(this->nameChinese);
}
/*-------------------------------------------------
		获取 - 去掉中文名的标识用字符
*/
QString C_FastClassParam::removeSpecialChineseCharacter(QString nameChinese){
	return nameChinese.replace(QRegExp("<-[^>]*->"), "");
}