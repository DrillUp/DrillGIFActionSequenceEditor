#include "stdafx.h"
#include "p_ActionSeqPart.h"

#include "Source/PluginModule/lengthEditor/s_LEAnnotationReader.h"

#include "../actionSeqData/s_ActionSeqDataContainer.h"

/*
-----==========================================================-----
		类：		动作序列块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		该部分提供动作序列放映，功能，并嵌套状态元、动作元的块。
		
		目标：		1.

		使用方法：
				>初始化

-----==========================================================-----
*/
P_ActionSeqPart::P_ActionSeqPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->m_cur_actionSeqIndex = -1;
	this->m_cur_actionSeq = QJsonObject();

	//-----------------------------------
	//----初始化ui
	ui.widget_editPart->setEnabled(false);

	// > 放映区、动作元、状态元
	this->m_playingPart = new P_PlayingPart(parent);
	ui.verticalLayout_playing->addWidget(this->m_playingPart);
	this->m_actionPart = new P_ActionPart(parent);
	ui.verticalLayout_action->addWidget(this->m_actionPart);
	this->m_statePart = new P_StatePart(parent);
	ui.verticalLayout_state->addWidget(this->m_statePart);
	
	// > 树
	this->m_p_tree = new P_FlexibleClassificationTree(ui.treeWidget_ActionSeq);
	QJsonObject obj_config = QJsonObject();		//默认配置
	QJsonObject obj_tree = QJsonObject();
	obj_tree.insert("rowHeight", 32);
	obj_config.insert("sortMode", "自定义分支（按id递增排序）");
	obj_config.insert("FCTConfig", obj_tree);
	this->m_p_tree->setData(obj_config);
	connect(this->m_p_tree, &P_FlexibleClassificationTree::currentLeafChanged, this, &P_ActionSeqPart::currentActionSeqChanged );

	// > 水平分割线
	ui.splitter->setStretchFactor(0, 1);
	ui.splitter->setStretchFactor(1, 4);
	ui.splitter->handle(1)->setAttribute(Qt::WA_Hover, true);

	//-----------------------------------
	//----事件绑定

}

P_ActionSeqPart::~P_ActionSeqPart(){
}


/*-------------------------------------------------
		全数据 - 树选择变化
*/
void P_ActionSeqPart::currentActionSeqChanged(QTreeWidgetItem* item, int id, QString name){
	if (id > this->m_actionSeq_list.length()){ return; }

	// > 旧数据存储
	if (this->m_cur_actionSeqIndex != -1){
		QList<QJsonObject> old_actionTank = this->m_actionPart->getData();
		QList<QJsonObject> old_stateTank = this->m_statePart->getData();

		// ... this->m_cur_actionSeq 赋值操作

		this->m_actionSeq_list.replace(this->m_cur_actionSeqIndex, this->m_cur_actionSeq);
	}

	//----------------------------------------------
	// > 新数据填充
	this->m_cur_actionSeqIndex = id - 1;
	this->m_cur_actionSeq = this->m_actionSeq_list.at(this->m_cur_actionSeqIndex);
	ui.lineEdit->setText(name);
	ui.widget_editPart->setEnabled(true);

	// > 加入数据（动作元）
	this->m_cur_actionTank = QList<QJsonObject>();
	for (int i = 0; i < this->m_realLen_action; i++){
		QJsonValue value = this->m_cur_actionSeq.value("动作元-" + QString::number(i + 1));
		if (value.isUndefined() == false){
			QString data = value.toString();
			QJsonDocument jsonDocument = QJsonDocument::fromJson(data.toUtf8());
			QJsonObject obj = jsonDocument.object();
			this->m_cur_actionTank.append(obj);
		}
		else{
			QJsonObject obj = QJsonObject();
			this->m_cur_actionTank.append(obj);
		}
	}
	this->m_actionPart->setData(this->m_cur_actionTank);

	// > 加入数据（状态元）
	this->m_cur_stateTank = QList<QJsonObject>();
	for (int i = 0; i < this->m_realLen_state; i++){
		QJsonValue value = this->m_cur_actionSeq.value("状态元-" + QString::number(i + 1));
		if (value.isUndefined() == false){
			QString data = value.toString();
			QJsonDocument jsonDocument = QJsonDocument::fromJson(data.toUtf8());
			QJsonObject obj = jsonDocument.object();
			this->m_cur_stateTank.append(obj);
		}else{
			QJsonObject obj = QJsonObject();
			this->m_cur_stateTank.append(obj);
		}
	}
	this->m_statePart->setData(this->m_cur_stateTank);

}


/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_ActionSeqPart::setData(QJsonObject actionSeq) {
	this->local_actionSeq = actionSeq;
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
QJsonObject P_ActionSeqPart::getData(){
	this->putUiToData();

	// > 校验
	//...
	
	return this->local_actionSeq;
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_ActionSeqPart::putDataToUi() {
	
	// > 动作序列上限
	QFileInfo plugin_file = S_ActionSeqDataContainer::getInstance()->getActionSeqPluginFile();
	C_LEAnnotation* c_le = S_LEAnnotationReader::getInstance()->readPlugin(plugin_file);
	this->m_realLen_actionSeq = c_le->getParamByKey("动作序列-%d").getRealLen();
	this->m_realLen_action = c_le->getParamByKey("动作元-%d").getRealLen();
	this->m_realLen_state = c_le->getParamByKey("状态元-%d").getRealLen();

	// > 树结构初始化
	QJsonObject obj_treeData = S_ActionSeqDataContainer::getInstance()->getTreeData();
	this->m_p_tree->setData(obj_treeData);		//（存储的数据需要在load前完成赋值）

	// > 加入数据（动作序列）
	this->m_actionSeq_list = QList<QJsonObject>();
	for (int i = 0; i < this->m_realLen_actionSeq; i++){
		QJsonValue value = this->local_actionSeq.value("动作序列-" + QString::number(i + 1));
		if (value.isUndefined() == false){
			QString data = value.toString();
			QJsonDocument jsonDocument = QJsonDocument::fromJson(data.toUtf8());
			QJsonObject obj = jsonDocument.object();
			obj.insert("id", i + 1);
			obj.insert("name", obj.value("标签").toString());
			this->m_actionSeq_list.append(obj);
		}
		else{
			QJsonObject obj = QJsonObject();
			obj.insert("id", i+1);
			obj.insert("name","");
			this->m_actionSeq_list.append(obj);
		}
	}
	this->m_p_tree->loadSource(this->m_actionSeq_list, "id", "name", "type");

	// > 载入动作序列数据
	//qDebug() << this->local_actionSeq;

}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_ActionSeqPart::putUiToData() {
	

}
