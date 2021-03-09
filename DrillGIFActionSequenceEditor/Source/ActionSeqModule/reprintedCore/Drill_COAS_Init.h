#ifndef Drill_COAS_Init_H
#define Drill_COAS_Init_H

#include <QtWidgets>

/*
-----==========================================================-----
		�ࣺ		������ȡ�������ࡿ.h
		���ߣ�		drill_up
		����ģ�飺	��������ģ��
		���ܣ�		�������к��Ĳ���ĸ����ࡣ
					����ϸ��cpp��
-----==========================================================-----
*/
class Drill_COAS_Init : public QObject
{
	Q_OBJECT

	public:
		Drill_COAS_Init(QObject *parent = 0);
		~Drill_COAS_Init();
		
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

	//-----------------------------------
	//----֡ˢ��
	public:
									//���� - ֡ˢ�£���Ҫ�����ֶ�ִ�У�
		void update();

};

#endif // Drill_COAS_Init_H
