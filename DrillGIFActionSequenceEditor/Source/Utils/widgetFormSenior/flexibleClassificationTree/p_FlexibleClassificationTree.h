#pragma once

#include "../flexiblePageTree/P_FlexiblePageTree.h"

/*
-----==========================================================-----
		�ࣺ		�������������Զ����֧��.cpp
		�汾��		v1.04
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ܹ���ʾһ�����ݣ����ҽ���Щ���ݷ����ת�Ƶ���ͬ����֦�У����ڲ�ѯ��
					ע�⣬������װ�κ���Ҫ�Ըÿؼ�����н�������Ҫ��������ֱ�ӽ�����
					����ϸ��cpp��
-----==========================================================-----
*/
class C_FCT_Config;
class C_FCT_Classify;
class W_FCT_Classify;
class P_FlexibleClassificationTree : public P_FlexiblePageTree
{
	Q_OBJECT

	public:
		P_FlexibleClassificationTree(QTreeWidget *parent);		//���캯��
		~P_FlexibleClassificationTree();						//��������
		

	//-----------------------------------
	//----����������̳и�д�ýӿڣ�
	public:
											//���� - ���� ������ ���ݣ���д���ࣩ
		virtual C_FPT_Config* createConfigData();
											//���� - ���� ���� ���ݣ��ɸ�д��
		virtual C_FCT_Classify* createClassifyData();
											//���� - ���� ���� �༭���ڣ��ɸ�д��
		virtual W_FCT_Classify* createClassifyWindow();
		
	//-----------------------------------
	//----������
	public:
									//������ - ����ȫ��
		virtual void clearAll() override;
	protected:
									//������ - ˢ���� - ��֧
		virtual void refreshTreeUi_special() override;
									//������ - ˢ���� - ��֧ - �Զ����֧_id����
		virtual void refreshTreeUi_classify_idInc();
									//������ - ˢ���� - ��֧ - �Զ����֧_���Ƶ���
		virtual void refreshTreeUi_classify_nameInc();
		

	//-----------------------------------
	//----Ҷ��
	public:
									//Ҷ�� - ��ȡ - ���������ȡ
		QList<I_FPT_Leaf*> getLeafByType(QString type);
									//Ҷ�� - ��ȡ - ����
		QString getLeafType(int id);
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
	public:
									//��֦ - ��ȡ��֦���Զ����֧ר�ã�
		I_FPT_Branch* getBranchByTypeName(QString classify_name);


	//-----------------------------------
	//----���ݣ������֧��
	public slots:
									//���� - ���
		void addClassify(C_FCT_Classify* classify);
									//���� - ��ӣ�action��
		void addClassifyInAction();
									//���� - ��� - ��֤������
		void addClassifyDistinguishedList(QStringList new_classify_nameList);
									//���� - �޸�
		void modifyClassify(QString classify_name, C_FCT_Classify* classify);
									//���� - �޸ģ�action��
		void modifyClassifyInAction();
									//���� - ȥ��
		void removeClassify(QString classify_name);
									//���� - ȥ����action��
		void removeClassifyInAction();
		void removeClassifyListInAction();
									//���� - ����
		void moveUpInAction();
									//���� - ����
		void moveDownInAction();
									//���� - ��ȡ - �Ƿ�Ϊ�Զ����֧
		bool isClassifyMode();
									//���� - ��ȡ - ȫ��������
		QStringList getAllClassifyName();
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
										//�޸ĵ����� - ��ȡҶ�ӵ�����
		QString getLeafTypeById(int id);
										//�޸ĵ����� - ��¼�޸ģ�ֻ��type������
		void appendChangedSource(C_ObjectSortData data);
										//�޸ĵ����� - �޸ĵ����ݣ�ֻ��type������
		QList<C_ObjectSortData> getChangedSource();


	//-----------------------------------
	//----�Ҽ��˵�
	public:
											//�Ҽ��˵� - һ���˵�
		virtual void drawMenuMain() override;
		virtual void drawMenuMainLast() override;
											//�Ҽ��˵� - �����˵���ģʽ��
		virtual void drawMenuMode() override;
		
		
	//-----------------------------------
	//----���ݣ������ã�
	protected:
										//������ - ���ò���
		virtual void setConfig(C_FPT_Config* config) override;
										//������ - ȡ������
		virtual C_FPT_Config* getConfig() override;
	public:
										//������ - ���ò���Ex
		virtual void setConfigEx(C_FCT_Config* config);
										//������ - ȡ������Ex
		virtual C_FCT_Config* getConfigEx();


	//-----------------------------------
	//----���ݣ���Դ���ݣ�
	protected slots:
										//��Դ���� - �ؽ����ݣ�˽�У�
		virtual void rebuildTreeData() override;
										//��Դ���� - �ؽ�����_�Զ����֧_id������˽�У�
		void rebuildTreeData_classify_idInc();
										//��Դ���� - �ؽ�����_�Զ����֧_���Ƶ�����˽�У�
		void rebuildTreeData_classify_nameInc();

};
