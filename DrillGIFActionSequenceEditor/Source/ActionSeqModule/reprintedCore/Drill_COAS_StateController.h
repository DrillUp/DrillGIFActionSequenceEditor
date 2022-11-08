#ifndef Drill_COAS_StateController_H
#define Drill_COAS_StateController_H

#include <QtWidgets>

/*
-----==========================================================-----
		�ࣺ		״̬Ԫ�������������ࡿ.h
		���ߣ�		drill_up
		����ģ�飺	��������ģ��
		���ܣ�		> ����һ��ר�ſ��� ״̬Ԫ �������ࡣ
					> ����ɱ��浽�浵�С�
					����ϸ��cpp��
-----==========================================================-----
*/
class Drill_COAS_StateController{

	public:
		Drill_COAS_StateController();
		Drill_COAS_StateController(QJsonObject data);
		~Drill_COAS_StateController();
		
	//-----------------------------------
	//----����
	public:
		QJsonObject _drill_data;			
		QString		_drill_controllerSerial;

		int			_drill_curTime;					//���� - ��ǰʱ��
		bool		_drill_needDestroy;				//���� - ����

		QString		_drill_curBitmapName;			//������� - ��ǰ�Ķ�����
		QString		_drill_curBitmapPath;			//������� - ��ǰ��·��
		int			_drill_curBitmapTint;			//������� - ��ǰ��ɫ��
		bool		_drill_curBitmapSmooth;			//������� - ��ǰ��ģ��
		
		int			_drill_curTickTime;				//���� - ��ǰ�ۼ�ʱ��
		int			_drill_curIndex;				//���� - ��ǰ����
		int			_drill_tarIndex;				//���� - ��������λ��

		QJsonArray	_drill_curIntervalTank;			//֡����б�

	public:
									//���� - ��ʼ������
		void drill_initData_State();
									//���� - ˽�����ݳ�ʼ��
		void drill_initPrivateData_State();
									//���� - ���ж�
		bool isNull();

	//-----------------------------------
	//----֡ˢ��
	public:
									//���� - ֡ˢ�£���Ҫ�����ֶ�ִ�У�
		void drill_COAS_update();
									//֡ˢ�� - ˢ��״̬Ԫ
		void drill_COAS_updateState();

	//-----------------------------------
	//----��׼����
	public:
									//���� - ״̬Ԫ - �������У��ӿڣ�
		void drill_COAS_setSequence(QStringList seq);

};

#endif // Drill_COAS_StateController_H
