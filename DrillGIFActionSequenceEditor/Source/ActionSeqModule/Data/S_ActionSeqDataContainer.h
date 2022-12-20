#pragma once
#include "Source/ProjectModule/Storage/S_StorageManagerTemplate.h"

#include "../DataPart/C_COAS_Data.h"
#include "LengthData/C_COAS_Length.h"
#include "Source/PluginModule/StorageData/C_PluginData.h"

/*
-----==========================================================-----
		类：		动画序列 数据容器.h
		所属模块：	动画序列模块
		功能：		动画序列的数据容器。
					会主动使用插件容器中的数据。
					（详细见.cpp）
-----==========================================================-----
*/
class S_ActionSeqDataContainer : public QObject, public S_StorageManagerTemplate
{
	Q_OBJECT

	public:
		S_ActionSeqDataContainer();
		~S_ActionSeqDataContainer();
		static S_ActionSeqDataContainer* cur_manager;			//单例
		static S_ActionSeqDataContainer* getInstance();			//单例，获取自己（必须要拿到全局配置才能进行计算）

	//-----------------------------------
	//----读取器（只读，不存）
	private:
		C_PluginData* data_ActionSeqPlugin;
	public:
										//读取器 - 重设插件数据（自动从插件数据读取器中去拿）
		void resetPluginData();
										//读取器 - 获取插件数据
		C_PluginData* getPluginData_ActionSeq();
										//读取器 - 获取插件文件
										//		【说明】：如果未打开工程文件，则返回的info文件不存在。
		QFileInfo getPluginFile_ActionSeq();
										//读取器 - 获取长度（插件的）
										//		【说明】：如果未打开工程文件，则返回的为null。
		C_COAS_Length getPluginData_ActionSeqLength();
		
	//-----------------------------------
	//----动画序列数据
	private:
		C_COAS_Length data_ActionSeqLength;			//长度
		QList<C_COAS_Data*> data_ActionSeqData;		//动画序列
	public:
										//数据 - 设置数据
		void setActionSeqData(QList<C_COAS_Data*> data_list);
		void setActionSeqData_Object(QJsonObject data_obj);
										//数据 - 获取数据
		QList<C_COAS_Data*> getActionSeqData();
		QJsonObject getActionSeqData_Object();
										//数据 - 设置长度
		void setActionSeqLength(C_COAS_Length data);
										//数据 - 获取长度
		C_COAS_Length getActionSeqLength();
										//数据 - 获取全部关联的文件
		QList<QFileInfo> getAllRelatedFile();
	protected:
										//数据 - 重刷长度
		void refreshActionSeqLength();

	//-----------------------------------
	//----树配置
	private:
		QJsonObject data_treeConfig;
	public:
										//树配置 - 修改配置
		void modifyTreeConfig(QJsonObject tree);
										//树配置 - 获取配置
		QJsonObject getTreeConfig();
										//树配置 - 获取树数据
		QList<QJsonObject> getTreeData();

	//-----------------------------------
	//----常量
	public:
										//常量 - 获取软件保存图片路径（F:/xxx/xxx/Special__actionSeq）
										//		【说明】：不是rmmv的文件路径，rmmv相关数据都在交互模块。
		QString getActionSeqDir();
		

	//-----------------------------------
	//----存储数据（继承）
	public:
										//数据 - 获取存储的名称
		QString getSaveName();
										//数据 - 清除当前管理器数据
		void clearAllData();
										//数据 - 全部激励源数据 -> QJsonObject
		QJsonObject getAllDataOfJsonObject();
										//数据 - QJsonObject -> 全部激励源数据
		void setAllDataFromJsonObject(QJsonObject obj);
		
	//-----------------------------------
	//----信号
	signals:
										//信号 - 存储数据被读取
		void dataAllReloaded();
};
