#ifndef Drill_COAS_Init_H
#define Drill_COAS_Init_H

#include <QJsonObject>

/*
-----==========================================================-----
		类：		全局变量【复刻类】.h
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		动画序列核心插件的复刻类。
					（详细见cpp）
-----==========================================================-----
*/
class Drill_COAS_Init{

	public:
		Drill_COAS_Init();
		~Drill_COAS_Init();
		static Drill_COAS_Init* cur_instance;		//单例
		static Drill_COAS_Init* getInstance();		//单例，获取自己
		void _init();								//初始化

	//-----------------------------------
	//----变量获取
	public:
									//变量获取 - 状态元（~struct~DrillCOASState）
		static QJsonObject drill_COAS_initState(QJsonObject dataFrom);
									//变量获取 - 状态节点（~struct~DrillCOASStateNode）
		static QJsonObject drill_COAS_initStateNode(QJsonObject dataFrom);
									//变量获取 - 动作元（~struct~DrillCOASAct）
		static QJsonObject drill_COAS_initAct(QJsonObject dataFrom);
									//变量获取 - 动画序列（~struct~DrillCOASSequence）
		static QJsonObject drill_COAS_initSequence(QJsonObject dataFrom);
		
	//-----------------------------------
	//----C++数据容器
	public:
		QJsonArray g_COAS_list;

	//-----------------------------------
	//----数据访问器
	public:
									//数据访问器 - 获取 - 动画序列
		QJsonObject drill_COAS_getSequenceData(int sequence_id);
									//数据访问器 - 获取 - 状态元
		QJsonObject drill_COAS_getStateData(int sequence_id, QString state_name);
									//数据访问器 - 获取 - 状态节点
		QJsonObject drill_COAS_getStateNodeData(int sequence_id, QString stateNode_name);
									//数据访问器 - 获取 - 动作元
		QJsonObject drill_COAS_getActData(int sequence_id, QString act_name);
									//数据访问器 - 是否存在 状态元
		bool drill_COAS_hasState(int sequence_id, QString state_name);
									//数据访问器 - 是否存在 状态节点
		bool drill_COAS_hasStateNode(int sequence_id, QString stateNode_name);
									//数据访问器 - 是否存在 动作元
		bool drill_COAS_hasAct(int sequence_id, QString act_name);
		
	//-----------------------------------
	//----数据校验器
	protected:
		QJsonArray g_drill_COAS_stateMiss_list;
		QJsonArray g_drill_COAS_stateNodeMiss_list;
	public:
									//数据校验器 - 检查 动画序列
		void drill_COAS_checkSequenceData(QJsonObject* sequence_data);
									//数据校验器 - 空检查 状态节点
		void drill_COAS_checkStateNodeMiss(QJsonObject* sequence_data, QJsonObject* stateNode_data);
									//数据校验器 - 嵌套检查 状态节点
		void drill_COAS_checkStateNodeRecursion(QJsonObject* sequence_data, QJsonObject* stateNode_data, int layer);

};

#endif // Drill_COAS_Init_H
