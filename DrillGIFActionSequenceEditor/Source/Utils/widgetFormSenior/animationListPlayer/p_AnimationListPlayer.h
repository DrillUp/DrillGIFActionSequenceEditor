#ifndef P_AnimationListPlayer_H
#define P_AnimationListPlayer_H

#include <QtWidgets>
#include "ui_P_AnimationListPlayer.h"

#include "Source/Utils/widgetFormSenior/animationListEditor/p_AnimationListEditor.h"

/*
-----==========================================================-----
		�ࣺ		����֡������.h
		�汾��		v1.01
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��ģ���ܹ����ƶ���֡���ţ��൱�� ����֡�༭�� ��һ����չճ�ϼ���
					�������� �� �༭�� �Ƕ����ģ������������źţ��༭�����ղ�����
					������ȥ�����༭�������źŴ���ʽ�����Դ�ʹ���������������Ŀؼ���
					����ϸ��cpp��
-----==========================================================-----
*/
class P_AnimationListPlayer : public QWidget
{
	Q_OBJECT

	public:
		P_AnimationListPlayer(QWidget *parent = 0);
		~P_AnimationListPlayer();
		
	//-----------------------------------
	//----����֡
	private:
		bool m_playing;					//���ڲ���
		bool m_backRun;					//�Ƿ񵹷�
		QTimer* m_timer;				//��ʱ��
		int m_curFrame;					//��ǰʱ��֡
		QList<int> m_IndexFrame;		//����֡��ʱ��֡��
		int m_IndexFrameCount;			//��ʱ��֡��

	public:
										//����֡ - ��ʼ
		void startFrame();
										//����֡ - ��ͣ
		void stopFrame();
										//����֡ - ���ڲ���
		bool isPlaying();
	public slots:
										//����֡ - ���ò�������
		void setPlayType(QStringList play_type);
										//����֡ - ����ʱ��֡�б�ÿ������֡ռ�õ�ʱ��֡����
		void setPlayFrame(QList<int> indexFrame);
										//����֡ - ���õ���
		void setPlayBackRun(bool backRun);

	private slots:
										//����֡ - ֡ˢ��
		void updateFrame();
		
	//-----------------------------------
	//----���⽻��
	signals:
										//���⽻�� - ͼƬ�л����źţ�
		void frameIndexChanged(int index);
										//���⽻�� - ������֡���źţ�
		void frameButton_homing();
										//���⽻�� - ��һ֡���źţ�
		void frameButton_last();
										//���⽻�� - ��ʼ���ţ��źţ�
		void frameButton_started();
										//���⽻�� - ��һ֡���źţ�
		void frameButton_next();
										//���⽻�� - �½�֡���źţ�
		void frameButton_new();
										//���⽻�� - �������ã��źţ�
		void frameButton_setting();
		
	//-----------------------------------
	//----��ť
	private:
		QString m_iconSrcPath;
		bool m_iconPlaying;
	public:
										//��ť - ������֡
		void btn_homing();
										//��ť - ��һ֡
		void btn_last();
										//��ť - ����
		void btn_play();
		void btn_pause();
										//��ť - ��һ֡
		void btn_next();
										//��ť - �½�֡
		void btn_new();
										//��ť - ����GIF
		void btn_exportGIF();
										//��ť - ��������
		void btn_setting();
	private:
										//��ť - ˢ�²���ͼ��
		void updateIcon();

	//-----------------------------------
	//----�༭��
	private:
		P_AnimationListEditor* m_animEditor;
	public:
										//�༭�� - ����
		void setAnimationListEditor(P_AnimationListEditor* animEditor);
										//�༭�� - ��ȡ
		P_AnimationListEditor* getAnimationListEditor();
	public slots:
										//�༭�� - ��ʼ����
		void animEditor_started();


	//-----------------------------------
	//----����
	public:
		bool m_slotBlock;
	private:
		Ui::P_AnimationListPlayer ui;

};

#endif // W_ImportMeshDialog_H
