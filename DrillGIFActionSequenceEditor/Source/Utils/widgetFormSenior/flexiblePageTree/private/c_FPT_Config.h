#pragma once


/*
-----==========================================================-----
		�ࣺ		������ ������.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���������� ������ �����ࡣ
					����ϸ��cpp��
-----==========================================================-----
*/
class P_FlexiblePageTree;
class C_FPT_Config{

	public:
		C_FPT_Config();
		~C_FPT_Config();
		
	//-----------------------------------
	//----��ʾ����
	public:
		int rowHeight;				//�и�
		bool zeroFill;				//�����
		int zeroFillCount;			//���λ��
		QChar zeroFillChar;			//����ַ�

	
	//-----------------------------------
	//----��֧ģʽ
	protected:
		QString m_mode;				//��ǰģʽ
		QStringList m_modeList;		//ģʽ�б�
	public:
									//��֧ģʽ - ���õ�ǰģʽ
		void setCurrentMode(QString sortMode);
									//��֧ģʽ - ��ȡ��ǰģʽ
		QString getCurrentMode();
									//��֧ģʽ - ����ģʽ�б�
		void setModeList(QStringList sortModeList);
									//��֧ģʽ - ��ȡģʽ�б�
		QStringList getModeList();
									//��֧ģʽ - �ж� ID��֧ ģʽ
		bool is_id_Mode();
									//��֧ģʽ - �ж� ���Ʒ�֧ ģʽ
		bool is_name_Mode();
		

	//-----------------------------------
	//----IDҳ
	public:
		int pagePerNum;				//ID��֧ÿҳ����
	protected:
		int m_id_maxCount;			//���ֵ��˽����ʱ����
	public:
									//IDҳ - �������ֵ
									//		��˵������ע�⣬����Ҫ�ȸ�ֵ����ִ�к���Ļ�ȡ��������Ȼ����õ�-1�Ϳ��ַ�����
		void set_id_MaxCount(int count);
									//IDҳ - ��ȡ���ֵ
		int get_id_MaxCount();
	public:
									//IDҳ - ��ȡҳ��
		int get_id_PageCount();
									//IDҳ - ��ȡ��
		int get_id_Bottom(int page_index);
									//IDҳ - ��ȡ��
		int get_id_Top(int page_index);
									//IDҳ - ��ȡҳ���ƣ�����ҳ������
		QString get_id_PageNameByIndex(int page_index);
									//IDҳ - ��ȡ��ʽ����ID
		QString get_id_FormatedId(int id);
		
	//-----------------------------------
	//----����ҳ
	protected:
		QStringList m_name_textList;	//�����б�
		QStringList m_name_symbolList;	//��ʶ�б�
	public:
									//����ҳ - ��ȡҳ��
		int get_name_PageCount();
									//����ҳ - ��ȡ�����б�
		QStringList get_name_PageNameList();
									//����ҳ - ��ȡ��ʶ�б�
		QStringList get_name_PageSymbolList();
		
	//-----------------------------------
	//----�Զ�������
	protected:
		QJsonObject data;
	public:
									//�Զ������� - ��ȡ����
		QJsonObject getCustomData();
		
	//-----------------------------------
	//----������
	public:
									//ʵ���� -> QJsonObject
		virtual QJsonObject getJsonObject();
									//QJsonObject -> ʵ����
		virtual void setJsonObject(QJsonObject obj, P_FlexiblePageTree* parent_obj);
};