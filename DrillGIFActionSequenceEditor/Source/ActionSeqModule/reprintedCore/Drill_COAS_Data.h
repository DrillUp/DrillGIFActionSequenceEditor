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
		int			_drill_time;			//����ʱ��
		bool		_drill_arrayCheck;		//�������Ԫ��
		QString		_drill_bitmapName;		//��ǰ��bitmap������
		QString		_drill_bitmapPath;		//��ǰ��bitmap·��
		int			_drill_bitmapTint;		//��ǰ��bitmapɫ��
		bool		_drill_bitmapSmooth;	//��ǰ��bitmapģ��
		QString		_drill_state_curCom;	//״̬Ԫ - ��ǰ״̬
		int			_drill_state_curTime;	//״̬Ԫ - ��ǰʱ��
		QStringList _drill_state_curSeq;	//״̬Ԫ - ��ǰ����
		QString		_drill_act_curCom;		//����Ԫ - ��ǰ����
		int			_drill_act_curTime;		//����Ԫ - ��ǰʱ��
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
									//֡ˢ�� - ˢ�¶���Ԫ
		void drill_COAS_updateAct();

	//-----------------------------------
	//----�ӿ�
	public:
									//���� - ״̬Ԫ - �������У��ӿڣ�
		void drill_COAS_setSequence(QStringList seq);
									//���� - ״̬Ԫ - �������У����̸ı䣨�ӿڣ�
		void drill_COAS_setSequenceImmediate(QStringList seq);
									//���� - ״̬Ԫ - ��ȡ���� ��������
		QJsonObject drill_COAS_getDataState(QString state_name);
									//���� - ����Ԫ - �жϲ���
		bool drill_COAS_isPlayingAct();
									//���� - ����Ԫ - ��Ӷ������ӿڣ�
		void drill_COAS_setAct(QString act_name);
									//���� - ����Ԫ - ������ֹ�������ӿڣ�
		void drill_COAS_stopAct();
									//���� - ����Ԫ - ��ȡ���� ��������
		QJsonObject drill_COAS_getDataAct(QString act_name);

};

#endif // Drill_COAS_Data_H
