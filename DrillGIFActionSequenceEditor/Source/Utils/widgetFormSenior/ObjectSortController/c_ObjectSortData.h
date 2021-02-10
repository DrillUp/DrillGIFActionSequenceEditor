#pragma once

/*
-----==========================================================-----
		�ࣺ		����������.h
		����ģ�飺	����ģ��
		���ܣ�		��������Ķ������ݡ�
					�����಻��QJsonObject�Ļ�ת����Ϊsymbol��ʶ����ȷ��
-----==========================================================-----
*/
class C_ObjectSortData{
	public:
		C_ObjectSortData();
		C_ObjectSortData(int arr_index, int id, QString name, QString type);
		~C_ObjectSortData();

	public:
		int arrIndex;			//����������λ��
		int id;					//ID
		QString name;			//����
		QString type;			//��������
	
	public:	
									//���������
		const bool operator== (const C_ObjectSortData& a)const;
									//���ж�
		bool isNull();
									//QJsonObject -> ʵ����
		void setJsonObject(QJsonObject obj);
		void setJsonObject(QJsonObject obj, QString id_symbol, QString name_symbol, QString type_symbol);
};

