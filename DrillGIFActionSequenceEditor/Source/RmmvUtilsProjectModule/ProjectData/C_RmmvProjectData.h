#pragma once
#include <QJsonObject>

/*
-----==========================================================-----
		类：		Rmmv工程数据 数据类.h
		版本：		v1.03
		作者：		drill_up
		所属模块：	工程模块
		功能：		工程基本信息数据。（只有工程路径的数据重要，需要存储）
					（详细见.cpp）
-----==========================================================-----
*/
class C_RmmvProjectData{

	public:
		C_RmmvProjectData();
		~C_RmmvProjectData();
		
	//-----------------------------------
	//----数据
	protected:
		QString name;			//工程名称
		QString version;		//工程版本
		QString path;			//工程根路径
	public:
										//数据 - 获取工程名称
										//		【说明】：工程名称来自文件 index.html 。
		QString getName();
										//数据 - 获取工程版本
										//		【说明】：工程版本来自文件 Game.rpgproject 。
		QString getVersion();
										//数据 - 获取工程根路径
										//		【说明】：工程根路径，返回"C:/aaa/插件集合示例"。
		QString getRootPath();
										//数据 - 是否存在
		bool isProjectExist();

	//-----------------------------------
	//----读取器
	public:
										//读取器 - 解析工程文件
										//		【说明】：文件必须是Game.rpgproject文件的全路径。
		void init_fromFile(QString full_filePath);
										//读取器 - 启用文件选择框
										//		【说明】：这是一个静态函数，返回值为工程数据对象。
		static C_RmmvProjectData callRmmvSelectDialog();
		

	//-----------------------------------
	//----获取文件
	public:
										//获取文件 - 插件（/js/plugins/xxxx.js）
		QFileInfo getRmmvFile_Plugin(QString plugin_name);
										//获取文件 - 插件存储数据（plugins.js）
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
	//----类属性
	public:
												//空判断
		bool isNull();
												//实体类 -> QJsonObject
		QJsonObject getJsonObject();
												//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);
												//运算符重载
		const bool operator== (const C_RmmvProjectData& a)const;
};
