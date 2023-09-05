#ifndef Drill_COAS_StateNodeController_H
#define Drill_COAS_StateNodeController_H

#include <QtWidgets>
#include "Drill_COAS_StateController.h"

/*
-----==========================================================-----
		类：		状态节点控制器【复刻类】.h
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		> 定义一个专门控制状态节点的数据类。
					> 该类可被存到存档中。
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
		QString		_drill_controllerSerial;

		int			_drill_curTime;					//A主体 - 当前时间
		bool		_drill_needDestroy;				//A主体 - 销毁

		int			_drill_parentDataId;			//C节点 - 父数据ID
		int			_drill_curLayer;				//C节点 - 当前层数
		int			_drill_curIndex;				//C节点 - 当前索引
		int			_drill_tarIndex;				//C节点 - 索引结束位置

		Drill_COAS_StateController*		_drill_curState;	//D子节点 - 状态控制器
		Drill_COAS_StateNodeController*	_drill_curNode;		//D子节点 - 状态节点控制器
		
	//-----------------------------------
	//----状态节点
	public:
									//状态节点 - 初始化数据【标准默认值】
		void drill_controllerNode_initData();
									//状态节点 - 初始化子功能
		void drill_controllerNode_initChild();
									//状态节点 - 帧刷新【标准函数】
									//		【说明】：需要父类手动执行。
		void drill_controllerNode_update();
									//状态节点 - 重设数据【标准函数】
		void drill_controllerNode_resetData(QJsonObject data);
									//状态节点 - 空判断
		bool isNull();

	//-----------------------------------
	//----开放函数
	public:
									//A主体 - 当前是否为 状态元类型【开放函数】
		bool drill_controllerNode_isTypeState();
									//A主体 - 当前是否为 集合类型【开放函数】
		bool drill_controllerNode_isTypeNode();
									//A主体 - 当前是否为 随机播放【开放函数】
		bool drill_controllerNode_isRandomPlay();
									//A主体 - 当前是否为 顺序播放【开放函数】
		bool drill_controllerNode_isPlainPlay();
									//A主体 - 当前优先级【开放函数】
		int drill_controllerNode_getPriority();
									//A主体 - 可被动作元打断【开放函数】
		bool drill_controllerNode_canBeInterrupted();

									//B输出数据 - 当前的资源名【开放函数】
		QString drill_controllerNode_curBitmapName();
									//B输出数据 - 当前的路径【开放函数】
		QString drill_controllerNode_curBitmapPath();
									//B输出数据 - 当前的色调【开放函数】
		int drill_controllerNode_curBitmapTint();
									//B输出数据 - 当前的模糊【开放函数】
		bool drill_controllerNode_curBitmapSmooth();
		
									//C节点 - 是否结束播放【开放函数】
		bool drill_controllerNode_isEnd();
									//C节点 - 重置播放【开放函数】
		void drill_controllerNode_resetTimer();
									//C节点 - 设置指定节点/状态元索引【开放函数】
		void drill_controllerNode_setCurIndex(int index);
									//C节点 - 设置父数据id【开放函数】
		void drill_controllerNode_setParentDataId(int data_id);
									//C节点 - 设置当前层数【开放函数】
		void drill_controllerNode_setLayer(int layer);
									//C节点 - 播放简单状态元集合【开放函数】
		void drill_COAS_setNewStateNameList(QStringList state_nameList);

									//D子节点 - 刷新子节点【开放函数】
		void drill_controllerNode_refreshNext();
									//D子节点 - 获取当前状态元对象【开放函数】
		Drill_COAS_StateController* drill_controllerNode_getState();
									//D子节点 - 获取当前状态元名称【开放函数】
		QString drill_controllerNode_getStateName();
									//D子节点 - 获取当前状态元名称（全路径）【开放函数】
		QString drill_controllerNode_getStateName_AllRoot();
		
		
	//-----------------------------------
	//----A主体
	public:
									//A主体 - 初始化子功能
		void drill_controllerNode_initAttr();
									//A主体 - 帧刷新
		void drill_controllerNode_updateAttr();
		
	//-----------------------------------
	//----B输出数据
	public:
									//B输出数据 - 初始化子功能
		void drill_controllerNode_initBitmapParam();
		
	//-----------------------------------
	//----C节点
	public:
									//C节点 - 初始化子功能
		void drill_controllerNode_initNode();
									//C节点 - 设置指定节点/状态元索引
		void drill_controllerNode_setCurIndex_Private(int index);
									//C节点 - 帧刷新（状态节点）
		void drill_controllerNode_updateNode();

	//-----------------------------------
	//----D子节点
	public:
									//D子节点 - 初始化子功能
		void drill_controllerNode_initNext();
									//D子节点 - 刷新子节点
		void drill_controllerNode_refreshNext_Private();
									//D子节点 - 刷新子节点 - 根据权重随机抽取
		QJsonObject drill_controllerNode_nextRollObjData(QJsonArray objData_list);
									//D子节点 - 刷新子节点 - 重设数据 状态元
		void drill_controllerNode_refreshNextState(QJsonObject next_data);
									//D子节点 - 刷新子节点 - 重设数据 状态节点
		void drill_controllerNode_refreshNextNode(QJsonObject next_data);

};

#endif // Drill_COAS_StateNodeController_H
