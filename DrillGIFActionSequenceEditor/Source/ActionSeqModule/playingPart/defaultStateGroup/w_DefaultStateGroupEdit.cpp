#include "stdafx.h"
#include "W_DefaultStateGroupEdit.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		默认状态元集合 编辑.cpp
		所属模块：	动画序列模块
		功能：		默认状态元集合的窗口。

		使用方法：
				>打开窗口
					W_DefaultStateGroupEdit d;
					d.exec();

-----==========================================================-----
*/
W_DefaultStateGroupEdit::W_DefaultStateGroupEdit(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);


	//-----------------------------------
	//----控件初始化
	this->m_p_RadioTable = new P_RadioTable(ui.tableWidget);
	C_RaTConfig rat_config = C_RaTConfig();
	rat_config.showNumber = false;
	rat_config.isMultiSelect = true;
	this->m_p_RadioTable->setConfigParam(rat_config);	//固定参数

	connect(this->m_p_RadioTable, &P_RadioTable::currentIndexChanged_Multi, this, &W_DefaultStateGroupEdit::selectedChanged);

	//-----------------------------------
	//----ui初始化
	TTool::_chinese_(ui.buttonBox);
}

W_DefaultStateGroupEdit::~W_DefaultStateGroupEdit(){
}


/*-------------------------------------------------
		控件 - 选中变化
*/
void W_DefaultStateGroupEdit::selectedChanged(QList<int> index_list){
	
	QStringList data_list = this->m_p_RadioTable->getTextByIndex(index_list);
	ui.label->setText("值：[" + data_list.join(",") + "]");
}


/*-------------------------------------------------
		窗口 - 设置数据
*/
void W_DefaultStateGroupEdit::setData(QStringList data, QStringList selected){
	this->m_p_RadioTable->setSource(data);
	this->m_p_RadioTable->selectText_Multi(selected);
}
/*-------------------------------------------------
		窗口 - 获取数据
*/
QStringList W_DefaultStateGroupEdit::getData(){
	return this->m_p_RadioTable->getSelectedText_Multi();
}