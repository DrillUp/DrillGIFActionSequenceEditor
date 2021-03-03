#ifndef DRILLGIFACTIONSEQUENCEEDITOR_H
#define DRILLGIFACTIONSEQUENCEEDITOR_H

#include <QtWidgets/QDialog>
#include "ui_drillgifactionsequenceeditor.h"

/*
-----==========================================================-----
		�ࣺ		������.h
		���ߣ�		drill_up
		����ģ�飺	������ģ��
		���ܣ�		��������������ƴ��ڡ�
					����ϸ��cpp��
-----==========================================================-----
*/
class P_ActionSeqPart;
class W_RmmvOperateBoard;
class DrillGIFActionSequenceEditor : public QDialog
{
	Q_OBJECT

	public:
		DrillGIFActionSequenceEditor(QWidget *parent = 0);
		~DrillGIFActionSequenceEditor();
		static DrillGIFActionSequenceEditor* cur_instance;		//����
		static DrillGIFActionSequenceEditor* getInstance();		//��������ȡ�Լ�
		void _init();											//��ʼ��
		
	//-----------------------------------
	//----�ؼ���rmmv�������ݣ�
	public:
		W_RmmvOperateBoard* m_w_RmmvOperateBoard;
	public:
								//�ؼ� - ��rmmv��������
		void openWindowRmmvInteractive();
	public slots:
								//�ؼ� - rmmv���������Ѷ�ȡ
		void rmmvInteractiveDataLoaded();

	//-----------------------------------
	//----�ؼ����������н������ݣ�
	public:
		P_ActionSeqPart* m_p_ActionSeqPart;
	public slots:
								//�ؼ� - ���������Ѷ�ȡ
		void actionSeqDataLoaded();
								//�ؼ� - �������������ؽ�
		void rebuildActionSeqData();
								//�ؼ� - �½���Ŀ
		void newProject();
								//�ؼ� - ����Ŀ
		void openProject();
								//�ؼ� - ������Ŀ
		void saveProject();
								//�ؼ� - ���Ϊ��Ŀ
		void saveAsProject();
								//�ؼ� - �޸��ļ�����
		void changeWindowTitle(QString title);
								//�ؼ� - �û��ֲ�
		void openUserManual();
								//�ؼ� - ����...
		void openAbout();

	//-----------------------------------
	//----�¼�
	private:
		QSize m_last_uiSize;
	public:
								//�¼� - �����л���С�¼�
		void resizeEvent(QResizeEvent *event);
								//�¼� - ���ڹر��¼�������رհ�ť��
		void closeEvent(QCloseEvent *event);
								//�¼� - �����ļ��ж�
		void dragEnterEvent(QDragEnterEvent *event);
								//�¼� - �����ļ�����
		void dropEvent(QDropEvent *event);

	//-----------------------------------
	//----����
	public:
								//���� - �û��Զ���UI��ȡ
		void ui_loadConfig();
								//���� - �û��Զ���UI�洢
		void ui_saveConfig();
	private:
		Ui::DrillGIFActionSequenceEditorClass ui;
};

#endif // DRILLGIFACTIONSEQUENCEEDITOR_H
