#ifndef Drill_COAS_Init_H
#define Drill_COAS_Init_H

#include <QJsonObject>

/*
-----==========================================================-----
		类：		变量获取【复刻类】.h
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		动画序列核心插件的复刻类。
					（详细见cpp）
-----==========================================================-----
*/
class Drill_COAS_Init{

	public:
		Drill_COAS_Init();
		~Drill_COAS_Init();

	//-----------------------------------
	//----变量获取
	public:
									//变量获取 - 状态元（~struct~DrillCOASState）
		static QJsonObject drill_COAS_initState(QJsonObject dataFrom);
									//变量获取 - 动作元（~struct~DrillCOASAct）
		static QJsonObject drill_COAS_initAct(QJsonObject dataFrom);
									//变量获取 - 动画序列（~struct~DrillCOASSequence）
		static QJsonObject drill_COAS_initSequence(QJsonObject dataFrom);

};

#endif // Drill_COAS_Init_H
