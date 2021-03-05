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
	public:
		enum DATA_UNIT{	
			FrameUnit,		//֡��λ��1��60֡��
			SecondUnit,		//�뵥λ��1��100֡��
		};
	protected:
		int id;								//��ʶ
		DATA_UNIT m_unit;					//��λ
		QList<QString> gif_src;				//��Դ�ļ���
		QString gif_src_file;				//��Դ�ļ���
		QList<int> gif_intervalTank;		//֡���-��ϸ��
		int gif_interval;					//֡���
		
	//-----------------------------------
	//----����
	public:
											//������ - ���ñ�ʶ
		void setData_Id(int id);
											//������ - �����ļ���Ŀ¼
		void setData_ParentFile(QString gif_src_file);
											//������ - ����Ĭ��֡���
		void setData_IntervalDefault(int gif_interval);
											//������ - ���õ�λ
											//		��˵���������ݵ�λ�Ա༭����Ϊ׼����λ�仯����Ӱ��ʵ�ʴ洢��֡�����
		void setData_Unit(C_ALEData::DATA_UNIT unit);

											//�ӿ� - ������Դ
											//		������1�����ļ���Ŀ¼
											//		������2���������ļ���׺���ļ�������
		void setSource(QString gif_src_file, QList<QString> gif_src);

											//�ӿ� - ����֡���
											//		������1����Ĭ��֡���
											//		������2����֡��� ��ϸ��
		void setInterval(int gif_interval, QList<int> gif_intervalTank);
											//�ӿ� - ����Ĭ��֡���
											//		����������Ĭ��֡���
											//		��˵��������ԭĬ��֡���һ����ֵ���ᱻͳһ�ı䡣
		void setIntervalDefaultWithFit(int gif_interval);


	protected:
		void checkInterval();
		void checkIntervalValue();

	//-----------------------------------
	//----��ȡ
	public:
											//������ - ��ȡ��ʶ
		int getData_Id();
											//������ - ��ȡĬ��֡���
		int getData_IntervalDefault();
											//������ - ��ȡ֡�����ϸ��
		QList<int> getData_IntervalTank();
											//������ - ��λ
											//		��˵���������ݵ�λ�Ա༭����Ϊ׼����λ�仯����Ӱ��ʵ�ʴ洢��֡�����
		C_ALEData::DATA_UNIT getData_Unit();

											//�ӿ� - ��ȡ�ļ�
		QFileInfo getFile(int index);
											//�ӿ� - ��ȡ�ļ��������
		QList<QFileInfo> getFile_Multi(QList<int> index_list);
											//�ӿ� - ��ȡȫ���ļ�
		QList<QFileInfo> getAllFile();
											//�ӿ� - ��ȡ�ļ�����
		int getFileCount();
											//�ӿ� - ��ȡ�ļ�·����F:/aaa/vvv ��
		QString getFileRoot();
											//�ӿ� - ����ļ��������ļ���׺��
		bool hasFileName(QString file_name);

											//�ӿ� - ��ȡĬ��֡���������λת����
		double getIntervalDefaultWithUnit();
											//�ӿ� - ��ȡ֡���������λת����
		double getIntervalWithUnit(int index);
		QList<double> getIntervalWithUnit_Multi(QList<int> index_list);
											//�ӿ� - ��ȡ֡�����ϸ������λת����
		QList<double> getIntervalTankWithUnit();
											//�ӿ� - ��ȡ֡����ı���"0.01"��ʵ��֡Ϊ 0.01666 * n ��
		QString getIntervalString(int index);

	protected:
		double intervalUnitTransform(int interval);
		

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