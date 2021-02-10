#pragma once

#include <QObject>

/*
-----==========================================================-----
		�ࣺ		�ı����ٶ�ȡ��.h
		�汾��		v1.10
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��ȡ �ı��ļ������ݣ�����������
					���ɶԶ�ƪ�ı��������ٶȿ죨���ȿ������ܣ���
					����ϸ��cpp��
-----==========================================================-----
*/

class P_TxtFastReader
{

	//-----------------------------------
	//----�����ṹ
	public:
		P_TxtFastReader(QString context);
		~P_TxtFastReader();
		
	//-----------------------------------
	//----����
	protected:
		QStringList log_context_list;	//log���ַ��������ģ�
	public:
										//���� - ��ȡȫ����
		QStringList getAllRows();

	//-----------------------------------
	//----ȫ��
	protected:
		bool m_trim;
		QStringList m_replace_strFrom;
		QStringList m_replace_strTo;
		QList<QRegExp> m_replace_expFrom;
		QStringList m_replace_expTo;
	public:
										//ȫ�� - ����ո�Ԥ����
		void prepare_trimAllRows();
										//ȫ�� - �滻�ַ���Ԥ����
		void prepare_replaceInStrings(QString re_str, QString str);
		void prepare_replaceInStrings(QRegExp re, QString str);
										//ȫ�� - �������
		void clearEmptyRows();
										//ȫ�� - ȥ��ע���У��������ַ���
		void clearComment(QString prefix);

	protected:
										//ȫ�� - ִ��Ԥ������
		QString doPrepare(QString str);

	//-----------------------------------
	//----ֱ�Ӳ�����directly��
	public:
										//ֱ�� - ��i��
		virtual QString d_rowAt(int i);
										//ֱ�� - ��ȡȫ������
		virtual int d_rowCount();
										//ֱ�� - ��ȡ������
										//		������1���������������ַ���
										//		�����ء�������ֵ��δ�ҵ�����-1��
		virtual int d_indexOf(QString contains);
		virtual int d_indexOf(QRegExp re_contains);
										//ֱ�� - ��ȡ�����У���ĩβ�ң�
										//		������1���������������ַ���
										//		�����ء�������ֵ��δ�ҵ�����-1��
		virtual int d_lastIndexOf(QString contains);
		virtual int d_lastIndexOf(QRegExp re_contains);
										//ֱ�� - ��ȡ�����У�ָ����Χ��
										//		������1����ʼ��
										//		������2����ֹ��
										//		�����ء�����ֵ��δ�ҵ�����-1��
		virtual int d_indexOf(QString contains, int n, int m = -1);
		virtual int d_indexOf(QRegExp re_contains, int n, int m = -1);
										//ֱ�� - ��ȡ�������µ�������
										//		������1����ʼ��
										//		������2������
										//		�����ء����б�δ�ҵ�/��곬�������� ���ؿ��б�
		virtual QStringList d_getRows(int n, int row_count = -1);
										//ֱ�� - ��ȡ������������
										//		������1�������������ַ���
										//		�����ء����а������У�δ�ҵ����ؿ��б�
		virtual QList<int> d_getAllRowIndexsContains(QString contains);
		virtual QList<int> d_getAllRowIndexsContains(QRegExp re_contains);
		virtual QStringList d_getAllRowsContains(QString contains);
		virtual QStringList d_getAllRowsContains(QRegExp re_contains);
		
		
	//-----------------------------------
	//----�ַ���
	public:
										//�ַ��� - ��ȡ����������եȡ�� 1.0e3 �����������пո�ű���ȡ��
										//		������1�������ַ���
										//		�����ء����б�
		QList<double> s_getDoubleInRow(QString row);
										//�ַ��� - ��ȡ��������եȡ��
		QList<double> s_getDoubleForceInRow(QString row);
										//�ַ��� - ��ȡ��������եȡ�� 23 �����������пո�ű���ȡ��
										//		������1�������ַ���
										//		�����ء����б�
		QList<int> s_getIntInRow(QString row);
										//�ַ��� - ��ȡ������եȡ��
		QList<int> s_getIntForceInRow(QString row);

};

