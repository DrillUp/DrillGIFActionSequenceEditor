#ifndef Drill_COAS_StateNodeController_H
#define Drill_COAS_StateNodeController_H

#include <QtWidgets>

/*
-----==========================================================-----
		类：		动画序列数据【复刻类】.h
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		动画序列核心插件的复刻类。
					（详细见cpp）
-----==========================================================-----
*/
class Drill_COAS_StateNodeController{

	public:
		Drill_COAS_StateNodeController();
		Drill_COAS_StateNodeController(QJsonObject data);
		~Drill_COAS_StateNodeController();
		
	//-----------------------------------
	//----数据
	public:
		QJsonObject _drill_data;			
		int			_drill_time;					//持续时间
		bool		_drill_arrayCheck;				//检查数组元素
		QString		_drill_bitmapName;				//当前的bitmap对象名
		QString		_drill_bitmapPath;				//当前的bitmap路径
		int			_drill_bitmapTint;				//当前的bitmap色调
		bool		_drill_bitmapSmooth;			//当前的bitmap模糊
		QString		_drill_state_curCom;			//状态元 - 当前状态
		int			_drill_state_curTime;			//状态元 - 当前时间
		QStringList _drill_state_curSeq;			//状态元 - 当前序列
		QString		_drill_state_lastAnnotation;	//状态元 - 上一个注解名
		QString		_drill_act_curCom;				//动作元 - 当前动作
		int			_drill_act_curTime;				//动作元 - 当前时间
	public:
									//创建 - 初始化
		void drill_initData();
									//数据 - 空判断
		bool isNull();

	//-----------------------------------
	//----帧刷新
	public:
									//数据 - 帧刷新（需要父类手动执行）
		void update();
									//帧刷新 - 刷新状态元
		void drill_COAS_updateState();
									//操作 - 抽取新的状态元
		void drill_COAS_rollCurrentState();
									//帧刷新 - 刷新动作元
		void drill_COAS_updateAct();

	//-----------------------------------
	//----接口
	public:
									//数据 - 状态元 - 设置序列（接口）
		void drill_COAS_setSequence(QStringList seq);
									//数据 - 状态元 - 设置序列，立刻改变（接口）
		void drill_COAS_setSequenceImmediate(QStringList seq);
									//数据 - 状态元 - 设置状态元序列[注解模式]（接口）
		bool drill_COAS_setSequenceByAnnotation(QString annotation);
									//数据 - 状态元 - 设置状态元序列[注解模式]-立刻改变（接口）
		bool drill_COAS_setSequenceImmediateByAnnotation(QString annotation);
									//数据 - 状态元 - 获取默认状态元集合（接口）
		QStringList drill_COAS_getDefaultStateGroup();
									//数据 - 状态元 - 获取当前状态元名称（接口）
		QString drill_COAS_getCurrentStateName();
									//数据 - 状态元 - 获取当前状态元集合名称（接口）
		QStringList drill_COAS_getCurrentStateSeqName();
									//数据 - 状态元 - 获取全部状态元名称（接口）
		QStringList drill_COAS_getAllStateName();
									//数据 - 状态元 - 获取数据 根据名称
		QJsonObject drill_COAS_getDataState(QString state_name);

									//数据 - 动作元 - 判断播放
		bool drill_COAS_isPlayingAct();
									//数据 - 动作元 - 添加动作（接口）
		void drill_COAS_setAct(QString act_name);
									//数据 - 动作元 - 立刻终止动作（接口）
		void drill_COAS_stopAct();
									//数据 - 动作元 - 获取当前动作元名称（接口）
		QString drill_COAS_getCurrentActName();
									//数据 - 动作元 - 获取全部动作元名称（接口）
		QStringList drill_COAS_getAllActName();
									//数据 - 动作元 - 获取数据 根据名称
		QJsonObject drill_COAS_getDataAct(QString act_name);

};

#endif // Drill_COAS_StateNodeController_H
