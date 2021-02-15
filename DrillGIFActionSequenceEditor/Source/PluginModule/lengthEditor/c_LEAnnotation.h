#pragma once
#include <QJsonObject>

/*
-----==========================================================-----
		�ࣺ		���ע�� ����.h��ֻ����
		����ģ�飺	����ģ��
		���ܣ�		����Ĺ̶��������ݡ�
					����ϸ��cpp��
-----==========================================================-----
*/
class C_LEAnnotation_Param{
	public:
		C_LEAnnotation_Param();
		~C_LEAnnotation_Param();
	private:
		QString paramKey;				//��������@Drill_LE_param��
		QString parentKey;				//���������@Drill_LE_parentKey��
		QString var;					//��������@Drill_LE_var��
		int varLen;						//�������ֵ������� var xxx_length = �����ֵ��
		int realLen;					//ʵ�����ֵ�������� @param �ҵ������ֵ��
	public:
										//���� - ��ʼ��
		void initParam(QString param, QString parentKey, QString var);
										//��ȡ - ������ "�׶�-%d"
		QString getParamKey();
										//��ȡ - ������� "--�׶���%d��%d--"
		QString getParentKey();
										//��ȡ - ������ "DrillUp.g_SSpE_list_length"
		QString getVarName();

										//���� - ���� �������ֵ����ȡʱ��Ҫ��θ�ֵ��
		void setVarLen(int len);
										//���� - ���� ʵ�����ֵ����ȡʱ��Ҫ��θ�ֵ��
		void setRealLen(int len);
										//��ȡ - �������ֵ
		int getVarLen();
										//��ȡ - ʵ�����ֵ
		int getRealLen();

										//��ȡ - ������ʾ�� "�׶�"
		QString getParamShowingName();
										//��ȡ - ������ "�׶�-210"����1��ʼ������
		QString getParamName(int index);
										//��ȡ - �������� "--�׶���21��40--"����1��ʼ������
		QString getParentName(int index);
										//��ȡ - ����ע���� "@param[ ]*�׶�-210$"��ȫ��ƥ���ã���1��ʼ������
		QRegExp getParamCommentRe(int index);
										//��ȡ - ����ע������ "@param[ ]*--�׶���21��40--$"��ȫ��ƥ���ã���1��ʼ������
		QRegExp getParentCommentRe(int index);
										//��ȡ - �Ƿ����
		bool isParentGrouping();
};
/*
-----==========================================================-----
		�ࣺ		�����Ϣ.h��ֻ����
		����ģ�飺	����ģ��
		���ܣ�		����Ĺ̶���Ϣ���ݡ�
-----==========================================================-----
*/
class C_LEAnnotation{
	public:
		C_LEAnnotation();
		~C_LEAnnotation();
	public:
		QString pluginName;						//�����
		QString pluginDesc;						//�����飨@plugindesc��
		QString pluginAuthor;					//������ߣ�@author��
		QList<C_LEAnnotation_Param> paramList;	//�������
		bool paramForbidden;					//��ֹ�༭��@Drill_LE_editForbidden��

		QString fullPath;					//����ļ�·��
		QString context;					//����ı�
		QString message;					//�����Ϣ�������ã���ȡ����ʾ���ַ�����
		QString editMessage;				//����޸���Ϣ�������ã��༭����ʾ�������ַ�����
	public:
											//���ݱ�������ȡ����
		C_LEAnnotation_Param getParamByKey(QString paramKey);
											//�ж� - ���ж�
		bool isNull();
											//�ж� - �Ƿ񱻽���
		bool isForbidden();
											//�ж� - �Ƿ�ɱ༭
		bool isEditable();
											//�ж� - �����Լ��
		bool isIntegrity();
};
