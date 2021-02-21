#include "stdafx.h"
#include "p_ActionSeqPart.h"

#include "Source/PluginModule/lengthEditor/s_LEAnnotationReader.h"

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

}

P_ActionSeqPart::~P_ActionSeqPart(){
}


/*-------------------------------------------------
		ȫ���� - ��ѡ��仯
*/
void P_ActionSeqPart::currentActionSeqChanged(QTreeWidgetItem* item, int id, QString name){
	if (id > this->m_actionSeq_list.length()){ return; }

	// > �����ݴ洢
	if (this->m_cur_actionSeqIndex != -1){
		QList<QJsonObject> old_actionTank = this->m_actionPart->getData();
		QList<QJsonObject> old_stateTank = this->m_statePart->getData();

		// ... this->m_cur_actionSeq ��ֵ����

		this->m_actionSeq_list.replace(this->m_cur_actionSeqIndex, this->m_cur_actionSeq);
	}

	//----------------------------------------------
	// > ���������
	this->m_cur_actionSeqIndex = id - 1;
	this->m_cur_actionSeq = this->m_actionSeq_list.at(this->m_cur_actionSeqIndex);
	ui.lineEdit->setText(name);
	ui.widget_editPart->setEnabled(true);

	// > �������ݣ�����Ԫ��
	this->m_cur_actionTank = QList<QJsonObject>();
	for (int i = 0; i < this->m_realLen_action; i++){
		QJsonValue value = this->m_cur_actionSeq.value("����Ԫ-" + QString::number(i + 1));
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

	// > �������ݣ�״̬Ԫ��
	this->m_cur_stateTank = QList<QJsonObject>();
	for (int i = 0; i < this->m_realLen_state; i++){
		QJsonValue value = this->m_cur_actionSeq.value("״̬Ԫ-" + QString::number(i + 1));
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
		���� - ��������
*/
void P_ActionSeqPart::setData(QJsonObject actionSeq) {
	this->local_actionSeq = actionSeq;
	this->putDataToUi();
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

	// > �������ݣ��������У�
	this->m_actionSeq_list = QList<QJsonObject>();
	for (int i = 0; i < this->m_realLen_actionSeq; i++){
		QJsonValue value = this->local_actionSeq.value("��������-" + QString::number(i + 1));
		if (value.isUndefined() == false){
			QString data = value.toString();
			QJsonDocument jsonDocument = QJsonDocument::fromJson(data.toUtf8());
			QJsonObject obj = jsonDocument.object();
			obj.insert("id", i + 1);
			obj.insert("name", obj.value("��ǩ").toString());
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

	// > ���붯����������
	//qDebug() << this->local_actionSeq;

}
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_ActionSeqPart::putUiToData() {
	

}
