#include "stdafx.h"

#include "p_StatePart.h"

/*
-----==========================================================-----
		类：		状态元块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		该部分提供状态元编辑功能。
		
		目标：		1.

		使用方法：
				>初始化

-----==========================================================-----
*/
P_StatePart::P_StatePart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	

	//-----------------------------------
	//----初始化ui
	
	// > 编辑表格
	this->m_table = new P_RadioTable(ui.tableWidget);
	QJsonObject obj_config = QJsonObject();
	obj_config.insert("zeroFillCount", 2);
	obj_config.insert("rowHeight", 22);
	this->m_table->setConfigParam(obj_config);	//固定参数

	// > 动画帧
	C_ALEData data = C_ALEData();
	data.setId(10);
	data.setSource("F:/rpg mv箱/mog插件中文全翻译(Drill_up)v2.41/插件集合示例/img/enemies/", QList<QString>()
		<< "小爱丽丝001"
		<< "小爱丽丝002"
		<< "小爱丽丝003"
		<< "小爱丽丝004"
		<< "小爱丽丝005"
		<< "小爱丽丝006"
		<< "小爱丽丝004"
		<< "小爱丽丝006");
	data.setInterval(3, QList<int>() << 6 << 5 << 4 << 3 << 2 << 1);

	this->m_p_AnimationListEditor = new P_AnimationListEditor(ui.listWidget);
	this->m_p_AnimationListEditor->setSource(data);

	C_ALEConfig config = C_ALEConfig();
	this->m_p_AnimationListEditor->setConfigParam_ALE(config);

	// > 播放器
	this->m_p_AnimationListPlayer = new P_AnimationListPlayer();
	this->m_p_AnimationListPlayer->setAnimationListEditor(m_p_AnimationListEditor);
	this->m_p_AnimationListPlayer->setPlayType(QStringList() << "永久循环");
	QHBoxLayout* layout = new QHBoxLayout(ui.groupBox_animPlayer);
	ui.groupBox_animPlayer->setLayout(layout);
	layout->setMargin(3);
	layout->addWidget(this->m_p_AnimationListPlayer);

	//-----------------------------------
	//----事件绑定
	connect(this->m_table, &P_RadioTable::currentIndexChanged, this, &P_StatePart::currentIndexChanged);
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::selectedIndexChanged_Multi, this, &P_StatePart::tableChanged_Multi);

}

P_StatePart::~P_StatePart(){
}

/*-------------------------------------------------
		动画帧 - 选项变化
*/
void P_StatePart::tableChanged_Multi(QList<int> index_list){
	QString text = "";
	for (int i = 0; i < index_list.count(); i++){
		text += QString::number( index_list.at(i) + 1 )+ "/";
	}
	//ui.label->setText("你选择了：" + text);
}


/*-------------------------------------------------
		控件 - 获取状态元名称
*/
QStringList P_StatePart::getNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_stateDataList.count(); i++){
		result.append(this->local_stateDataList.at(i).value("状态元名称").toString());
	}
	return result;
}

/*-------------------------------------------------
		控件 - 动作元切换
*/
void P_StatePart::currentIndexChanged(int index){

	// > 旧的内容存储


	// > 填入新的内容
	ui.lineEdit_name->setText(this->getNameList().at(index));
}



/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_StatePart::setData(QList<QJsonObject> state) {
	this->local_stateDataList = state;
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
QList<QJsonObject> P_StatePart::getData(){
	this->putUiToData();

	// > 校验
	//...
	
	return this->local_stateDataList;
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_StatePart::putDataToUi() {

	this->m_table->setSource(this->getNameList());
}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_StatePart::putUiToData() {
	

}
