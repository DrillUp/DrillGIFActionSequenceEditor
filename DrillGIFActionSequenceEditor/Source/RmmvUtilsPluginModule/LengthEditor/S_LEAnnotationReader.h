#pragma once
#include <QObject>
#include "C_LEAnnotation.h"

#include "Source/Utils/Manager/TxtRead/P_TxtFastReader.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		最大值 注解 读取器.h
		版本：		v1.01
		作者：		drill_up
		所属模块：	插件模块
		功能：		读取最大值的相关注解。
					（详细见cpp）
-----==========================================================-----
*/
class S_LEAnnotationReader : public QObject
{
	Q_OBJECT

	public:
		S_LEAnnotationReader();
		~S_LEAnnotationReader();
		static S_LEAnnotationReader* cur_manager;			//单例
		static S_LEAnnotationReader* getInstance();			//单例，获取自己
		
	//-----------------------------------
	//----读取
	public:
												//读取 - 获取注解内容
												//		【说明】：C_LEAnnotation中含文件名。
		C_LEAnnotation* readPlugin(QFileInfo file);
												//读取 - 刷新最大值信息
												//		【说明】：插件文本被 写入器 修改后，需要刷新。
		void refreshPluginAnnotation(C_LEAnnotation* plugin);

												//读取 - 检查文本是否存在"xxx-10"序列格式
		bool isPluginIncludedLengthParam(QString plugin_context);

};

