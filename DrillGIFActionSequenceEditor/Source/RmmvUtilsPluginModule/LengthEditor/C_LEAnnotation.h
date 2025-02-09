#pragma once
#include <QJsonObject>

/*
-----==========================================================-----
		类：		单条最大值注解 数据类.h（只读）
		作者：		drill_up
		所属模块：	插件模块
		功能：		插件中 @ 注解中的数据。
					数据结构为：插件注解信息 > 单条最大值注解。
					（详细见cpp）
-----==========================================================-----
*/
class C_LEAnnotation_Param{

	public:
		C_LEAnnotation_Param();
		~C_LEAnnotation_Param();
		
	//-----------------------------------
	//----数据
	protected:
		QString paramKey;				//变量键（@Drill_LE_param）
		QString parentKey;				//变量组键（@Drill_LE_parentKey）
		QString var;					//定义名（@Drill_LE_var）
		int varLen;						//定义最大值【定义的 var xxx_length = 后面的值】
		int realLen;					//实际最大值【遍历的 @param 找到的最大值】
	public:
										//数据 - 初始化
										//		【说明】：这里参数只读，一次性读取即可。
		void initParam(QString param, QString parentKey, QString var);
										//数据 - 设置 定义最大值
										//		【说明】：该参数可修改，会被多次赋值。
		void setVarLen(int len);
										//数据 - 设置 实际最大值
										//		【说明】：该参数可修改，会被多次赋值。
		void setRealLen(int len);

										//数据 - 获取 变量键
										//		【说明】：格式为 "阶段-%d"。
		QString getParamKey();
										//数据 - 获取 变量组键
										//		【说明】：格式为 "--阶段组%d至%d--"。
		QString getParentKey();
										//数据 - 获取 定义名
										//		【说明】：格式为 "DrillUp.g_SSpE_list_length"。
		QString getVarName();
										//数据 - 获取 定义最大值
		int getVarLen();
										//数据 - 获取 实际最大值
		int getRealLen();

										//数据 - 获取 变量显示名
										//		【说明】：格式为 "阶段"。
		QString getParamShowingName();
										//数据 - 获取 变量名
										//		【说明】：格式为 "阶段-210"（从1开始计数）。
		QString getParamName(int index);
										//数据 - 获取 变量组名
										//		【说明】：格式为 "--阶段组21至40--"（从1开始计数）。
		QString getParentName(int index);
										//数据 - 获取 变量注释名
										//		【说明】：格式为 "@param[ ]*阶段-210$"（全词匹配用，从1开始计数）。
		QRegExp getParamCommentRe(int index);
										//数据 - 获取 变量注释组名
										//		【说明】：格式为 "@param[ ]*--阶段组21至40--$"（全词匹配用，从1开始计数）。
		QRegExp getParentCommentRe(int index);
										//数据 - 获取 是否分组
		bool isParentGrouping();
};
/*
-----==========================================================-----
		类：		插件注解信息 数据类.h（只读）
		作者：		drill_up
		所属模块：	插件模块
		功能：		插件中 @ 注解中的数据。
					数据结构为：插件注解信息 > 单条最大值注解。
-----==========================================================-----
*/
class C_LEAnnotation{

	public:
		C_LEAnnotation();
		~C_LEAnnotation();
		
	//-----------------------------------
	//----数据
	public:
		QString pluginName;						//插件名
		QString pluginDesc;						//插件简介（@plugindesc）
		QString pluginAuthor;					//插件作者（@author）
		QList<C_LEAnnotation_Param> paramList;	//插件参数
		bool paramForbidden;					//禁止编辑（@Drill_LE_editForbidden）
	public:
												//数据 - 根据变量键获取参数
		C_LEAnnotation_Param getParamByKey(QString paramKey);
												//数据 - 空判断
		bool isNull();
												//数据 - 是否被禁用
		bool isForbidden();
												//数据 - 是否可编辑
		bool isEditable();
												//数据 - 是否配置完整
		bool isIntegrity();
		
	//-----------------------------------
	//----暂存数据
	public:
		QString fullPath;					//插件文件路径
		QString context;					//插件文本
		QString message;					//插件消息【流程用，读取后显示的字符串】
		QString editMessage;				//插件修改消息【流程用，编辑后显示的数据字符串】
};
