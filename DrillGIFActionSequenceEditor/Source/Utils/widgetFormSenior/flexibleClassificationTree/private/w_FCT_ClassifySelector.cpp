#include "stdafx.h"
#include "w_FCT_ClassifySelector.h"

#include "c_FCT_Config.h"
#include "c_FCT_Classify.h"
#include "w_FCT_Classify.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		���� ѡ�񴰿�.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��������������ڸô������á�
-----==========================================================-----
*/

W_FCT_ClassifySelector::W_FCT_ClassifySelector(P_FlexibleClassificationTree *p_obj)
	: QDialog(p_obj->getTree())
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������
	this->m_parentObj = p_obj;

	//-----------------------------------
	//----�¼���
	connect(ui.pushButton, &QPushButton::clicked, this->m_parentObj, &P_FlexibleClassificationTree::addClassifyInAction);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_FCT_ClassifySelector::acceptData);

	//-----------------------------------
	//----��ʼ��ui
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);				//������Ӧ
	TTool::_chinese_(ui.buttonBox);

}
W_FCT_ClassifySelector::~W_FCT_ClassifySelector(){
}


/*-------------------------------------------------
		���� - �������ݣ��޸ģ�
*/
void W_FCT_ClassifySelector::setData(C_FCT_Config* config, QString last_selectedName) {
	this->m_configPtr = config;
	this->setWindowTitle("�ƶ���");

	// > ˢ���б�
	QList<C_FCT_Classify*> c_list = this->m_configPtr->get_classify_DataList();
	ui.tableWidget->clearContents();
	ui.tableWidget->setRowCount(c_list.count());
	for (int i = 0; i < c_list.count(); i++){
		C_FCT_Classify* c_c = c_list.at(i);
		QString name = c_c->getCustomObjectName();
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(name));
	}

	// > ѡ��֮ǰѡ�е�
	if (last_selectedName != ""){
		for (int i = 0; i < c_list.count(); i++){
			C_FCT_Classify* c_c = c_list.at(i);
			if (c_c->getName() == last_selectedName){
				ui.tableWidget->selectRow(i);
			}
		}
	}

}

/*-------------------------------------------------
		���� - ȡ������
*/
QString W_FCT_ClassifySelector::getSelectedData(){

	QList<QTableWidgetSelectionRange> range = ui.tableWidget->selectedRanges();
	if (range.size() == 0) {
		QMessageBox::warning(this, ("��ʾ"), ("��ѡ��һ�С�"));
		return "";
	}
	int pos = range.at(0).topRow();

	return this->m_configPtr->get_classify_DataList().at(pos)->getName();
};

/*-------------------------------------------------
		���� - �ύ���ݣ�У�飩
*/
void W_FCT_ClassifySelector::acceptData(){
	//���޲�����

	this->accept();
};