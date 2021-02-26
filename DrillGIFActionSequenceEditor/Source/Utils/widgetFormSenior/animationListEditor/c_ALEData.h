#pragma once
#include <QList>

/*
-----==========================================================-----
		�ࣺ		����֡ ������.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		����֡ �������ࡣ
					ע�⣬ֻ�����������������ݺϲ�����취����һ���ࡣ
					����ϸ��cpp��
-----==========================================================-----
*/
class C_ALEData {

	public:
		C_ALEData();
		~C_ALEData();
		
	//-----------------------------------
	//----����
	protected:
		int id;								//��ʶ
		QList<QString> gif_src;				//��Դ�ļ���
		QString gif_src_file;				//��Դ�ļ���
		QList<int> gif_intervalTank;		//֡���-��ϸ��
		int gif_interval;					//֡���
		
	//-----------------------------------
	//----����
	public:
											//���� - ��ʶ
		void setId(int id);

											//���� - ��Դ
											//		������1�����ļ���Ŀ¼
											//		������2���������ļ���׺���ļ�������
		void setSource(QString gif_src_file, QList<QString> gif_src);
											//���� - ֻ�����ļ���Ŀ¼
		void setSourceParentFile(QString gif_src_file);

											//���� - ֡���
		void setInterval(int gif_interval, QList<int> gif_intervalTank);
											//���� - Ĭ��֡���
		void setIntervalDefault(int gif_interval);
											//���� - Ĭ��֡�����ͳһ�ı䣩
		void setIntervalDefaultAndChange(int gif_interval);

	protected:
		void checkInterval();

	//-----------------------------------
	//----��ȡ
	public:
											//��ȡ - ��ȡ�ļ�����
		int getFileCount();
											//��ȡ - ��ȡ�ļ�
		QFileInfo getFile(int index);
		QList<QFileInfo> getFileList(QList<int> index_list);
		QList<QFileInfo> getAllFile();
											//��ȡ - ��ȡ�ļ�·����F:/aaa/vvv ��
		QString getFileRoot();

											//��ȡ - ��ȡ֡���
		QList<int> getAllInterval();
											//��ȡ - ��ȡ֡����ı���"0.01"��ʵ��֡Ϊ 0.01666 * n ��
		QString getIntervalString(int index);

											//��ȡ - ��ȡ��ʶ
		int getId();
											//��ȡ - ����ļ��������ļ���׺��
		bool hasFileName(QString file_name);
		

	//-----------------------------------
	//----����
	public:
									//���� - ���
		void op_append(QString gif_src);
		void op_insert(int index, QString gif_src);
		void op_insert(int index, QStringList gif_src_list, QList<int> interval_list = QList<int>());
									//���� - �滻
		void op_replace(int index, QString gif_src);
		void op_replace_interval(int index, int interval);
		void op_replace_interval(QList<int> index, int interval);
									//���� - �Ƴ�
		void op_remove(int index);
									//���� - ����λ��
		void op_swap(int index_a, int index_b);
	public:
									//�����ļ�
		static bool copyFile(QString filePath_from, QString filePath_to);

	//-----------------------------------
	//----������
	public:
									//���ж�
		bool isNull();
									//ʵ���� -> QJsonObject
		QJsonObject getJsonObject();
									//QJsonObject -> ʵ����
		void setJsonObject(QJsonObject obj);
};