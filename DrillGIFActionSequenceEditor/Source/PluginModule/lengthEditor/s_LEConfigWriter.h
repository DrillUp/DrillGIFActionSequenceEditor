#pragma once
#include <QObject>

#include "C_LEConfigData.h"
#include "C_LEAnnotation.h"

#include "Source/Utils/Common/TTool.h"
#include "Source/Utils/Manager/TxtRead/P_TxtFastReader.h"

/*
-----==========================================================-----
		类：		文本扫描器.h
		所属模块：	核心模块
		功能：		提供扫描、覆写直接功能。
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
		static S_LEConfigWriter* getInstance();			//单例，获取自己（必须要拿到全局配置才能进行计算）
		void init();									//初始化

	//-----------------------------------
	//----覆写
	public:
												//覆写 - 最大值修改
												//		【说明1】:每次只能将一个注解参数覆盖写入（多个注解需要多次修改）
												//		【说明2】:失败返回空字符串
		QString doOverwritePlugin(QString context, C_LEAnnotation_Param param, C_LEConfigData config);

	private:
												//覆写 - 根据 "阶段-1" 获取到 "@param 阶段-1" 的全部参数字符串
		QStringList getParamStringList(QStringList contextList, QRegExp re);
		
};

