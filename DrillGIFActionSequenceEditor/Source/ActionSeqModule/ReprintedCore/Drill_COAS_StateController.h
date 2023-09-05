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

		int			_drill_curTime;					//A主体 - 当前时间
		bool		_drill_needDestroy;				//A主体 - 销毁

		QString		_drill_curBitmapName;			//B输出数据 - 当前的资源名
		
		int			_drill_curTickTime;				//C播放GIF - 当前累计时间
		int			_drill_curIndex;				//C播放GIF - 当前索引
		int			_drill_tarIndex;				//C播放GIF - 索引结束位置
		QJsonArray	_drill_curIntervalTank;			//C播放GIF - 帧间隔列表
		

	//-----------------------------------
	//----状态元
	public:
									//状态元 - 初始化数据【标准默认值】
		void drill_controllerState_initData();
									//状态元 - 初始化子功能
		void drill_controllerState_initChild();
									//状态元 - 帧刷新【标准函数】
									//		【说明】：需要父类手动执行。
		void drill_controllerState_update();
									//状态元 - 重设数据【标准函数】
		void drill_controllerState_resetData(QJsonObject data);
									//状态元 - 空判断
		bool isNull();

	//-----------------------------------
	//----开放函数
	public:
									//A主体 - 当前名称【开放函数】
		QString drill_controllerState_getName();
									//A主体 - 当前优先级【开放函数】
		int drill_controllerState_getPriority();
									//A主体 - 可被动作元打断【开放函数】
		int drill_controllerState_canBeInterrupted();

									//B输出数据 - 当前的资源名【开放函数】
		QString drill_controllerState_curBitmapName();
									//B输出数据 - 当前的路径【开放函数】
		QString drill_controllerState_curBitmapPath();
									//B输出数据 - 当前的色调【开放函数】
		int drill_controllerState_curBitmapTint();
									//B输出数据 - 当前的模糊【开放函数】
		bool drill_controllerState_curBitmapSmooth();

									//C播放GIF - 是否结束播放【开放函数】
		bool drill_controllerState_isEnd();
									//C播放GIF - 重置播放【开放函数】
		void drill_controllerState_resetTimer();
									//C播放GIF - 设置指定帧【开放函数】
		void drill_controllerState_setCurIndex(int index);
		

	//-----------------------------------
	//----A主体
	public:
									//A主体 - 初始化子功能
		void drill_controllerState_initAttr();
									//A主体 - 帧刷新
		void drill_controllerState_updateAttr();

	//-----------------------------------
	//----B输出数据
	public:
									//B输出数据 - 初始化子功能
		void drill_controllerState_initBitmapParam();

	//-----------------------------------
	//----C播放GIF
	public:
									//C播放GIF - 初始化子功能
		void drill_controllerState_initGIF();
									//C播放GIF - 设置指定帧
		void drill_controllerState_setCurIndex_Private(int index);
									//C播放GIF - 帧刷新（状态元）
		void drill_controllerState_updateGIF();

};

#endif // Drill_COAS_StateController_H
