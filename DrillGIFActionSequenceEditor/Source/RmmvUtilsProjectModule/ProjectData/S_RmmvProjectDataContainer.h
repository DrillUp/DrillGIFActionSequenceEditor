#pragma once
#include "Source/ProjectModule/Storage/S_StorageManagerTemplate.h"

#include "C_RmmvProjectData.h"

/*
-----==========================================================-----
		类：		Rmmv工程数据 数据容器.h
		版本：		v1.03
		作者：		drill_up
		所属模块：	工程模块
		功能：		工程数据的容器。
					并提供一系列工程文件的获取功能。
					（详细见.cpp）
-----==========================================================-----
*/
class S_RmmvProjectDataContainer : public QObject, public S_StorageManagerTemplate
{
	Q_OBJECT

	public:
		S_RmmvProjectDataContainer();
		~S_RmmvProjectDataContainer();
		static S_RmmvProjectDataContainer* cur_container;		//单例
		static S_RmmvProjectDataContainer* getInstance();		//单例，获取自己
		

	//-----------------------------------
	//----工程数据
	private:
		C_RmmvProjectData data_RmmvProjectData;
	public:
										//工程数据 - 修改
		void modifyData(C_RmmvProjectData data);
										//工程数据 - 获取
		C_RmmvProjectData getData();
	

	//-----------------------------------
	//----获取文件
	public:
										//获取文件 - 插件（plugins/xxxx.js）
		QFileInfo getRmmvFile_Plugin(QString plugin_name);
										//获取文件 - 插件数据（plugins.js）
		QFileInfo getRmmvFile_PluginsData();
		
										//获取文件 - 地图（Map001.json）
		QFileInfo getRmmvFile_Map(int i);
										//获取文件 - 地图信息（MapInfos.json）
		QFileInfo getRmmvFile_MapInfos();
										//获取文件 - 角色（Actors.json）
		QFileInfo getRmmvFile_Actors();
										//获取文件 - 职业（Classes.json）
		QFileInfo getRmmvFile_Classes();
										//获取文件 - 物品（Items.json）
		QFileInfo getRmmvFile_Items();
										//获取文件 - 技能（Skills.json）
		QFileInfo getRmmvFile_Skills();
										//获取文件 - 武器（Weapons.json）
		QFileInfo getRmmvFile_Weapons();
										//获取文件 - 护甲（Armors.json）
		QFileInfo getRmmvFile_Armors();
										//获取文件 - 状态（States.json）
		QFileInfo getRmmvFile_States();
										//获取文件 - 敌人（Enemies.json）
		QFileInfo getRmmvFile_Enemies();
										//获取文件 - 敌群（Troops.json）
		QFileInfo getRmmvFile_Troops();
										//获取文件 - 公共事件（CommonEvents.json）
		QFileInfo getRmmvFile_CommonEvents();
										//获取文件 - 动画（Animations.json）
		QFileInfo getRmmvFile_Animations();
										//获取文件 - 图块（Tilesets.json）
		QFileInfo getRmmvFile_Tilesets();
										//获取文件 - 系统（System.json）
		QFileInfo getRmmvFile_System();
		
	//-----------------------------------
	//----获取文件夹
	public:
										//获取文件夹 - 声音BGM（根路径/audio/bgm/）
		QDir getRmmvDir_Audio_BGM();
										//获取文件夹 - 声音BGS（根路径/audio/bgs/）
		QDir getRmmvDir_Audio_BGS();
										//获取文件夹 - 声音ME（根路径/audio/me/）
		QDir getRmmvDir_Audio_ME();
										//获取文件夹 - 声音SE（根路径/audio/se/）
		QDir getRmmvDir_Audio_SE();
										//获取文件夹 - 数据（根路径/data/）
		QDir getRmmvDir_Data();
										//获取文件夹 - 字体（根路径/fonts/）【游戏中使用，编辑器用不到】
		//QDir getRmmvDir_Fonts();
										//获取文件夹 - 插件（根路径/icon/）【游戏中使用，编辑器用不到】
		//QDir getRmmvDir_Icon();
										//获取文件夹 - 插件（根路径/img/）
		QDir getRmmvDir_Img();
										//获取文件夹 - 插件（根路径/js/）
		QDir getRmmvDir_Js();
										//获取文件夹 - 插件（根路径/js/plugins/）
		QDir getRmmvDir_Plugins();
										//获取文件夹 - 插件（根路径/movies/）
		QDir getRmmvDir_Movies();
										//获取文件夹 - 存档（根路径/save/）【游戏中使用，编辑器用不到】
		//QDir getRmmvDir_Save();
		

	//-----------------------------------
	//----信号
	public:
	signals:
										//信号 - 存储数据被读取
		void signal_dataAllReloaded();

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
