#pragma once

#include "Source/Utils/widgetFormSenior/flexiblePageTree/private/c_FPT_Config.h"
#include "c_FCT_Classify.h"

/*
-----==========================================================-----
		�ࣺ		������ �����ࣨ����+�����֧��.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���������� ������ �����ࡣ
					����ϸ��cpp��
-----==========================================================-----
*/
class C_FCT_Config : public C_FPT_Config{

	public:
		C_FCT_Config();
		~C_FCT_Config();
		
	//-----------------------------------
	//----��֧ģʽ
	public:
									//��֧ģʽ - �ж� �����֧_ID���� ģʽ
		bool is_classify_idInc_Mode();
									//��֧ģʽ - �ж� �����֧_���Ƶ��� ģʽ
		bool is_classify_nameInc_Mode();
		
	//-----------------------------------
	//----����ҳ
	protected:
		C_FCT_Classify* m_emptyClassify;
		QList<C_FCT_Classify*> m_classifyList;
	public:
									//����ҳ - ��ȡ�б�
		QList<C_FCT_Classify*> get_classify_DataList();
									//����ҳ - �����б�
		void clear_classify_DataList();

									//����ҳ - ������ݣ���������
		void add_classify(C_FCT_Classify* data);
									//����ҳ - ɾ�����ݣ���������
		void remove_classify(QString classify_name);
									//����ҳ - ��������
		void set_classify_moveUp(QString classify_name);
									//����ҳ - ��������
		void set_classify_moveDown(QString classify_name);

									//����ҳ - ��ȡ
		C_FCT_Classify* get_classify(QString classify_name);
									//����ҳ - ��ȡ - ����ҳ����
		int get_classify_Index(QString classify_name);
									//����ҳ - ��ȡ - ����
		int get_classify_Count();
									//����ҳ - ��ȡ - �ж��������
		bool has_classify(QString classify_name);
									//����ҳ - ��ȡ - ȫ������
		QStringList getAll_classify_NameList();
	protected:
									//˽�� - ����б�
		void checkClassifyList();
		
	//-----------------------------------
	//----������
	public:
									//ʵ���� -> QJsonObject
		virtual QJsonObject getJsonObject() override;
									//QJsonObject -> ʵ����
									//		��˵���������������P_FlexibleClassificationTree��ֵparent_obj��
									//				  ֻ�и������� ���� �����������
		virtual void setJsonObject(QJsonObject obj, P_FlexiblePageTree* parent_obj) override;
};