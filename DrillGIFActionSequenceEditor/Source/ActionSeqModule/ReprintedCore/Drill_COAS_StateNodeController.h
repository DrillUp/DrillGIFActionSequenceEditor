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

		int			_drill_curTime;					//常规 - 当前时间
		bool		_drill_needDestroy;				//常规 - 销毁

		int			_drill_parentDataId;			//节点 - 父数据ID
		int			_drill_curLayer;				//节点 - 当前层数

		int			_drill_curIndex;				//播放 - 当前索引
		int			_drill_tarIndex;				//播放 - 索引结束位置

		Drill_COAS_StateController*		_drill_curState;	//对象 - 状态控制器
		Drill_COAS_StateNodeController*	_drill_curNode;		//对象 - 状态节点控制器

	public:
									//数据 - 初始化数据【标准默认值】
		void drill_initData_Node();
									//数据 - 私有数据初始化
		void drill_initPrivateData_Node();
									//数据 - 空判断
		bool isNull();

	//-----------------------------------
	//----标准函数
	public:
									//状态节点 - 帧刷新【标准函数】
									//		【说明】：需要父类手动执行。
		void drill_COAS_update();
									//状态节点 - 重设数据【标准函数】
		void drill_COAS_resetData_Node(QJsonObject data);
									//状态节点 - 播放时间重置
		void drill_COAS_resetTimer(QJsonObject data = QJsonObject());

	//-----------------------------------
	//----开放函数
	public:
									//状态节点 - 数据 - 当前的对象名【开放函数】
		QString drill_COAS_curBitmapName();
									//状态节点 - 数据 - 当前的路径【开放函数】
		QString drill_COAS_curBitmapPath();
									//状态节点 - 数据 - 当前的色调【开放函数】
		int drill_COAS_curBitmapTint();
									//状态节点 - 数据 - 当前的模糊【开放函数】
		bool drill_COAS_curBitmapSmooth();
									//状态节点 - 节点 - 当前是否为 状态元类型【开放函数】
		bool drill_COAS_isTypeState();
									//状态节点 - 节点 - 当前是否为 集合类型【开放函数】
		bool drill_COAS_isTypeNode();
									//状态节点 - 节点 - 当前是否为 随机播放【开放函数】
		bool drill_COAS_isRandomPlay();
									//状态节点 - 节点 - 当前是否为 顺序播放【开放函数】
		bool drill_COAS_isPlainPlay();
									//状态节点 - 节点 - 设置父数据id【开放函数】
		void drill_COAS_setParentDataId(int data_id);
									//状态节点 - 节点 - 设置当前层数【开放函数】
		void drill_COAS_setLayer(int layer);
									//状态节点 - 节点 - 是否结束播放【开放函数】
		bool drill_COAS_isNodeEnd();
									//状态节点 - 节点 - 当前状态元优先级【开放函数】
		int drill_COAS_getCurStatePriority();
									//状态节点 - 节点 - 可被动作元打断【开放函数】
		bool drill_COAS_canBeInterrupted();
									//状态节点 - 子节点 - 刷新子节点【开放函数】
		void drill_COAS_refreshNext();
									//状态节点 - 子节点 - 获取当前状态元名称【开放函数】
		QString drill_COAS_getCurStateName();
									//状态节点 - 子节点 - 获取当前状态元名称（全路径）【开放函数】
		QString drill_COAS_getCurStateName_AllRoot();
									//状态节点 - 操作 - 播放简单状态元集合【开放函数】
		void drill_COAS_setNewStateNameList(QStringList state_nameList);

	//-----------------------------------
	//----状态节点
	public:
									//状态节点 - 子节点 - 刷新子节点
		void drill_COAS_refreshNext_Private();
									//状态节点 - 子节点 - 根据权重随机抽取
		QJsonObject drill_COAS_rollObjData(QJsonArray objData_list);
									//状态节点 - 子节点 - 重设数据 状态元
		void drill_COAS_refreshNextState(QJsonObject next_data);
									//状态节点 - 子节点 - 重设数据 状态节点
		void drill_COAS_refreshNextNode(QJsonObject next_data);
									//状态节点 - 帧刷新状态节点
		void drill_COAS_updateNode();

};

#endif // Drill_COAS_StateNodeController_H
