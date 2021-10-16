#pragma once

#include <QKeyEvent>
#include <QTreeWidget>

#include "private/i_FPT_Branch.h"
#include "private/i_FPT_Leaf.h"
#include "private/c_FPT_Config.h"
#include "private/w_FPT_Config.h"

/*
-----==========================================================-----
		�ࣺ		��������.cpp
		�汾��		v1.04
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ܹ���ʾһ�����ݣ����ҽ���Щ���ݷ����ת�Ƶ���ͬ����֦�У����ڲ�ѯ��
					ע�⣬������װ�κ���Ҫ�Ըÿؼ�����н�������Ҫ��������ֱ�ӽ�����
					������id��֧�����Ʒ�֧�����������֧��
					����ϸ��cpp��
-----==========================================================-----
*/
class P_ObjectSortController;
class C_ObjectSortData;
class P_FlexiblePageTree : public QObject
{
	Q_OBJECT

	public:
		P_FlexiblePageTree(QTreeWidget *parent );	//���캯��
		~P_FlexiblePageTree();						//��������
		

	//-----------------------------------
	//----����������̳и�д�ýӿڣ�
	public:
											//���� - ���� ������ ���ݣ��ɸ�д��
		virtual C_FPT_Config* createConfigData();
											//���� - ���� ������ �༭���ڣ��ɸ�д��
		virtual W_FPT_Config* createConfigWindow();
											//���� - ���� Ҷ�ӿؼ����ɸ�д��
		virtual I_FPT_Leaf* createFCTLeaf();
											//���� - ���� ��֦�ؼ����ɸ�д��
		virtual I_FPT_Branch* createFCTBranch();

	//-----------------------------------
	//----������
	protected:
		QTreeWidget* m_tree;				//������
		QString m_treeStyle;				//��Ĭ����ʽ
	public:
									//������ - ��ȡ��
		QTreeWidget* getTree();
									//������ - ˢ����
		void refreshTreeUi();
									//������ - ����ȫ�������������ã�
		virtual void clearAll();
	protected:
									//������ - ˢ���� - ��֧
		virtual void refreshTreeUi_special();
									//������ - ˢ���� - ��֧ - ID��֧
		virtual void refreshTreeUi_id_inc();
									//������ - ˢ���� - ��֧ - ���Ʒ�֧
		virtual void refreshTreeUi_name_inc();

	//-----------------------------------
	//----Ҷ��
	protected:
		QList<I_FPT_Leaf*> m_leafItem;		//Ҷ���б�
	public:
									//Ҷ�� - ��ȡ - ��ȡ���󣨸���ID��
		I_FPT_Leaf* getLeafById(int id);
		I_FPT_Leaf* getLeafByName(QString name);
									//Ҷ�� - ��ȡ - ����
		QString getLeafName(int id);
									//Ҷ�� - ��ȡ - �ж�Ҷ��
		bool hasLeafName(QString name);
									//Ҷ�� - ��ȡ - �ж϶���
		bool isLeaf(QTreeWidgetItem* item);
		bool isLeafList(QList<QTreeWidgetItem*> item_list);
	public slots:
									//Ҷ�� - ���ⲿ�޸ġ�Ҷ������
		virtual void outerModifyLeafName(int id, QString name);
									//Ҷ�� - ���ⲿ�޸ġ�Ҷ������
		virtual void outerModifyLeafType(int id, QString type);
									//Ҷ�� - ���ⲿ�޸ġ�ѡ�е�Ҷ�����ƣ�ѡ��֦��Ӱ�죬ֻ��¼Ҷ�ӣ�
		virtual void outerModifySelectedLeafName( QString name);
									//Ҷ�� - ���ⲿ�޸ġ�ѡ�е�Ҷ�����ͣ�ѡ��֦��Ӱ�죬ֻ��¼Ҷ�ӣ�
		virtual void outerModifySelectedLeafType( QString type);

	//-----------------------------------
	//----��֦
	protected:
		QList<I_FPT_Branch*> m_branchItem;	//��֦�б�
	public:
									//��֦ - ��ȡ��֦�����Ʒ�֧ר�ã�
		I_FPT_Branch* getBranchByNameSymbol(QString symbol);
									//��֦ - ��ȡ - �ж϶���
		bool isBranch(QTreeWidgetItem* item);
		bool isBranchList(QList<QTreeWidgetItem*> item_list);
		
	//-----------------------------------
	//----ѡ��
	protected:
		QTreeWidgetItem* m_last_selectedItem;
		I_FPT_Leaf* m_last_selectedLeaf;
	signals:
									//ѡ�� - ���ڵ�仯���źţ�
									//		��˵����������Ҷ�Ӻ���֦��
		void currentItemChanged(QTreeWidgetItem* item);
									//ѡ�� - Ҷ�ӱ仯���źţ�
		void currentLeafChanged(QTreeWidgetItem* item, int id, QString name);


	//-----------------------------------
	//----���¼�
	public:
		bool m_slotBlock;					//������
	public slots:
											//�����¼�
		void sltItemClicked(QTreeWidgetItem *item, int index);
											//˫���¼�
		void sltItemDoubleClicked(QTreeWidgetItem *item, int index);
											//�Ҽ��¼�
		void sltItemRightClicked(QPoint point);
											//Ȧѡ�仯�¼�
		void sltItemSelectionChanged();
		
	//-----------------------------------
	//----�Ҽ��˵�
	protected:
		QMenu* m_mainMenu;						//�Ҽ��˵�
		QMenu* m_modeMenu;						//��֧�˵�
		bool m_leafOuterControlEnabled;			//�˵� - ����
		bool m_leafOuterControl_CopyActive;		//�˵� - ���Ƽ���
		bool m_leafOuterControl_PasteActive;	//�˵� - ճ������
		bool m_leafOuterControl_ClearActive;	//�˵� - ��ռ���
	public:
											//�Ҽ��˵� - һ���˵�
		virtual void drawMenuMain();
		virtual void drawMenuMainLast();
											//�Ҽ��˵� - �����˵���ģʽ��
		virtual void drawMenuMode();
	public slots:
											//�Ҽ��˵� - ���أ�Ĭ�Ͽ�����
		void setLeafOuterControlEnabled(bool enabled);
											//�Ҽ��˵� - �������
		void setLeafOuterControl_CopyActive(bool enabled);
		void setLeafOuterControl_PasteActive(bool enabled);
		void setLeafOuterControl_ClearActive(bool enabled);
											//�Ҽ��˵� - ���ư���
		void menuCopyLeafInAction();
											//�Ҽ��˵� - ճ������
		void menuPasteLeafInAction();
											//�Ҽ��˵� - ��հ���
		void menuClearLeafInAction();
	signals:
											//�Ҽ��˵� - ���ư��£��źţ�
		void menuCopyLeafTriggered(int id);
											//�Ҽ��˵� - ճ�����£��źţ�
		void menuPasteLeafTriggered(int id);
											//�Ҽ��˵� - ��հ��£��źţ�
		void menuClearLeafTriggered(int id);

		
	//-----------------------------------
	//----���ݣ������ã�
	protected:
		C_FPT_Config* m_config;			//������
	public:
										//������ - ���ò���
		virtual void setConfig(C_FPT_Config* config);
										//������ - ȡ������
		virtual C_FPT_Config* getConfig();
										//������ - �л�ģʽ
		void changeSortMode(QString sortMode);
		void changeSortModeInAction();
										//������ - ��ȡ��ǰģʽ
		QString getCurrentSortMode();
	protected slots:
										//������ - �༭����
		void openConfigParamWindow();
		
	//-----------------------------------
	//----���ݣ���Դ���ݣ�
	protected:
		P_ObjectSortController* m_source_ObjectSortController;		//�������������
		QList<C_ObjectSortData> m_source_list;						//���������ݣ������ݻ��ڱ��ر��޸ģ�
	public:
										//��Դ���� - ����
										//			���������setData��ʼ���������棩
										//			�����ڱ���Ҫ�����޸��������ݣ��������ﲻ��ָ�롣��
		void loadSource(QList<QJsonObject> obj_list);
		void loadSource(QList<QJsonObject> obj_list, QString id_symbol, QString name_symbol, QString type_symbol);
										//��Դ���� - ��ȡѡ�е����ݣ���ѡ������֦������-1��""��
		int getSelectedSourceId();
		QString getSelectedSourceName();
		QString getSelectedSourceType();
	protected:
										//��Դ���� - ��ȡѡ�еĶ���˽�У�
		C_ObjectSortData* getSelectedSource();
	protected slots:
										//��Դ���� - �ؽ����ݣ�˽�У�
		virtual void rebuildTreeData();
										//��Դ���� - �ؽ�����_ID��֧��˽�У�
		void rebuildTreeData_id_inc();
										//��Դ���� - �ؽ�����_���Ʒ�֧��˽�У�
		void rebuildTreeData_name_inc();

};
