#include "stdafx.h"
#include "w_FCTClassifySelector.h"

#include "w_FCTClassify.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		���� ѡ�񴰿�.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���͵����������ڸô������á�
-----==========================================================-----
*/

W_FCTClassifySelector::W_FCTClassifySelector(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������


	//-----------------------------------
	//----�¼���
	connect(ui.pushButton, &QPushButton::clicked, this, &W_FCTClassifySelector::openWindowAddClassify);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_FCTClassifySelector::acceptData);

	//-----------------------------------
	//----��ʼ��ui
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);				//������Ӧ
	TTool::_chinese_(ui.buttonBox);

}
W_FCTClassifySelector::~W_FCTClassifySelector(){
}


/*-------------------------------------------------
		���� - �������ݣ��޸ģ�
*/
void W_FCTClassifySelector::openWindowAddClassify(){

	W_FCTClassify d(this);
	d.setDataInAddMode(this->m_classifyList);
	if (d.exec() == QDialog::Accepted){
		C_FCTClassify c_c = d.getData();
		
		// > ˢ�µ�ǰ�б�
		int index = ui.tableWidget->rowCount();
		ui.tableWidget->setRowCount(index + 1);
		ui.tableWidget->setItem(index - 1, 0, new QTableWidgetItem(c_c.name));
		ui.tableWidget->setItem(index , 0, new QTableWidgetItem("δ����"));
		
		// > �����ź�
		emit classifyAdded(c_c);
	}
}


/*-------------------------------------------------
		���� - �������ݣ��޸ģ�
*/
void W_FCTClassifySelector::setDataInModifyMode(QString selected_name, QList<C_FCTClassify>* dataFrom) {
	this->m_classifyList = dataFrom;
	this->setWindowTitle("�ƶ���");

	// > ˢ���б�
	ui.tableWidget->clearContents();
	ui.tableWidget->setRowCount(this->m_classifyList->count());
	for (int i = 0; i < this->m_classifyList->count(); i++){
		C_FCTClassify c_c = this->m_classifyList->at(i);
		QString name = c_c.name;
		if (name == ""){
			name = "δ����";
		}
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(name));
	}

	// > ѡ��֮ǰѡ�е�
	for (int i = 0; i < this->m_classifyList->count(); i++){
		C_FCTClassify c_c = this->m_classifyList->at(i);
		if (c_c.name == selected_name){
			ui.tableWidget->selectRow(i);
		}
	}

}

/*-------------------------------------------------
		���� - ȡ������
*/
C_FCTClassify W_FCTClassifySelector::getSelectedData(){

	QList<QTableWidgetSelectionRange> range = ui.tableWidget->selectedRanges();
	if (range.size() == 0) {
		QMessageBox::warning(this, ("��ʾ"), ("��ѡ��һ�С�"));
		return C_FCTClassify();
	}
	int pos = range.at(0).topRow();

	return this->m_classifyList->at(pos);
};

/*-------------------------------------------------
		���� - �ύ���ݣ�У�飩
*/
void W_FCTClassifySelector::acceptData(){

	this->accept();
};