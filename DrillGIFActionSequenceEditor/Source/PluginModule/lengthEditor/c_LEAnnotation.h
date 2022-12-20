#pragma once
#include <QJsonObject>

/*
-----==========================================================-----
		类：		插件注解 参数.h（只读）
		所属模块：	核心模块
		功能：		插件的固定参数数据。
					（详细见cpp）
-----==========================================================-----
*/
class C_LEAnnotation_Param{
	public:
		C_LEAnnotation_Param();
		~C_LEAnnotation_Param();
	private:
		QString paramKey;				//变量键（@Drill_LE_param）
		QString parentKey;				//变量组键（@Drill_LE_parentKey）
		QString var;					//定义名（@Drill_LE_var）
		int varLen;						//定义最大值【定义的 var xxx_length = 后面的值】
		int realLen;					//实际最大值【遍历的 @param 找到的最大值】
	public:
										//参数 - 初始化
		void initParam(QString param, QString parentKey, QString var);
										//获取 - 变量键 "阶段-%d"
		QString getParamKey();
										//获取 - 变量组键 "--阶段组%d至%d--"
		QString getParentKey();
										//获取 - 定义名 "DrillUp.g_SSpE_list_length"
		QString getVarName();

										//参数 - 设置 定义最大值（读取时需要多次赋值）
		void setVarLen(int len);
										//参数 - 设置 实际最大值（读取时需要多次赋值）
		void setRealLen(int len);
										//获取 - 定义最大值
		int getVarLen();
										//获取 - 实际最大值
		int getRealLen();

										//获取 - 变量显示名 "阶段"
		QString getParamShowingName();
										//获取 - 变量名 "阶段-210"（从1开始计数）
		QString getParamName(int index);
										//获取 - 变量组名 "--阶段组21至40--"（从1开始计数）
		QString getParentName(int index);
										//获取 - 变量注释名 "@param[ ]*阶段-210$"（全词匹配用，从1开始计数）
		QRegExp getParamCommentRe(int index);
										//获取 - 变量注释组名 "@param[ ]*--阶段组21至40--$"（全词匹配用，从1开始计数）
		QRegExp getParentCommentRe(int index);
										//获取 - 是否分组
		bool isParentGrouping();
};
/*
-----==========================================================-----
		类：		插件信息.h（只读）
		所属模块：	核心模块
		功能：		插件的固定信息数据。
-----==========================================================-----
*/
class C_LEAnnotation{
	public:
		C_LEAnnotation();
		~C_LEAnnotation();
	public:
		QString pluginName;						//插件名
		QString pluginDesc;						//插件简介（@plugindesc）
		QString pluginAuthor;					//插件作者（@author）
		QList<C_LEAnnotation_Param> paramList;	//插件参数
		bool paramForbidden;					//禁止编辑（@Drill_LE_editForbidden）

		QString fullPath;					//插件文件路径
		QString context;					//插件文本
		QString message;					//插件消息【流程用，读取后显示的字符串】
		QString editMessage;				//插件修改消息【流程用，编辑后显示的数据字符串】
	public:
											//根据变量键获取参数
		C_LEAnnotation_Param getParamByKey(QString paramKey);
											//判断 - 空判断
		bool isNull();
											//判断 - 是否被禁用
		bool isForbidden();
											//判断 - 是否可编辑
		bool isEditable();
											//判断 - 完整性检查
		bool isIntegrity();
};
