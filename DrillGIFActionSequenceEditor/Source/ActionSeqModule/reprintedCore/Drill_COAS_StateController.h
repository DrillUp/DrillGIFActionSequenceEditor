#ifndef Drill_COAS_StateController_H
#define Drill_COAS_StateController_H

#include <QtWidgets>

/*
-----==========================================================-----
		类：		状态元控制器【复刻类】.h
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		> 定义一个专门控制 状态元 的数据类。
					> 该类可被存到存档中。
					（详细见cpp）
-----==========================================================-----
*/
class Drill_COAS_StateController{

	public:
		Drill_COAS_StateController();
		Drill_COAS_StateController(QJsonObject data);
		~Drill_COAS_StateController();
		
	//-----------------------------------
	//----数据
	public:
		QJsonObject _drill_data;			
		QString		_drill_controllerSerial;

		int			_drill_curTime;					//常规 - 当前时间
		bool		_drill_needDestroy;				//常规 - 销毁

		QString		_drill_curBitmapName;			//输出数据 - 当前的对象名
		QString		_drill_curBitmapPath;			//输出数据 - 当前的路径
		int			_drill_curBitmapTint;			//输出数据 - 当前的色调
		bool		_drill_curBitmapSmooth;			//输出数据 - 当前的模糊
		
		int			_drill_curTickTime;				//播放 - 当前累计时间
		int			_drill_curIndex;				//播放 - 当前索引
		int			_drill_tarIndex;				//播放 - 索引结束位置

		QJsonArray	_drill_curIntervalTank;			//帧间隔列表

	public:
									//数据 - 初始化数据【标准默认值】
		void drill_initData_State();
									//数据 - 私有数据初始化
		void drill_initPrivateData_State();
									//数据 - 空判断
		bool isNull();

	//-----------------------------------
	//----标准函数
	public:
									//状态元 - 帧刷新【标准函数】
									//		【说明】：需要父类手动执行。
		void drill_COAS_update();
									//状态元 - 重设数据【标准函数】
		void drill_COAS_resetData_State(QJsonObject data);

	//-----------------------------------
	//----开放函数
	public:
									//状态元 - 输出数据 - 当前的对象名【开放函数】
		QString drill_COAS_curBitmapName();
									//状态元 - 输出数据 - 当前的对象名【开放函数】
		QString drill_COAS_curBitmapPath();
									//状态元 - 输出数据 - 当前的色调【开放函数】
		int drill_COAS_curBitmapTint();
									//状态元 - 输出数据 - 当前的模糊【开放函数】
		bool drill_COAS_curBitmapSmooth();
									//状态元 - 节点 - 当前状态元名称【开放函数】
		QString drill_COAS_getCurStateName();
									//状态元 - 节点 - 当前状态元优先级【开放函数】
		int drill_COAS_getCurStatePriority();
									//状态元 - 节点 - 是否结束播放【开放函数】
		bool drill_COAS_isStateEnd();

	//-----------------------------------
	//----状态元
	public:
									//状态元 - 帧刷新状态元
		void drill_COAS_updateState();

};

#endif // Drill_COAS_StateController_H
