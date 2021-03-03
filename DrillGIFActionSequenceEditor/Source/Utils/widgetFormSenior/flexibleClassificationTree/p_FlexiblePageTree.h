#pragma once

#include <QKeyEvent>
#include <QTreeWidget>

#include "private/c_FCTClassify.h"
#include "private/c_FCTConfig.h"
#include "private/i_FCTBranch.h"
#include "private/i_FCTLeaf.h"

/*
-----==========================================================-----
		�ࣺ		��������.cpp
		�汾��		v1.02
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ܹ���ʾһ�����ݣ����ҽ���Щ���ݷ����ת�Ƶ���ͬ����֦�У����ڲ�ѯ��
					ע�⣬������װ�κ���Ҫ�Ըÿؼ�����н�������Ҫ��������ֱ�ӽ�����
					������id��֧�����Ʒ�֧�������Զ����֧��
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
	//----�ؼ�
	protected:
		QTreeWidget* m_tree;						//������
		QString m_treeStyle;						//��Ĭ����ʽ
		bool m_slotBlock;							//������
		QList<I_FCTLeaf*> m_leafItem;				//Ҷ���б�
		QList<I_FCTBranch*> m_branchItem;			//��֦�б�
	public:
									//�ؼ� - ˢ����
		void refreshTreeUi();
									//�ؼ� - ˢ����_��֧����
		virtual void refreshTreeUi_special();
									//�ؼ� - ˢ����_ID��֧
		void refreshTreeUi_id_inc();
									//�ؼ� - ˢ����_���Ʒ�֧
		void refreshTreeUi_name_inc();
									//�ؼ� - ����ȫ�������������ã�
		virtual void clearAll();

	//-----------------------------------
	//----�ؼ���Ҷ�ӣ�
	public:
									//Ҷ�� - ��ȡ - ��ȡ���󣨸���ID��
		I_FCTLeaf* getLeafById(int id);
		I_FCTLeaf* getLeafByName(QString name);
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
	//----�ؼ���Ҷ�ӽӿڣ�
	protected:
		QTreeWidgetItem* m_last_item;
		I_FCTLeaf* m_last_leaf;
		bool m_leafOuterControlEnabled;			//Ҷ�ӱ༭ - ����
		bool m_leafOuterControl_CopyActive;		//Ҷ�ӱ༭ - ���Ƽ���
		bool m_leafOuterControl_PasteActive;	//Ҷ�ӱ༭ - ճ������
		bool m_leafOuterControl_ClearActive;	//Ҷ�ӱ༭ - ��ռ���
	signals:
									//Ҷ�ӽӿ� - ��ǰҶ�ӱ仯���źţ�
		void currentLeafChanged(QTreeWidgetItem* item, int id, QString name);
									//�ؼ��ӿ� - ��ǰ���ڵ�仯������Ҷ�Ӻ���֦���źţ�
		void currentItemChanged(QTreeWidgetItem* item);
									//Ҷ�ӽӿ� - ���ư��£��źţ�
		void menuCopyLeafTriggered(int id);
									//Ҷ�ӽӿ� - ճ�����£��źţ�
		void menuPasteLeafTriggered(int id);
									//Ҷ�ӽӿ� - ��հ��£��źţ�
		void menuClearLeafTriggered(int id);
	public slots:
									//Ҷ�ӽӿ� - ���أ�Ĭ�Ͽ�����
		void setLeafOuterControlEnabled(bool enabled);
									//Ҷ�ӽӿ� - �������
		void setLeafOuterControl_CopyActive(bool enabled);
		void setLeafOuterControl_PasteActive(bool enabled);
		void setLeafOuterControl_ClearActive(bool enabled);
									//Ҷ�ӽӿ� - ���ư���
		void menuCopyLeafInAction();
									//Ҷ�ӽӿ� - ճ������
		void menuPasteLeafInAction();
									//Ҷ�ӽӿ� - ��հ���
		void menuClearLeafInAction();

	//-----------------------------------
	//----�ؼ�����֦��
	public:
									//��֦ - ��ȡ��֦�����Ʒ�֧ר�ã�
		I_FCTBranch* getBranchByNameSymbol(QString symbol);
									//��֦ - ��ȡ - �ж϶���
		bool isBranch(QTreeWidgetItem* item);
		bool isBranchList(QList<QTreeWidgetItem*> item_list);

	//-----------------------------------
	//----����
	protected:
		QJsonObject local_treeData;			//�����ݣ����� ģʽ�� ������ �� �Զ����֧��
	public:
									//���� - ��������
		void setData(QJsonObject obj);
									//���� - ȡ��
		QJsonObject getData();
									//���� - �������� -> ui����
		virtual void putDataToUi();
									//���� - ui���� -> ��������
		virtual void putUiToData();

	//-----------------------------------
	//----���ݣ�ģʽ��
	protected:
		QString m_sortMode;				//��ǰģʽ
		QStringList m_sortModeList;		//ģʽ����
	public slots:
									//ģʽ - �л�ģʽ
		void changeSortMode(QString sortMode);
		void changeSortModeInAction();
									//ģʽ - ��ȡ
		QString getCurSortMode();
		
	//-----------------------------------
	//----���ݣ������ã�
	protected:
		C_FCTConfig m_config;	//������
	public:
										//������ - ���ò���
		void setConfigParam(QJsonObject config);
										//������ - ȡ������
		QJsonObject getConfigParam();
	protected slots:
										//������ - �༭����
		void openConfigParamWindow();

	//-----------------------------------
	//----���¼�
	public:
		bool m_selectionSignalBlock_Root;		//Ȧѡ ��
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
		QMenu* m_mainMenu;
		QMenu* m_modeMenu;
		QString m_menuIconSrcPath;
	public:
											//�Ҽ��˵� - һ���˵�
		virtual void drawMenuMain();
		virtual void drawMenuMainLast();
											//�Ҽ��˵� - �����˵���ģʽ��
		virtual void drawMenuMode();
		
	//-----------------------------------
	//----��Դ����
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
