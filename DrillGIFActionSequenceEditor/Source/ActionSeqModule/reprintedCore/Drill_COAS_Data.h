#ifndef Drill_COAS_Data_H
#define Drill_COAS_Data_H

#include <QtWidgets>

/*
-----==========================================================-----
		类：		动画序列数据【复刻类】.h
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		动作序列核心插件的复刻类。
					（详细见cpp）
-----==========================================================-----
*/
class Drill_COAS_Data : public QObject
{
	Q_OBJECT

	public:
		Drill_COAS_Data(QJsonObject data, QObject *parent = 0);
		~Drill_COAS_Data();
		
	//-----------------------------------
	//----数据
	public:
		QJsonObject _drill_data;			
		int			_drill_time;			//持续时间
		bool		_drill_arrayCheck;		//检查数组元素
		QString		_drill_bitmapName;		//当前的bitmap对象名
		QString		_drill_bitmapPath;		//当前的bitmap路径
		int			_drill_bitmapTint;		//当前的bitmap色调
		bool		_drill_bitmapSmooth;	//当前的bitmap模糊
		QString		_drill_state_curCom;	//状态元 - 当前状态
		int			_drill_state_curTime;	//状态元 - 当前时间
		QStringList _drill_state_curSeq;	//状态元 - 当前序列
		QString		_drill_act_curCom;		//动作元 - 当前动作
		int			_drill_act_curTime;		//动作元 - 当前时间
	public:
									//创建 - 初始化
		void drill_initData();

	//-----------------------------------
	//----帧刷新
	public:
									//数据 - 帧刷新（需要父类手动执行）
		void update();

};

#endif // Drill_COAS_Data_H
