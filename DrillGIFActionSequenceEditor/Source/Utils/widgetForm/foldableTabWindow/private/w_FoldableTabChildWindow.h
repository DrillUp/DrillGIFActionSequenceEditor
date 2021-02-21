#ifndef W_FoldableTabChildWindow_H
#define W_FoldableTabChildWindow_H

#include "ui_W_FoldableTabChildWindow.h"
#include "c_FoldableTabPrivate.h"

/*
-----==========================================================-----
		�ࣺ		�۵��Ӵ���.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		������д�������Ĵ��ڡ�
					����ϸ��cpp��
-----==========================================================-----
*/

class P_FoldableTabRelater;
class W_FoldableTabChildWindow : public QDialog
{
	Q_OBJECT

	public:
		W_FoldableTabChildWindow(P_FoldableTabRelater* parentManager, C_FoldableTabPrivate* part, QWidget *parent = 0);
		~W_FoldableTabChildWindow();
		
	//-----------------------------------
	//----�ؼ�
	private:
		P_FoldableTabRelater* m_parentManager;
		C_FoldableTabPrivate* m_part;
		QHBoxLayout* m_layout;
	public slots:
										//�ؼ� - ˢ���ӿ�ؼ�
		void refreshPart();
										//�ؼ� - ����ؼ�
		void clearPart();
		
	//-----------------------------------
	//----����
	private:
										//���� - ������ʾ�¼�
		void showEvent(QShowEvent *event);
										//���� - �ر��¼�������رհ�ť��
		void closeEvent(QCloseEvent *event);
	private slots:
										//���� - �ύ����
		void acceptData();
		void rejectData();

	private:
		Ui::W_FoldableTabChildWindow ui;
	//-----------------------------------
};

#endif // W_FoldableTabChildWindow_H
