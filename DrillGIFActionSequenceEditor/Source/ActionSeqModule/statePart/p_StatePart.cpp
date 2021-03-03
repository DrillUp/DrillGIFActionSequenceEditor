#include "stdafx.h"
#include "p_StatePart.h"

#include "../actionSeqData/s_ActionSeqDataDefiner.h"
#include "../actionSeqData/s_ActionSeqDataContainer.h"
#include "Source/ProjectModule/s_ProjectManager.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		״̬Ԫ��.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ò����ṩ״̬Ԫ�༭���ܡ�
					���ù��ܿ���һ���󼯺���������ܽ�֯���ӣ���ע��ע�Ͳ��֡�
		
		Ŀ�꣺		->�ؼ�
						->��ѡ���
						->���ٱ�
					->�ؼ�������֡��
						->����֡�༭��
						->����֡������
						->ͼƬ�鿴��
					->��ݼ�
						->����
						->ճ��
						->���
					->��������
						->�б�
						->����
						->���ڽ���

		ʹ�÷�����
				>��ʼ��

-----==========================================================-----
*/
P_StatePart::P_StatePart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������
	this->m_last_index = -1;
	this->m_slotBlock_source = false;

	//-----------------------------------
	//----��ʼ��ui

	// > �༭���
	this->m_table = new P_RadioTable(ui.tableWidget);
	C_RaTConfig rat_config = C_RaTConfig();
	rat_config.zeroFillCount = 2;
	rat_config.rowHeight = 22;
	this->m_table->setConfigParam(rat_config);			//�̶�����
	this->m_table->setItemOuterControlEnabled(true);	//�����Ҽ��˵�

	// > ���ٱ�
	C_FastClass c_class = S_ActionSeqDataDefiner::getInstance()->getTable_State();
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
	this->m_p_AnimationListPlayer->setPlayType(QStringList() << "����ѭ��");
	QHBoxLayout* layout = new QHBoxLayout(ui.groupBox_animPlayer);
	ui.groupBox_animPlayer->setLayout(layout);
	layout->setMargin(3);
	layout->addWidget(this->m_p_AnimationListPlayer);

	// > ͼƬ�鿴��
	this->m_p_AnimPictureViewer = new P_AnimPictureViewer(ui.widget_view);
	this->m_p_AnimPictureViewer->rebuildUI();

	//-----------------------------------
	//----�¼���
	connect(this->m_table, &P_RadioTable::currentIndexChanged, this, &P_StatePart::currentIndexChanged);
	connect(this->m_table, &P_RadioTable::menuPasteItemTriggered, this, &P_StatePart::shortcut_pasteData);
	connect(this->m_table, &P_RadioTable::menuCopyItemTriggered, this, &P_StatePart::shortcut_copyData);
	connect(this->m_table, &P_RadioTable::menuClearItemTriggered, this, &P_StatePart::shortcut_clearData);
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::selectedIndexChanged_Multi, this, &P_StatePart::tableChanged_Multi);

	// > ���仯��
	connect(this->m_FastForm->getQLineEditByName("״̬Ԫ����"), &QLineEdit::textEdited, this->m_table, &P_RadioTable::modifyText_Selected);
	connect(this->m_FastForm->getQCheckBoxByName("�Ƿ񵹷�"), &QCheckBox::toggled, this->m_p_AnimationListPlayer, &P_AnimationListPlayer::setPlayBackRun);

	// > ͼƬ�鿴�� - ����֡�л�
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::currentIndexChanged, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::setAnimFrame);
	// > ͼƬ�鿴�� - ������Դ�л�
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::animBitmapChanged, this, &P_StatePart::bitmapChanged);
	// > ͼƬ�鿴�� - ����
	connect(ui.toolButton_zoom_in, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomIn);
	connect(ui.toolButton_zoom_out, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomOut);
	connect(ui.toolButton_zoom_regular, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomReset);
	connect(this->m_p_AnimPictureViewer, &P_AnimPictureViewer::scaleChanged, this, &P_StatePart::zoomValueChanged);

}

P_StatePart::~P_StatePart(){
}

/*-------------------------------------------------
		����֡ - ѡ��仯
*/
void P_StatePart::tableChanged_Multi(QList<int> index_list){
	//�����޲�����
}
/*-------------------------------------------------
		����֡ - ��Դ�л�
*/
void P_StatePart::bitmapChanged(){
	C_ALEData data = this->m_p_AnimationListEditor->getSource();
	this->m_p_AnimPictureViewer->setSource(data.getAllFile());
}
/*-------------------------------------------------
		����֡ - ���ű����л�
*/
void P_StatePart::zoomValueChanged(double value){
	ui.label_zoomValue->setText( QString::number(value * 100)+"%" );
}

/*-------------------------------------------------
		�ؼ� - ��ȡ״̬Ԫ����
*/
QStringList P_StatePart::getNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->m_stateDataList.count(); i++){
		result.append(this->m_stateDataList.at(i).value("״̬Ԫ����").toString());
	}
	return result;
}

/*-------------------------------------------------
		�ؼ� - ״̬Ԫ�л�
*/
void P_StatePart::currentIndexChanged(int index){
	if (this->m_slotBlock_source == true){ return; }

	// > �ɵ����ݴ洢
	this->local_saveCurIndexData();

	// > �����µ�����
	this->local_loadIndexData(index);
}



/*-------------------------------------------------
		��ݼ� - �¼�
*/
void P_StatePart::keyPressEvent(QKeyEvent *event){
	if (event->modifiers() & Qt::ControlModifier){
		if (event->key() == Qt::Key_C){
			this->m_p_AnimationListEditor->shortcut_copy();
			this->shortcut_copyData();
		}
		if (event->key() == Qt::Key_V){
			this->m_p_AnimationListEditor->shortcut_paste();
			this->shortcut_pasteData();
		}
		if (event->key() == Qt::Key_A){
			this->m_p_AnimationListEditor->shortcut_selectAll();
		}
	}
	if (event->key() == Qt::Key_Delete){
		this->m_p_AnimationListEditor->shortcut_delete();
		this->shortcut_clearData();
	}
}
/*-------------------------------------------------
		���� - �滻
*/
void P_StatePart::op_replace(int index, QJsonObject state){
	if (index < 0){ return; }
	if (index >= this->m_table->count()){ return; }
	if (state.isEmpty()){ return; }

	// > �༭���
	S_ProjectManager::getInstance()->setDirty();

	// > ִ���滻
	this->m_stateDataList.replace(index, state);
	this->local_loadIndexData(index);

	// > ���±�������
	this->m_table->modifyText_Selected(this->m_FastForm->getQLineEditByName("״̬Ԫ����")->text());
}
/*-------------------------------------------------
		���� - ���
*/
void P_StatePart::op_clear(int index){
	if (index < 0){ return; }
	if (index >= this->m_table->count()){ return; }

	// > �༭���
	S_ProjectManager::getInstance()->setDirty();

	// > ִ���滻
	this->m_stateDataList.replace(index, QJsonObject());
	this->local_loadIndexData(index);

	// > ���±�������
	this->m_table->modifyText_Selected(this->m_FastForm->getQLineEditByName("״̬Ԫ����")->text());
}
/*-------------------------------------------------
		��ݼ� - ����
*/
void P_StatePart::shortcut_copyData(){
	if (ui.tableWidget->hasFocus() == false){ return; }
	if (this->m_last_index == -1 ){ return; }
	this->m_copyed_data = this->m_stateDataList.at(m_last_index);
	this->m_table->setItemOuterControl_PasteActive(true);		//����ճ��
}
/*-------------------------------------------------
		��ݼ� - ճ��
*/
void P_StatePart::shortcut_pasteData(){
	if (ui.tableWidget->hasFocus() == false){ return; }
	if (this->m_last_index == -1){ return; }
	this->op_replace(this->m_last_index, this->m_copyed_data);
}
/*-------------------------------------------------
		��ݼ� - ���
*/
void P_StatePart::shortcut_clearData(){
	if (ui.tableWidget->hasFocus() == false){ return; }
	if (this->m_last_index == -1){ return; }
	this->op_clear(this->m_last_index);
}


/*-------------------------------------------------
		���� - ���汾������
*/
void P_StatePart::local_saveCurIndexData(){
	if (this->m_last_index < 0){ return; }
	if (this->m_last_index >= this->m_stateDataList.count()){ return; }

	// > ������
	QJsonObject obj_edit = this->m_FastForm->getJsonObject();
	QJsonObject obj_org = this->m_stateDataList.at(this->m_last_index);
	TTool::_QJsonObject_put_(&obj_org, obj_edit);

	// > ͼƬ����
		C_ALEData data = this->m_p_AnimationListEditor->getSource();
		QJsonObject obj_edit2 = QJsonObject();
		obj_edit2.insert("֡���", QString::number( data.getData_IntervalDefault() ));
		//����Դ�ļ��в���Ҫ��
		QList<QString> gif_src = QList<QString>();
		QList<QFileInfo> info_list = data.getAllFile();
		for (int i = 0; i < info_list.count(); i++){
			gif_src.append(info_list.at(i).completeBaseName());
		}
		obj_edit2.insert("��Դ-״̬Ԫ", TTool::_QListQString_To_QJsonArrayString_(gif_src));
		QList<int> gif_intervalTank = data.getData_IntervalTank();
		QList<QString> gif_intervalTank_strList = TTool::_QList_IntToQString_(gif_intervalTank);
		obj_edit2.insert("֡���-��ϸ��", TTool::_QListQString_To_QJsonArrayString_(gif_intervalTank_strList));
		//qDebug() << obj_edit2;
	TTool::_QJsonObject_put_(&obj_org, obj_edit2);

	// > �༭���
	S_ProjectManager::getInstance()->setDirty();

	this->m_stateDataList.replace(this->m_last_index, obj_org);
}
/*-------------------------------------------------
		���� - ��ȡ��������
*/
void P_StatePart::local_loadIndexData(int index){
	if (index < 0){ return; }
	if (index >= this->m_stateDataList.count()){ return; }

	// > ������
	QJsonObject obj_data = this->m_stateDataList.at(index);
	this->m_FastForm->setJsonObjectAutoDefault(obj_data);
	//qDebug() << obj_data;

	// > ͼƬ����
		int gif_interval = obj_data.value("֡���").toString().toInt();								//֡���
		QString gif_src_file = S_ActionSeqDataContainer::getInstance()->getActionSeqDir();			//��Դ�ļ���
	
		QString gif_src_str = obj_data.value("��Դ-״̬Ԫ").toString();
		QList<QString> gif_src = TTool::_QJsonArrayString_To_QListQString_(gif_src_str);			//��Դ�ļ���
		QString gif_intervalTank_str = obj_data.value("֡���-��ϸ��").toString();
		QList<QString> gif_intervalTank_strList = TTool::_QJsonArrayString_To_QListQString_(gif_intervalTank_str);	//֡���-��ϸ��
		QList<int> gif_intervalTank = TTool::_QList_QStringToInt_(gif_intervalTank_strList);

	C_ALEData data = C_ALEData();
	data.setData_Id(index);
	data.setSource(gif_src_file, gif_src);
	data.setInterval(gif_interval, gif_intervalTank);
	this->m_p_AnimationListEditor->setSource(data);
	this->m_p_AnimationListEditor->selectStart();

	this->m_last_index = index;
}



/*-------------------------------------------------
		���� - ��������
*/
void P_StatePart::setData(QList<QJsonObject> state) {
	this->m_slotBlock_source = true;
	this->m_stateDataList = state;
	this->m_p_AnimationListPlayer->stopFrame();		//����������ʱ����ͣ���ţ�
	this->putDataToUi();
	this->m_slotBlock_source = false;
	this->m_table->selectStart();
}
/*-------------------------------------------------
		���� - ȡ������
*/
QList<QJsonObject> P_StatePart::getData(){
	this->putUiToData();

	// > У��
	//...
	
	return this->m_stateDataList;
}
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void P_StatePart::putDataToUi() {

	// > �����б�
	this->m_table->setSource(this->getNameList());

	// > ��ǰѡ�е�����
	this->local_loadIndexData(this->m_last_index);
}
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_StatePart::putUiToData() {

	// > ���浱ǰ����
	this->local_saveCurIndexData();

}
