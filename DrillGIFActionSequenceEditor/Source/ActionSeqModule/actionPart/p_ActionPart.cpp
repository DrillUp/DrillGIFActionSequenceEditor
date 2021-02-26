#include "stdafx.h"
#include "p_ActionPart.h"

#include "../actionSeqData/s_ActionSeqDataDefiner.h"
#include "../actionSeqData/s_ActionSeqDataContainer.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		����Ԫ��.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ò����ṩ����Ԫ�༭���ܡ�
		
		Ŀ�꣺		1.

		ʹ�÷�����
				>��ʼ��

-----==========================================================-----
*/
P_ActionPart::P_ActionPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������

	//-----------------------------------
	//----��ʼ��ui
	this->m_last_index = -1;
	
	// > �༭��
	this->m_table = new P_RadioTable(ui.tableWidget);
	QJsonObject obj_config = QJsonObject();
	obj_config.insert("zeroFillCount", 2);
	obj_config.insert("rowHeight", 22);
	this->m_table->setConfigParam(obj_config);	//�̶�����

	// > ���ٱ�
	C_FastClass c_class = S_ActionSeqDataDefiner::getInstance()->getTable_Action();
	this->m_FastForm = new P_FastForm(ui.widget_form);
	this->m_FastForm->prepareFastClass(c_class);
	this->m_FastForm->rebuildUI();

	// > ����֡
	this->m_p_AnimationListEditor = new P_AnimationListEditor(ui.listWidget);
	this->m_p_AnimationListEditor->setSource(C_ALEData());

	C_ALEConfig config = C_ALEConfig();
	this->m_p_AnimationListEditor->setConfigParam_ALE(config);

	// > ����֡������
	this->m_p_AnimationListPlayer = new P_AnimationListPlayer();
	this->m_p_AnimationListPlayer->setAnimationListEditor(m_p_AnimationListEditor);
	this->m_p_AnimationListPlayer->setPlayType(QStringList() << "����һ��" << "��������" << "��������");
	QHBoxLayout* layout = new QHBoxLayout(ui.groupBox_animPlayer);
	ui.groupBox_animPlayer->setLayout(layout);
	layout->setMargin(3);
	layout->addWidget(this->m_p_AnimationListPlayer);

	// > ͼƬ�鿴��
	this->m_p_AnimPictureViewer = new P_AnimPictureViewer(ui.widget_view);
	this->m_p_AnimPictureViewer->rebuildUI();

	//-----------------------------------
	//----�¼���
	connect(this->m_table, &P_RadioTable::currentIndexChanged, this, &P_ActionPart::currentIndexChanged);
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::selectedIndexChanged_Multi, this, &P_ActionPart::tableChanged_Multi);

	// > ͼƬ�鿴�� - ����֡�л�
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::currentIndexChanged, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::setAnimFrame);
	// > ͼƬ�鿴�� - ������Դ�л�
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::animBitmapChanged, this, &P_ActionPart::bitmapChanged);
	// > ͼƬ�鿴�� - ����
	connect(ui.toolButton_zoom_in, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomIn);
	connect(ui.toolButton_zoom_out, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomOut);
	connect(ui.toolButton_zoom_regular, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomReset);
	connect(this->m_p_AnimPictureViewer, &P_AnimPictureViewer::scaleChanged, this, &P_ActionPart::zoomValueChanged);

}

P_ActionPart::~P_ActionPart(){
}

/*-------------------------------------------------
		����֡ - ѡ��仯
*/
void P_ActionPart::tableChanged_Multi(QList<int> index_list){
	//�����޲�����
}
/*-------------------------------------------------
		����֡ - ��Դ�л�
*/
void P_ActionPart::bitmapChanged(){
	C_ALEData data = this->m_p_AnimationListEditor->getSource();
	this->m_p_AnimPictureViewer->setSource(data.getAllFile());
}
/*-------------------------------------------------
		����֡ - ���ű����л�
*/
void P_ActionPart::zoomValueChanged(double value){
	ui.label_zoomValue->setText( QString::number(value * 100)+"%" );
}

/*-------------------------------------------------
		�ؼ� - ��ȡ����Ԫ����
*/
QStringList P_ActionPart::getNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_actionDataList.count(); i++){
		result.append(this->local_actionDataList.at(i).value("����Ԫ����").toString());
	}
	return result;
}

/*-------------------------------------------------
		�ؼ� - ����Ԫ�л�
*/
void P_ActionPart::currentIndexChanged(int index){

	// > �ɵ����ݴ洢
	this->local_saveCurIndexData();

	// > �����µ�����
	this->local_loadIndexData(index);
}


/*-------------------------------------------------
		��ݼ� - �¼�
*/
void P_ActionPart::keyPressEvent(QKeyEvent *event){
	if (event->modifiers() & Qt::ControlModifier){
		if (event->key() == Qt::Key_C){
			this->m_p_AnimationListEditor->shortcut_copy();
		}
		if (event->key() == Qt::Key_V){
			this->m_p_AnimationListEditor->shortcut_paste();
		}
		if (event->key() == Qt::Key_A){
			this->m_p_AnimationListEditor->shortcut_selectAll();
		}
	}
	if (event->key() == Qt::Key_Delete){
		this->m_p_AnimationListEditor->shortcut_delete();
	}
}


/*-------------------------------------------------
		���� - ���汾������
*/
void P_ActionPart::local_saveCurIndexData(){
	if (this->m_last_index < 0){ return; }
	if (this->m_last_index > this->local_actionDataList.count()){ return; }

	// > ������
	QJsonObject obj_edit = this->m_FastForm->getJsonObject();
	QJsonObject obj_org = this->local_actionDataList.at(this->m_last_index);
	TTool::_QJsonObject_put_(&obj_org, obj_edit);

	// > ͼƬ����
	QJsonObject obj_org2 = QJsonObject();
	//....

	TTool::_QJsonObject_put_(&obj_org, obj_edit);

	this->local_actionDataList.replace(this->m_last_index, obj_org);
}
/*-------------------------------------------------
		���� - ��ȡ��������
*/
void P_ActionPart::local_loadIndexData(int index){
	if (index < 0){ return; }
	if (index > this->local_actionDataList.count()){ return; }

	// > ������
	QJsonObject obj_data = this->local_actionDataList.at(index);
	this->m_FastForm->setJsonObjectAutoDefault(obj_data);
	qDebug() << obj_data;

	// > ͼƬ����
		int gif_interval = obj_data.value("֡���").toString().toInt();					//֡���
		QString gif_src_file = S_ActionSeqDataContainer::getInstance()->getActionSeqDir();				//��Դ�ļ���
	
		QString gif_src_str = obj_data.value("��Դ-����Ԫ").toString();
		QList<QString> gif_src = TTool::_QJsonArrayString_To_QListQString_(gif_src_str);	//��Դ�ļ���
		QString gif_intervalTank_str = obj_data.value("֡���-��ϸ��").toString();
		QList<int> gif_intervalTank = TTool::_QJsonArrayString_To_QListInt_(gif_intervalTank_str);;		//֡���-��ϸ��

	C_ALEData data = C_ALEData();
	data.setId(index);
	data.setSource(gif_src_file, gif_src);
	data.setInterval(gif_interval, gif_intervalTank);
	this->m_p_AnimationListEditor->setSource(data);
	this->m_p_AnimationListEditor->selectStart();

	this->m_last_index = index;
}

/*-------------------------------------------------
		���� - ��������
*/
void P_ActionPart::setData(QList<QJsonObject> actionData) {
	this->local_actionDataList = actionData;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ȡ������
*/
QList<QJsonObject> P_ActionPart::getData(){
	this->putUiToData();
	return this->local_actionDataList;
}
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void P_ActionPart::putDataToUi() {
	
	// > �����б�
	this->m_table->setSource(this->getNameList());

	// > ��ǰѡ�е�����
	this->local_loadIndexData(this->m_last_index);
}
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_ActionPart::putUiToData() {

	// > ���浱ǰ����
	this->local_saveCurIndexData();

}
