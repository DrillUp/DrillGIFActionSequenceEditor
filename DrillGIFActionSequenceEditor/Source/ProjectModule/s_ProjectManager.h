#pragma once

#include "storage/s_StorageManagerTemplate.h"
#include "data/c_ProjectData.h"

/*
-----==========================================================-----
		类：		项目管理.h
		版本：		v1.12
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
		static S_ProjectManager* getInstance();			//单例，获取自己（必须要拿到全局配置才能进行计算）
		
	//-----------------------------------
	//----全局
	private:
		bool m_isDirty;							//修改标记
		QList<QWidget*> m_lockingWidgets;		//锁定标记
	private:
										//#全局 - 修改名称
		void changeTitle();
	public:
										//全局 - 设置数据已修改
										//		【说明】 任何一个窗口accepted，就应该调用此方法，确保数据被修改过。
		void setDirty();
										//全局 - 保存修改部分提示
										//		【返回】 true：成功执行，false：中途被取消。返回false，父流程也应被取消。
										//		【说明】 相当于一个中间插入节点，提示你走保存流程。
		bool dirtyTip();
										//全局 - 锁定项目
										//		【参数】 锁定的非模态对话框。（模态直接就阻塞了，不需要考虑该方法。）
										//		【说明】 打开任何一个窗口，都应该锁定项目，确保不能造成特殊操作。
		void addLock(QWidget* widget);
										//全局 - 解锁项目
										//		【说明】 关闭窗口后，需要调用该函数，函数会根据非模态对话框数量来控制解锁。
		void removeLock(QWidget* widget);
										//全局 - 判断锁定
		bool isLocked();
										//全局 - 判断修改
		bool isDirty();

	signals:							//全局 - 修改名称
		void changeWindowTitle(QString title);
	signals:							//全局 - 锁定变化
		void lockChanged(bool isLock);
	
	//-----------------------------------
	//----项目
	public slots:
										//项目 - 新建（对话框）
										//		【返回】 true：成功执行，false：中途被取消。
										//		【说明】 新建的实际效果，是清空项目。并且项目名也为空。
		bool newProject();
										//项目 - 打开（对话框）
										//		【返回】 true：成功执行，false：中途被取消。
		bool openProject();	
										//项目 - 打开（输入值）
										//		【返回】 true：成功执行，false：输入无效。
		bool openProjectByArgs(int argc, char *argv[]);
										//项目 - 打开（拖拽文件）
										//		【返回】 true：成功执行，false：中途被取消。
		bool openProjectByDropFile(QDropEvent *event);
										//项目 - 保存（对话框）
										//		【返回】 true：成功执行，false：中途被取消。
		bool saveProject();	
										//项目 - 另存为（对话框）
										//		【返回】 true：成功执行，false：中途被取消。
		bool saveAs();
										//项目 - 强制保存
										//		【返回】 true：成功执行，false：中途被取消。
		bool saveInForce();

	private:
										//#项目 - 清除项目数据
		void clearProject();	
										//#项目 - 打开（根据文件名）
		void openProjectDirectly(QString open_path);
										//#项目 - 保存（目标文件夹）
		void saveAll(QString url);
										//#项目 - 保存（文件）
		void createSaveFile();
										//#项目 - 读取（文件）
		void readSaveFile();
										//#项目 - 获取文件（根据后缀，F:/aaa/bbb.suf完整路径）
		QString getOneFileBySuffix(QString url,QString suffix);	
		

	//-----------------------------------
	//----存储数据（父类见s_StorageManager）
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
	//----其他
	public slots:
									//其他 - 打开项目路径文件夹
									//		【返回】 不成功返回false
		bool openDesktopProjectUrl();
									//其他 - 打开项目路径中指定文件
									//		【参数】 "name.xxx"（文件）（不含路径）
									//		【返回】 不成功返回false
		bool openDesktopProjectFile(QString filename);
};

