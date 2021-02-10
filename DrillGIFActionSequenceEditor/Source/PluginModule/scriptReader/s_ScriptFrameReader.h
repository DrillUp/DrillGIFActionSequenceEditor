#pragma once

#include "c_ScriptFrame.h"

/*
-----==========================================================-----
		类：		脚本参数解析器.h
		所属模块：	插件模块
		功能：		读取js文件内的所有数据框架内容。
					（详细见.cpp）
-----==========================================================-----
*/

class S_ScriptFrameReader : public QObject
{
	Q_OBJECT

	public:
		S_ScriptFrameReader();
		~S_ScriptFrameReader();
		static S_ScriptFrameReader* cur_manager;			//单例
		static S_ScriptFrameReader* getInstance();			//单例，获取自己（必须要拿到全局配置才能进行计算）

	//-----------------------------------
	//----读取
	public:
										//读取 - 插件参数（简要）
		C_ScriptFrame readPlugin_simple(QString plugin_context);
										//读取 - 插件参数（完整）
		C_ScriptFrame readPlugin_complete(QString plugin_context);

		
	//-----------------------------------
	//----解析
	public:
										//解析 - 作用域
		//QString getContentField(QString plugin_helpContext);
										//解析 - 关联文件
		//QString getContentFiles(QString plugin_helpContext);
};
