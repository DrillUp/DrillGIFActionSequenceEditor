#pragma once
#include "stdafx.h"
#include "Source/Utils/Common/P_FileOperater.h"

/*
-----==========================================================-----
		类：		缓存文件夹管理器.h
		版本：		v1.30
		作者：		drill_up
		所属模块：	项目管理模块
		功能：		该管理器本质上是一个文件操作器。
					默认链接exe路径下的temp文件夹，也可以自定义父路径并建立temp文件夹。
					提供文件生成、复制、移动、删除、查找等基本功能。
					【该管理器只管temp文件夹，不管下面的子文件，只提供子文件的操作函数】
					（详细内容见.cpp）
-----==========================================================-----
*/

class S_TempFileManager : public P_FileOperater
{
	Q_OBJECT

	public:
		S_TempFileManager();
		~S_TempFileManager();
		static S_TempFileManager* cur_instance;				//单例
		static S_TempFileManager* getInstance();			//单例，获取自己
		void init();										//初始化
		void destroyInstance();								//销毁


	//-----------------------------------
	//----父路径（当前）
	protected:
		bool m_dir_isUseCustom;				//父路径设置
	public:
									//父路径 - 当前主路径
									//		【返回】 "F:/.../temp"
		QString getMainUrl();
									//父路径 - 当前工作区路径
									//		【返回】 "F:/.../temp/workspace"
		QString getTempFileUrl();
	protected:
									//父路径 - 获取可用工作区ID
		int findEmptyWorkspaceId(QString parent_path);
									//父路径 - 获取可用工作区路径
		QString getEmptyWorkspacePath(QString parent_path);

		
	//-----------------------------------
	//----父路径（默认）
	protected:
		QString m_dir_defaultTemp;			//默认主路径
		QString m_dir_defaultWorkspace;		//默认工作区路径
	public:
									//父路径 - 默认主路径
									//		【返回】 "程序根目录/temp"
		QString getMainUrl_Default();
									//父路径 - 默认工作区路径
									//		【返回】 "程序根目录/temp/workspace"
		QString getTempFileUrl_Default();

		
	//-----------------------------------
	//----父路径（自定义）
	protected:
		QString m_dir_customTemp;			//自定义主路径
		QString m_dir_customWorkspace;		//自定义工作区路径
	public:
									//父路径 - 自定义主路径
									//		【返回】 "自定义路径/temp"
		QString getMainUrl_Custom();
									//父路径 - 自定义工作区路径
									//		【返回】 "自定义路径/temp/workspace"
		QString getTempFileUrl_Custom();
									//父路径 - 转移缓存工作区路径
									//		【参数】 "F:/.../自定义路径"
									//		【说明】 注意，若调用此函数，将会在自定义路径下创建temp文件夹，然后创建workspace，原路径的文件会全部转移过来。
		bool setCustomMainUrl(QString parent_path);
		
		
	//-----------------------------------
	//----生成
	public:
									//生成 - 生成一个文件，写入数据（文件已存在，则不操作）
									//		【参数1】 "name.xxx"（文件）（可含路径如"aaa/name.xxx"）
									//		【参数2】 字符串内容（utf8存储）
									//		【参数3】 编码设置（默认utf8，可选 toUtf8/toLocal8Bit/toLatin1）
									//		【返回】 无
		void generateTempFile(QString filename, QString filedata, QString code = "toUtf8");
									//生成 - 生成一个文件，写入数据（文件已存在，则覆盖）
									//		【参数1】 "name.xxx"（文件）（可含路径如"aaa/name.xxx"）
									//		【参数2】 字符串内容
									//		【参数3】 编码设置（默认utf8，可选 toUtf8/toLocal8Bit/toLatin1）
									//		【返回】 无
		void generateTempFileStrict(QString filename, QString filedata, QString code = "toUtf8");


	//-----------------------------------
	//----复制
	public:
									//复制 - 指定文件/文件夹 -> temp文件夹（全部）
									//		【返回】 成功情况（禁止后缀会阻止该行为）
		bool copyResourceToTemp_File(QString src_url);
		bool copyResourceToTemp_Dir(QString src_url);
		bool copyResourceToTemp_DirWithDepth(QString src_url, int depth);
		bool copyResourceToTemp_DirWithAllSubfolders(QString src_url);
									//复制 - temp文件夹 -> 指定文件夹 （全部）
									//		【返回】 成功情况（禁止后缀会阻止该行为）
		bool copyTempToTarget_Dir(QString tar_url);
		bool copyTempToTarget_DirWithDepth(QString tar_url, int depth);
		bool copyTempToTarget_DirWithAllSubfolders(QString tar_url);

									//复制 - 多个文件 -> temp文件夹
									//		【参数1】 "F:/name"（文件夹）（要含根目录 如F:）
									//		【参数2】 ["aa.txt","bb.vcut"…]（列表）（不含路径）
									//		【返回】 无
		void copyResourceToTemp_SeveralFile(QString src_url, QStringList file_names);
									//复制 - 多个temp文件 -> 指定文件夹 
									//		【参数1】 "F:/name"（文件夹）（要含根目录 如F:）
									//		【参数2】 ["aa.txt","bb.vcut"…]（列表）（不含路径）
									//		【返回】 无
		void copyTempToTarget_SeveralFile(QString tar_url, QStringList file_names);
									//复制 - temp文件夹 的文件，并命名新的名字
									//		【参数1】 "name.xxx"（文件）（不含路径）
									//		【参数2】 "new_name.xxx"（文件）（不含路径）
									//		【返回】 无
		void copyTempToTemp_FileWithNewName(QString filename, QString file_new_name);
									//复制 - temp文件夹 的文件夹，并命名新的文件夹名
									//		【参数1】 "xxxx"（文件夹）（不含路径）
									//		【参数2】 "xxxx/xx"（文件夹）（不含全路径）
									//		【返回】 无
		bool copyTempToTemp_Dir(QString dir_name, QString dir_new_name);
		bool copyTempToTemp_DirWithDepth(QString dir_name, QString dir_new_name, int depth);
		bool copyTempToTemp_DirWithAllSubfolders(QString dir_name, QString dir_new_name);
		

	//-----------------------------------
	//----移动（暂无）
	//		


	//-----------------------------------
	//----删除
	public:
									//删除 - 删除temp文件
		void removeInTemp_Dir(QString dirname);
		void removeInTemp_File(QString filename);
		void removeInTemp_FileBySuffix(QString suffix);
		void removeInTemp_FileBySuffix_WithAllSubfolders(QString suffix);
		void removeInTemp_FileByNameNoSuffix(QString onlyname);
		void removeInTemp_FileByNameNoSuffix_WithAllSubfolders(QString onlyname);
									//删除 - 清空所有文件（包括文件夹）
		void removeAllTempFile();
		

	//-----------------------------------
	//----查找
	public:
									//查找 - 是否存在指定文件
									//		【参数】 "name.xxx"（文件）（不含路径）
									//		【返回】 ture/false
		bool hasTempFile(QString filename);
									//查找 - 是否存在指定文件名
									//		【参数】 "basename"（只文件名，不含后缀）
									//		【返回】 ture/false
		bool hasTempFileBaseName(QString basename);
		bool hasTempFileBaseName_WithAllSubfolders(QString basename);
									//查找 - 是否存在指定后缀
									//		【参数】 "xxx"（只后缀）
									//		【返回】 ture/false
		bool hasTempFileSuffix(QString suffix);
		bool hasTempFileSuffix_WithAllSubfolders(QString suffix);
									//查找 - 指定文件是否在temp文件夹中
									//		【参数】 "F:/.../..."（文件）
									//		【返回】 ture/false
		bool isInCurTempFile(QString filename);
									//查找 - 获取指定文件名的文件
									//		【参数】 "basename"（只文件名，不含后缀）
									//		【返回】 ture/false
		QList<QFileInfo> getTempFile_ByBaseName(QString basename);
		QList<QFileInfo> getTempFile_ByBaseName_WithAllSubfolders(QString basename);
									//查找 - 获取指定后缀的文件
									//		【参数】 "xxx"（只后缀）
									//		【返回】 ture/false
		QList<QFileInfo> getTempFile_BySuffix(QString suffix);
		QList<QFileInfo> getTempFile_BySuffix_WithAllSubfolders(QString suffix);

		
	//-----------------------------------
	//----后缀管理
	protected:
		QStringList m_suffix_skip;					//忽略后缀
		QStringList m_suffix_forbidden;				//禁止后缀
		QStringList m_suffix_defaultSkip;			//默认忽略后缀
		QStringList m_suffix_defaultForbidden;		//默认禁止后缀
	public:
									//后缀管理 - 过滤忽略的后缀（文件夹复制时）
									//		【返回】 ["txt","vcut"]（列表）（不含路径）
		QStringList suffix_getSkip();
									//后缀管理 - 设置过滤后缀（文件夹复制时）
									//		【参数1】 ["txt","vcut"]（列表）（不含路径）
		void suffix_setSkip(QStringList suffix_list);
									//后缀管理 - 重设过滤后缀
		void suffix_resetSkip();
									//后缀管理 - 禁止后缀（文件夹禁止复制）
									//		【返回】 ["exe"]（列表）（不含路径）
		QStringList suffix_getForbidden();
									//后缀管理 - 设置禁止后缀（文件夹禁止复制）
									//		【参数1】 ["exe"]（列表）（不含路径）
		void suffix_setForbidden(QStringList suffix_list);
									//后缀管理 - 重设禁止后缀
		void suffix_resetForbidden();
	protected:
									//后缀管理 - 忽略文件
		bool suffix_isSkipFile(QFileInfo info);
									//后缀管理 - 禁止文件
		bool suffix_isForbiddenFile(QFileInfo info);
	protected:
									//后缀管理 - 复制文件夹（覆写）
		virtual bool copyDirPrivate(QString dirPath_from, QString dirPath_to) override;


	//-----------------------------------
	//----log文件
	public:
									//log文件 - 生成log文件，追加
									//		【参数1】 "name.txt"（文件）（不含路径）
									//		【参数2】 log内容（utf8存储）
									//		【返回】 无
		void addDebugLog(QString filename, QString logdata, bool record_time = true);
									//log文件 - 生成log文件，覆写
									//		【参数1】 "name.txt"（文件）（不含路径）
									//		【参数2】 log内容（utf8存储）
									//		【返回】 无
		void replaceDebugLog(QString filename, QString logdata);
									//log文件 - 生成log文件，覆写
									//		【参数1】 "name.txt"（文件）（不含路径）
									//		【参数2】 字节流（utf8）
									//		【返回】 无
		void replaceDebugLog(QString filename, QByteArray logdata);
									//log文件 - 生成bat文件，覆写
									//		【参数1】 "name.bat"（文件）（不含路径）
									//		【参数2】 log内容（local8bit存储）
									//		【返回】 无
		void replaceDebugBat(QString filename, QString logdata);

		
	//-----------------------------------
	//----其他
	public slots:
									//其他 - 打开缓存路径文件夹
									//		【返回】 不成功返回false
		bool openDesktopTempUrl();
									//其他 - 打开缓存路径中指定文件
									//		【参数】 "name.xxx"（文件）（不含路径）
									//		【返回】 不成功返回false
		bool openDesktopTempFile(QString filename);


};

