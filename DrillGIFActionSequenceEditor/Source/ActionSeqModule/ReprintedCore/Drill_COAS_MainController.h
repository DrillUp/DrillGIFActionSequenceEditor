#ifndef Drill_COAS_MainController_H
#define Drill_COAS_MainController_H

#include <QtWidgets>
#include "Drill_COAS_StateNodeController.h"
#include "Drill_COAS_ActController.h"

/*
-----==========================================================-----
		类：		动画序列主控制器【复刻类】.h
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		> 定义一个专门控制 动画序列 的数据类。
					> 该类可被存到存档中。
					（详细见cpp）
-----==========================================================-----
*/
class Drill_COAS_MainController{

	public:
		Drill_COAS_MainController();
		Drill_COAS_MainController(QJsonObject data);
		~Drill_COAS_MainController();
		
	//-----------------------------------
	//----数据
	public:
		QJsonObject _drill_data;			
		QString		_drill_controllerSerial;

		int			_drill_curTime;					//常规 - 当前时间
		bool		_drill_needDestroy;				//常规 - 销毁
		bool		_drill_checkArrayEnabled;		//常规 - 校验数据是否为数组

		QString		_drill_curBitmapName;			//输出数据 - 当前的对象名
		QString		_drill_curBitmapPath;			//输出数据 - 当前的路径
		int			_drill_curBitmapTint;			//输出数据 - 当前的色调
		bool		_drill_curBitmapSmooth;			//输出数据 - 当前的模糊
		
		QString		_drill_node_curSerial;							//状态节点 - 当前序列号
		Drill_COAS_StateNodeController* _drill_node_curController;	//状态节点 - 当前控制器

		QString		_drill_act_curName;								//动作元 - 当前每次
		QString		_drill_act_curSerial;							//动作元 - 当前序列号
		Drill_COAS_ActController* _drill_act_curController;			//动作元 - 当前控制器
		bool		_drill_act_interrupt;							//动作元 - 打断设置

	public:
									//数据 - 初始化数据【标准默认值】
		void drill_initData_Main();
									//数据 - 私有数据初始化
		void drill_initPrivateData_Main();
									//数据 - 空判断
		bool isNull();

	//-----------------------------------
	//----标准函数
	public:
									//动画序列 - 帧刷新【标准函数】
									//		【说明】：需要父类手动执行。
		void drill_COAS_update();
									//动画序列 - 重设数据【标准函数】
		void drill_COAS_resetData_Main(QJsonObject data);
									//动画序列 - 显示/隐藏【标准函数】
		void drill_COAS_setVisible(bool visible);
									//动画序列 - 暂停/继续【标准函数】
		void drill_COAS_setPause(bool pause);
									//动画序列 - 设置销毁【标准函数】
		void drill_COAS_destroy();

	//-----------------------------------
	//----开放函数
	public:
									//动画序列-状态元 - 获取数据 - 全部【开放函数】
		QJsonArray drill_COAS_getStateData_All();
									//动画序列-状态元 - 获取数据 - 根据名称【开放函数】
		QJsonObject drill_COAS_getStateData_ByName(QString state_name);
									//动画序列-状态元 - 获取数据 - 全部名称【开放函数】
		QStringList drill_COAS_getStateData_AllName();
		
									//动画序列-状态节点 - 获取数据 - 全部【开放函数】
		QJsonArray drill_COAS_getStateNodeData_All();
									//动画序列-状态节点 - 获取数据 - 根据名称【开放函数】
		QJsonObject drill_COAS_getStateNodeData_ByName(QString stateNode_name);
									//动画序列-状态节点 - 获取数据 - 全部名称【开放函数】
		QStringList drill_COAS_getStateNodeData_AllName();
									//动画序列-状态节点 - 操作 - 获取当前状态元名称【开放函数】
		QString drill_COAS_getCurStateName();
									//动画序列-状态节点 - 操作 - 获取当前状态元名称（全路径）【开放函数】
		QString drill_COAS_getCurStateName_AllRoot();
									//动画序列-状态节点 - 操作 - 获取当前状态元优先级【开放函数】
		int drill_COAS_getCurStatePriority();
									//动画序列-状态节点 - 操作 - 播放默认的状态元集合【开放函数】
		void drill_COAS_setStateNodeDefault();
									//动画序列-状态节点 - 操作 - 播放状态节点【开放函数】
		void drill_COAS_setStateNode(QString node_name);
									//动画序列-状态节点 - 操作 - 播放简单状态元集合【开放函数】
		void drill_COAS_setSimpleStateNode(QStringList state_nameList);
									//动画序列-状态节点 - 操作 - 播放状态元 根据标签【开放函数】
									//		【说明】：没有任何标签时，返回false。
		bool drill_COAS_setAnnotation(QStringList annotation_list);
		
									//动画序列-动作元 - 获取数据 - 全部【开放函数】
		QJsonArray drill_COAS_getActData_All();
									//动画序列-动作元 - 获取数据 - 根据名称【开放函数】
		QJsonObject drill_COAS_getActData_ByName(QString state_name);
									//动画序列-动作元 - 获取数据 - 全部名称【开放函数】
		QStringList drill_COAS_getActData_AllName();
									//动画序列-动作元 - 操作 - 获取当前动作元名称【开放函数】
		QString drill_COAS_getCurActName();
									//动画序列-动作元 - 操作 - 播放动作元【开放函数】
		void drill_COAS_setAct(QString act_name);
									//动画序列-动作元 - 操作 - 立刻终止动作【开放函数】
		void drill_COAS_stopAct();


	//-----------------------------------
	//----动画序列
	public:
									//动画序列 - 校验数据是否为数组
		void drill_COAS_checkArray(QJsonValue arr);

									//动画序列-状态节点 - 帧刷新
		void drill_COAS_updateStateNode();

									//动画序列-动作元 - 帧刷新
		void drill_COAS_updateAct();
									//动画序列-动作元 - 判断播放
		bool drill_COAS_isPlayingAct();

};

#endif // Drill_COAS_MainController_H
