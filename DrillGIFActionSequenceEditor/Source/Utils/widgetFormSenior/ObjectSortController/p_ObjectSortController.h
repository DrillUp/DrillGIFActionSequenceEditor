#ifndef W_ObjectSortList_H
#define W_ObjectSortList_H

#include "c_ObjectSortData.h"

/*
-----==========================================================-----
		�ࣺ		�������� ������.h
		�汾��		v1.01
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ܽ�ָ��������б�������򣬷����������ꡢҲ���ṩ��ҳ���ܡ�
					�������ݣ�setData��ʱ����������������һ�Ρ�������ˢ���������������ݡ�
					����ϸ��cpp��
-----==========================================================-----
*/
class P_ObjectSortController : public QObject
{
	Q_OBJECT

	public:
		P_ObjectSortController(QWidget *parent = 0);
		~P_ObjectSortController();
		
	//-----------------------------------
	//----����
	protected:
		QList<QJsonObject>* local_org_objList;						//����ʱ��obj
		QList<C_ObjectSortData*> local_org_dataList;				//����ʱ�����������б�
		QString m_symbol_id;										//��ʶ��id
		QString m_symbol_name;										//��ʶ��name
		QString m_symbol_type;										//��ʶ��type
	public:
												//���� - �������ݣ������б�
												//		������1��obj�����б�Ĭ�ϻ����� id, name, type��
												//		������2��id��ʶ��������ƺ��ã�����ŵ���"sid"��
												//		������3��name��ʶ��������ƺ��ã�
												//		������4��type��ʶ��������ƺ��ã�
												//		��˵���������˶����б����ܴ� JsonObject��ȡ���� �л�ȡ������
		void setData_FromObject(QList<QJsonObject>* obj_list);
		void setData_FromObject(QList<QJsonObject>* obj_list, QString id_symbol, QString name_symbol, QString type_symbol);
												//���� - �������ݣ����������б�
												//		��������id�б�
												//		��˵����arrIndex���Բ����ֵʱ�ò��ϡ�
		void setData_FromSortData(QList<C_ObjectSortData> sort_data_list);
												//���� - �������ݣ������б�
												//		������1��id�б�
												//		������2��name�б�
												//		������3��type�б�
												//		��˵�����������б��ȱ���һ�¡�������Ϊ�����б��� JsonObject��ȡ���� ֻ��Ϊ�ա�
		void setData_FromBasic(QList<int> id_list, QList<QString> name_list);
		void setData_FromBasic(QList<int> id_list, QList<QString> name_list, QList<QString> type_list);

												//���� - �����ǰȫ������
		void clearAll();
												//���� - ��ȡ��������
		int getDataCount();
												//����JsonObject - ���õ������Ƿ�ΪJsonObject����
		bool isObjectData();
												//����JsonObject - �޸ı�ʶ��
		void modifySymbol(QString id_symbol, QString name_symbol, QString type_symbol);
												//����JsonObject - ��ȡ��ʶ��
		QString getSymbolId();
		QString getSymbolName();
		QString getSymbolType();

	//-----------------------------------
	//----������
	public:
												//������ - �����������У���ȡ������
		C_ObjectSortData* getSortData_ByIndex(int arrIndex);
		QList<C_ObjectSortData*> getSortDataList_ByIndex(QList<int> arrIndex_list);
												//������ - �����������У���ȡ�����ݣ�ע�⣬�����ֵʱ����FromObject���򷵻ؿգ�
		QJsonObject getObjectData_ByIndex(int arrIndex);
		QList<QJsonObject> getObjectDataList_ByIndex(QList<int> arrIndex_list);

	//-----------------------------------
	//----ID����
	protected:
		QList<C_ObjectSortData*> local_dataList_orderBy_IdInc;		//��ID�������б�
		QList<C_ObjectSortData*> local_dataList_orderBy_IdDec;		//��ID�ݼ����б�
	public:
												//ID - ��ʼ�����Զ���
		void initSort_Id_IfRequire();
												//ID - ��ȡ�������У�����
		QList<int> get_IdInc_IndexSeq();
		QList<int> get_IdInc_IndexSeq(int bottom, int top);
												//ID - ��ȡָ��ҳ���������У�����
		QList<int> get_IdInc_PageIndexList(int page_index, int page_per_count);

												//ID - ��ȡ�������У�����
		QList<int> get_IdDec_IndexSeq();
		QList<int> get_IdDec_IndexSeq(int bottom, int top);
												//ID - ��ȡָ��ҳ���������У�����
		QList<int> get_IdDec_PageIndexList(int page_index, int page_per_count);
		
	//-----------------------------------
	//----��������
	protected:
		QList<C_ObjectSortData*> local_dataList_orderBy_NameInc;	//�����Ƶ������б�
		QList<C_ObjectSortData*> local_dataList_orderBy_NameDec;	//�����Ƶݼ����б�
		QString m_firstCharSeq;										//����ĸ����
	public:
												//���� - ��ʼ�����Զ���
		void initSort_Name_IfRequire();
												//���� - ��ȡ�������У�����
		QList<int> get_NameInc_IndexSeq();
		QList<int> get_NameInc_IndexSeq(int bottom, int top);
												//���� - ��ȡ�������У�����
		QList<int> get_NameDec_IndexSeq();
		QList<int> get_NameDec_IndexSeq(int bottom, int top);
												//���� - �ж��Ƿ�ָ������ĸ������
		bool hasAnyData_Name_ByCharacter(QChar ch);
												//���� - ��ȡָ������ĸ����������
		QList<int> get_Name_IndexListByCharacter(QChar ch);
												//���� - ��ȡ����Ϊ�յ���������
		QList<int> get_Name_IndexListByEmptyName();
		

	//-----------------------------------
	//----��������
	protected:
		QList<C_ObjectSortData*> local_dataList_orderBy_TypeAndId;			//�� ����+ID ������б�
		QList<C_ObjectSortData*> local_dataList_orderBy_TypeAndName;		//�� ����+���� ������б�
		QStringList m_distinguishedType;
	public:
												//���� - ��ʼ�����Զ���
		void initSort_Type_IfRequire();
												//���� - �ж��Ƿ����ͷ��������
		bool hasAnyData_Type();
												//���� - ��ȡ��������
		QStringList get_Type_DistinguishedList();
												//���� - ��ȡ�������У�����+ID ˳��
		QList<int> get_TypeAndId_IndexSeq();
		QList<int> get_TypeAndId_IndexSeq(int bottom, int top);
												//���� - ��ȡָ�����͵������У�����+ID ˳��
		QList<int> get_TypeAndId_IndexListByType(QString type);
												//���� - ��ȡ�������У�����+���� ˳��
		QList<int> get_TypeAndName_IndexSeq();
		QList<int> get_TypeAndName_IndexSeq(int bottom, int top);
												//���� - ��ȡָ�����͵������У�����+���� ˳��
		QList<int> get_TypeAndName_IndexListByType(QString type);

		
	//-----------------------------------
	//----��̬����
	public:
		static QList<QJsonObject> _to_objectList_(QList<int> id_list, QList<QString> name_list);
		static QList<QJsonObject> _to_objectList_(QList<int> id_list, QList<QString> name_list, QList<QString> type_list);
};

#endif // W_ObjectSortList_H
