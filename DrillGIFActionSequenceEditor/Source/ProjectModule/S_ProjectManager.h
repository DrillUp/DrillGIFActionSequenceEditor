﻿#pragma once
#include "stdafx.h"

#include "S_ProjectManager_Custom.h"

#include "Data/C_ProjectData.h"
#include "Storage/S_StorageManagerTemplate.h"

/*
-----==========================================================-----
		类：		项目管理.h
		版本：		v1.22
		作者：		drill_up
		所属模块：	项目管理模块
		功能：		项目管理相关数据、相关操作都在这里主要控制。
					（详细见cpp）
-----==========================================================-----
*/

class S_ProjectManager : public QObject, public S_StorageManagerTemplate
{
	Q_OBJECT

	public:
		S_ProjectManager();
		~S_ProjectManager();
		static S_ProjectManager* project_manager;		//单例
		static S_ProjectManager* getInstance();			//单例，获取自己
		

	//-----------------------------------
	//----全局
	private:
		QString m_fixTitle;						//固定名称
		bool m_isDirty;							//修改标记
		QList<QWidget*> m_lockingWidgets;		//锁定标记
	public:
										//全局 - 修改名称
		void refreshTitle();
										//全局 - 使用固定名称
		void setFixTitle(QString fix_title);
		void clearFixTitle();
	public:
										//全局 - 设置数据已修改
										//		【说明】：任何一个窗口accepted，就应该调用此方法，确保数据被修改过。
		void setDirty();
										//全局 - 保存修改部分提示
										//		【返回】：true成功执行，false中途被取消。返回false时，父流程也应被取消。
										//		【说明】：相当于一个中间插入节点，提示你走保存流程。
		bool dirtyTip();
										//全局 - 锁定项目
										//		【参数】：锁定的非模态对话框。（模态直接就阻塞了，不需要考虑该方法。）
										//		【说明】：打开任何一个窗口，都应该锁定项目，确保不能造成特殊操作。
		void addLock(QWidget* widget);
										//全局 - 解锁项目
										//		【说明】：关闭窗口后，需要调用该函数，函数会根据非模态对话框数量来控制解锁。
		void removeLock(QWidget* widget);
										//全局 - 判断锁定
		bool isLocked();
										//全局 - 判断修改
		bool isDirty();
		
	signals:
										//全局 - 名称变化（信号）
		void signal_titleChanged(QString title);
										//全局 - 修改变化（信号）
		void signal_dirtyChanged(bool dirty);
										//全局 - 锁定变化（信号）
		void signal_lockChanged(bool isLock);
		

	//-----------------------------------
	//----项目
	public slots:
										//项目 - 新建（对话框）
										//		【返回】：true成功执行，false中途被取消。
										//		【说明】：新建的实际效果，是清空项目。并且项目名也为空。
		bool newProject();
										//项目 - 打开（对话框）
										//		【返回】：true成功执行，false中途被取消。
		bool openProject();	
										//项目 - 打开（输入值）
										//		【返回】：true成功执行，false输入无效。
		bool openProjectByArgs(int argc, char *argv[]);
										//项目 - 打开（拖拽文件）
										//		【返回】：true成功执行，false中途被取消。
		bool openProjectByDropFile(QDropEvent *event);
										//项目 - 保存（对话框）
										//		【返回】：true成功执行，false中途被取消。
		bool saveProject();	
										//项目 - 另存为（对话框）
										//		【返回】：true成功执行，false中途被取消。
		bool saveAs();
										//项目 - 强制保存
										//		【返回】：true成功执行，false中途被取消。
		bool saveInForce();

	signals:
										//项目 - 项目新建清空后（信号）
		void signal_newProjectFinished();
										//项目 - 项目打开后（信号）
		void signal_openProjectFinished();
										//项目 - 项目保存后（信号）
		void signal_saveProjectFinished();

	private:
										//项目 - 清除项目数据
		void clearProject();	
										//项目 - 打开（根据文件名）
		void openProjectDirectly(QString open_path);
										//项目 - 保存（目标文件夹）
										//		【说明】：进入到此函数后，this->data_ProjectData 已被赋值为新工程数据。
										//		【说明】：如果是另存为情况，则 saveAs_last_fileDir 必然会有值。
		void saveAll(QString tar_fileDir, QString saveAs_last_fileDir = "");
										//项目 - 保存（文件）
										//		【说明】：进入到此函数后，this->data_ProjectData 已被赋值为新工程数据。
										//		【说明】：如果是另存为情况，则 saveAs_last_fileDir 必然会有值。
		void createSaveFile(QString tar_fileDir, QString saveAs_last_fileDir);
										//项目 - 读取（文件）
		void readSaveFile();
										//项目 - 获取文件（根据后缀，F:/aaa/bbb.suf完整路径）
		QString getOneFileBySuffix(QString url,QString suffix);	
		

	//-----------------------------------
	//----样例工程
	private:
		bool m_isSampleProject;
	public:
										//样例工程 - 设置
										//		【说明】：有样例工程标记时，保存会跳转到另存为工程。
										//		【说明】：打开工程后使用此函数设置标记。新建、保存、打开都会清理此标记。
		void setSampleProject(bool enabled);
		bool isSampleProject();


	//-----------------------------------
	//----存储数据
	public:
		C_ProjectData data_ProjectData;
	public:
										//数据 - 获取存储的名称
		QString getSaveName();
										//数据 - 清除当前管理器数据
		void clearAllData();
										//数据 - 全部项目管理数据 -> QJsonObject
		QJsonObject getAllDataOfJsonObject();
										//数据 - QJsonObject -> 全部项目管理数据
		void setAllDataFromJsonObject(QJsonObject obj);
		

	//-----------------------------------
	//----历史记录
	protected:
		QList<QFileInfo> m_historyProjectTank;	//历史项目
		QList<QDir> m_historyDirTank;			//历史根目录
		int m_historyProjectMax;				//历史项目存量
		int m_historyDirMax;					//历史根目录存量
	public:
										//历史记录 - 获取项目
		QList<QFileInfo> getHistoryProjectTank();
										//历史记录 - 添加项目
		void addHistoryProject(QFileInfo info);
										//历史记录 - 去除项目
		void removeHistoryProject(int index);
										//历史记录 - 获取根目录
		QList<QDir> getHistoryDirTank();
										//历史记录 - 添加根目录
		void addHistoryDir(QDir dir);
										//历史记录 - 去除根目录
		void removeHistoryDir(int index);
	protected:
										//历史记录 - 保存记录（私有）
		void saveHistory();
										//历史记录 - 读取记录（私有）
		void loadHistory();
		

	//-----------------------------------
	//----其他
	public slots:
									//其他 - 打开项目路径文件夹
									//		【返回】：不成功返回false。
		bool openDesktopProjectUrl();
									//其他 - 打开项目路径中指定文件
									//		【参数】："name.xxx"（文件）（不含路径）
									//		【返回】：不成功返回false。
		bool openDesktopProjectFile(QString filename);
};

