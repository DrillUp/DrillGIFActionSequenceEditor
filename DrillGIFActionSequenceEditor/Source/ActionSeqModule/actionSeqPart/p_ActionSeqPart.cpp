#include "stdafx.h"
#include "p_ActionSeqPart.h"

#include "Source/ProjectModule/s_ProjectManager.h"
#include "Source/ProjectModule/storageGlobal/s_IniManager.h"

#include "../actionSeqData/lengthData/w_ActionSeqLength.h"
#include "../actionSeqData/s_ActionSeqDataContainer.h"
#include "Source/Utils/widgetFormSenior/flexibleClassificationTree/private/c_FCT_Config.h"

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

	// > ��ӳ��������Ԫ��״̬Ԫ
	this->m_statePart = new P_StatePart(parent);
	this->m_actionPart = new P_ActionPart(parent);
	this->m_playingPart = new P_PlayingPart(this->m_statePart, this->m_actionPart, parent);

	// > ���۵�ѡ�
	this->m_p_FoldableTabRelater = new P_FoldableTabRelater(ui.tabWidget);	//��ui�е�ֻ��ʾ�⣬�ù�������ؽ�tab��
	this->m_p_FoldableTabRelater->addPart(" ��ӳ��  ", this->m_playingPart);
	this->m_p_FoldableTabRelater->addPart(" ״̬Ԫ  ", this->m_statePart);
	this->m_p_FoldableTabRelater->addPart(" ����Ԫ  ", this->m_actionPart);
	this->setPartGray();

	// > ��
	this->m_p_tree = new P_FlexibleClassificationTree(ui.treeWidget_ActionSeq);
	C_FCT_Config* config = this->m_p_tree->getConfigEx();
	config->rowHeight = 32;
	config->setCurrentMode("�Զ����֧����id��������");
	this->m_p_tree->setConfigEx(config);
	connect(this->m_p_tree, &P_FlexibleClassificationTree::currentLeafChanged, this, &P_ActionSeqPart::currentActionSeqChanged);
	connect(this->m_p_tree, &P_FlexibleClassificationTree::menuCopyLeafTriggered, this, &P_ActionSeqPart::shortcut_copyData);
	connect(this->m_p_tree, &P_FlexibleClassificationTree::menuPasteLeafTriggered, this, &P_ActionSeqPart::shortcut_pasteData);
	connect(this->m_p_tree, &P_FlexibleClassificationTree::menuClearLeafTriggered, this, &P_ActionSeqPart::shortcut_clearData);

	// > ˮƽ�ָ���
	ui.splitter->setStretchFactor(0, 4);		//�������仯�Ƚ���֣���ʱ�����ɣ�
	ui.splitter->setStretchFactor(1, 5);
	ui.splitter->handle(1)->setAttribute(Qt::WA_Hover, true);

	//-----------------------------------
	//----�¼���
	connect(ui.lineEdit, &QLineEdit::textEdited, this->m_p_tree, &P_FlexibleClassificationTree::outerModifySelectedLeafName);
	connect(ui.toolButton_modifyLength, &QToolButton::clicked, this, &P_ActionSeqPart::modifyDataLengthInAction);

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

	// > �Զ���� Ĭ��״̬Ԫ����
	QStringList default_list = TTool::_JSON_parse_To_QListQString_(this->m_cur_actionSeq.value("Ĭ�ϵ�״̬Ԫ����").toString());
	if (default_list.count() == 0){
		for (int i = 0; i < old_stateTank.count(); i++){
			QJsonObject obj = old_stateTank.at(i);
			QString state_name = obj.value("״̬Ԫ����").toString();
			if (state_name != ""){
				default_list.append(state_name);
				break;
			}
		}
	}
	this->m_cur_actionSeq.insert("Ĭ�ϵ�״̬Ԫ����", TTool::_JSON_stringify_(default_list));

	// > �������ݣ���ӳ����
	QStringList default_state_list = this->m_playingPart->getDefaultStateData();
	if (default_state_list.count() != 0){
		this->m_cur_actionSeq.insert("Ĭ�ϵ�״̬Ԫ����", TTool::_JSON_stringify_(default_state_list));
	}
	
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
	for (int i = 0; i < S_ActionSeqDataContainer::getInstance()->getActionSeqLength().realLen_action; i++){
		QJsonValue value = this->m_cur_actionSeq.value("����Ԫ-" + QString::number(i + 1));
		actionTank_str.append(value.toString());
	}
	this->m_cur_actionTank = TTool::_QList_QStringToQJsonObject_(actionTank_str);
	this->m_actionPart->setData(this->m_cur_actionTank);

	// > �������ݣ�״̬Ԫ��
	QStringList stateTank_str = QStringList();
	for (int i = 0; i < S_ActionSeqDataContainer::getInstance()->getActionSeqLength().realLen_state; i++){
		QJsonValue value = this->m_cur_actionSeq.value("״̬Ԫ-" + QString::number(i + 1));
		stateTank_str.append(value.toString());
	}
	this->m_cur_stateTank = TTool::_QList_QStringToQJsonObject_(stateTank_str);
	this->m_statePart->setData(this->m_cur_stateTank);

	// > �������ݣ���ӳ����
	QStringList defaultState = TTool::_JSON_parse_To_QListQString_(this->m_cur_actionSeq.value("Ĭ�ϵ�״̬Ԫ����").toString());
	this->m_playingPart->stopFrame();
	this->m_playingPart->setDefaultStateData(defaultState);
	this->m_playingPart->refreshSource();

	this->m_cur_actionSeqIndex = index;
}



/*-------------------------------------------------
		���� - �滻
*/
void P_ActionSeqPart::op_replace(int index, QJsonObject actiong_seq){
	if (index < 0){ return; }
	if (index >= this->m_actionSeq_list.count()){ return; }
	if (actiong_seq.isEmpty()){ return; }

	// > �༭���
	S_ProjectManager::getInstance()->setDirty();

	// > ִ���滻
	this->m_actionSeq_list.replace(index, actiong_seq);
	this->local_loadIndexData(index);

	// > ������������
	this->m_p_tree->outerModifySelectedLeafName(ui.lineEdit->text());
}
/*-------------------------------------------------
		���� - ���
*/
void P_ActionSeqPart::op_clear(int index){
	if (index < 0){ return; }
	if (index >= this->m_actionSeq_list.count()){ return; }

	// > �༭���
	S_ProjectManager::getInstance()->setDirty();

	// > ִ���滻
	this->m_actionSeq_list.replace(index, QJsonObject());
	this->local_loadIndexData(index);

	// > ������������
	this->m_p_tree->outerModifySelectedLeafName(ui.lineEdit->text());
}
/*-------------------------------------------------
		��ݼ� - �¼�
*/
void P_ActionSeqPart::keyPressEvent(QKeyEvent *event){
	if (event->modifiers() & Qt::ControlModifier){
		if (event->key() == Qt::Key_C){
			this->shortcut_copyData();
		}
		if (event->key() == Qt::Key_V){
			this->shortcut_pasteData();
		}
	}
	if (event->key() == Qt::Key_Delete){
		this->shortcut_clearData();
	}
}
/*-------------------------------------------------
		��ݼ� - ����
*/
void P_ActionSeqPart::shortcut_copyData(){
	if (ui.treeWidget_ActionSeq->hasFocus() == false){ return; }
	this->m_copyed_actionSeq = this->m_cur_actionSeq;
	this->m_p_tree->setLeafOuterControl_PasteActive(true);		//����ճ��
}
/*-------------------------------------------------
		��ݼ� - ճ��
*/
void P_ActionSeqPart::shortcut_pasteData(){
	if (ui.treeWidget_ActionSeq->hasFocus() == false){ return; }
	this->op_replace(this->m_cur_actionSeqIndex, this->m_copyed_actionSeq);
}
/*-------------------------------------------------
		��ݼ� - ���
*/
void P_ActionSeqPart::shortcut_clearData(){
	if (ui.treeWidget_ActionSeq->hasFocus() == false){ return; }
	this->op_clear(this->m_cur_actionSeqIndex);
}



/*-------------------------------------------------
		��ؼ� - �û�
*/
void P_ActionSeqPart::setPartGray(){
	this->m_p_FoldableTabRelater->homingAllTab();
	ui.tabWidget->setCurrentIndex(0);
	this->m_cur_actionSeqIndex = -1;
	ui.widget_editPart->setEnabled(false);
}
/*-------------------------------------------------
		��ؼ� - ֹͣ����
*/
void P_ActionSeqPart::stopPlaying(){
	this->m_playingPart->stopFrame();
}


/*-------------------------------------------------
		���� - ��������
*/
void P_ActionSeqPart::setData(QJsonObject actionSeq, C_ActionSeqLength length) {
	this->m_slotBlock_source = true;
	S_ActionSeqDataContainer::getInstance()->setActionSeqData(actionSeq);
	S_ActionSeqDataContainer::getInstance()->setActionSeqLength(length);	//��lengthȡ������������ʵʱ�仯��
	this->putDataToUi();
	this->m_slotBlock_source = false;
}
/*-------------------------------------------------
		���� - ȡ������
*/
QJsonObject P_ActionSeqPart::getDataActionSeqData(){
	this->putUiToData();
	return S_ActionSeqDataContainer::getInstance()->getActionSeqData();
}
C_ActionSeqLength P_ActionSeqPart::getDataActionSeqLength(){
	this->putUiToData();
	return S_ActionSeqDataContainer::getInstance()->getActionSeqLength();
}
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void P_ActionSeqPart::putDataToUi() {

	// > ���ṹ��ʼ��
	QJsonObject obj_treeData = S_ActionSeqDataContainer::getInstance()->getTreeData();
	C_FCT_Config* config = dynamic_cast<C_FCT_Config*>(this->m_p_tree->createConfigData());
	config->setJsonObject(obj_treeData, this->m_p_tree);		//���洢��������Ҫ��loadǰ��ɸ�ֵ��
	this->m_p_tree->setConfigEx(config);

	// > �ֽ����ݣ��������� --> ���������б�
	int data_actionSeqCount = S_ActionSeqDataContainer::getInstance()->getActionSeqLength().realLen_actionSeq;
	QJsonObject data_actionSeq = S_ActionSeqDataContainer::getInstance()->getActionSeqData();

	this->m_actionSeq_list = QList<QJsonObject>();
	for (int i = 0; i < data_actionSeqCount; i++){

		// > �������޸ģ��������� -> �������У�
		QJsonValue v = data_actionSeq.value("��������-" + QString::number(i + 1));
		if (v.isUndefined() == false){
			data_actionSeq.insert("��������-" + QString::number(i + 1), v);
			data_actionSeq.remove("��������-" + QString::number(i + 1));
		}

		// > �������
		QJsonValue value = data_actionSeq.value("��������-" + QString::number(i + 1));
		if (value.isUndefined() == false){
			QJsonObject obj = TTool::_JSON_parse_To_Obj_(value.toString());
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

	// > �༭���û�
	this->setPartGray();	//����Ҫ�������ѡ��һ���������У�
	
	// > ���붯����������
	//qDebug() << data_actionSeq;

}
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_ActionSeqPart::putUiToData() {

	// > ���浱ǰ����
	this->local_saveCurIndexData();

	// > �ϲ����ݣ����������б� --> �������� ��
	int data_actionSeqCount = S_ActionSeqDataContainer::getInstance()->getActionSeqLength().realLen_actionSeq;
	QJsonObject data_actionSeq = S_ActionSeqDataContainer::getInstance()->getActionSeqData();

	for (int i = 0; i < data_actionSeqCount; i++){
		QJsonObject data_obj = this->m_actionSeq_list.at(i);

		// > �����ݣ��ϲ����������У�
		int id = data_obj.value("COAS_id").toInt();
		data_obj.insert("COAS_type", this->m_p_tree->getLeafType(id));
		data_obj.remove("COAS_id");
		data_obj.remove("COAS_name");	//��ȥ��id��name��type������

		QString data_str = TTool::_JSON_stringify_(data_obj);
		data_actionSeq.insert("��������-" + QString::number(i + 1), data_str);
		data_actionSeq.remove("��������-" + QString::number(i + 1));
	}

	S_ActionSeqDataContainer::getInstance()->setActionSeqData(data_actionSeq);
	S_ActionSeqDataContainer::getInstance()->modifyTreeData(this->m_p_tree->getConfigEx()->getJsonObject());
}


/*-------------------------------------------------
		���� - �޸ĳ���
*/
void P_ActionSeqPart::modifyDataLengthInAction(){
	this->m_slotBlock_source = true;

	W_ActionSeqLength d(this);
	d.setDataInModifyMode(S_ActionSeqDataContainer::getInstance()->getActionSeqLength());
	if (d.exec() == QDialog::Accepted){

		this->putUiToData();
		C_ActionSeqLength result = d.getData();
		S_ActionSeqDataContainer::getInstance()->setActionSeqLength(result);
		this->putDataToUi();
	}
	this->m_slotBlock_source = false;

}


/*-------------------------------------------------
		���� - �û��Զ���UI��ȡ
*/
void P_ActionSeqPart::ui_loadConfig(){

	// > �ָ���
	QString data = S_IniManager::getInstance()->getConfig("COAS_MainWindow_Spliter");
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
	S_IniManager::getInstance()->setConfig("COAS_MainWindow_Spliter", data.join(","));
}