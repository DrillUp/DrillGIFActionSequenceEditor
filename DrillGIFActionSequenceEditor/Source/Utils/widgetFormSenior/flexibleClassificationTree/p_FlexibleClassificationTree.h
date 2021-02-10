#pragma once

#include "P_FlexiblePageTree.h"

/*
-----==========================================================-----
		�ࣺ		�������������Զ����֧��.cpp
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ܹ���ʾһ�����ݣ����ҽ���Щ���ݷ����ת�Ƶ���ͬ����֦�У����ڲ�ѯ��
					ע�⣬������װ�κ���Ҫ�Ըÿؼ�����н�������Ҫ��������ֱ�ӽ�����
					����ϸ��cpp��
-----==========================================================-----
*/
class P_FlexibleClassificationTree : public P_FlexiblePageTree
{
	Q_OBJECT

	public:
		P_FlexibleClassificationTree(QTreeWidget *parent);		//���캯��
		~P_FlexibleClassificationTree();						//��������
		
	//-----------------------------------
	//----�ؼ�
	public:
									//�ؼ� - ˢ����
		virtual void refreshTreeUi_special();
									//�ؼ� - ˢ����_�Զ����֧_id����
		void refreshTreeUi_classify_idInc();
									//�ؼ� - ˢ����_�Զ����֧_���Ƶ���
		void refreshTreeUi_classify_nameInc();
									//�ؼ� - ����ȫ��
		virtual void clearAll();
		
	//-----------------------------------
	//----�ؼ���Ҷ�ӣ�
	public:
									//Ҷ�� - ���ⲿ�޸ġ�Ҷ������
		virtual void outerModifyLeafName(int id, QString name);
									//Ҷ�� - ���ⲿ�޸ġ�Ҷ������
		virtual void outerModifyLeafType(int id, QString type);
		
									//Ҷ�� - ��ȡ - �������ͻ�ȡ
		QList<I_FCTLeaf*> getLeafByType(QString type);
		
	//-----------------------------------
	//----�ؼ�����֦��
	public:
									//��֦ - ��ȡ��֦���Զ����֧ר�ã�
		I_FCTBranch* getBranchByTypeName(QString classify_name);
		
	//-----------------------------------
	//----����
	public:
									//���� - �������� -> ui����
		virtual void putDataToUi();
									//���� - ui���� -> ��������
		virtual void putUiToData();

	//-----------------------------------
	//----���ݣ��Զ����֧��
	protected:
		QList<C_FCTClassify> m_classifyData;		//�Զ����֧��ֻ��putDataToUiʱ��ʼ����
	public slots:
									//���� - ���
		void addClassify(C_FCTClassify classify);
		void addClassifyInAction();
									//���� - �޸�
		void modifyClassify(QString classify_name, C_FCTClassify classify);
		void modifyClassifyInAction();
									//���� - ȥ��
		void removeClassify(QString classify_name);
		void removeClassifyInAction();
		void removeClassifyListInAction();
									//���� - ����
		void moveUpInAction();
									//���� - ����
		void moveDownInAction();
									//���� - ��ȡ - ȫ������
		QStringList getAllClassify();
									//���� - ��ȡ - ��ȡ��������
		int indexOfClassify(QString classify_name);
									//���� - ��ȡ - �ж����ʹ���
		bool hasClassify(QString classify_name);
									//���� - ��ǰΪ�Զ����֧ģʽ
		bool isClassifyMode();
	protected:
									//���� - �ӱ��������У���ȡ�����б�
		void appendClassifyData_FromLocalObj();
	
	//-----------------------------------
	//----���ݣ��޸ĵ����ݣ�
	protected:
		QList<C_ObjectSortData> m_changedSource;
	public:
										//�޸ĵ����� - ���ڲ��޸ġ�Ҷ�ӵ�����
		void innerModifyLeafType(int id, QString type);
		void innerModifyLeafTypeInAction();
										//�޸ĵ����� - ��¼�޸ģ�ֻ��type������
		void appendChangedSource(C_ObjectSortData data);
										//�޸ĵ����� - �޸ĵ����ݣ�ֻ��type������
		QList<C_ObjectSortData> getChangedSource();


	//-----------------------------------
	//----�Ҽ��˵�
	public:
											//�Ҽ��˵� - һ���˵�
		virtual void drawMenuMain();
		virtual void drawMenuMainLast();
											//�Ҽ��˵� - �����˵���ģʽ��
		virtual void drawMenuMode();
		
	//-----------------------------------
	//----��Դ����
	protected slots:
										//��Դ���� - �ؽ����ݣ�˽�У�
		virtual void rebuildTreeData();
										//��Դ���� - �ؽ�����_�Զ����֧_id������˽�У�
		void rebuildTreeData_classify_idInc();
										//��Դ���� - �ؽ�����_�Զ����֧_���Ƶ�����˽�У�
		void rebuildTreeData_classify_nameInc();

};
