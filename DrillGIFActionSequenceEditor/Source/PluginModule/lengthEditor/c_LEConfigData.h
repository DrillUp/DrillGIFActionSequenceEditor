#pragma once
#include <QJsonObject>

/*
-----==========================================================-----
		�ࣺ		�޸ĵĲ���.h���洢��
		����ģ�飺	���ģ��
		���ܣ�		��������ò������ݡ�
-----==========================================================-----
*/
class C_LEConfigData {
	public:
		C_LEConfigData();
		~C_LEConfigData();
	private:
		QString pluginName;			//�����
		QString paramKey;			//������
		int configLen;				//�������ֵ
	public:
											//���� - ��ʼ��
		void initParam(QString pluginName, QString paramKey, int realLen);
											//��ȡ - �������Ӣ�ģ�
		QString getPluginName();
											//��ȡ - ������ "�׶�-%d"
		QString getParamKey();
											//��ȡ - ������ʾ�� "�׶�"
		QString getParamShowingName();
											//��ȡ - �������ֵ
		int getConfigLen();

											//ʵ���� -> QJsonObject
		QJsonObject getJsonObject();
											//QJsonObject -> ʵ����
		void setJsonObject(QJsonObject obj);
											//���ж�
		bool isNull();
											//���������
		const bool operator== (const C_LEConfigData& a)const;
};
inline uint qHash(C_LEConfigData &key, uint seed){	//qHasgȫ������
	return qHash(key.getPluginName(), seed) * qHash(key.getParamKey(), seed);
};