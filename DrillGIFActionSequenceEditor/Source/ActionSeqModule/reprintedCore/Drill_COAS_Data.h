#ifndef Drill_COAS_Data_H
#define Drill_COAS_Data_H

#include <QtWidgets>

/*
-----==========================================================-----
		�ࣺ		�����������ݡ������ࡿ.h
		���ߣ�		drill_up
		����ģ�飺	��������ģ��
		���ܣ�		�������к��Ĳ���ĸ����ࡣ
					����ϸ��cpp��
-----==========================================================-----
*/
class Drill_COAS_Data{

	public:
		Drill_COAS_Data();
		Drill_COAS_Data(QJsonObject data);
		~Drill_COAS_Data();
		
	//-----------------------------------
	//----����
	public:
		QJsonObject _drill_data;			
		int			_drill_time;					//����ʱ��
		bool		_drill_arrayCheck;				//�������Ԫ��
		QString		_drill_bitmapName;				//��ǰ��bitmap������
		QString		_drill_bitmapPath;				//��ǰ��bitmap·��
		int			_drill_bitmapTint;				//��ǰ��bitmapɫ��
		bool		_drill_bitmapSmooth;			//��ǰ��bitmapģ��
		QString		_drill_state_curCom;			//״̬Ԫ - ��ǰ״̬
		int			_drill_state_curTime;			//״̬Ԫ - ��ǰʱ��
		QStringList _drill_state_curSeq;			//״̬Ԫ - ��ǰ����
		QString		_drill_state_lastAnnotation;	//״̬Ԫ - ��һ��ע����
		QString		_drill_act_curCom;				//����Ԫ - ��ǰ����
		int			_drill_act_curTime;				//����Ԫ - ��ǰʱ��
	public:
									//���� - ��ʼ��
		void drill_initData();
									//���� - ���ж�
		bool isNull();

	//-----------------------------------
	//----֡ˢ��
	public:
									//���� - ֡ˢ�£���Ҫ�����ֶ�ִ�У�
		void update();
									//֡ˢ�� - ˢ��״̬Ԫ
		void drill_COAS_updateState();
									//���� - ��ȡ�µ�״̬Ԫ
		void drill_COAS_rollCurrentState();
									//֡ˢ�� - ˢ�¶���Ԫ
		void drill_COAS_updateAct();

	//-----------------------------------
	//----�ӿ�
	public:
									//���� - ״̬Ԫ - �������У��ӿڣ�
		void drill_COAS_setSequence(QStringList seq);
									//���� - ״̬Ԫ - �������У����̸ı䣨�ӿڣ�
		void drill_COAS_setSequenceImmediate(QStringList seq);
									//���� - ״̬Ԫ - ����״̬Ԫ����[ע��ģʽ]���ӿڣ�
		bool drill_COAS_setSequenceByAnnotation(QString annotation);
									//���� - ״̬Ԫ - ����״̬Ԫ����[ע��ģʽ]-���̸ı䣨�ӿڣ�
		bool drill_COAS_setSequenceImmediateByAnnotation(QString annotation);
									//���� - ״̬Ԫ - ��ȡĬ��״̬Ԫ���ϣ��ӿڣ�
		QStringList drill_COAS_getDefaultStateGroup();
									//���� - ״̬Ԫ - ��ȡ��ǰ״̬Ԫ���ƣ��ӿڣ�
		QString drill_COAS_getCurrentStateName();
									//���� - ״̬Ԫ - ��ȡ��ǰ״̬Ԫ�������ƣ��ӿڣ�
		QStringList drill_COAS_getCurrentStateSeqName();
									//���� - ״̬Ԫ - ��ȡȫ��״̬Ԫ���ƣ��ӿڣ�
		QStringList drill_COAS_getAllStateName();
									//���� - ״̬Ԫ - ��ȡ���� ��������
		QJsonObject drill_COAS_getDataState(QString state_name);

									//���� - ����Ԫ - �жϲ���
		bool drill_COAS_isPlayingAct();
									//���� - ����Ԫ - ��Ӷ������ӿڣ�
		void drill_COAS_setAct(QString act_name);
									//���� - ����Ԫ - ������ֹ�������ӿڣ�
		void drill_COAS_stopAct();
									//���� - ����Ԫ - ��ȡ��ǰ����Ԫ���ƣ��ӿڣ�
		QString drill_COAS_getCurrentActName();
									//���� - ����Ԫ - ��ȡȫ������Ԫ���ƣ��ӿڣ�
		QStringList drill_COAS_getAllActName();
									//���� - ����Ԫ - ��ȡ���� ��������
		QJsonObject drill_COAS_getDataAct(QString act_name);

};

#endif // Drill_COAS_Data_H
