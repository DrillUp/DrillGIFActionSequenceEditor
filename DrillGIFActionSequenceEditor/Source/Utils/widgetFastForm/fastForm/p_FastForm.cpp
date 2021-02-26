#include "stdafx.h"

#include "p_FastForm.h"
#include "../../common/TTool.h"

/*
-----==========================================================-----
		类：		快速表单.cpp
		版本：		v1.13
		作者：		drill_up
		所属模块：	工具模块
		功能：		快速绘制出需要填写的表单列。
					并且能够快速获取表单的值。

		功能：		1.简单参数组建
					2.快速类参数组建（包括嵌套类）
					3.获取参数群组
					
		使用方法：
				>简单准备
					m_fastForm_style = new P_FastForm(ui.widget_styleParams);
					m_fastForm_style->prepareQLineEdit("径向尺寸重合度", "0.5");
					m_fastForm_style->prepareQLineEdit("频谱比", "0.9");
					m_fastForm_style->prepareQLineEdit("干信比", "2.5");
					m_fastForm_style->rebuildUI();
				>标准建立
					m_fastForm = new P_FastForm(ui.widget);				//将所有各种复杂的参数写入class
					m_fastForm->prepareFastClass(m_deviceStateClass);	//然后一次性生成【fastform会逐步展开里面的全部class然后生成控件】
					m_fastForm->rebuildUI();
				>删除
					m_fastForm_style->clearUI();
				>参数设置
					m_fastForm->setValueByName("发射机数量", 2);		//嵌套类的中文参数也能修改
					m_fastForm->setValueByName("接收机数量", 3);
				>参数获取
					QString name = m_fastForm_style->getValueByName("频谱比");		//注意，名字要与设置的一模一样
					QLineEdit* lineEdit = m_fastForm_style->getQLineEditByName("频谱比");
					

-----==========================================================-----
*/
P_FastForm::P_FastForm(QWidget* _parent)
{
	this->initWidgetAndLayout(_parent);
	this->m_form_list = QList<C_FastForm*>();
	this->m_depth = 0;
}

P_FastForm::~P_FastForm(){
}

/*-------------------------------------------------
		父控件初始化
*/
void P_FastForm::initWidgetAndLayout(QWidget* _parent){
	if (_parent->layout() == nullptr){
		this->m_layout = new QGridLayout();
		_parent->setLayout(this->m_layout);
	}
	QString layout_name = _parent->layout()->metaObject()->className();
	Q_ASSERT(layout_name == "QGridLayout");		//必须是QGridLayout
	this->m_parent = _parent;
	this->m_layout = qobject_cast<QGridLayout*>(_parent->layout());
}


/*-------------------------------------------------
		简单准备 - 单个标签
*/
void P_FastForm::prepareQLabel(QString name) {
	C_FastClassParam c_p = C_FastClassParam();
	c_p.nameChinese = "<-SPAN->"+name;
	c_p.widgetType = "QLabel";
	c_p.nameShowing = name;
	this->m_param_list.push_back(c_p);
}
/*-------------------------------------------------
		简单准备 - 单个弹簧
*/
void P_FastForm::prepareQSpacerItem() {
	C_FastClassParam c_p = C_FastClassParam();
	c_p.nameChinese = "<-SPAN->弹簧";
	c_p.widgetType = "QSpacerItem";
	this->m_param_list.push_back(c_p);
} 
/*-------------------------------------------------
		简单准备 - 标签 + 输入框
*/
void P_FastForm::prepareQLineEdit(QString name, QString default_value) {
	C_FastClassParam c_p = C_FastClassParam();
	c_p.nameChinese = name;
	c_p.defaultValue = default_value;
	c_p.type = "QString";
	c_p.widgetType = "QLineEdit";
	this->m_param_list.push_back(c_p);
}
/*-------------------------------------------------
		简单准备 - 标签 + 下拉框
*/
void P_FastForm::prepareQComboBox(QString name, QStringList choose_list, QString default_value) {
	C_FastClassParam c_p = C_FastClassParam();
	c_p.nameChinese = name;
	c_p.chooseList = choose_list;
	c_p.defaultValue = default_value;
	c_p.type = "QString";
	c_p.widgetType = "QComboBox";
	this->m_param_list.push_back(c_p);
}

/*-------------------------------------------------
		准备 - 快速类
*/
void P_FastForm::prepareFastClass(C_FastClass c_fc) {
	QList<C_FastClassParam> param_list = c_fc.getParamList();
	for (int i = 0; i < param_list.count(); i++){
		C_FastClassParam c_fcp = param_list.at(i);
		this->m_param_list.push_back(c_fcp);
	}
}
/*-------------------------------------------------
		准备 - 清理全部参数
*/
void P_FastForm::clearPrepare() {
	this->m_param_list.clear();
}
/*-------------------------------------------------
		准备 - 是否含准备参数
*/
bool P_FastForm::hasPreparedSequence(){
	return this->m_param_list.count() > 0;
}


/*-------------------------------------------------
		控件 - 重建控件
*/
void P_FastForm::rebuildUI(){

	// > 内容删除
	this->clearUI();

	// > 获取列
	int colCount = 0;
	for (int i = 0; i < this->m_param_list.length(); i++){
		C_FastClassParam c_fp = this->m_param_list.at(i);
		if (c_fp.columnPos > colCount){
			colCount = c_fp.columnPos;
		}
	}
	colCount += 1;

	// > 行初始化
	QList<int> rowIndex = QList<int>();
	for (int i = 0; i < colCount; i++){
		rowIndex.push_back(0);
	}

	// > 新建
	this->m_depth = 0;
	for (int i = 0; i < this->m_param_list.length(); i++){
		C_FastClassParam c_fp = this->m_param_list.at(i);
		int curRow = rowIndex.at(c_fp.columnPos);
		int curCol = c_fp.columnPos;

		this->_addSingle(c_fp, this->m_layout, curRow, curCol);

		rowIndex[c_fp.columnPos] += 1;
	}

	// > 清理预备的内容
	this->clearPrepare();
}

/*-------------------------------------------------
		构建 - 变量容器
*/
C_FastForm* P_FastForm::_createForm(){
	return new C_FastForm();
}
/*-------------------------------------------------
		构建 - 获取深度
*/
int P_FastForm::getDepth(QString statusTipData){
	int result = -1;
	QStringList datas = statusTipData.split(",");
	for (int i = 0; i < datas.count(); i++){
		QString data = datas.at(i);
		if (data.contains("depth=")){
			result = data.replace("depth=","").toInt();
			return result;
		}
	}
	return result;
}
/*-------------------------------------------------
		构建 - 初始化
*/
void P_FastForm::_initForm(C_FastForm* c_f){
	C_FastClassParam c_fp = c_f->params;

	// > 弹簧
	if (c_fp.widgetType == "QSpacerItem"){
		c_f->spacerItem = new QSpacerItem(20, 255, QSizePolicy::Minimum, QSizePolicy::Expanding);
	}
	// > 标签
	if (c_fp.widgetType == "QLabel"){
		c_f->label = new QLabel();
		c_f->label->setObjectName(c_fp.nameEnglish);
		c_f->label->setText(c_fp.getShowingName());
	}
	// > 编辑框
	if (c_fp.widgetType == "QLineEdit"){
		c_f->lineEdit = new QLineEdit();
		c_f->lineEdit->setObjectName(c_fp.nameEnglish);
		c_f->lineEdit->setText(c_fp.defaultValue);
		c_f->lineEdit->setEnabled(c_fp.isEditable);
		// > 必填项
		c_f->label = new QLabel();
		c_f->label->setObjectName(c_fp.nameEnglish);
		c_f->label->setText(c_fp.getShowingName());
		if (c_fp.notNull){
			c_f->label->setText(c_f->label->text() + " <span style = \"color:#ff0000;\">*</span>");
		}
		// > 后缀
		c_f->label_suffix = new QLabel();
		c_f->label_suffix->setObjectName(c_fp.nameEnglish);
		c_f->label_suffix->setText(c_fp.suffix);
		// > 校验器
		if (c_fp.type == "Double"){
			c_f->lineEdit->setValidator(new QDoubleValidator);
		}
		if (c_fp.isSI){
			c_f->lineEdit->setValidator(TTool::_getValidator_re_double_SI_());
		}
	}
	// > 下拉框
	if (c_fp.widgetType == "QComboBox"){
		c_f->comboBox = new QComboBox();
		c_f->comboBox->setObjectName(c_fp.nameEnglish);
		c_f->comboBox->setEnabled(c_fp.isEditable);

		// > 修改下拉框选项高度
		c_f->comboBoxDelegate = new QStyledItemDelegate(c_f->comboBox);
		c_f->comboBox->setItemDelegate(c_f->comboBoxDelegate);
		c_f->comboBox->setStyleSheet("QComboBox QAbstractItemView::item {min-height: " + QString::number(c_fp.comboxHeight) + "px;}");

		// > 显示列表 与 实际值列表
		QString default_textShowing = "";
		for (int i = 0; i < c_fp.chooseList.count(); i++){
			QString choose = c_fp.chooseList.at(i);
			QString chooseShowing = "";
			// > 显示列表 长度不足时，用实际值列表填充
			if (i < c_fp.chooseShowingList.count()){
				chooseShowing = c_fp.chooseShowingList.at(i);
			}else{
				chooseShowing = c_fp.chooseList.at(i);
			}
			c_f->comboBox->addItem(chooseShowing, choose);
			// > 默认显示的值
			if (choose == c_fp.defaultValue){
				default_textShowing = chooseShowing;
			}
		}
		c_f->comboBox->setCurrentText(default_textShowing);
		// > 必填项
		c_f->label = new QLabel();
		c_f->label->setObjectName(c_fp.nameEnglish);
		c_f->label->setText(c_fp.getShowingName());
		if (c_fp.notNull){
			c_f->label->setText(c_f->label->text() + " <span style = \"color:#ff0000;\">*</span>");
		}
		// > 后缀
		c_f->label_suffix = new QLabel();
		c_f->label_suffix->setObjectName(c_fp.nameEnglish);
		c_f->label_suffix->setText(c_fp.suffix);
	}
	// > 勾选框
	if (c_fp.widgetType == "QCheckBox"){
		c_f->checkBox = new QCheckBox();
		c_f->checkBox->setObjectName(c_fp.nameEnglish);
		c_f->checkBox->setEnabled(c_fp.isEditable);
		c_f->checkBox->setChecked(c_fp.defaultValue == "true");
		c_f->checkBox->setText(c_fp.suffix);		//（注意，勾选框的名称就是后缀）
		// > 必填项
		c_f->label = new QLabel();
		c_f->label->setObjectName(c_fp.nameEnglish);
		c_f->label->setText(c_fp.getShowingName());
		if (c_fp.notNull){
			c_f->label->setText(c_f->label->text() + " <span style = \"color:#ff0000;\">*</span>");
		}
	}
	// > 数字控件
	if (c_fp.widgetType == "QSpinBox"){
		c_f->spinBox = new QSpinBox();
		c_f->spinBox->setObjectName(c_fp.nameEnglish);
		c_f->spinBox->setEnabled(c_fp.isEditable);
		c_f->spinBox->setMaximum(c_fp.valueMax);
		c_f->spinBox->setMinimum(c_fp.valueMin);
		c_f->spinBox->setSingleStep(c_fp.singleStep);
		c_f->spinBox->setPrefix(c_fp.num_prefix);
		c_f->spinBox->setSuffix(c_fp.num_suffix);
		c_f->spinBox->setValue(c_fp.defaultValue.toLong());
		// > 必填项
		c_f->label = new QLabel();
		c_f->label->setObjectName(c_fp.nameEnglish);
		c_f->label->setText(c_fp.getShowingName());
		if (c_fp.notNull){
			c_f->label->setText(c_f->label->text() + " <span style = \"color:#ff0000;\">*</span>");
		}
		// > 后缀
		c_f->label_suffix = new QLabel();
		c_f->label_suffix->setObjectName(c_fp.nameEnglish);
		c_f->label_suffix->setText(c_fp.suffix);
	}
	// > 数字控件（double）
	if (c_fp.widgetType == "QDoubleSpinBox"){
		c_f->doubleSpinBox = new QDoubleSpinBox();
		c_f->doubleSpinBox->setObjectName(c_fp.nameEnglish);
		c_f->doubleSpinBox->setEnabled(c_fp.isEditable);
		c_f->doubleSpinBox->setMaximum(c_fp.valueMax);
		c_f->doubleSpinBox->setMinimum(c_fp.valueMin);
		c_f->doubleSpinBox->setSingleStep(c_fp.singleStep);
		c_f->doubleSpinBox->setDecimals(c_fp.decimals);
		c_f->doubleSpinBox->setPrefix(c_fp.num_prefix);
		c_f->doubleSpinBox->setSuffix(c_fp.num_suffix);
		c_f->doubleSpinBox->setValue(c_fp.defaultValue.toDouble());
		// > 必填项
		c_f->label = new QLabel();
		c_f->label->setObjectName(c_fp.nameEnglish);
		c_f->label->setText(c_fp.getShowingName());
		if (c_fp.notNull){
			c_f->label->setText(c_f->label->text() + " <span style = \"color:#ff0000;\">*</span>");
		}
		// > 后缀
		c_f->label_suffix = new QLabel();
		c_f->label_suffix->setObjectName(c_fp.nameEnglish);
		c_f->label_suffix->setText(c_fp.suffix);
	}
	// > 文本域
	if (c_fp.widgetType == "QPlainTextEdit"){
		c_f->plainTextEdit = new QPlainTextEdit();
		c_f->plainTextEdit->setObjectName(c_fp.nameEnglish);
		c_f->plainTextEdit->setPlainText(c_fp.defaultValue);
		c_f->plainTextEdit->setEnabled(c_fp.isEditable);
		c_f->plainTextEdit->setFixedHeight(c_fp.plainTextHeight);
		// > 必填项
		c_f->label = new QLabel();
		c_f->label->setObjectName(c_fp.nameEnglish);
		c_f->label->setText(c_fp.getShowingName());
		if (c_fp.notNull){
			c_f->label->setText(c_f->label->text() + " <span style = \"color:#ff0000;\">*</span>");
		}
	}
}
/*-------------------------------------------------
		构建 - 单个控件
*/
void P_FastForm::_addSingle(C_FastClassParam c_fp, QGridLayout* layout, int curRow, int curCol){
	curCol *= 3;		//每3列算1列
	m_depth += 1;

	// > 初始化
	C_FastForm* c_f = this->_createForm();
	c_f->params = c_fp;
	this->_initForm(c_f);

	if (c_fp.type != "FastClass"){
		// > 布局-弹簧
		if ( c_fp.widgetType == "QSpacerItem"){
			layout->addItem(c_f->spacerItem, curRow, curCol);
		}
		// > 布局-标签
		if (c_fp.widgetType == "QLabel"){
			layout->addWidget(c_f->label, curRow, curCol, 1, 2);
			layout->setColumnStretch(curCol, 1);
		}
		// > 布局-编辑框
		if (c_fp.widgetType == "QLineEdit"){
			layout->addWidget(c_f->label, curRow, curCol + 0);
			layout->addWidget(c_f->lineEdit, curRow, curCol + 1);
			layout->addWidget(c_f->label_suffix, curRow, curCol + 2);
			layout->setColumnStretch(curCol+0, 1);
			layout->setColumnStretch(curCol+1, 3);
			layout->setColumnStretch(curCol+2, 0);
		}
		// > 布局-下拉框
		if (c_fp.widgetType == "QComboBox"){
			layout->addWidget(c_f->label, curRow, curCol + 0);
			layout->addWidget(c_f->comboBox, curRow, curCol + 1);
			layout->addWidget(c_f->label_suffix, curRow, curCol + 2);
			layout->setColumnStretch(curCol + 0, 1);
			layout->setColumnStretch(curCol + 1, 3);
			layout->setColumnStretch(curCol + 2, 0);
		}
		// > 布局-勾选框
		if (c_fp.widgetType == "QCheckBox"){
			layout->addWidget(c_f->label, curRow, curCol + 0);
			layout->addWidget(c_f->checkBox, curRow, curCol + 1 , 1, 2 );
			layout->setColumnStretch(curCol + 0, 1);
			layout->setColumnStretch(curCol + 1, 4);
		}
		// > 布局-数字控件
		if (c_fp.widgetType == "QSpinBox"){
			layout->addWidget(c_f->label, curRow, curCol + 0);
			layout->addWidget(c_f->spinBox, curRow, curCol + 1);
			layout->addWidget(c_f->label_suffix, curRow, curCol + 2);
			layout->setColumnStretch(curCol + 0, 1);
			layout->setColumnStretch(curCol + 1, 3);
			layout->setColumnStretch(curCol + 2, 0);
		}
		// > 布局-数字控件（double）
		if (c_fp.widgetType == "QDoubleSpinBox"){
			layout->addWidget(c_f->label, curRow, curCol + 0);
			layout->addWidget(c_f->doubleSpinBox, curRow, curCol + 1);
			layout->addWidget(c_f->label_suffix, curRow, curCol + 2);
			layout->setColumnStretch(curCol + 0, 1);
			layout->setColumnStretch(curCol + 1, 3);
			layout->setColumnStretch(curCol + 2, 0);
		}
		// > 布局-文本域
		if (c_fp.widgetType == "QPlainTextEdit"){
			layout->addWidget(c_f->label, curRow, curCol + 0);
			layout->addWidget(c_f->plainTextEdit, curRow, curCol + 1, 1, 2);
			layout->setColumnStretch(curCol + 0, 1);
			layout->setColumnStretch(curCol + 1, 3);
		}
		// > 添加至formlist（FastClass不算参数）
		this->m_form_list.push_back(c_f);

	}
	else{
		// > 布局-并列控件
		if (c_fp.widgetType == "OneLine"){
			this->_addOneLine(c_fp.fastClass, layout, curRow, curCol, c_fp.isEditable);
		}
		// > 布局-群组控件
		if (c_fp.widgetType == "QGroupBox"){
			this->_addQGroupBox(c_fp.fastClass, layout, curRow, curCol, c_fp.isEditable);
		}
		// > 布局-块组控件
		if (c_fp.widgetType == "QWidget"){
			this->_addQWidgetGroup(c_fp.fastClass, layout, curRow, curCol, c_fp.isEditable);
		}
	}
	m_depth -= 1;
}
/*-------------------------------------------------
		布局 - 并列控件
*/
void P_FastForm::_addOneLine(C_FastClass c_fc, QGridLayout* p_layout, int curRow, int curCol, bool editable){
	QList<C_FastClassParam> group_list = c_fc.getParamList();
	if (group_list.count() == 0){ return; }

	// > 并列控件-初始化
	QWidget* widget = new QWidget(m_parent);
	QHBoxLayout* layout = new QHBoxLayout(widget);
	layout->setMargin(0);
	widget->setLayout(layout);
	widget->setObjectName("OneLine_" + c_fc.getClassNameChinese());
	widget->setStatusTip("depth=" + QString::number(m_depth) + ",");		//写入特殊参数
	widget->setEnabled(editable);
	QLabel* group_label = new QLabel(c_fc.getClassNameShowing());
	QLabel* last_label = new QLabel(group_list.last().suffix);

	// > 初始化
	for (int k = 0; k < group_list.count(); k++){
		C_FastClassParam c_fp = group_list.at(k);
		C_FastForm* c_f = this->_createForm();
		c_f->params = c_fp;
		this->_initForm(c_f);
		this->m_form_list.push_back(c_f);

		// > 编辑框
		if (c_fp.widgetType == "QLineEdit"){		//必须多个都是lineEdit才能合并
			layout->addWidget(c_f->lineEdit, 1);

			if (k != group_list.count() - 1){		//分隔符
				QString suffix = c_fp.suffix + " - ";
				QLabel* label = new QLabel(suffix);
				layout->addWidget(label, 0);
			}
		}
		if (k == group_list.count()-1){		//末尾控件
			c_f->label_suffix = last_label;
		}
	}

	// > 布局-并列控件
	m_frames.push_back(group_label);
	m_frames.push_back(widget);
	p_layout->addWidget(group_label, curRow, curCol + 0);
	p_layout->addWidget(widget, curRow, curCol + 1);
	p_layout->addWidget(last_label, curRow, curCol + 2);
	p_layout->setColumnStretch(curCol + 0, 1);
	p_layout->setColumnStretch(curCol + 1, 3);
	p_layout->setColumnStretch(curCol + 2, 0);

}
/*-------------------------------------------------
		布局 - 群组控件
*/
void P_FastForm::_addQGroupBox(C_FastClass c_fc, QGridLayout* p_layout, int curRow, int curCol, bool editable){
	QList<C_FastClassParam> group_list = c_fc.getParamList();
	if (group_list.count() == 0){ return; }

	// > 群组控件-初始化
	QGroupBox* group_box = new QGroupBox(m_parent);
	QGridLayout* group_layout = new QGridLayout(group_box);
	group_box->setObjectName("QGroupBox_"+ c_fc.getClassNameChinese());
	group_box->setStatusTip("depth=" + QString::number(m_depth) + ",");		//写入特殊参数
	group_box->setLayout(group_layout);
	group_box->setTitle(c_fc.getClassNameShowing());
	group_box->setEnabled(editable);

	// > 获取列（群组套多列）
	int colCount = 0;
	for (int i = 0; i < group_list.length(); i++){
		C_FastClassParam c_fp = group_list.at(i);
		if (c_fp.columnPos > colCount){
			colCount = c_fp.columnPos;
		}
	}
	colCount += 1;
	// > 行初始化（群组套多列）
	QList<int> rowIndex = QList<int>();
	for (int i = 0; i < colCount; i++){ rowIndex.push_back(0);}

	// > 初始化
	for (int k = 0; k < group_list.count(); k++){
		C_FastClassParam c_fp = group_list.at(k);
		
		int curRow = rowIndex.at(c_fp.columnPos);
		int curCol = c_fp.columnPos;

		this->_addSingle(c_fp, group_layout, curRow, curCol);

		rowIndex[c_fp.columnPos] += 1;
	}

	// > 添加弹簧
	//C_FastForm* c_f = this->_createForm();
	//C_FastClassParam c_p = C_FastClassParam();
	//c_p.nameChinese = "<-SPAN->弹簧";
	//c_p.widgetType = "QSpacerItem";
	//c_f->params = c_p;
	//this->_initForm(c_f);
	//this->m_form_list.push_back(c_f);
	//group_layout->addItem(c_f->spacerItem, rowIndex.at(0)+1, 0);

	// > 布局-群组控件
	m_frames.push_back(group_box);
	p_layout->addWidget(group_box, curRow, curCol,1,3);
	p_layout->setColumnStretch(curCol, 4);
}
/*-------------------------------------------------
		布局 - 块组控件（和群组控件一样）
*/
void P_FastForm::_addQWidgetGroup(C_FastClass c_fc, QGridLayout* p_layout, int curRow, int curCol, bool editable){
	QList<C_FastClassParam> group_list = c_fc.getParamList();
	if (group_list.count() == 0){ return; }

	// > 群组控件-初始化
	QWidget* group_box = new QWidget(m_parent);
	QGridLayout* group_layout = new QGridLayout(group_box);
	group_box->setObjectName("QWidgetGroup_" + c_fc.getClassNameChinese());
	group_box->setStatusTip("depth=" + QString::number(m_depth) + ",");		//写入特殊参数
	group_box->setLayout(group_layout);
	group_box->setEnabled(editable);

	// > 获取列（群组套多列）
	int colCount = 0;
	for (int i = 0; i < group_list.length(); i++){
		C_FastClassParam c_fp = group_list.at(i);
		if (c_fp.columnPos > colCount){
			colCount = c_fp.columnPos;
		}
	}
	colCount += 1;
	// > 行初始化（群组套多列）
	QList<int> rowIndex = QList<int>();
	for (int i = 0; i < colCount; i++){ rowIndex.push_back(0);}

	// > 初始化
	for (int k = 0; k < group_list.count(); k++){
		C_FastClassParam c_fp = group_list.at(k);
		
		int curRow = rowIndex.at(c_fp.columnPos);
		int curCol = c_fp.columnPos;

		this->_addSingle(c_fp, group_layout, curRow, curCol);

		rowIndex[c_fp.columnPos] += 1;
	}

	// > 添加弹簧
	//C_FastForm* c_f = this->_createForm();
	//C_FastClassParam c_p = C_FastClassParam();
	//c_p.nameChinese = "<-SPAN->弹簧";
	//c_p.widgetType = "QSpacerItem";
	//c_f->params = c_p;
	//this->_initForm(c_f);
	//this->m_form_list.push_back(c_f);
	//group_layout->addItem(c_f->spacerItem, curRow, curCol);

	// > 布局-群组控件
	m_frames.push_back(group_box);
	p_layout->addWidget(group_box, curRow, curCol,1,3);
	p_layout->setColumnStretch(curCol, 4);
}

/*-------------------------------------------------
		控件 - 清理UI
*/
void P_FastForm::clearUI(){

	// > 清理单控件
	for (int i = 0; i < this->m_form_list.length(); i++){
		C_FastForm* c_f = m_form_list.at(i);

		if (c_f->label != nullptr){
			m_layout->removeWidget(c_f->label);
			c_f->label->setParent(nullptr);
			c_f->label->deleteLater();
		}
		if (c_f->spacerItem != nullptr){
			m_layout->removeItem(c_f->spacerItem);
			delete c_f->spacerItem;
		}
		if (c_f->lineEdit != nullptr){
			m_layout->removeWidget(c_f->lineEdit);
			c_f->lineEdit->setParent(nullptr);
			c_f->lineEdit->deleteLater();
		}
		if (c_f->comboBox != nullptr){
			m_layout->removeWidget(c_f->comboBox);
			c_f->comboBox->setParent(nullptr);
			c_f->comboBox->deleteLater();
		}
		if (c_f->label_suffix != nullptr){
			m_layout->removeWidget(c_f->label_suffix);
			c_f->label_suffix->setParent(nullptr);
			c_f->label_suffix->deleteLater();
		}
	}
	this->m_form_list.clear();
	
	// > 清理构建框架
	for (int i = 0; i < this->m_frames.length(); i++){
		QWidget* widget = m_frames.at(i);
		m_layout->removeWidget(widget);
		widget->setParent(nullptr);
		widget->deleteLater();
	}
	this->m_frames.clear();
}

/*-------------------------------------------------
		控件 - 锁定后缀宽度
*/
void P_FastForm::lockSuffixWidth(int width){
	for (int i = 0; i < this->m_form_list.length(); i++){
		C_FastForm* c_f = m_form_list.at(i);
		if (c_f->label_suffix != nullptr){
			c_f->label_suffix->setMinimumWidth(width);
		}
	}
}


/*-------------------------------------------------
		参数 - 获取名称列表
*/
QStringList P_FastForm::getAllNameChinese(){
	QStringList result_list = QStringList();
	for (int i = 0; i < this->m_form_list.length(); i++){
		C_FastForm* c_f = m_form_list.at(i);
		result_list.append(c_f->params.nameChinese);
	}
	return result_list;
}
QStringList P_FastForm::getAllNameEnglish(){
	QStringList result_list = QStringList();
	for (int i = 0; i < this->m_form_list.length(); i++){
		C_FastForm* c_f = m_form_list.at(i);
		result_list.append(c_f->params.nameEnglish);
	}
	return result_list;
}
QStringList P_FastForm::getAllNameShowing(){
	QStringList result_list = QStringList();
	for (int i = 0; i < this->m_form_list.length(); i++){
		C_FastForm* c_f = m_form_list.at(i);
		result_list.append(c_f->params.getShowingName());
	}
	return result_list;
}

/*-------------------------------------------------
		参数 - 获取参数值
*/
QString P_FastForm::getValueByName(QString name){
	C_FastForm* c_f = this->getFastFormByName(name);
	if (c_f == nullptr){ return ""; }

	if (c_f->params.widgetType == "QLineEdit"){
		QString text = c_f->lineEdit->text();
		if (c_f->params.isSI){
			text = QString::number(TTool::_to_double_bySI_(text));	//SI特殊转换
		}
		return text;
	}
	if (c_f->params.widgetType == "QComboBox"){
		return c_f->comboBox->currentData().toString();		//（实际字符串值）
	}
	if (c_f->params.widgetType == "QCheckBox"){
		return c_f->checkBox->isChecked() ? "true":"false";
	}
	if (c_f->params.widgetType == "QSpinBox"){
		return QString::number( c_f->spinBox->value() );
	}
	if (c_f->params.widgetType == "QDoubleSpinBox"){
		return QString::number( c_f->doubleSpinBox->value() );
	}
	if (c_f->params.widgetType == "QPlainTextEdit"){
		return c_f->plainTextEdit->toPlainText();
	}
	return "";
}
/*-------------------------------------------------
		参数 - 设置某个控件的值
*/
void P_FastForm::setValueByName(QString name, QString value){
	C_FastForm* c_f = this->getFastFormByName(name);
	if (c_f == nullptr){ return ; }

	if (c_f->params.widgetType == "QLineEdit"){
		if (c_f->params.isSI){
			c_f->lineEdit->setText(TTool::_to_QString_bySI_(value));	//SI特殊转换
		}
		else{
			c_f->lineEdit->setText(value);
		}
	}
	if (c_f->params.widgetType == "QComboBox"){
		c_f->comboBox->setCurrentText(value);
	}
	if (c_f->params.widgetType == "QCheckBox"){
		c_f->checkBox->setChecked(value == "true");
	}
	if (c_f->params.widgetType == "QSpinBox"){
		c_f->spinBox->setValue(value.toLong());
	}
	if (c_f->params.widgetType == "QDoubleSpinBox"){
		c_f->doubleSpinBox->setValue(value.toDouble());
	}
	if (c_f->params.widgetType == "QPlainTextEdit"){
		c_f->plainTextEdit->setPlainText(value);
	}
}

/*-------------------------------------------------
		参数 - 获取参数集（ { 名字:值 } ）
*/
QJsonObject P_FastForm::getJsonObject(){
	QJsonObject result = QJsonObject();
	for (int i = 0; i < this->m_form_list.length(); i++){
		C_FastForm* c_f = m_form_list.at(i);
		if (c_f->params.nameChinese == ""){ continue;}
		result.insert(c_f->params.nameChinese, this->getValueByName(c_f->params.nameChinese));
	}
	return result;
}
/*-------------------------------------------------
		参数 - 设置参数集（ { 名字:值 } ）
*/
void P_FastForm::setJsonObject(QJsonObject p){
	for (int i = 0; i < this->m_form_list.length(); i++){
		C_FastForm* c_f = m_form_list.at(i);
		if (p.value(c_f->params.nameChinese).isString()){
			this->setValueByName(c_f->params.nameChinese, p.value(c_f->params.nameChinese).toString());
		}
	}
}
/*-------------------------------------------------
		参数 - 设置参数集（如果空则填默认值）
*/
void P_FastForm::setJsonObjectAutoDefault(QJsonObject p){
	for (int i = 0; i < this->m_form_list.length(); i++){
		C_FastForm* c_f = m_form_list.at(i);
		QString value = p.value(c_f->params.nameChinese).toString();
		if (value == ""){
			value = c_f->params.defaultValue; 
		}
		this->setValueByName(c_f->params.nameChinese, value);
	}
}

/*-------------------------------------------------
		参数 - 检查必填项是否非空
*/
bool P_FastForm::isAllNotNullConform(){
	for (int i = 0; i < this->m_form_list.length(); i++){
		C_FastForm* c_f = m_form_list.at(i);
		if (c_f->params.notNull){
			QString value = this->getValueByName(c_f->params.nameChinese);
			if (value == ""){
				return false;
			}
		}
	}
	return true;
}


/*-------------------------------------------------
		获取 - 编辑框（没有则返回nullptr）
*/
QLineEdit* P_FastForm::getQLineEditByName(QString name){
	C_FastForm* c_f = this->getFastFormByName(name);
	if (c_f == nullptr){ return nullptr; }
	return c_f->lineEdit;
}
QList<QLineEdit*> P_FastForm::getAllQLineEdit(){
	QList<QLineEdit*> result_list = QList<QLineEdit*>();
	QList<C_FastForm*> f_list = this->getAllFastFormByType("QLineEdit");
	for (int i = 0; i < f_list.count(); i++){
		QLineEdit* temp_widget = f_list.at(i)->lineEdit;
		if (temp_widget == nullptr){ continue; }
		result_list.push_back(temp_widget);
	}
	return result_list;
}
/*-------------------------------------------------
		获取 - 下拉框（没有则返回nullptr）
*/
QComboBox* P_FastForm::getQComboBoxByName(QString name){
	C_FastForm* c_f = this->getFastFormByName(name);
	if (c_f == nullptr){ return nullptr; }
	return c_f->comboBox;
}
QList<QComboBox*> P_FastForm::getAllQComboBox(){
	QList<QComboBox*> result_list = QList<QComboBox*>();
	QList<C_FastForm*> f_list = this->getAllFastFormByType("QComboBox");
	for (int i = 0; i < f_list.count(); i++){
		QComboBox* temp_widget = f_list.at(i)->comboBox;
		if (temp_widget == nullptr){ continue; }
		result_list.push_back(temp_widget);
	}
	return result_list;
}
/*-------------------------------------------------
		获取 - 勾选框（没有则返回nullptr）
*/
QCheckBox* P_FastForm::getQCheckBoxByName(QString name){
	C_FastForm* c_f = this->getFastFormByName(name);
	if (c_f == nullptr){ return nullptr; }
	return c_f->checkBox;
}
QList<QCheckBox*> P_FastForm::getAllQCheckBox(){
	QList<QCheckBox*> result_list = QList<QCheckBox*>();
	QList<C_FastForm*> f_list = this->getAllFastFormByType("QCheckBox");
	for (int i = 0; i < f_list.count(); i++){
		QCheckBox* temp_widget = f_list.at(i)->checkBox;
		if (temp_widget == nullptr){ continue; }
		result_list.push_back(temp_widget);
	}
	return result_list;
}
/*-------------------------------------------------
		获取 - 数字控件（没有则返回nullptr）
*/
QSpinBox* P_FastForm::getQSpinBoxByName(QString name){
	C_FastForm* c_f = this->getFastFormByName(name);
	if (c_f == nullptr){ return nullptr; }
	return c_f->spinBox;
}
QList<QSpinBox*> P_FastForm::getAllQSpinBox(){
	QList<QSpinBox*> result_list = QList<QSpinBox*>();
	QList<C_FastForm*> f_list = this->getAllFastFormByType("QSpinBox");
	for (int i = 0; i < f_list.count(); i++){
		QSpinBox* temp_widget = f_list.at(i)->spinBox;
		if (temp_widget == nullptr){ continue; }
		result_list.push_back(temp_widget);
	}
	return result_list;
}
/*-------------------------------------------------
		获取 - 数字控件double（没有则返回nullptr）
*/
QDoubleSpinBox* P_FastForm::getQDoubleSpinBoxByName(QString name){
	C_FastForm* c_f = this->getFastFormByName(name);
	if (c_f == nullptr){ return nullptr; }
	return c_f->doubleSpinBox;
}
QList<QDoubleSpinBox*> P_FastForm::getAllQDoubleSpinBox(){
	QList<QDoubleSpinBox*> result_list = QList<QDoubleSpinBox*>();
	QList<C_FastForm*> f_list = this->getAllFastFormByType("QDoubleSpinBox");
	for (int i = 0; i < f_list.count(); i++){
		QDoubleSpinBox* temp_widget = f_list.at(i)->doubleSpinBox;
		if (temp_widget == nullptr){ continue; }
		result_list.push_back(temp_widget);
	}
	return result_list;
}

/*-------------------------------------------------
		获取 - 表单组（没有则返回nullptr）
*/
C_FastForm* P_FastForm::getFastFormByName(QString name){
	for (int i = 0; i < this->m_form_list.length(); i++){
		C_FastForm* c_f = m_form_list.at(i);
		if (c_f->params.nameChinese == name){
			return c_f;
		}
	}
	return nullptr;
}
QList<C_FastForm*> P_FastForm::getAllFastFormByType(QString type){
	QList<C_FastForm*> result_list = QList<C_FastForm*>();
	for (int i = 0; i < this->m_form_list.length(); i++){
		C_FastForm* c_f = m_form_list.at(i);
		if (c_f->params.widgetType == type){
			result_list.push_back(c_f);
		}
	}
	return result_list;
}

/*-------------------------------------------------
		获取 - 群组-并列控件（没有则返回nullptr）
*/
QWidget* P_FastForm::getOneLineByName(QString class_name){
	for (int i = 0; i < this->m_frames.length(); i++){
		QWidget* widget = m_frames.at(i);
		if (widget->objectName() == "OneLine_" + class_name){
			return widget;
		}
	}
	return nullptr;
}
QList<QWidget*> P_FastForm::getAllOneLine(){
	QList<QWidget*> result_list = QList<QWidget*>();
	for (int i = 0; i < this->m_frames.length(); i++){
		QWidget* widget = m_frames.at(i);
		if (widget->objectName().contains("OneLine_")){
			result_list.push_back(widget);
		}
	}
	return result_list;
}
QList<QWidget*> P_FastForm::getOneLineByDepth(int depth){
	QList<QWidget*> result_list = QList<QWidget*>();
	QList<QWidget*> widget_list = this->getAllOneLine();
	for (int i = 0; i < widget_list.count(); i++){
		QWidget* widget = widget_list.at(i);
		if (this->getDepth(widget->statusTip()) == depth){
			result_list.push_back(widget);
		}
	}
	return result_list;
}

/*-------------------------------------------------
		获取 - 群组-群组控件（没有则返回nullptr）
*/
QGroupBox* P_FastForm::getQGroupBoxByName(QString name){
	for (int i = 0; i < this->m_frames.length(); i++){
		QWidget* widget = m_frames.at(i);
		if (widget->objectName() == "QGroupBox_"+name){
			return dynamic_cast<QGroupBox*>(widget);
		}
	}
	return nullptr;
}
QList<QGroupBox*> P_FastForm::getAllQGroupBox(){
	QList<QGroupBox*> result_list = QList<QGroupBox*>();
	for (int i = 0; i < this->m_frames.length(); i++){
		QWidget* widget = m_frames.at(i);
		if (widget->objectName().contains("QGroupBox_")){
			result_list.push_back(dynamic_cast<QGroupBox*>(widget));
		}
	}
	return result_list;
}
QList<QGroupBox*> P_FastForm::getQGroupBoxByDepth(int depth){
	QList<QGroupBox*> result_list = QList<QGroupBox*>();
	QList<QGroupBox*> group_list = this->getAllQGroupBox();
	for (int i = 0; i < group_list.count(); i++){
		QGroupBox* group_box = group_list.at(i);
		if (this->getDepth(group_box->statusTip()) == depth){
			result_list.push_back(group_box);
		}
	}
	return result_list;
}

/*-------------------------------------------------
		获取 - 群组-块组控件（没有则返回nullptr）
*/
QWidget* P_FastForm::getQWidgetGroupByName(QString name){
	for (int i = 0; i < this->m_frames.length(); i++){
		QWidget* widget = m_frames.at(i);
		if (widget->objectName() == "QWidgetGroup_"+name){
			return widget;
		}
	}
	return nullptr;
}
QList<QWidget*> P_FastForm::getAllQWidgetGroup(){
	QList<QWidget*> result_list = QList<QWidget*>();
	for (int i = 0; i < this->m_frames.length(); i++){
		QWidget* widget = m_frames.at(i);
		if (widget->objectName().contains("QWidgetGroup_")){
			result_list.push_back(widget);
		}
	}
	return result_list;
}
QList<QWidget*> P_FastForm::getQWidgetGroupByDepth(int depth){
	QList<QWidget*> result_list = QList<QWidget*>();
	QList<QWidget*> widget_list = this->getAllQWidgetGroup();
	for (int i = 0; i < widget_list.count(); i++){
		QWidget* widget = widget_list.at(i);
		if (this->getDepth(widget->statusTip()) == depth){
			result_list.push_back(widget);
		}
	}
	return result_list;
}


/*
-----==========================================================-----
		类：		表单组.cpp
		所属模块：	工具模块
		功能：		多个表单的组合结构。
-----==========================================================-----
*/
C_FastForm::C_FastForm()
{
	this->params = C_FastClassParam();		//参数

	this->label = nullptr;					//标签（显示名称）
	this->lineEdit = nullptr;				//文本编辑框
	this->comboBox = nullptr;				//下拉框
	this->comboBoxDelegate = nullptr;		//下拉框选项设置
	this->checkBox = nullptr;				//勾选框
	this->spinBox = nullptr;				//数字控件
	this->doubleSpinBox = nullptr;			//数字控件（double）

	this->spacerItem = nullptr;				//后缀
	this->label_suffix = nullptr;			//弹簧
}

C_FastForm::~C_FastForm(){
}
void C_FastForm::xxx(){

}