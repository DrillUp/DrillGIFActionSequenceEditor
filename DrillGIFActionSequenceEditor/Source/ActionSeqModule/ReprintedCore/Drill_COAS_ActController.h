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

		int			_drill_curTime;					//A主体 - 当前时间
		bool		_drill_needDestroy;				//A主体 - 销毁

		QString		_drill_curBitmapName;			//B输出数据 - 当前的资源名
		
		int			_drill_curTickTime;				//C播放GIF - 当前累计时间
		int			_drill_curIndex;				//C播放GIF - 当前索引
		int			_drill_tarIndex;				//C播放GIF - 索引结束位置
		QJsonArray	_drill_curIntervalTank;			//C播放GIF - 帧间隔列表
		
	//-----------------------------------
	//----动作元
	public:
									//动作元 - 初始化数据【标准默认值】
		void drill_controllerAct_initData();
									//动作元 - 初始化子功能
		void drill_controllerAct_initChild();
									//动作元 - 帧刷新【标准函数】
									//		【说明】：需要父类手动执行。
		void drill_controllerAct_update();
									//动作元 - 重设数据【标准函数】
		void drill_controllerAct_resetData(QJsonObject data);
									//动作元 - 空判断
		bool isNull();

	//-----------------------------------
	//----开放函数
	public:
									//A主体 - 当前名称【开放函数】
		QString drill_controllerAct_getName();
									//A主体 - 当前优先级【开放函数】
		int drill_controllerAct_getPriority();

									//B输出数据 - 当前的资源名【开放函数】
		QString drill_controllerAct_curBitmapName();
									//B输出数据 - 当前的路径【开放函数】
		QString drill_controllerAct_curBitmapPath();
									//B输出数据 - 当前的色调【开放函数】
		int drill_controllerAct_curBitmapTint();
									//B输出数据 - 当前的模糊【开放函数】
		bool drill_controllerAct_curBitmapSmooth();

									//C播放GIF - 是否结束播放【开放函数】
		bool drill_controllerAct_isEnd();
									//C播放GIF - 重置播放【开放函数】
		void drill_controllerAct_resetTimer();
									//C播放GIF - 设置指定帧【开放函数】
		void drill_controllerAct_setCurIndex(int index);
		

	//-----------------------------------
	//----A主体
	public:
									//A主体 - 初始化子功能
		void drill_controllerAct_initAttr();
									//A主体 - 帧刷新
		void drill_controllerAct_updateAttr();

	//-----------------------------------
	//----B输出数据
	public:
									//B输出数据 - 初始化子功能
		void drill_controllerAct_initBitmapParam();

	//-----------------------------------
	//----C播放GIF
	public:
									//C播放GIF - 初始化子功能
		void drill_controllerAct_initGIF();
									//C播放GIF - 设置指定帧
		void drill_controllerAct_setCurIndex_Private(int index);
									//C播放GIF - 帧刷新（动作元）
		void drill_controllerAct_updateGIF();

};

#endif // Drill_COAS_ActController_H
