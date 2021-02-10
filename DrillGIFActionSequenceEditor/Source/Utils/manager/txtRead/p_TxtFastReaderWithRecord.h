#pragma once

#include "p_TxtFastReader.h"

/*
-----==========================================================-----
		�ࣺ		�ı����ٶ�ȡ������¼�У�.h
		�汾��		v1.10
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��ȡ �ı��ļ������ݣ�����������
					���ɶԶ�ƪ�ı��������ٶȿ죨���ȿ������ܣ���
					����ϸ��cpp��
-----==========================================================-----
*/

class P_TxtFastReaderWithRecord : public P_TxtFastReader
{

	//-----------------------------------
	//----�����ṹ
	public:
		P_TxtFastReaderWithRecord(QString context);
		~P_TxtFastReaderWithRecord();
		
	//-----------------------------------
	//----����
	private:
		QSet<int> m_recorded_list;		//log���ַ��������ģ�
		
	public:
										//���� - ��ȡȫ���У�����ɫ��ǣ�
		QStringList getAllRows_FillUsedColor();

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
		
		

};

