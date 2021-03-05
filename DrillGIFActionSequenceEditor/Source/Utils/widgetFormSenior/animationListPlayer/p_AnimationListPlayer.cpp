#include "stdafx.h"
#include "p_AnimationListPlayer.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		����֡������.cpp
		�汾��		v1.01
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��ģ���ܹ����ƶ���֡���ţ��൱�� ����֡�༭�� ��һ����չճ�ϼ���
					�������� �� �༭�� �Ƕ����ģ������������źţ��༭�����ղ�����
					������ȥ�����༭�������źŴ���ʽ�����Դ�ʹ���������������Ŀؼ���

		�ӹ��ܣ�	-> ����֡
						-> �л�ѡ��źţ�
						-> ��ť������źţ�
						-> �̶�������ͣ / ����ѭ��
						-> ʱ��֡��ʼ��
					-> �ؼ�����
						-> ���� ����֡�༭��

		˵����		> ͨ������ �ź� ��ʵ�ֲ���Ч������Ҫ���ӵ��ӿؼ��Ĳۡ�
					> ʱ��֡��Ҫ��ʼ��

		ʹ�÷�����
				> ��ʼ��
					this->m_p_AnimationListPlayer = new P_AnimationListPlayer(this);
					this->m_p_AnimationListPlayer->setAnimationListEditor(m_p_AnimationListEditor);

					QHBoxLayout* layout = new QHBoxLayout(ui.widget_player);
					ui.widget_player->setLayout(layout);
					layout->setMargin(0);
					layout->addWidget(this->m_p_AnimationListPlayer);

-----==========================================================-----
*/
P_AnimationListPlayer::P_AnimationListPlayer(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������
	this->m_playing = false;					//���ڲ���
	this->m_backRun = false;					//�Ƿ񵹷�
	this->m_timer = new QTimer();				//��ʱ��
	this->m_curFrame = 0;						//��ǰʱ��֡
	this->m_IndexFrame = QList<int>();			//����֡��ʱ��֡��
	this->m_IndexFrameCount = 0;				//��ʱ��֡��
	this->m_timerInterval = 10;					//����֡���
	connect(this->m_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));	

	this->m_iconSrcPath = ":/DrillGIFActionSequenceEditor/Resources/icons";
	this->m_iconPlaying = false;

	this->m_animEditor = nullptr;
	this->m_slotBlock = false;

	//-----------------------------------
	//----��ʼ��ui
	ui.combo->clear();
	ui.combo->addItems(QStringList() << "����һ��" << "���Ŷ���" << "��������" << "�����Ĵ�" << "�������" << "����ѭ��");

	ui.toolButton_jumpToStart->setIcon(QIcon(this->m_iconSrcPath + "/player/Play_Start.png"));
	ui.toolButton_last->setIcon(QIcon(this->m_iconSrcPath + "/player/Play_Last.png"));
	ui.toolButton_play->setIcon(QIcon(this->m_iconSrcPath + "/player/Play_Run.png"));
	ui.toolButton_next->setIcon(QIcon(this->m_iconSrcPath + "/player/Play_Next.png"));
	ui.toolButton_new->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_New.png"));
	ui.toolButton_ExportGIF->setIcon(QIcon(this->m_iconSrcPath + "/menu/ExportGIF.png"));
	ui.toolButton_setting->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Setting.png"));

	//-----------------------------------
	//----�¼���
	connect(ui.toolButton_jumpToStart, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_homing);
	connect(ui.toolButton_last, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_last);
	connect(ui.toolButton_play, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_play);
	connect(ui.toolButton_next, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_next);
	connect(ui.toolButton_new, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_new);
	connect(ui.toolButton_ExportGIF, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_exportGIF);
	connect(ui.toolButton_setting, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_setting);

	ui.toolButton_ExportGIF->setVisible(false);
}

P_AnimationListPlayer::~P_AnimationListPlayer(){
}


/*-------------------------------------------------
		����֡ - ֡ˢ��
*/
void P_AnimationListPlayer::updateFrame(){
	if (this->m_IndexFrameCount == 0){ this->stopFrame(); return; }

	// > �л�ѡ��
	int index = this->m_curFrame % this->m_IndexFrameCount;
	if (this->m_backRun == false){

		// > ���򲥷�
		for (int i = 0; i < this->m_IndexFrame.count(); i++){
			int i_count = this->m_IndexFrame.at(i);
			if (index < i_count){

				// > �źű仯
				emit frameIndexChanged(i);

				break;
			}
			index -= i_count;
		}
	
	}else{

		// > ����
		for (int i = this->m_IndexFrame.count()-1; i >=0; i--){
			int i_count = this->m_IndexFrame.at(i);
			if (index < i_count){

				// > �źű仯
				emit frameIndexChanged(i);

				break;
			}
			index -= i_count;
		}
	}


	// > �̶�������ͣ
	if ( (this->m_backRun == false && index == 0) ||
		 (this->m_backRun == true && index == this->m_IndexFrame.count()-1) ){
		int loop_time = qFloor(this->m_curFrame / this->m_IndexFrameCount);
		
		if (loop_time >= 1 && ui.combo->currentText() == "����һ��"){ this->stopFrame(); return; }
		if (loop_time >= 2 && ui.combo->currentText() == "���Ŷ���"){ this->stopFrame(); return; }
		if (loop_time >= 2 && ui.combo->currentText() == "��������"){ this->stopFrame(); return; }
		if (loop_time >= 3 && ui.combo->currentText() == "��������"){ this->stopFrame(); return; }
		if (loop_time >= 4 && ui.combo->currentText() == "�����Ĵ�"){ this->stopFrame(); return; }
		if (loop_time >= 5 && ui.combo->currentText() == "�������"){ this->stopFrame(); return; }

	}

	// > ֡��+1
	this->m_curFrame += 1;
}
/*-------------------------------------------------
		����֡ - ��ʼ
*/
void P_AnimationListPlayer::startFrame(){
	this->m_timer->start(this->m_timerInterval);
	this->m_curFrame = 0; 
	this->m_playing = true;
	this->updateIcon();
	emit frameButton_started();
}
/*-------------------------------------------------
		����֡ - ��ͣ
*/
void P_AnimationListPlayer::stopFrame(){
	this->m_timer->stop();
	this->m_curFrame = 0;
	this->m_playing = false;
	this->updateIcon();
}
/*-------------------------------------------------
		����֡ - ���ڲ���
*/
bool P_AnimationListPlayer::isPlaying(){
	return this->m_playing;
}
/*-------------------------------------------------
		����֡ - ���ò�������
*/
void P_AnimationListPlayer::setPlayType(QStringList play_type){
	ui.combo->clear();
	ui.combo->addItems(play_type);
}
/*-------------------------------------------------
		����֡ - ����ʱ��֡�б�ÿ������֡ռ�õ�ʱ��֡����
*/
void P_AnimationListPlayer::setPlayFrame(QList<int> indexFrame){
	this->m_IndexFrame = indexFrame;
	this->m_IndexFrameCount = 0;
	for (int i = 0; i < this->m_IndexFrame.count(); i++){
		this->m_IndexFrameCount += this->m_IndexFrame.at(i);
	}
}
/*-------------------------------------------------
		����֡ - ���õ���
*/
void P_AnimationListPlayer::setPlayBackRun(bool backRun){
	this->m_backRun = backRun;
}
/*-------------------------------------------------
		����֡ - ���ò��ż��
*/
void P_AnimationListPlayer::setPlayTimerInterval(int timerInterval){
	this->m_timerInterval = timerInterval;
}



/*-------------------------------------------------
		��ť - ˢ�²���ͼ��
*/
void P_AnimationListPlayer::updateIcon(){
	if (this->m_iconPlaying == this->m_playing){ return; }
	this->m_iconPlaying = this->m_playing;
	
	if (this->m_iconPlaying == true){
		ui.toolButton_play->setIcon(QIcon(this->m_iconSrcPath + "/player/Play_Pause.png"));
	}else{
		ui.toolButton_play->setIcon(QIcon(this->m_iconSrcPath + "/player/Play_Run.png"));
	}
}
/*-------------------------------------------------
		��ť - ������֡
*/
void P_AnimationListPlayer::btn_homing(){
	this->btn_pause();
	emit frameButton_homing();
}
/*-------------------------------------------------
		��ť - ��һ֡
*/
void P_AnimationListPlayer::btn_last(){
	this->btn_pause();
	emit frameButton_last();
}
/*-------------------------------------------------
		��ť - ����
*/
void P_AnimationListPlayer::btn_play(){
	if (this->isPlaying() == true){
		this->btn_pause();	//�����ε��Ϊ��ͣ��
		return;
	}
	this->startFrame();
}
void P_AnimationListPlayer::btn_pause(){
	this->stopFrame();
}
/*-------------------------------------------------
		��ť - ��һ֡
*/
void P_AnimationListPlayer::btn_next(){
	this->btn_pause();
	emit frameButton_next();
}
/*-------------------------------------------------
		��ť - �½�֡
*/
void P_AnimationListPlayer::btn_new(){
	this->btn_pause();
	emit frameButton_new();
}
/*-------------------------------------------------
		��ť - ��������
*/
void P_AnimationListPlayer::btn_setting(){
	this->btn_pause();
	emit frameButton_setting();
}
/*-------------------------------------------------
		��ť - ����GIF������Ҫ�༭��������
*/
void P_AnimationListPlayer::btn_exportGIF(){
	if (this->m_animEditor == nullptr){ return; }
	
	this->m_animEditor->op_exportAll_GIFInAction();
}



/*-------------------------------------------------
		�༭�� - ����
*/
void P_AnimationListPlayer::setAnimationListEditor(P_AnimationListEditor* animEditor){
	this->m_animEditor = animEditor;
	ui.toolButton_ExportGIF->setVisible(true);

	if (this->m_animEditor->getUnit() == C_ALEData::SecondUnit){
		this->m_timerInterval = 10;
	}
	if (this->m_animEditor->getUnit() == C_ALEData::FrameUnit){
		this->m_timerInterval = 16;
	}

	connect(this, &P_AnimationListPlayer::frameIndexChanged, this->m_animEditor, &P_AnimationListEditor::selectIndex);					//������ ���� �ź�����
	connect(this, &P_AnimationListPlayer::frameButton_homing, this->m_animEditor, &P_AnimationListEditor::selectStart);					//������ ������֡ �ź�����
	connect(this, &P_AnimationListPlayer::frameButton_last, this->m_animEditor, &P_AnimationListEditor::selectLast);					//������ ��һ֡ �ź�����
	connect(this, &P_AnimationListPlayer::frameButton_next, this->m_animEditor, &P_AnimationListEditor::selectNext);					//������ ��һ֡ �ź�����
	connect(this, &P_AnimationListPlayer::frameButton_new, this->m_animEditor, &P_AnimationListEditor::op_appendInAction);				//������ �½� �ź�����
	connect(this, &P_AnimationListPlayer::frameButton_setting, this->m_animEditor, &P_AnimationListEditor::openWindow_setConfigParam);	//������ ui���� �ź�����

	connect(this, &P_AnimationListPlayer::frameButton_started, this, &P_AnimationListPlayer::animEditor_started);				//������ ��ʼ���� �ź�����
}
/*-------------------------------------------------
		�༭�� - ��ȡ
*/
P_AnimationListEditor* P_AnimationListPlayer::getAnimationListEditor(){
	return this->m_animEditor;
}
/*-------------------------------------------------
		�༭�� - ��ʼ����
*/
void P_AnimationListPlayer::animEditor_started(){
	if (this->m_animEditor == nullptr){ return; }
	this->setPlayFrame(this->m_animEditor->getSource().getData_IntervalTank());		//�ڿ�ʼ���ź���������ʱ��֡����Ϊʱ��֡��䣩
}
