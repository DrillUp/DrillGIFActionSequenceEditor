#pragma once
#include <QObject>
#include "c_LEAnnotation.h"

#include "Source/Utils/common/TTool.h"
#include "Source/Utils/manager/txtRead/p_TxtFastReader.h"

/*
-----==========================================================-----
		类：		最大值 注解 读取器.h
		所属模块：	插件模块
		功能：		只扫描最大值的相关注解。
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
		static S_LEAnnotationReader* getInstance();			//单例，获取自己（必须要拿到全局配置才能进行计算）
		void init();										//初始化
		
	//-----------------------------------
	//----读取
	public:
												//读取 - 获取注解内容
												//		【说明】：C_LEAnnotation中含文件名。
		C_LEAnnotation* readPlugin(QFileInfo file);
												//读取 - 刷新参数最大值
												//		【说明】：插件文本被修改后，需要刷新。
		void refreshPluginAnnotation(C_LEAnnotation* plugin);

												//读取 - 检查文本是否存在"xxx-10"序列格式
		bool isPluginIncludedLengthParam(QString plugin_context);

};

