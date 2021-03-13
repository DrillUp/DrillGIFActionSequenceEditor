#ifndef Drill_COAS_Init_H
#define Drill_COAS_Init_H

#include <QJsonObject>

/*
-----==========================================================-----
		�ࣺ		������ȡ�������ࡿ.h
		���ߣ�		drill_up
		����ģ�飺	��������ģ��
		���ܣ�		�������к��Ĳ���ĸ����ࡣ
					����ϸ��cpp��
-----==========================================================-----
*/
class Drill_COAS_Init{

	public:
		Drill_COAS_Init();
		~Drill_COAS_Init();

	//-----------------------------------
	//----������ȡ
	public:
									//������ȡ - ״̬Ԫ��~struct~DrillCOASState��
		static QJsonObject drill_COAS_initState(QJsonObject dataFrom);
									//������ȡ - ����Ԫ��~struct~DrillCOASAct��
		static QJsonObject drill_COAS_initAct(QJsonObject dataFrom);
									//������ȡ - �������У�~struct~DrillCOASSequence��
		static QJsonObject drill_COAS_initSequence(QJsonObject dataFrom);

};

#endif // Drill_COAS_Init_H
