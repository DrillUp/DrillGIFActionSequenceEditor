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

		int			_drill_curTime;									//A主体 - 当前时间
		bool		_drill_needDestroy;								//A主体 - 销毁
		bool		_drill_checkArrayEnabled;						//A主体 - 校验数据是否为数组

		QString		_drill_curBitmapName;							//B输出数据 - 当前的资源名
		QString		_drill_curBitmapPath;							//B输出数据 - 当前的路径
		int			_drill_curBitmapTint;							//B输出数据 - 当前的色调
		bool		_drill_curBitmapSmooth;							//B输出数据 - 当前的模糊
		
		QString		_drill_node_curSerial;							//D管理状态节点 - 当前序列号
		Drill_COAS_StateNodeController* _drill_node_curController;	//D管理状态节点 - 当前控制器

		QString		_drill_act_curName;								//E管理动作元 - 当前名称
		QString		_drill_act_curSerial;							//E管理动作元 - 当前序列号
		Drill_COAS_ActController* _drill_act_curController;			//E管理动作元 - 当前控制器
		bool		_drill_act_interrupt;							//E管理动作元 - 打断设置
		
	//-----------------------------------
	//----动画序列
	public:
									//动画序列 - 初始化数据【标准默认值】
		void drill_controllerMain_initData();
									//动画序列 - 初始化子功能
		void drill_controllerMain_initChild();
									//动画序列 - 帧刷新【标准函数】
									//		【说明】：需要父类手动执行。
		void drill_controllerMain_update();
									//动画序列 - 重设数据【标准函数】
		void drill_controllerMain_resetData(QJsonObject data);
									//动画序列 - 显示/隐藏【标准函数】
		void drill_controllerMain_setVisible(bool visible);
									//动画序列 - 暂停/继续【标准函数】
		void drill_controllerMain_setPause(bool pause);
									//动画序列 - 设置销毁【标准函数】
		void drill_controllerMain_destroy();
									//动画序列 - 空判断
		bool isNull();

		
	//-----------------------------------
	//----A主体
	public:
									//A主体 - 初始化子功能
		void drill_controllerMain_initAttr();
									//A主体 - 校验数据是否为数组
		void drill_controllerMain_checkArray(QJsonValue arr);
									//A主体 - 帧刷新
		void drill_controllerMain_updateAttr();

	//-----------------------------------
	//----B输出数据
	public:
									//B输出数据 - 初始化子功能
		void drill_controllerMain_initBitmapParam();

	//-----------------------------------
	//----C管理状态元
	public:
									//C管理状态元 - 初始化子功能
		void drill_controllerMain_initState();

									//C管理状态元 - 获取数据 - 全部【开放函数】
		QJsonArray drill_controllerMain_getStateData_All();
									//C管理状态元 - 获取数据 - 根据名称【开放函数】
		QJsonObject drill_controllerMain_getStateData_ByName(QString state_name);
									//C管理状态元 - 获取数据 - 全部名称【开放函数】
		QStringList drill_controllerMain_getStateData_AllName();

	//-----------------------------------
	//----D管理状态节点
	public:
									//D管理状态节点 - 初始化子功能
		void drill_controllerMain_initNode();
									//D管理状态节点 - 帧刷新
		void drill_controllerMain_updateStateAndNode();

									//D管理状态节点 - 获取数据 - 全部【开放函数】
		QJsonArray drill_controllerMain_getNodeData_All();
									//D管理状态节点 - 获取数据 - 根据名称【开放函数】
		QJsonObject drill_controllerMain_getNodeData_ByName(QString stateNode_name);
									//D管理状态节点 - 获取数据 - 全部名称【开放函数】
		QStringList drill_controllerMain_getNodeData_AllName();

									//D管理状态节点 - 获取当前状态元名称【开放函数】
		QString drill_controllerMain_Node_getCurStateName();
									//D管理状态节点 - 获取当前状态元名称（全路径）【开放函数】
		QString drill_controllerMain_Node_getCurStateName_AllRoot();
									//D管理状态节点 - 获取当前优先级【开放函数】
		int drill_controllerMain_Node_getCurPriority();

									//D管理状态节点 - 操作 - 播放默认的状态元集合【开放函数】
		void drill_controllerMain_setStateNodeDefault();
									//D管理状态节点 - 操作 - 播放状态节点【开放函数】
		void drill_controllerMain_setStateNode(QString node_name);
									//D管理状态节点 - 操作 - 播放简单状态元集合【开放函数】
		void drill_controllerMain_setSimpleStateNode(QStringList state_nameList);
									//D管理状态节点 - 操作 - 播放状态元/状态节点 根据标签【开放函数】
									//		【说明】：播放不成功时(没有标签)，返回false。
		bool drill_controllerMain_setAnnotation(QString annotation);
									//D管理状态节点 - 操作 - 播放状态元 根据标签列表【开放函数】
									//		【说明】：播放不成功时(没有标签)，返回false。
		bool drill_controllerMain_setAnnotation(QStringList annotation_list);

	//-----------------------------------
	//----E管理动作元
	public:
									//E管理动作元 - 初始化子功能
		void drill_controllerMain_initAct();
									//E管理动作元 - 帧刷新
		void drill_controllerMain_updateAct();

									//E管理动作元 - 获取数据 - 全部【开放函数】
		QJsonArray drill_controllerMain_getActData_All();
									//E管理动作元 - 获取数据 - 根据名称【开放函数】
		QJsonObject drill_controllerMain_getActData_ByName(QString state_name);
									//E管理动作元 - 获取数据 - 全部名称【开放函数】
		QStringList drill_controllerMain_getActData_AllName();

									//E管理动作元 - 获取当前动作元名称【开放函数】
		QString drill_controllerMain_Act_getCurName();

									//E管理动作元 - 操作 - 播放动作元【开放函数】
		void drill_controllerMain_Act_setAct(QString act_name);
									//E管理动作元 - 操作 - 立刻终止动作【开放函数】
		void drill_controllerMain_Act_stopAct();
									//E管理动作元 - 操作 - 是否正在播放动作元【开放函数】
		bool drill_controllerMain_Act_isPlayingAct();

	//-----------------------------------
	//----F管理装饰器
	public:
									//F管理装饰器 - 初始化子功能
		void drill_controllerMain_initDecorator();


};

#endif // Drill_COAS_MainController_H
