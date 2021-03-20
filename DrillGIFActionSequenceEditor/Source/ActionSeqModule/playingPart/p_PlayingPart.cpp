#include "stdafx.h"
#include "p_PlayingPart.h"

#include "defaultStateGroup/w_DefaultStateGroupEdit.h"

#include "Source/ActionSeqModule/actionSeqData/s_ActionSeqDataContainer.h"
#include "Source/ProjectModule/s_ProjectManager.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		��ӳ��.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ò����ṩ����Ԫ�༭���ܡ�
		
		Ŀ�꣺		->�ؼ�
						->��ѡ��񣨶���Ԫ��
						->��ѡ���״̬Ԫ��
						->���ٱ�
					->�ؼ�������֡��
						->����֡������
						->ͼƬ�鿴��
					->��������
						->���ڽ���

		ʹ�÷�����
				>��ʼ��

-----==========================================================-----
*/
P_PlayingPart::P_PlayingPart(P_StatePart* state_part, P_ActionPart* action_part, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������
	this->m_statePart = state_part;
	this->m_actionPart = action_part;

	// > ����ֵ
	this->m_playing = false;					//���ڲ���
	this->m_timer = new QTimer();				//��ʱ��
	connect(this->m_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));

	//-----------------------------------
	//----��ʼ��ui

	// > �༭���
	this->m_table_action = new P_RadioTable(ui.tableWidget_action);
	this->m_table_state = new P_RadioTable(ui.tableWidget_state);
	C_RaTConfig rat_config = C_RaTConfig();
	rat_config.zeroFillCount = 2;
	rat_config.rowHeight = 22;
	this->m_table_action->setConfigParam(rat_config);	//�̶�����
	this->m_table_action->setItemOuterControlEnabled(false);
	C_RaTConfig rat_config2 = C_RaTConfig();
	rat_config2.zeroFillCount = 2;
	rat_config2.rowHeight = 22;
	rat_config2.isMultiSelect = true;
	this->m_table_state->setConfigParam(rat_config2);
	this->m_table_state->setItemOuterControlEnabled(false);

	// > ͼƬ�鿴��
	this->m_p_AnimPictureViewer = new P_AnimPictureViewer(ui.widget_view);
	this->m_p_AnimPictureViewer->rebuildUI();


	//-----------------------------------
	//----�¼���
	connect(ui.toolButton_open, &QToolButton::clicked, this, &P_PlayingPart::btn_play);
	connect(ui.toolButton_playDefault, &QToolButton::clicked, this, &P_PlayingPart::btn_playDefault);
	connect(ui.toolButton_playState, &QToolButton::clicked, this, &P_PlayingPart::btn_playState);
	connect(ui.toolButton_playAction, &QToolButton::clicked, this, &P_PlayingPart::btn_playAction);
	connect(ui.toolButton_editGroup, &QToolButton::clicked, this, &P_PlayingPart::editDefaultStateGroup);

	// > ͼƬ�鿴�� - ����
	connect(ui.toolButton_zoom_in, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomIn);
	connect(ui.toolButton_zoom_out, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomOut);
	connect(ui.toolButton_zoom_regular, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomReset);
	connect(this->m_p_AnimPictureViewer, &P_AnimPictureViewer::scaleChanged, this, &P_PlayingPart::zoomValueChanged);

}

P_PlayingPart::~P_PlayingPart(){
}

/*-------------------------------------------------
		�ؼ� - �༭״̬Ԫ����
*/
void P_PlayingPart::editDefaultStateGroup(){
	// > δ����ʱ��ˢ����Դ
	if (this->isPlaying() == false){
		this->refreshSource();
	}

	// > ��Դ���
	QStringList state_list = this->getStateNameListWithoutEmpty();
	if (state_list.count() == 0){
		QMessageBox::information(this, "��ʾ", "����Ҫ��������һ�� ״̬Ԫ ���ܱ༭��", QMessageBox::Yes);
		return;
	}

	// > ����
	W_DefaultStateGroupEdit d(this);
	d.setData(state_list, this->local_defaultStateList);
	if (d.exec() == QDialog::Accepted){
		this->local_defaultStateList = d.getData();
		this->putDataToUi();
	}

}
/*-------------------------------------------------
		�ؼ� - �����ļ�����ȡ��Դ�ļ�
*/
QFileInfo P_PlayingPart::getSrcFileByName(QString file_name){
	return QFileInfo(S_ActionSeqDataContainer::getInstance()->getActionSeqDir() + "/" + this->m_COAS_data._drill_bitmapName + ".png");
}
/*-------------------------------------------------
		�ؼ� - ��ȡ����
*/
QStringList P_PlayingPart::getStateNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_stateDataList.count(); i++){
		result.append(this->local_stateDataList.at(i).value("״̬Ԫ����").toString());
	}
	return result;
}
QStringList P_PlayingPart::getStateNameListWithoutEmpty(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_stateDataList.count(); i++){
		QString name = this->local_stateDataList.at(i).value("״̬Ԫ����").toString();
		if (name == ""){ continue; }
		result.append(name);
	}
	return result;
}
QStringList P_PlayingPart::getActionNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_actionDataList.count(); i++){
		result.append(this->local_actionDataList.at(i).value("����Ԫ����").toString());
	}
	return result;
}
/*-------------------------------------------------
		�ؼ� - ��ȡȫ�������ļ�����ȥ�أ�
*/
QStringList P_PlayingPart::getRelatFileNameList(){
	QStringList result = QStringList();

	// > ����Ԫ��Դ
	for (int i = 0; i < this->local_actionDataList.count(); i++){
		QJsonObject obj_action = this->local_actionDataList.at(i);
		QString str_src = obj_action.value("��Դ-����Ԫ").toString();
		QStringList src_list = TTool::_JSON_parse_To_QListQString_(str_src);
		result.append(src_list);
	}

	// > ״̬Ԫ��Դ
	for (int i = 0; i < this->local_stateDataList.count(); i++){
		QJsonObject obj_state = this->local_stateDataList.at(i);
		QString str_src = obj_state.value("��Դ-״̬Ԫ").toString();
		QStringList src_list = TTool::_JSON_parse_To_QListQString_(str_src);
		result.append(src_list);
	}

	// > ȥ��
	result = result.toSet().toList();

	return result;
}
/*-------------------------------------------------
		�ؼ� - ���ű����л�
*/
void P_PlayingPart::zoomValueChanged(double value){
	ui.label_zoomValue->setText( QString::number(value * 100)+"%" );
}


/*-------------------------------------------------
		����֡ - ֡ˢ��
*/
void P_PlayingPart::updateFrame(){
	if (this->m_playing == false){ return; }

	// > ����ˢ��
	this->m_COAS_data.update();

	// > ����ͼƬ
	this->m_p_AnimPictureViewer->setAnimFile(this->getSrcFileByName(this->m_COAS_data._drill_bitmapName));
	
	QString cur_state = this->m_COAS_data._drill_state_curCom;
	if (this->m_COAS_data.drill_COAS_isPlayingAct()){
		ui.label_playingName->setText(QString("����Ԫ��")+this->m_COAS_data._drill_act_curCom+"��");
	}else{
		if (cur_state != ""){		//��cur_stateΪ��ʱ����仯��ԴͼƬ��
			ui.label_playingName->setText(QString("״̬Ԫ��") + cur_state + "��");
		}
	}
	if (cur_state != ""){ ui.label_playingState->setText(cur_state); }
	ui.label_playingAction->setText(this->m_COAS_data._drill_act_curCom);
	ui.label_stateSeq->setText(this->m_COAS_data._drill_state_curSeq.join(","));

}
/*-------------------------------------------------
		����֡ - ��ʼ
*/
void P_PlayingPart::startFrame(){

	// > ������ʱ��
	this->m_timer->start(16);		//�����ڲ�������gifģʽ���������ﲥ��֡�ٶȹ̶���
	this->m_playing = true;

	// > ���ݳ�ʼ��
	this->refreshSource();
	QStringList src_name = this->getRelatFileNameList();
	QList<QFileInfo> scr_infoList = QList<QFileInfo>();
	for (int i = 0; i < src_name.count(); i++){
		scr_infoList.append(this->getSrcFileByName(src_name.at(i)));
	}
	this->m_p_AnimPictureViewer->setSource(scr_infoList);

	// > ui�仯
	ui.groupBox_seq->setEnabled(true);
	ui.widget_operateFrame->setEnabled(true);
	ui.toolButton_playDefault->setEnabled(true);
	this->updateIcon();
	emit playStarted();
}
/*-------------------------------------------------
		����֡ - ��ͣ
*/
void P_PlayingPart::stopFrame(){
	if (this->isPlaying() == false){ return; }

	// > �رռ�ʱ��
	this->m_timer->stop();
	this->m_playing = false;

	// > ui�仯
	ui.groupBox_seq->setEnabled(false);
	ui.widget_operateFrame->setEnabled(false);
	ui.toolButton_playDefault->setEnabled(false);
	this->updateIcon();
}
/*-------------------------------------------------
		����֡ - ���ڲ���
*/
bool P_PlayingPart::isPlaying(){
	return this->m_playing;
}

/*-------------------------------------------------
		��ӳ�� - ����
*/
void P_PlayingPart::btn_play(){
	if (this->isPlaying() == true){
		this->stopFrame();
		return;
	}
	this->startFrame();
}
/*-------------------------------------------------
		��ť - ����Ĭ��״̬Ԫ
*/
void P_PlayingPart::btn_playDefault(){
	this->m_COAS_data.drill_COAS_setSequence(this->local_defaultStateList);
}
/*-------------------------------------------------
		��ť - ����״̬Ԫ
*/
void P_PlayingPart::btn_playState(){
	QStringList state_list = this->m_table_state->getSelectedText_Multi();
	this->m_COAS_data.drill_COAS_setSequence(state_list);
}
/*-------------------------------------------------
		��ť - ���붯��Ԫ
*/
void P_PlayingPart::btn_playAction(){
	QString action_name = this->m_table_action->getSelectedText();
	this->m_COAS_data.drill_COAS_setAct(action_name);
}
/*-------------------------------------------------
		��ӳ�� - ˢ�²���ͼ��
*/
void P_PlayingPart::updateIcon(){
	
	// > ��������ʱ
	if (this->m_playing == true){
		ui.toolButton_open->setIcon(QIcon(QRC_IconSrcPath + "/player/Play_Pause.png"));
		ui.widget_playingPart->setStyleSheet("#widget_playingPart{ background-color: rgb(255, 255, 255); }");
	
	// > ��ͣ����ʱ
	}else{
		ui.toolButton_open->setIcon(QIcon(QRC_IconSrcPath + "/player/Play_Run.png"));
		ui.widget_playingPart->setStyleSheet("#widget_playingPart{ background-color: rgb(242, 242, 242); }");
		ui.label_playingAction->setText("");
		ui.label_playingName->setText("");
		ui.label_playingState->setText("");
		ui.label_stateSeq->setText("");
	}
}



/*-------------------------------------------------
		���� - ��������
*/
void P_PlayingPart::refreshSource(){
	this->local_stateDataList = this->m_statePart->getData();
	this->local_actionDataList = this->m_actionPart->getData();
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ��������
*/
void P_PlayingPart::setDefaultStateData(QStringList defaultStateList) {
	this->local_defaultStateList = defaultStateList;
}
/*-------------------------------------------------
		���� - ȡ������
*/
QStringList P_PlayingPart::getDefaultStateData(){
	this->putUiToData();

	// > У��
	//...
	
	return this->local_defaultStateList;
}
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void P_PlayingPart::putDataToUi() {

	// > ���ѡ��
	this->m_table_action->setSource(this->getActionNameList());
	this->m_table_state->setSource(this->getStateNameList());
	
	// > ״̬Ԫ
	QString text = "";
	for (int i = 0; i < this->local_defaultStateList.count(); i++){
		text = text + "�� " + this->local_defaultStateList.at(i);
		if (i < this->local_defaultStateList.count()-1){
			text = text + "\n";
		}
	}
	if (this->local_defaultStateList.count() == 0){ text = "��"; }
	ui.label_defaultState->setText(text);

	// > �����������У�ֱ�Ӹ��� Ĭ�ϼ���+״̬Ԫ�б�+����Ԫ�б� ���ö������У�
	QJsonObject data = QJsonObject();
	data["state_default_randomSeq"] = TTool::_QJsonArray_QStringToA_(this->local_defaultStateList);
	QJsonArray state_tank = QJsonArray();
	for (int i = 0; i < this->local_stateDataList.count(); i++){
		state_tank.append(Drill_COAS_Init::drill_COAS_initState(this->local_stateDataList.at(i)));
	}
	data["state_tank"] = state_tank;
	QJsonArray act_tank = QJsonArray();
	for (int i = 0; i < this->local_actionDataList.count(); i++){
		act_tank.append(Drill_COAS_Init::drill_COAS_initAct(this->local_actionDataList.at(i)));
	}
	data["act_tank"] = act_tank;

	// > �������к��ĳ�ʼ��
	this->m_COAS_data = Drill_COAS_Data(data);
	this->m_COAS_data.update();
	this->m_p_AnimPictureViewer->clearSource();
	this->m_p_AnimPictureViewer->setAnimFile(this->getSrcFileByName(this->m_COAS_data._drill_bitmapName));
}
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_PlayingPart::putUiToData() {
	

}
