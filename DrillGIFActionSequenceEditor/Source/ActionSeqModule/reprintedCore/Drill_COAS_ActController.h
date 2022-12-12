#ifndef Drill_COAS_ActController_H
#define Drill_COAS_ActController_H

#include <QtWidgets>

/*
-----==========================================================-----
		类：		动作元控制器【复刻类】.h
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		> 定义一个专门控制 动作元 的数据类。
					> 该类可被存到存档中。
					（详细见cpp）
-----==========================================================-----
*/
class Drill_COAS_ActController{

	public:
		Drill_COAS_ActController();
		Drill_COAS_ActController(QJsonObject data);
		~Drill_COAS_ActController();
		
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
		void drill_initData_Act();
									//数据 - 私有数据初始化
		void drill_initPrivateData_Act();
									//数据 - 空判断
		bool isNull();

	//-----------------------------------
	//----标准函数
	public:
									//动作元 - 帧刷新【标准函数】
									//		【说明】：需要父类手动执行。
		void drill_COAS_update();
									//动作元 - 重设数据【标准函数】
		void drill_COAS_resetData_Act(QJsonObject data);

	//-----------------------------------
	//----开放函数
	public:
									//动作元 - 输出数据 - 当前的对象名【开放函数】
		QString drill_COAS_curBitmapName();
									//动作元 - 输出数据 - 当前的路径【开放函数】
		QString drill_COAS_curBitmapPath();
									//动作元 - 输出数据 - 当前的色调【开放函数】
		int drill_COAS_curBitmapTint();
									//动作元 - 输出数据 - 当前的模糊【开放函数】
		bool drill_COAS_curBitmapSmooth();
									//动作元 - 节点 - 是否结束播放【开放函数】
		bool drill_COAS_isActEnd();

	//-----------------------------------
	//----动作元
	public:
									//动作元 - 帧刷新动作元
		void drill_COAS_updateAct();

};

#endif // Drill_COAS_ActController_H
