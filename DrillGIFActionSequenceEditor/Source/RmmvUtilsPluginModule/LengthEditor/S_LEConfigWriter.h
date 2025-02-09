#pragma once
#include <QObject>

#include "C_LEConfigData.h"
#include "C_LEAnnotation.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		最大值 注解 写入器.h
		版本：		v1.01
		作者：		drill_up
		所属模块：	插件模块
		功能：		提供 将单条注解参数覆盖写入 的功能。
					（详细见cpp）
-----==========================================================-----
*/
class S_LEConfigWriter : public QObject
{
	Q_OBJECT

	public:
		S_LEConfigWriter();
		~S_LEConfigWriter();
		static S_LEConfigWriter* cur_manager;			//单例
		static S_LEConfigWriter* getInstance();			//单例，获取自己

	//-----------------------------------
	//----写入
	public:
												//写入 - 单条最大值修改
												//		【参数1】：插件原文/修改的文本
												//		【参数2】：单条最大值注解（读取器读到的）
												//		【参数3】：待修改的值
												//		【说明1】：每次只能将单条注解参数覆盖写入（多个注解需要多次修改）
												//		【说明2】：写入失败则返回空字符串
		QString doOverwritePlugin(QString context, C_LEAnnotation_Param param, C_LEConfigData config);
	
	protected:
												//写入 - 找到第一个变量结构（私有）
												//		【说明】：根据 "阶段-1" 获取到 "@param 阶段-1" 的全部参数字符串
		QStringList getParamStringList(QStringList contextList, QRegExp re);
		
};

