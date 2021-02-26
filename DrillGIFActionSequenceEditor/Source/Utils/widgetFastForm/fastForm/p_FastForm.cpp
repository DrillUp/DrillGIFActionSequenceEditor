#include "stdafx.h"

#include "p_FastForm.h"
#include "../../common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		���ٱ�.cpp
		�汾��		v1.13
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���ٻ��Ƴ���Ҫ��д�ı��С�
					�����ܹ����ٻ�ȡ����ֵ��

		���ܣ�		1.�򵥲����齨
					2.����������齨������Ƕ���ࣩ
					3.��ȡ����Ⱥ��
					
		ʹ�÷�����
				>��׼��
					m_fastForm_style = new P_FastForm(ui.widget_styleParams);
					m_fastForm_style->prepareQLineEdit("����ߴ��غ϶�", "0.5");
					m_fastForm_style->prepareQLineEdit("Ƶ�ױ�", "0.9");
					m_fastForm_style->prepareQLineEdit("���ű�", "2.5");
					m_fastForm_style->rebuildUI();
				>��׼����
					m_fastForm = new P_FastForm(ui.widget);				//�����и��ָ��ӵĲ���д��class
					m_fastForm->prepareFastClass(m_deviceStateClass);	//Ȼ��һ�������ɡ�fastform����չ�������ȫ��classȻ�����ɿؼ���
					m_fastForm->rebuildUI();
				>ɾ��
					m_fastForm_style->clearUI();
				>��������
					m_fastForm->setValueByName("���������", 2);		//Ƕ��������Ĳ���Ҳ���޸�
					m_fastForm->setValueByName("���ջ�����", 3);
				>������ȡ
					QString name = m_fastForm_style->getValueByName("Ƶ�ױ�");		//ע�⣬����Ҫ�����õ�һģһ��
					QLineEdit* lineEdit = m_fastForm_style->getQLineEditByName("Ƶ�ױ�");
					

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
		���ؼ���ʼ��
*/
void P_FastForm::initWidgetAndLayout(QWidget* _parent){
	if (_parent->layout() == nullptr){
		this->m_layout = new QGridLayout();
		_parent->setLayout(this->m_layout);
	}
	QString layout_name = _parent->layout()->metaObject()->className();
	Q_ASSERT(layout_name == "QGridLayout");		//������QGridLayout
	this->m_parent = _parent;
	this->m_layout = qobject_cast<QGridLayout*>(_parent->layout());
}


/*-------------------------------------------------
		��׼�� - ������ǩ
*/
void P_FastForm::prepareQLabel(QString name) {
	C_FastClassParam c_p = C_FastClassParam();
	c_p.nameChinese = "<-SPAN->"+name;
	c_p.widgetType = "QLabel";
	c_p.nameShowing = name;
	this->m_param_list.push_back(c_p);
}
/*-------------------------------------------------
		��׼�� - ��������
*/
void P_FastForm::prepareQSpacerItem() {
	C_FastClassParam c_p = C_FastClassParam();
	c_p.nameChinese = "<-SPAN->����";
	c_p.widgetType = "QSpacerItem";
	this->m_param_list.push_back(c_p);
} 
/*-------------------------------------------------
		��׼�� - ��ǩ + �����
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
		��׼�� - ��ǩ + ������
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
		׼�� - ������
*/
void P_FastForm::prepareFastClass(C_FastClass c_fc) {
	QList<C_FastClassParam> param_list = c_fc.getParamList();
	for (int i = 0; i < param_list.count(); i++){
		C_FastClassParam c_fcp = param_list.at(i);
		this->m_param_list.push_back(c_fcp);
	}
}
/*-------------------------------------------------
		׼�� - ����ȫ������
*/
void P_FastForm::clearPrepare() {
	this->m_param_list.clear();
}
/*-------------------------------------------------
		׼�� - �Ƿ�׼������
*/
bool P_FastForm::hasPreparedSequence(){
	return this->m_param_list.count() > 0;
}


/*-------------------------------------------------
		�ؼ� - �ؽ��ؼ�
*/
void P_FastForm::rebuildUI(){

	// > ����ɾ��
	this->clearUI();

	// > ��ȡ��
	int colCount = 0;
	for (int i = 0; i < this->m_param_list.length(); i++){
		C_FastClassParam c_fp = this->m_param_list.at(i);
		if (c_fp.columnPos > colCount){
			colCount = c_fp.columnPos;
		}
	}
	colCount += 1;

	// > �г�ʼ��
	QList<int> rowIndex = QList<int>();
	for (int i = 0; i < colCount; i++){
		rowIndex.push_back(0);
	}

	// > �½�
	this->m_depth = 0;
	for (int i = 0; i < this->m_param_list.length(); i++){
		C_FastClassParam c_fp = this->m_param_list.at(i);
		int curRow = rowIndex.at(c_fp.columnPos);
		int curCol = c_fp.columnPos;

		this->_addSingle(c_fp, this->m_layout, curRow, curCol);

		rowIndex[c_fp.columnPos] += 1;
	}

	// > ����Ԥ��������
	this->clearPrepare();
}

/*-------------------------------------------------
		���� - ��������
*/
C_FastForm* P_FastForm::_createForm(){
	return new C_FastForm();
}
/*-------------------------------------------------
		���� - ��ȡ���
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
		���� - ��ʼ��
*/
void P_FastForm::_initForm(C_FastForm* c_f){
	C_FastClassParam c_fp = c_f->params;

	// > ����
	if (c_fp.widgetType == "QSpacerItem"){
		c_f->spacerItem = new QSpacerItem(20, 255, QSizePolicy::Minimum, QSizePolicy::Expanding);
	}
	// > ��ǩ
	if (c_fp.widgetType == "QLabel"){
		c_f->label = new QLabel();
		c_f->label->setObjectName(c_fp.nameEnglish);
		c_f->label->setText(c_fp.getShowingName());
	}
	// > �༭��
	if (c_fp.widgetType == "QLineEdit"){
		c_f->lineEdit = new QLineEdit();
		c_f->lineEdit->setObjectName(c_fp.nameEnglish);
		c_f->lineEdit->setText(c_fp.defaultValue);
		c_f->lineEdit->setEnabled(c_fp.isEditable);
		// > ������
		c_f->label = new QLabel();
		c_f->label->setObjectName(c_fp.nameEnglish);
		c_f->label->setText(c_fp.getShowingName());
		if (c_fp.notNull){
			c_f->label->setText(c_f->label->text() + " <span style = \"color:#ff0000;\">*</span>");
		}
		// > ��׺
		c_f->label_suffix = new QLabel();
		c_f->label_suffix->setObjectName(c_fp.nameEnglish);
		c_f->label_suffix->setText(c_fp.suffix);
		// > У����
		if (c_fp.type == "Double"){
			c_f->lineEdit->setValidator(new QDoubleValidator);
		}
		if (c_fp.isSI){
			c_f->lineEdit->setValidator(TTool::_getValidator_re_double_SI_());
		}
	}
	// > ������
	if (c_fp.widgetType == "QComboBox"){
		c_f->comboBox = new QComboBox();
		c_f->comboBox->setObjectName(c_fp.nameEnglish);
		c_f->comboBox->setEnabled(c_fp.isEditable);

		// > �޸�������ѡ��߶�
		c_f->comboBoxDelegate = new QStyledItemDelegate(c_f->comboBox);
		c_f->comboBox->setItemDelegate(c_f->comboBoxDelegate);
		c_f->comboBox->setStyleSheet("QComboBox QAbstractItemView::item {min-height: " + QString::number(c_fp.comboxHeight) + "px;}");

		// > ��ʾ�б� �� ʵ��ֵ�б�
		QString default_textShowing = "";
		for (int i = 0; i < c_fp.chooseList.count(); i++){
			QString choose = c_fp.chooseList.at(i);
			QString chooseShowing = "";
			// > ��ʾ�б� ���Ȳ���ʱ����ʵ��ֵ�б����
			if (i < c_fp.chooseShowingList.count()){
				chooseShowing = c_fp.chooseShowingList.at(i);
			}else{
				chooseShowing = c_fp.chooseList.at(i);
			}
			c_f->comboBox->addItem(chooseShowing, choose);
			// > Ĭ����ʾ��ֵ
			if (choose == c_fp.defaultValue){
				default_textShowing = chooseShowing;
			}
		}
		c_f->comboBox->setCurrentText(default_textShowing);
		// > ������
		c_f->label = new QLabel();
		c_f->label->setObjectName(c_fp.nameEnglish);
		c_f->label->setText(c_fp.getShowingName());
		if (c_fp.notNull){
			c_f->label->setText(c_f->label->text() + " <span style = \"color:#ff0000;\">*</span>");
		}
		// > ��׺
		c_f->label_suffix = new QLabel();
		c_f->label_suffix->setObjectName(c_fp.nameEnglish);
		c_f->label_suffix->setText(c_fp.suffix);
	}
	// > ��ѡ��
	if (c_fp.widgetType == "QCheckBox"){
		c_f->checkBox = new QCheckBox();
		c_f->checkBox->setObjectName(c_fp.nameEnglish);
		c_f->checkBox->setEnabled(c_fp.isEditable);
		c_f->checkBox->setChecked(c_fp.defaultValue == "true");
		c_f->checkBox->setText(c_fp.suffix);		//��ע�⣬��ѡ������ƾ��Ǻ�׺��
		// > ������
		c_f->label = new QLabel();
		c_f->label->setObjectName(c_fp.nameEnglish);
		c_f->label->setText(c_fp.getShowingName());
		if (c_fp.notNull){
			c_f->label->setText(c_f->label->text() + " <span style = \"color:#ff0000;\">*</span>");
		}
	}
	// > ���ֿؼ�
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
		// > ������
		c_f->label = new QLabel();
		c_f->label->setObjectName(c_fp.nameEnglish);
		c_f->label->setText(c_fp.getShowingName());
		if (c_fp.notNull){
			c_f->label->setText(c_f->label->text() + " <span style = \"color:#ff0000;\">*</span>");
		}
		// > ��׺
		c_f->label_suffix = new QLabel();
		c_f->label_suffix->setObjectName(c_fp.nameEnglish);
		c_f->label_suffix->setText(c_fp.suffix);
	}
	// > ���ֿؼ���double��
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
		// > ������
		c_f->label = new QLabel();
		c_f->label->setObjectName(c_fp.nameEnglish);
		c_f->label->setText(c_fp.getShowingName());
		if (c_fp.notNull){
			c_f->label->setText(c_f->label->text() + " <span style = \"color:#ff0000;\">*</span>");
		}
		// > ��׺
		c_f->label_suffix = new QLabel();
		c_f->label_suffix->setObjectName(c_fp.nameEnglish);
		c_f->label_suffix->setText(c_fp.suffix);
	}
	// > �ı���
	if (c_fp.widgetType == "QPlainTextEdit"){
		c_f->plainTextEdit = new QPlainTextEdit();
		c_f->plainTextEdit->setObjectName(c_fp.nameEnglish);
		c_f->plainTextEdit->setPlainText(c_fp.defaultValue);
		c_f->plainTextEdit->setEnabled(c_fp.isEditable);
		c_f->plainTextEdit->setFixedHeight(c_fp.plainTextHeight);
		// > ������
		c_f->label = new QLabel();
		c_f->label->setObjectName(c_fp.nameEnglish);
		c_f->label->setText(c_fp.getShowingName());
		if (c_fp.notNull){
			c_f->label->setText(c_f->label->text() + " <span style = \"color:#ff0000;\">*</span>");
		}
	}
}
/*-------------------------------------------------
		���� - �����ؼ�
*/
void P_FastForm::_addSingle(C_FastClassParam c_fp, QGridLayout* layout, int curRow, int curCol){
	curCol *= 3;		//ÿ3����1��
	m_depth += 1;

	// > ��ʼ��
	C_FastForm* c_f = this->_createForm();
	c_f->params = c_fp;
	this->_initForm(c_f);

	if (c_fp.type != "FastClass"){
		// > ����-����
		if ( c_fp.widgetType == "QSpacerItem"){
			layout->addItem(c_f->spacerItem, curRow, curCol);
		}
		// > ����-��ǩ
		if (c_fp.widgetType == "QLabel"){
			layout->addWidget(c_f->label, curRow, curCol, 1, 2);
			layout->setColumnStretch(curCol, 1);
		}
		// > ����-�༭��
		if (c_fp.widgetType == "QLineEdit"){
			layout->addWidget(c_f->label, curRow, curCol + 0);
			layout->addWidget(c_f->lineEdit, curRow, curCol + 1);
			layout->addWidget(c_f->label_suffix, curRow, curCol + 2);
			layout->setColumnStretch(curCol+0, 1);
			layout->setColumnStretch(curCol+1, 3);
			layout->setColumnStretch(curCol+2, 0);
		}
		// > ����-������
		if (c_fp.widgetType == "QComboBox"){
			layout->addWidget(c_f->label, curRow, curCol + 0);
			layout->addWidget(c_f->comboBox, curRow, curCol + 1);
			layout->addWidget(c_f->label_suffix, curRow, curCol + 2);
			layout->setColumnStretch(curCol + 0, 1);
			layout->setColumnStretch(curCol + 1, 3);
			layout->setColumnStretch(curCol + 2, 0);
		}
		// > ����-��ѡ��
		if (c_fp.widgetType == "QCheckBox"){
			layout->addWidget(c_f->label, curRow, curCol + 0);
			layout->addWidget(c_f->checkBox, curRow, curCol + 1 , 1, 2 );
			layout->setColumnStretch(curCol + 0, 1);
			layout->setColumnStretch(curCol + 1, 4);
		}
		// > ����-���ֿؼ�
		if (c_fp.widgetType == "QSpinBox"){
			layout->addWidget(c_f->label, curRow, curCol + 0);
			layout->addWidget(c_f->spinBox, curRow, curCol + 1);
			layout->addWidget(c_f->label_suffix, curRow, curCol + 2);
			layout->setColumnStretch(curCol + 0, 1);
			layout->setColumnStretch(curCol + 1, 3);
			layout->setColumnStretch(curCol + 2, 0);
		}
		// > ����-���ֿؼ���double��
		if (c_fp.widgetType == "QDoubleSpinBox"){
			layout->addWidget(c_f->label, curRow, curCol + 0);
			layout->addWidget(c_f->doubleSpinBox, curRow, curCol + 1);
			layout->addWidget(c_f->label_suffix, curRow, curCol + 2);
			layout->setColumnStretch(curCol + 0, 1);
			layout->setColumnStretch(curCol + 1, 3);
			layout->setColumnStretch(curCol + 2, 0);
		}
		// > ����-�ı���
		if (c_fp.widgetType == "QPlainTextEdit"){
			layout->addWidget(c_f->label, curRow, curCol + 0);
			layout->addWidget(c_f->plainTextEdit, curRow, curCol + 1, 1, 2);
			layout->setColumnStretch(curCol + 0, 1);
			layout->setColumnStretch(curCol + 1, 3);
		}
		// > �����formlist��FastClass���������
		this->m_form_list.push_back(c_f);

	}
	else{
		// > ����-���пؼ�
		if (c_fp.widgetType == "OneLine"){
			this->_addOneLine(c_fp.fastClass, layout, curRow, curCol, c_fp.isEditable);
		}
		// > ����-Ⱥ��ؼ�
		if (c_fp.widgetType == "QGroupBox"){
			this->_addQGroupBox(c_fp.fastClass, layout, curRow, curCol, c_fp.isEditable);
		}
		// > ����-����ؼ�
		if (c_fp.widgetType == "QWidget"){
			this->_addQWidgetGroup(c_fp.fastClass, layout, curRow, curCol, c_fp.isEditable);
		}
	}
	m_depth -= 1;
}
/*-------------------------------------------------
		���� - ���пؼ�
*/
void P_FastForm::_addOneLine(C_FastClass c_fc, QGridLayout* p_layout, int curRow, int curCol, bool editable){
	QList<C_FastClassParam> group_list = c_fc.getParamList();
	if (group_list.count() == 0){ return; }

	// > ���пؼ�-��ʼ��
	QWidget* widget = new QWidget(m_parent);
	QHBoxLayout* layout = new QHBoxLayout(widget);
	layout->setMargin(0);
	widget->setLayout(layout);
	widget->setObjectName("OneLine_" + c_fc.getClassNameChinese());
	widget->setStatusTip("depth=" + QString::number(m_depth) + ",");		//д���������
	widget->setEnabled(editable);
	QLabel* group_label = new QLabel(c_fc.getClassNameShowing());
	QLabel* last_label = new QLabel(group_list.last().suffix);

	// > ��ʼ��
	for (int k = 0; k < group_list.count(); k++){
		C_FastClassParam c_fp = group_list.at(k);
		C_FastForm* c_f = this->_createForm();
		c_f->params = c_fp;
		this->_initForm(c_f);
		this->m_form_list.push_back(c_f);

		// > �༭��
		if (c_fp.widgetType == "QLineEdit"){		//����������lineEdit���ܺϲ�
			layout->addWidget(c_f->lineEdit, 1);

			if (k != group_list.count() - 1){		//�ָ���
				QString suffix = c_fp.suffix + " - ";
				QLabel* label = new QLabel(suffix);
				layout->addWidget(label, 0);
			}
		}
		if (k == group_list.count()-1){		//ĩβ�ؼ�
			c_f->label_suffix = last_label;
		}
	}

	// > ����-���пؼ�
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
		���� - Ⱥ��ؼ�
*/
void P_FastForm::_addQGroupBox(C_FastClass c_fc, QGridLayout* p_layout, int curRow, int curCol, bool editable){
	QList<C_FastClassParam> group_list = c_fc.getParamList();
	if (group_list.count() == 0){ return; }

	// > Ⱥ��ؼ�-��ʼ��
	QGroupBox* group_box = new QGroupBox(m_parent);
	QGridLayout* group_layout = new QGridLayout(group_box);
	group_box->setObjectName("QGroupBox_"+ c_fc.getClassNameChinese());
	group_box->setStatusTip("depth=" + QString::number(m_depth) + ",");		//д���������
	group_box->setLayout(group_layout);
	group_box->setTitle(c_fc.getClassNameShowing());
	group_box->setEnabled(editable);

	// > ��ȡ�У�Ⱥ���׶��У�
	int colCount = 0;
	for (int i = 0; i < group_list.length(); i++){
		C_FastClassParam c_fp = group_list.at(i);
		if (c_fp.columnPos > colCount){
			colCount = c_fp.columnPos;
		}
	}
	colCount += 1;
	// > �г�ʼ����Ⱥ���׶��У�
	QList<int> rowIndex = QList<int>();
	for (int i = 0; i < colCount; i++){ rowIndex.push_back(0);}

	// > ��ʼ��
	for (int k = 0; k < group_list.count(); k++){
		C_FastClassParam c_fp = group_list.at(k);
		
		int curRow = rowIndex.at(c_fp.columnPos);
		int curCol = c_fp.columnPos;

		this->_addSingle(c_fp, group_layout, curRow, curCol);

		rowIndex[c_fp.columnPos] += 1;
	}

	// > ��ӵ���
	//C_FastForm* c_f = this->_createForm();
	//C_FastClassParam c_p = C_FastClassParam();
	//c_p.nameChinese = "<-SPAN->����";
	//c_p.widgetType = "QSpacerItem";
	//c_f->params = c_p;
	//this->_initForm(c_f);
	//this->m_form_list.push_back(c_f);
	//group_layout->addItem(c_f->spacerItem, rowIndex.at(0)+1, 0);

	// > ����-Ⱥ��ؼ�
	m_frames.push_back(group_box);
	p_layout->addWidget(group_box, curRow, curCol,1,3);
	p_layout->setColumnStretch(curCol, 4);
}
/*-------------------------------------------------
		���� - ����ؼ�����Ⱥ��ؼ�һ����
*/
void P_FastForm::_addQWidgetGroup(C_FastClass c_fc, QGridLayout* p_layout, int curRow, int curCol, bool editable){
	QList<C_FastClassParam> group_list = c_fc.getParamList();
	if (group_list.count() == 0){ return; }

	// > Ⱥ��ؼ�-��ʼ��
	QWidget* group_box = new QWidget(m_parent);
	QGridLayout* group_layout = new QGridLayout(group_box);
	group_box->setObjectName("QWidgetGroup_" + c_fc.getClassNameChinese());
	group_box->setStatusTip("depth=" + QString::number(m_depth) + ",");		//д���������
	group_box->setLayout(group_layout);
	group_box->setEnabled(editable);

	// > ��ȡ�У�Ⱥ���׶��У�
	int colCount = 0;
	for (int i = 0; i < group_list.length(); i++){
		C_FastClassParam c_fp = group_list.at(i);
		if (c_fp.columnPos > colCount){
			colCount = c_fp.columnPos;
		}
	}
	colCount += 1;
	// > �г�ʼ����Ⱥ���׶��У�
	QList<int> rowIndex = QList<int>();
	for (int i = 0; i < colCount; i++){ rowIndex.push_back(0);}

	// > ��ʼ��
	for (int k = 0; k < group_list.count(); k++){
		C_FastClassParam c_fp = group_list.at(k);
		
		int curRow = rowIndex.at(c_fp.columnPos);
		int curCol = c_fp.columnPos;

		this->_addSingle(c_fp, group_layout, curRow, curCol);

		rowIndex[c_fp.columnPos] += 1;
	}

	// > ��ӵ���
	//C_FastForm* c_f = this->_createForm();
	//C_FastClassParam c_p = C_FastClassParam();
	//c_p.nameChinese = "<-SPAN->����";
	//c_p.widgetType = "QSpacerItem";
	//c_f->params = c_p;
	//this->_initForm(c_f);
	//this->m_form_list.push_back(c_f);
	//group_layout->addItem(c_f->spacerItem, curRow, curCol);

	// > ����-Ⱥ��ؼ�
	m_frames.push_back(group_box);
	p_layout->addWidget(group_box, curRow, curCol,1,3);
	p_layout->setColumnStretch(curCol, 4);
}

/*-------------------------------------------------
		�ؼ� - ����UI
*/
void P_FastForm::clearUI(){

	// > �����ؼ�
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
	
	// > ���������
	for (int i = 0; i < this->m_frames.length(); i++){
		QWidget* widget = m_frames.at(i);
		m_layout->removeWidget(widget);
		widget->setParent(nullptr);
		widget->deleteLater();
	}
	this->m_frames.clear();
}

/*-------------------------------------------------
		�ؼ� - ������׺���
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
		���� - ��ȡ�����б�
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
		���� - ��ȡ����ֵ
*/
QString P_FastForm::getValueByName(QString name){
	C_FastForm* c_f = this->getFastFormByName(name);
	if (c_f == nullptr){ return ""; }

	if (c_f->params.widgetType == "QLineEdit"){
		QString text = c_f->lineEdit->text();
		if (c_f->params.isSI){
			text = QString::number(TTool::_to_double_bySI_(text));	//SI����ת��
		}
		return text;
	}
	if (c_f->params.widgetType == "QComboBox"){
		return c_f->comboBox->currentData().toString();		//��ʵ���ַ���ֵ��
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
		���� - ����ĳ���ؼ���ֵ
*/
void P_FastForm::setValueByName(QString name, QString value){
	C_FastForm* c_f = this->getFastFormByName(name);
	if (c_f == nullptr){ return ; }

	if (c_f->params.widgetType == "QLineEdit"){
		if (c_f->params.isSI){
			c_f->lineEdit->setText(TTool::_to_QString_bySI_(value));	//SI����ת��
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
		���� - ��ȡ�������� { ����:ֵ } ��
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
		���� - ���ò������� { ����:ֵ } ��
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
		���� - ���ò����������������Ĭ��ֵ��
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
		���� - ���������Ƿ�ǿ�
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
		��ȡ - �༭��û���򷵻�nullptr��
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
		��ȡ - ������û���򷵻�nullptr��
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
		��ȡ - ��ѡ��û���򷵻�nullptr��
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
		��ȡ - ���ֿؼ���û���򷵻�nullptr��
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
		��ȡ - ���ֿؼ�double��û���򷵻�nullptr��
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
		��ȡ - ���飨û���򷵻�nullptr��
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
		��ȡ - Ⱥ��-���пؼ���û���򷵻�nullptr��
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
		��ȡ - Ⱥ��-Ⱥ��ؼ���û���򷵻�nullptr��
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
		��ȡ - Ⱥ��-����ؼ���û���򷵻�nullptr��
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
		�ࣺ		����.cpp
		����ģ�飺	����ģ��
		���ܣ�		���������Ͻṹ��
-----==========================================================-----
*/
C_FastForm::C_FastForm()
{
	this->params = C_FastClassParam();		//����

	this->label = nullptr;					//��ǩ����ʾ���ƣ�
	this->lineEdit = nullptr;				//�ı��༭��
	this->comboBox = nullptr;				//������
	this->comboBoxDelegate = nullptr;		//������ѡ������
	this->checkBox = nullptr;				//��ѡ��
	this->spinBox = nullptr;				//���ֿؼ�
	this->doubleSpinBox = nullptr;			//���ֿؼ���double��

	this->spacerItem = nullptr;				//��׺
	this->label_suffix = nullptr;			//����
}

C_FastForm::~C_FastForm(){
}
void C_FastForm::xxx(){

}