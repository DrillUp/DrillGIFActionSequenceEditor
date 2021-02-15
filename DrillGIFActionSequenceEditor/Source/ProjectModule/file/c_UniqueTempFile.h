#pragma once
#include <QList>
#include <QMap>
#include <QUuid>

/*
-----==========================================================-----
		类：		唯一文件 实体类.h
		版本：		v1.17
		所属模块：	项目管理模块
		功能：		控制文件为唯一文件的实体类。
					
		说明：		1.与temp文件夹完全绑定专用。
					2.只是一个文件指针，如果要存储，需要存该实体类，同时需要将原文件转移。
					3.只在temp文件夹一层目录下使用，不能多级文件夹。

		结构：		【OriginalFile 原文件】（ID名）
								固定 + 唯一 + 不可修改 的原文件
					【CommonFile 当前文件】（name名）
								可以随时被覆盖、修改，或者恢复为原文件。
-----==========================================================-----
*/
class C_UniqueTempFile{

	public:
		C_UniqueTempFile();
		~C_UniqueTempFile();
		
	//-----------------------------------
	//----初始化
	private:
		QString id;						//编号
		QString name;					//名称
		QString suffix;					//文件后缀
		//QFile							//当前文件（实时属性）
	public:
										//初始化 - 复制当前文件用（自动存入temp文件夹中）
										//		【参数】 "F:/../filename.facet"
										//		【说明1】不能重复执行该函数。
										//		【说明2】注意，会覆盖原有temp文件夹中的"filename.facet"文件，你可能需要备份。
		void initFormRealFile(QString fileurl);
										//初始化 - 生成新文件用
										//		【参数1】 "xxx_xxx_xxx"
										//		【参数2】 "filename"
										//		【参数3】 "facet"
										//		【说明1】可以重复执行该函数，重复执行时若id不同，则原文件会被删除重建。
										//		【说明2】如果你需要专门建立一个文件控制，需要完全生成文件时，使用该方法。
										//				 初始化后，没有当前文件，需要手动生成。
		void initFormData(QString id, QString filename, QString suffix);
		
	//-----------------------------------
	//----文件
	public:
										//文件 - 文件是否存在
										//		【返回】 ture/false
		bool exists();
										//文件 - 是否为空（id、名称、后缀、当前文件 缺一则返回true）
										//		【返回】 ture/false
		bool isNull();
										//文件 - 清空信息包括当前文件和原文件
										//		【说明】 只有id保留，名称、后缀、文件、原文件全清
		void clearAll();
		void deleteAll();

	//-----------------------------------
	//----属性
	public:
										//属性 - 获取唯一ID
										//		【返回】 "unique_xxxxxxxxx"（不含路径）
		QString getID();
										//属性 - 获取后缀
										//		【返回】 "facet"
		QString getSuffix();
										//属性 - 获取字符串
										//		【返回】 "filename"
		QString getName();
										//属性 - 修改字符串
										//		【参数】 "filename"
		void setName(QString name);
										//属性 - 修改字符串
										//		【参数1】 "filename"
										//		【参数2】 "facet"
		void setNameAndSuffix(QString name, QString suffix);
	private:
										//属性 - 属性是否缺项
		bool isAnyDataEmpty();
		
	//-----------------------------------
	//----原文件（私有）
	public:
										//原文件 - 获取原文件
										//			【说明】只读，不可修改。如果要修改，先recoverCommonFile再操作CommonFile。
		QFileInfo getOriginalFile();
	private:
										//原文件 - 父路径（F:/../temp/workspace/unique/）
		QString getOriginalFileParentPath();
										//原文件 - 获取原文件名（含后缀）
		QString getOriginalFileName();
										//原文件 - 检查并建立原文件
		void checkAndBuildOriginalFile();
										//原文件 - 删除原文件
		void deleteOriginalFile();
		
	//-----------------------------------
	//----当前文件
	public:
										//当前文件 - 获取当前文件
		QFileInfo getCommonFile();
										//当前文件 - 获取文件名（含后缀）
										//			【返回】 "filename.facet"（不含路径）
										//			【返回】 ""（文件不存在情况）
		QString getCommonFileName();
										//当前文件 - 获取文件完整路径
										//			【返回】 "F:/../temp/workspace/filename.facet"
										//			【返回】 ""（文件不存在情况）
		QString getCommonFileFullPath();
										//当前文件 - 恢复为原文件
		void recoverCommonFile();
										//当前文件 - 删除
										//			【说明】 id、名称、后缀保留，只是当前文件被删除
		void deleteCommonFile();
	private:
										//当前文件 - 检查并建立当前文件
		void checkAndBuildCommonFile();

	//-----------------------------------
	//----类属性
	public:
										//实体类 -> QJsonObject
		QJsonObject getJsonObject();
										//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);
										//运算符重载
		const bool operator== (const C_UniqueTempFile& a)const;
};

