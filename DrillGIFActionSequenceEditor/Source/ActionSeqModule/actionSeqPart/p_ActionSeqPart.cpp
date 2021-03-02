#include "stdafx.h"
#include "p_ActionSeqPart.h"

#include "Source/PluginModule/lengthEditor/s_LEAnnotationReader.h"
#include "Source/ProjectModule/s_ProjectManager.h"
#include "Source/ProjectModule/storageGlobal/s_IniManager.h"

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
	this->m_slotBlock_source = false;
	this->m_cur_actionSeqIndex = -1;
	this->m_cur_actionSeq = QJsonObject();

	//-----------------------------------
	//----初始化ui
	ui.widget_editPart->setEnabled(false);

	// > 放映区、动作元、状态元
	this->m_playingPart = new P_PlayingPart(parent);
	this->m_statePart = new P_StatePart(parent);
	this->m_actionPart = new P_ActionPart(parent);

	// > 可折叠选项卡
	this->m_p_FoldableTabRelater = new P_FoldableTabRelater(ui.tabWidget);	//（ui中的只是示意，该工具类会重建tab）
	this->m_p_FoldableTabRelater->addPart(" 放映区  ", this->m_playingPart);
	this->m_p_FoldableTabRelater->addPart(" 状态元  ", this->m_statePart);
	this->m_p_FoldableTabRelater->addPart(" 动作元  ", this->m_actionPart);

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
	ui.splitter->setStretchFactor(0, 4);		//（比例变化比较奇怪，暂时这样吧）
	ui.splitter->setStretchFactor(1, 5);
	ui.splitter->handle(1)->setAttribute(Qt::WA_Hover, true);

	//-----------------------------------
	//----事件绑定
	connect(ui.lineEdit, &QLineEdit::textEdited, this->m_p_tree, &P_FlexibleClassificationTree::outerModifySelectedLeafName);

}

P_ActionSeqPart::~P_ActionSeqPart(){
}


/*-------------------------------------------------
		全数据 - 树选择变化
*/
void P_ActionSeqPart::currentActionSeqChanged(QTreeWidgetItem* item, int id, QString name){
	if (id > this->m_actionSeq_list.length()){ return; }
	if (this->m_slotBlock_source == true){ return; }

	// > 旧数据存储
	this->local_saveCurIndexData();

	// > 新数据填充
	this->local_loadIndexData(id - 1);
	
}



/*-------------------------------------------------
		数据 - 保存本地数据
*/
void P_ActionSeqPart::local_saveCurIndexData(){
	if (this->m_cur_actionSeqIndex < 0){ return; }
	if (this->m_cur_actionSeqIndex >= this->m_actionSeq_list.count()){ return; }

	// > 表单数据
	this->m_cur_actionSeq.insert("标签", ui.lineEdit->text());

	// > 保存数据（动作元）
	QList<QJsonObject> old_actionTank = this->m_actionPart->getData();
	QStringList actionTank_str = TTool::_QList_QJsonObjectToQString_(old_actionTank);
	for (int i = 0; i < actionTank_str.count(); i++){
		this->m_cur_actionSeq.insert("动作元-" + QString::number(i + 1), actionTank_str.at(i));
	}

	// > 保存数据（状态元）
	QList<QJsonObject> old_stateTank = this->m_statePart->getData();
	QStringList stateTank_str = TTool::_QList_QJsonObjectToQString_(old_stateTank);
	for (int i = 0; i < stateTank_str.count(); i++){
		this->m_cur_actionSeq.insert("状态元-" + QString::number(i + 1), stateTank_str.at(i));
	}

	// > 保存数据（放映区）
	// ... this->m_cur_actionSeq 赋值操作
	
	// > 编辑标记
	S_ProjectManager::getInstance()->setDirty();

	this->m_actionSeq_list.replace(this->m_cur_actionSeqIndex, this->m_cur_actionSeq);
}
/*-------------------------------------------------
		数据 - 读取本地数据
*/
void P_ActionSeqPart::local_loadIndexData(int index){
	if (index < 0){ return; }
	if (index >= this->m_actionSeq_list.count()){ return; }

	// > 表单数据
	this->m_cur_actionSeq = this->m_actionSeq_list.at(index);
	ui.widget_editPart->setEnabled(true);
	ui.lineEdit->setText(this->m_cur_actionSeq.value("标签").toString());
	
	// > 加入数据（动作元）
	QStringList actionTank_str = QStringList();
	for (int i = 0; i < this->m_realLen_action; i++){
		QJsonValue value = this->m_cur_actionSeq.value("动作元-" + QString::number(i + 1));
		actionTank_str.append(value.toString());
	}
	this->m_cur_actionTank = TTool::_QList_QStringToQJsonObject_(actionTank_str);
	this->m_actionPart->setData(this->m_cur_actionTank);

	// > 加入数据（状态元）
	QStringList stateTank_str = QStringList();
	for (int i = 0; i < this->m_realLen_state; i++){
		QJsonValue value = this->m_cur_actionSeq.value("状态元-" + QString::number(i + 1));
		stateTank_str.append(value.toString());
	}
	this->m_cur_stateTank = TTool::_QList_QStringToQJsonObject_(stateTank_str);
	this->m_statePart->setData(this->m_cur_stateTank);

	// > 加入数据（放映区）
	QStringList defaultState = TTool::_QJsonArrayString_To_QListQString_(this->m_cur_actionSeq.value("默认的状态元集合").toString());
	this->m_playingPart->setDefaultStateData(defaultState);
	this->m_playingPart->setSource(this->m_cur_stateTank, this->m_cur_actionTank);

	this->m_cur_actionSeqIndex = index;
}



/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_ActionSeqPart::setData(QJsonObject actionSeq) {
	this->m_slotBlock_source = true;
	this->local_actionSeq = actionSeq;
	this->putDataToUi();
	this->m_slotBlock_source = false;
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

	// > 分解数据（动作序列 --> 动作序列列表）
	this->m_actionSeq_list = QList<QJsonObject>();
	for (int i = 0; i < this->m_realLen_actionSeq; i++){
		QJsonValue value = this->local_actionSeq.value("动作序列-" + QString::number(i + 1));
		if (value.isUndefined() == false){
			QString data = value.toString();
			QJsonDocument jsonDocument = QJsonDocument::fromJson(data.toUtf8());
			QJsonObject obj = jsonDocument.object();
			obj.insert("COAS_id", i + 1);
			obj.insert("COAS_name", obj.value("标签").toString());
			this->m_actionSeq_list.append(obj);
		}
		else{
			QJsonObject obj = QJsonObject();
			obj.insert("COAS_id", i+1);
			obj.insert("COAS_name","");
			this->m_actionSeq_list.append(obj);
		}
	}
	this->m_p_tree->loadSource(this->m_actionSeq_list, "COAS_id", "COAS_name", "COAS_type");

	// > 载入动作序列数据
	//qDebug() << this->local_actionSeq;

}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_ActionSeqPart::putUiToData() {

	// > 保存当前数据
	this->local_saveCurIndexData();


	// > 合并数据（动作序列列表 --> 动作序列 ）
	for (int i = 0; i < this->m_realLen_actionSeq; i++){
		QJsonObject data_obj = this->m_actionSeq_list.at(i);

		// > 树数据（合并到动作序列）
		int id = data_obj.value("COAS_id").toInt();
		data_obj.insert("COAS_type", this->m_p_tree->getLeafType(id));
		data_obj.remove("COAS_id");
		data_obj.remove("COAS_name");	//（去除id和name，type保留）

		QString data_str = QJsonDocument(data_obj).toJson(QJsonDocument::Compact);
		this->local_actionSeq.insert("动作序列-" + QString::number(i + 1), data_str);
	}

	S_ActionSeqDataContainer::getInstance()->setActionSeqData(this->local_actionSeq);
	S_ActionSeqDataContainer::getInstance()->modifyTreeData(this->m_p_tree->getData());
}

/*-------------------------------------------------
		窗口 - 用户自定义UI读取
*/
void P_ActionSeqPart::ui_loadConfig(){

	// > 分割条
	QString data = S_IniManager::getInstance()->getConfig("COAS_MainWindowSpliter");
	if (data != ""){
		ui.splitter->setSizes(TTool::_QList_QStringToInt_(data.split(",")));
	}

}
/*-------------------------------------------------
		窗口 - 用户自定义UI存储
*/
void P_ActionSeqPart::ui_saveConfig(){

	// > 分割条
	QStringList data = TTool::_QList_IntToQString_( ui.splitter->sizes() );
	S_IniManager::getInstance()->setConfig("COAS_MainWindowSpliter", data.join(","));
}