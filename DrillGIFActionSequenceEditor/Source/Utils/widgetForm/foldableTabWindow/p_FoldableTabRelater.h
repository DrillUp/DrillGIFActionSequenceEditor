#ifndef P_FoldableTabManager_H
#define P_FoldableTabManager_H

#include "private/c_FoldableTabPrivate.h"
#include "private/w_FoldableTabChildWindow.h"

/*
-----==========================================================-----
		�ࣺ		���۵�ѡ� ������.h
		�汾��		v1.02
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�۵�ѡ����ڵĹ�������
					����ϸ��cpp��
-----==========================================================-----
*/

class P_FoldableTabRelater : public QObject
{
	Q_OBJECT

	public:
		P_FoldableTabRelater(QTabWidget *parent);
		~P_FoldableTabRelater();
		
	//-----------------------------------
	//----���ؼ�
	protected:
		QTabWidget* m_tab;				//���ؼ�
		QString m_tabStyle;				//��ʽ
		QString m_iconSrcPath;			//ͼ����Դ·��
	public slots:
								//���ؼ� - �رձ�ǩ
		void tabClosed(int index);
								//���ؼ� - ��ǩȫ��λ
		void homingAllTab();

	//-----------------------------------
	//----�ӿ�
	protected:
		QList<C_FoldableTabPrivate*> m_partList;
	public slots:
								//�ӿ� - ����ӿ�
		void addPart(QString name, QWidget* partWidget);	
		void addPart(QString name, QWidget* partWidget, QJsonObject param);	
		
								//�ӿ� - ��ȡ�ӿ飨�������ƣ�
		C_FoldableTabPrivate* getPartByName(QString name);
								//�ӿ� - ��ȡȫ���ӿ�
		QList<C_FoldableTabPrivate*> getAllPart();

								//�ӿ� - ˢ���ӿ鴰�ڷֲ����
		void refreshAllPart();

	//-----------------------------------
	//----�Ӵ���
	protected:
		QList<W_FoldableTabChildWindow*> m_childWindowList;
	public slots:
								//�Ӵ��� - ��ʾ
		void showChildWindow(QString name);	
								//�Ӵ��� - ����
		void hideChildWindow(QString name);
								//�Ӵ��� - ȫ�����أ�ȫ���ص���ǩҳ�У�
		void hideAllChildWindow();
								//�Ӵ��� - ��ȡ
		W_FoldableTabChildWindow* getChildWindow(QString name);
		
};

#endif // P_FoldableTabManager_H
