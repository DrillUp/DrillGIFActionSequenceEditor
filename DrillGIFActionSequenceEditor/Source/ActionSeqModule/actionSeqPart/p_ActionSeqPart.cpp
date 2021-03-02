#include "stdafx.h"
#include "p_ActionSeqPart.h"

#include "Source/PluginModule/lengthEditor/s_LEAnnotationReader.h"
#include "Source/ProjectModule/s_ProjectManager.h"
#include "Source/ProjectModule/storageGlobal/s_IniManager.h"

#include "../actionSeqData/s_ActionSeqDataContainer.h"

/*
-----==========================================================-----
		�ࣺ		�������п�.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ò����ṩ�������з�ӳ�����ܣ���Ƕ��״̬Ԫ������Ԫ�Ŀ顣
		
		Ŀ�꣺		1.

		ʹ�÷�����
				>��ʼ��

-----==========================================================-----
*/
P_ActionSeqPart::P_ActionSeqPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������
	this->m_slotBlock_source = false;
	this->m_cur_actionSeqIndex = -1;
	this->m_cur_actionSeq = QJsonObject();

	//-----------------------------------
	//----��ʼ��ui
	ui.widget_editPart->setEnabled(false);

	// > ��ӳ��������Ԫ��״̬Ԫ
	this->m_playingPart = new P_PlayingPart(parent);
	this->m_statePart = new P_StatePart(parent);
	this->m_actionPart = new P_ActionPart(parent);

	// > ���۵�ѡ�
	this->m_p_FoldableTabRelater = new P_FoldableTabRelater(ui.tabWidget);	//��ui�е�ֻ��ʾ�⣬�ù�������ؽ�tab��
	this->m_p_FoldableTabRelater->addPart(" ��ӳ��  ", this->m_playingPart);
	this->m_p_FoldableTabRelater->addPart(" ״̬Ԫ  ", this->m_statePart);
	this->m_p_FoldableTabRelater->addPart(" ����Ԫ  ", this->m_actionPart);

	// > ��
	this->m_p_tree = new P_FlexibleClassificationTree(ui.treeWidget_ActionSeq);
	QJsonObject obj_config = QJsonObject();		//Ĭ������
	QJsonObject obj_tree = QJsonObject();
	obj_tree.insert("rowHeight", 32);
	obj_config.insert("sortMode", "�Զ����֧����id��������");
	obj_config.insert("FCTConfig", obj_tree);
	this->m_p_tree->setData(obj_config);
	connect(this->m_p_tree, &P_FlexibleClassificationTree::currentLeafChanged, this, &P_ActionSeqPart::currentActionSeqChanged );

	// > ˮƽ�ָ���
	ui.splitter->setStretchFactor(0, 4);		//�������仯�Ƚ���֣���ʱ�����ɣ�
	ui.splitter->setStretchFactor(1, 5);
	ui.splitter->handle(1)->setAttribute(Qt::WA_Hover, true);

	//-----------------------------------
	//----�¼���
	connect(ui.lineEdit, &QLineEdit::textEdited, this->m_p_tree, &P_FlexibleClassificationTree::outerModifySelectedLeafName);

}

P_ActionSeqPart::~P_ActionSeqPart(){
}


/*-------------------------------------------------
		ȫ���� - ��ѡ��仯
*/
void P_ActionSeqPart::currentActionSeqChanged(QTreeWidgetItem* item, int id, QString name){
	if (id > this->m_actionSeq_list.length()){ return; }
	if (this->m_slotBlock_source == true){ return; }

	// > �����ݴ洢
	this->local_saveCurIndexData();

	// > ���������
	this->local_loadIndexData(id - 1);
	
}



/*-------------------------------------------------
		���� - ���汾������
*/
void P_ActionSeqPart::local_saveCurIndexData(){
	if (this->m_cur_actionSeqIndex < 0){ return; }
	if (this->m_cur_actionSeqIndex >= this->m_actionSeq_list.count()){ return; }

	// > ������
	this->m_cur_actionSeq.insert("��ǩ", ui.lineEdit->text());

	// > �������ݣ�����Ԫ��
	QList<QJsonObject> old_actionTank = this->m_actionPart->getData();
	QStringList actionTank_str = TTool::_QList_QJsonObjectToQString_(old_actionTank);
	for (int i = 0; i < actionTank_str.count(); i++){
		this->m_cur_actionSeq.insert("����Ԫ-" + QString::number(i + 1), actionTank_str.at(i));
	}

	// > �������ݣ�״̬Ԫ��
	QList<QJsonObject> old_stateTank = this->m_statePart->getData();
	QStringList stateTank_str = TTool::_QList_QJsonObjectToQString_(old_stateTank);
	for (int i = 0; i < stateTank_str.count(); i++){
		this->m_cur_actionSeq.insert("״̬Ԫ-" + QString::number(i + 1), stateTank_str.at(i));
	}

	// > �������ݣ���ӳ����
	// ... this->m_cur_actionSeq ��ֵ����
	
	// > �༭���
	S_ProjectManager::getInstance()->setDirty();

	this->m_actionSeq_list.replace(this->m_cur_actionSeqIndex, this->m_cur_actionSeq);
}
/*-------------------------------------------------
		���� - ��ȡ��������
*/
void P_ActionSeqPart::local_loadIndexData(int index){
	if (index < 0){ return; }
	if (index >= this->m_actionSeq_list.count()){ return; }

	// > ������
	this->m_cur_actionSeq = this->m_actionSeq_list.at(index);
	ui.widget_editPart->setEnabled(true);
	ui.lineEdit->setText(this->m_cur_actionSeq.value("��ǩ").toString());
	
	// > �������ݣ�����Ԫ��
	QStringList actionTank_str = QStringList();
	for (int i = 0; i < this->m_realLen_action; i++){
		QJsonValue value = this->m_cur_actionSeq.value("����Ԫ-" + QString::number(i + 1));
		actionTank_str.append(value.toString());
	}
	this->m_cur_actionTank = TTool::_QList_QStringToQJsonObject_(actionTank_str);
	this->m_actionPart->setData(this->m_cur_actionTank);

	// > �������ݣ�״̬Ԫ��
	QStringList stateTank_str = QStringList();
	for (int i = 0; i < this->m_realLen_state; i++){
		QJsonValue value = this->m_cur_actionSeq.value("״̬Ԫ-" + QString::number(i + 1));
		stateTank_str.append(value.toString());
	}
	this->m_cur_stateTank = TTool::_QList_QStringToQJsonObject_(stateTank_str);
	this->m_statePart->setData(this->m_cur_stateTank);

	// > �������ݣ���ӳ����
	QStringList defaultState = TTool::_QJsonArrayString_To_QListQString_(this->m_cur_actionSeq.value("Ĭ�ϵ�״̬Ԫ����").toString());
	this->m_playingPart->setDefaultStateData(defaultState);
	this->m_playingPart->setSource(this->m_cur_stateTank, this->m_cur_actionTank);

	this->m_cur_actionSeqIndex = index;
}



/*-------------------------------------------------
		���� - ��������
*/
void P_ActionSeqPart::setData(QJsonObject actionSeq) {
	this->m_slotBlock_source = true;
	this->local_actionSeq = actionSeq;
	this->putDataToUi();
	this->m_slotBlock_source = false;
}
/*-------------------------------------------------
		���� - ȡ������
*/
QJsonObject P_ActionSeqPart::getData(){
	this->putUiToData();

	// > У��
	//...
	
	return this->local_actionSeq;
}
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void P_ActionSeqPart::putDataToUi() {
	
	// > ������������
	QFileInfo plugin_file = S_ActionSeqDataContainer::getInstance()->getActionSeqPluginFile();
	C_LEAnnotation* c_le = S_LEAnnotationReader::getInstance()->readPlugin(plugin_file);
	this->m_realLen_actionSeq = c_le->getParamByKey("��������-%d").getRealLen();
	this->m_realLen_action = c_le->getParamByKey("����Ԫ-%d").getRealLen();
	this->m_realLen_state = c_le->getParamByKey("״̬Ԫ-%d").getRealLen();

	// > ���ṹ��ʼ��
	QJsonObject obj_treeData = S_ActionSeqDataContainer::getInstance()->getTreeData();
	this->m_p_tree->setData(obj_treeData);		//���洢��������Ҫ��loadǰ��ɸ�ֵ��

	// > �ֽ����ݣ��������� --> ���������б�
	this->m_actionSeq_list = QList<QJsonObject>();
	for (int i = 0; i < this->m_realLen_actionSeq; i++){
		QJsonValue value = this->local_actionSeq.value("��������-" + QString::number(i + 1));
		if (value.isUndefined() == false){
			QString data = value.toString();
			QJsonDocument jsonDocument = QJsonDocument::fromJson(data.toUtf8());
			QJsonObject obj = jsonDocument.object();
			obj.insert("COAS_id", i + 1);
			obj.insert("COAS_name", obj.value("��ǩ").toString());
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

	// > ���붯����������
	//qDebug() << this->local_actionSeq;

}
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_ActionSeqPart::putUiToData() {

	// > ���浱ǰ����
	this->local_saveCurIndexData();


	// > �ϲ����ݣ����������б� --> �������� ��
	for (int i = 0; i < this->m_realLen_actionSeq; i++){
		QJsonObject data_obj = this->m_actionSeq_list.at(i);

		// > �����ݣ��ϲ����������У�
		int id = data_obj.value("COAS_id").toInt();
		data_obj.insert("COAS_type", this->m_p_tree->getLeafType(id));
		data_obj.remove("COAS_id");
		data_obj.remove("COAS_name");	//��ȥ��id��name��type������

		QString data_str = QJsonDocument(data_obj).toJson(QJsonDocument::Compact);
		this->local_actionSeq.insert("��������-" + QString::number(i + 1), data_str);
	}

	S_ActionSeqDataContainer::getInstance()->setActionSeqData(this->local_actionSeq);
	S_ActionSeqDataContainer::getInstance()->modifyTreeData(this->m_p_tree->getData());
}

/*-------------------------------------------------
		���� - �û��Զ���UI��ȡ
*/
void P_ActionSeqPart::ui_loadConfig(){

	// > �ָ���
	QString data = S_IniManager::getInstance()->getConfig("COAS_MainWindowSpliter");
	if (data != ""){
		ui.splitter->setSizes(TTool::_QList_QStringToInt_(data.split(",")));
	}

}
/*-------------------------------------------------
		���� - �û��Զ���UI�洢
*/
void P_ActionSeqPart::ui_saveConfig(){

	// > �ָ���
	QStringList data = TTool::_QList_IntToQString_( ui.splitter->sizes() );
	S_IniManager::getInstance()->setConfig("COAS_MainWindowSpliter", data.join(","));
}