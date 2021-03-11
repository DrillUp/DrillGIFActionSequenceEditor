#include "stdafx.h"
#include "p_PlayingPart.h"

#include "defaultStateGroup/w_DefaultStateGroupEdit.h"
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
	this->m_iconSrcPath = ":/DrillGIFActionSequenceEditor/Resources/icons";
	this->m_playing = false;					//���ڲ���
	this->m_timer = new QTimer();				//��ʱ��
	this->m_curFrame = 0;						//��ǰʱ��֡
	this->m_IndexFrame = QList<int>();			//����֡��ʱ��֡��
	this->m_IndexFrameCount = 0;				//��ʱ��֡��
	this->m_timerInterval = 10;					//����֡���
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
	connect(ui.toolButton_editGroup, &QToolButton::clicked, this, &P_PlayingPart::editDefaultStateGroup);

}

P_PlayingPart::~P_PlayingPart(){
}


/*-------------------------------------------------
		����֡ - ֡ˢ��
*/
void P_PlayingPart::updateFrame(){
	if (this->m_IndexFrameCount == 0){ this->stopFrame(); return; }

	// > �л�ѡ��
	int index = this->m_curFrame % this->m_IndexFrameCount;

	// > ����
	for (int i = 0; i < this->m_IndexFrame.count(); i++){
		int i_count = this->m_IndexFrame.at(i);
		if (index < i_count){

			// > ͼƬ�仯
			//...

			break;
		}
		index -= i_count;
	}

	// > ֡��+1
	this->m_curFrame += 1;
}
/*-------------------------------------------------
		����֡ - ��ʼ
*/
void P_PlayingPart::startFrame(){

	// > ������ʱ��
	this->m_timer->start(this->m_timerInterval);
	this->m_curFrame = 0; 
	this->m_playing = true;

	// > ���ݳ�ʼ��
	this->m_IndexFrameCount = 10;
	this->refreshSource();

	// > ui�仯
	ui.widget_viewFrame->setEnabled(true);
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
	this->m_curFrame = 0;
	this->m_playing = false;

	// > ui�仯
	ui.widget_viewFrame->setEnabled(false);
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
	QMessageBox::information(this, "��ʾ", "��ӳ�����ܻ�δ��ɣ���ť���û��Ч����", QMessageBox::Yes);
	if (this->isPlaying() == true){
		this->stopFrame();
		return;
	}
	this->startFrame();
}
/*-------------------------------------------------
		��ӳ�� - ˢ�²���ͼ��
*/
void P_PlayingPart::updateIcon(){
	
	if (this->m_playing == true){
		ui.toolButton_open->setIcon(QIcon(this->m_iconSrcPath + "/player/Play_Pause.png"));
	}else{
		ui.toolButton_open->setIcon(QIcon(this->m_iconSrcPath + "/player/Play_Run.png"));
	}
}


/*-------------------------------------------------
		�ؼ� - �༭״̬Ԫ����
*/
void P_PlayingPart::editDefaultStateGroup(){
	QStringList state_list = this->getStateNameListWithoutEmpty();
	if (state_list.count() == 0){
		QMessageBox::information(this, "��ʾ", "����Ҫ��������һ�� ״̬Ԫ ���ܱ༭��", QMessageBox::Yes);
		return;
	}

	W_DefaultStateGroupEdit d(this);
	d.setData(state_list, this->local_defaultStateList);
	if (d.exec() == QDialog::Accepted){
		this->local_defaultStateList = d.getData();
		this->putDataToUi();
	}

}
/*-------------------------------------------------
		�ؼ� - ��ȡ״̬Ԫ����
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
/*-------------------------------------------------
		�ؼ� - ��ȡ����Ԫ����
*/
QStringList P_PlayingPart::getActionNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_actionDataList.count(); i++){
		result.append(this->local_actionDataList.at(i).value("����Ԫ����").toString());
	}
	return result;
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
		state_tank[i] = Drill_COAS_Init::drill_COAS_initState(this->local_stateDataList.at(i));
	}
	data["state_tank"] = state_tank;
	QJsonArray act_tank = QJsonArray();
	for (int i = 0; i < this->local_actionDataList.count(); i++){
		act_tank[i] = Drill_COAS_Init::drill_COAS_initState(this->local_actionDataList.at(i));
	}
	data["act_tank"] = act_tank;

	this->m_COAS_data = Drill_COAS_Data(data);
}
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_PlayingPart::putUiToData() {
	

}
