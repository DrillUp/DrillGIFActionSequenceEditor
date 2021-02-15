#pragma once

#include "c_ScriptAnnotation.h"

/*
-----==========================================================-----
		类：		脚本注解解析器.h
		所属模块：	插件模块
		功能：		读取js文件内的所有数据 @的注解 内容。
					（详细见.cpp）
-----==========================================================-----
*/

class S_ScriptAnnotationReader : public QObject
{
	Q_OBJECT

	public:
		S_ScriptAnnotationReader();
		~S_ScriptAnnotationReader();
		static S_ScriptAnnotationReader* cur_manager;			//单例
		static S_ScriptAnnotationReader* getInstance();			//单例，获取自己（必须要拿到全局配置才能进行计算）

	//-----------------------------------
	//----读取
	public:
										//读取 - 插件参数（简要）
		C_ScriptAnnotation readPlugin_simple(QString plugin_context);
										//读取 - 插件参数（完整）
		C_ScriptAnnotation readPlugin_complete(QString plugin_context);

		
	//-----------------------------------
	//----解析
	public:
										//解析 - 作用域
		//QString getContentField(QString plugin_helpContext);
										//解析 - 关联文件
		//QString getContentFiles(QString plugin_helpContext);
};
