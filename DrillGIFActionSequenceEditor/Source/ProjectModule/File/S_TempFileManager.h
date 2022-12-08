#pragma once
#include "stdafx.h"
#include "Source/Utils/Common/P_FileOperater.h"

/*
-----==========================================================-----
		类：		temp文件夹管理.h
		版本：		v1.22
		所属模块：	项目管理模块
		功能：		对temp文件夹的任何操作都在这里进行。
					直接在exe文件目录下建立temp文件夹，对文件作转移、临时处理。
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
		static S_TempFileManager* getInstance();			//单例，获取自己（必须要拿到全局配置才能进行计算）
		void init();										//初始化
		void destroyInstance();								//销毁单例
		
	//-----------------------------------
	//----私有数据
	protected:
		QString temp_file_url;		//当前缓存文件夹路径
		QString workspace_url;		//当前工作区路径
	protected:
		virtual bool copyDirPrivate(QString dirPath_from, QString dirPath_to);		//复制文件夹（覆写）

	//-----------------------------------
	//----log文件
	public:
									//混杂 - 生成log文件，覆写
									//		【参数1】 "name.txt"（文件）（不含路径）
									//		【参数2】 log内容（utf8存储）
									//		【返回】 无
		void replaceDebugLog(QString filename, QString logdata);
									//混杂 - 生成log文件，追加
									//		【参数1】 "name.txt"（文件）（不含路径）
									//		【参数2】 log内容（utf8存储）
									//		【返回】 无
		void addDebugLog(QString filename, QString logdata, bool record_time = true);
									//混杂 - 生成log文件，覆写
									//		【参数1】 "name.txt"（文件）（不含路径）
									//		【参数2】 字节流（utf8）
									//		【返回】 无
		void replaceDebugLog(QString filename, QByteArray logdata);

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
									//		【参数2】 ["aa.xds","bb.vcut"…]（列表）（不含路径）
									//		【返回】 无
		void copyResourceToTemp_SeveralFile(QString src_url, QStringList file_names);
									//复制 - 多个temp文件 -> 指定文件夹 
									//		【参数1】 "F:/name"（文件夹）（要含根目录 如F:）
									//		【参数2】 ["aa.xds","bb.vcut"…]（列表）（不含路径）
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
	//----生成
	public:
									//生成 - 生成一个文件，写入数据（文件已存在，则不操作）
									//		【参数1】 "name.xxx"（文件）（可含路径如"aaa/name.xxx"）
									//		【参数2】 字符串内容（utf8存储）
									//		【返回】 无
		void generateTempFile(QString filename, QString filedata);
									//生成 - 生成一个文件，写入数据（文件已存在，则覆盖）
									//		【参数1】 "name.xxx"（文件）（可含路径如"aaa/name.xxx"）
									//		【参数2】 字符串内容
									//		【参数3】 编码设置（默认utf8，可选 utf8/toLocal8Bit/toLatin1）
									//		【返回】 无
		void generateTempFileStrict(QString filename, QString filedata, QString code = "utf8");
		
	//-----------------------------------
	//----改
	/*	改文件名直接用：bool ok = QFile::rename(path1, path2);	*/

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
	//----获取
	public:
									//获取 - 缓存文件夹主路径
									//		【返回】 "F:/.../temp"
		QString getMainUrl();
									//获取 - 当前缓存工作区路径
									//		【返回】 "F:/.../temp/workspace"
		QString getTempFileUrl();
									//获取 - 是否存在指定文件
									//		【参数】 "name.xxx"（文件）（不含路径）
									//		【返回】 ture/false
		bool hasTempFile(QString filename);
									//获取 - 指定文件是否在temp文件夹中
									//		【参数】 "F:/.../..."（文件）
									//		【返回】 ture/false
		bool isInCurTempFile(QString filename);

		
	//-----------------------------------
	//----后缀
	private:
		QStringList skip_suffix;					//忽略后缀
		QStringList forbidden_suffix;				//禁止后缀
		QStringList default_skip_suffix;			//默认忽略后缀
		QStringList default_forbidden_suffix;		//默认禁止后缀
	public:
									//后缀 - 过滤忽略的后缀（文件夹复制时）
									//		【返回】 ["xds","vcut"]（列表）（不含路径）
		QStringList getSkipSuffix();
									//后缀 - 设置过滤后缀（文件夹复制时）
									//		【参数1】 ["xds","vcut"]（列表）（不含路径）
		void setSkipSuffix(QStringList suffix_list);
									//后缀 - 重设过滤后缀
		void resetSkipSuffix();
									//后缀 - 禁止后缀（文件夹禁止复制）
									//		【返回】 ["exe"]（列表）（不含路径）
		QStringList getForbiddenSuffix();
									//后缀 - 设置禁止后缀（文件夹禁止复制）
									//		【参数1】 ["exe"]（列表）（不含路径）
		void setForbiddenSuffix(QStringList suffix_list);
									//后缀 - 重设禁止后缀
		void resetForbiddenSuffix();

	private:
									//后缀 - 忽略文件
		bool isSkipFile(QFileInfo info);
									//后缀 - 禁止文件
		bool isForbiddenFile(QFileInfo info);

		
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

