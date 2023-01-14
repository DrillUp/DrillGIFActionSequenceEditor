#pragma once
#include "Source/ProjectModule/storage/s_StorageManagerTemplate.h"

#include "../base/c_RmmvProjectData.h"

/*
-----==========================================================-----
		类：		rmmv交互 数据容器.h
		版本：		v1.02【此类不再更新】
		所属模块：	交互模块
		功能：		【只存储】rmmv工程数据的容器。
					并提供一系列工程文件的获取功能。
					（详细见.cpp）
-----==========================================================-----
*/

class S_RmmvDataContainer : public QObject, public S_StorageManagerTemplate
{
	Q_OBJECT

	public:
		S_RmmvDataContainer();
		~S_RmmvDataContainer();
		static S_RmmvDataContainer* cur_container;		//单例
		static S_RmmvDataContainer* getInstance();		//单例，获取自己
		
	//-----------------------------------
	//----数据
	private:
		C_RmmvProjectData data_RmmvProjectData;
	public:
										//数据 - 修改
		void modify(C_RmmvProjectData data);
										//数据 - 获取
		C_RmmvProjectData getRmmvProjectData();
	
	//-----------------------------------
	//----数据文件
	public:
										//数据 - 获取文件 - 插件
		QFileInfo getRmmvFile_Plugin(QString plugin_name);
										//数据 - 获取文件 - 插件数据
		QFileInfo getRmmvFile_PluginsData();
										//数据 - 获取文件 - 地图
		QFileInfo getRmmvFile_Map(int i);
		QFileInfo getRmmvFile_MapInfos();
										//数据 - 获取文件 - 角色
		QFileInfo getRmmvFile_Actors();
										//数据 - 获取文件 - 职业
		QFileInfo getRmmvFile_Classes();
										//数据 - 获取文件 - 物品
		QFileInfo getRmmvFile_Items();
										//数据 - 获取文件 - 技能
		QFileInfo getRmmvFile_Skills();
										//数据 - 获取文件 - 武器
		QFileInfo getRmmvFile_Weapons();
										//数据 - 获取文件 - 护甲
		QFileInfo getRmmvFile_Armors();
										//数据 - 获取文件 - 状态
		QFileInfo getRmmvFile_States();
										//数据 - 获取文件 - 敌人
		QFileInfo getRmmvFile_Enemies();
										//数据 - 获取文件 - 敌群
		QFileInfo getRmmvFile_Troops();
										//数据 - 获取文件 - 公共事件
		QFileInfo getRmmvFile_CommonEvents();
										//数据 - 获取文件 - 动画
		QFileInfo getRmmvFile_Animations();
										//数据 - 获取文件 - 图块
		QFileInfo getRmmvFile_Tilesets();
										//数据 - 获取文件 - 系统
		QFileInfo getRmmvFile_System();
		

	//-----------------------------------
	//----信号
	signals:
										//信号 - 存储数据被读取
		void dataAllReloaded();

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
};
