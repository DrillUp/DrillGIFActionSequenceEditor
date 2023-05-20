#pragma once
#include "stdafx.h"
#include <QJsonObject>

/*
-----==========================================================-----
		类：		数据容器 基类.h
		版本：		v1.04
		作者：		drill_up
		所属模块：	项目管理模块
		功能：		数据容器中的 实体类，定义了 ID 和 子类类名。
					【数据类初始化之后，必须手动执行init函数，否则为空对象】

		说明：		建议 所有 在数据容器中存储的类对象，都继承此类。【规范化管理】
					> ID是所有类的必须项。 对象获取/空校验/action事件传递/代码debug/后期维护 都需要ID。
					> 子类类名是所有类的标识项。 数据容器可以使用此父类对不同的子类进行分别维护，而不需要创建一大堆的枚举/处理函数。
-----==========================================================-----
*/

class C_ContainerDataBase {

	public:
		C_ContainerDataBase();
		~C_ContainerDataBase();
	
	//-----------------------------------
	//----基本
	protected:
		QString id;					//ID
		QString __CLASS_NAME__;		//子类类名
	public:
									//初始化
									//		【说明】：此类必须手动执行，不然为空对象。
		void init();

									//访问器 - 设置ID
		void setId(QString id);
		void generateRandomId();
									//访问器 - 获取ID
		QString getId();
									//访问器 - 获取子类类名
									//		【说明】：此接口子类必须继承。
		virtual QString get__CLASS_NAME__() = 0;
									//访问器 - 获取子类类名（中文名）
									//		【说明】：此接口子类必须继承。
		virtual QString get__CLASS_NAME__Chinese() = 0;

									//访问器 - 获取所有继承的类名
									//		【说明】：此函数主要用于区分 多重继承 情况，尤其是多个父类交叉继承，确保指针转换时，能转成那个父类。
									//		【说明】：此接口没法强制子类继承，如果你要用此函数，最好检查一下各个子类是否继承完善了。
		virtual QStringList get__CLASS_INHERITED_LIST__();
									//访问器 - 是否继承了某类
									//		【说明】：此函数主要用于区分 多重继承 情况，尤其是多个父类交叉继承，确保指针转换时，能转成那个父类。
									//		【说明】：此接口没法强制子类继承，如果你要用此函数，最好检查一下各个子类是否继承完善了。
		virtual bool get__CLASS_IS_INSTANCE_OF__(QString class_name);

	//-----------------------------------
	//----类属性
	public:
												//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject() final;
												//QJsonObject -> 实体类
		virtual void setJsonObject(QJsonObject obj) final;
												//运算符重载
		virtual	const bool operator== (const C_ContainerDataBase& a)const;
												//空判断
		virtual bool isNull();
		
	protected:
												//实体类 -> QJsonObject（子类用）
												//		【说明】：此函数不需要考虑给QJsonObject赋值id。
		virtual QJsonObject getJsonObject_childData() = 0;
												//QJsonObject -> 实体类（子类用）
												//		【说明】：此函数不需要考虑获取QJsonObject后给id赋值。
		virtual void setJsonObject_childData(QJsonObject obj) = 0;

};
typedef QSharedPointer<C_ContainerDataBase> C_ContainerDataBasePtr;


/*
-----==========================================================-----
		宏定义：	获取子数据函数（头文件实现）
		所属模块：	项目管理模块
		功能：		将四个常用函数一套打包，可直接使用宏定义：
		包含:
						getData_xxx_List
						getData_xxx_ByIndex
						getData_xxx_ById
						getData_xxx_AllId
					【注意，如果使用此宏，那么数据容器必须实现 QList<C_ContainerDataBasePtr> getDataListByClass(QString class_name); 】
-----==========================================================-----
*/
#define C_ContainerDataBase_COMMON_DATA_GET_Header(CDBChildContainerName,CDBClassName,CDBPartName) \
	QList<CDBClassName> getData##CDBPartName##List(); \
	CDBClassName getData##CDBPartName##ByIndex(int index); \
	CDBClassName getData##CDBPartName##ById(QString id); \
	QStringList getData##CDBPartName##AllId();
	//（最后一行不要加"\"，会出现错误"无法识别的标记"）


/*
-----==========================================================-----
		宏定义：	获取子数据函数（函数实现）
		所属模块：	项目管理模块
		功能：		将四个常用函数一套打包，可直接使用宏定义：
					包含:
						getData_xxx_ByIndex
						getData_xxx_List
						getData_xxx_ById
						getData_xxx_AllId
					【注意，如果使用此宏，那么数据容器必须实现 QList<C_ContainerDataBasePtr> getDataListByClass(QString class_name); 】
-----==========================================================-----
*/
#define C_ContainerDataBase_COMMON_DATA_GET_Method(CDBChildContainerName,CDBClassName,CDBPartName) \
	QList<CDBClassName> CDBChildContainerName::getData##CDBPartName##List(){ \
		QList<C_ContainerDataBasePtr> data_list = this->getDataListByClass(#CDBClassName);	\
		QList<CDBClassName> result_list = QList<CDBClassName>();	\
		for (int i = 0; i < data_list.count(); i++){	\
			QSharedPointer<CDBClassName> data = data_list.at(i).dynamicCast<CDBClassName>(); \
			result_list.append(data->clone());	\
		} \
		return result_list;	\
	} \
	CDBClassName CDBChildContainerName::getData##CDBPartName##ByIndex(int index){ \
		QList<C_ContainerDataBasePtr> data_list = this->getDataListByClass(#CDBClassName);	\
		if (index < 0 || index >= data_list.count()){ \
			return CDBClassName(); \
		} \
		return data_list.at(index).dynamicCast<CDBClassName>()->clone(); \
	} \
	CDBClassName CDBChildContainerName::getData##CDBPartName##ById(QString id){ \
		QList<C_ContainerDataBasePtr> data_list = this->getDataListByClass(#CDBClassName);	\
		for (int i = 0; i < data_list.count(); i++) {	\
			QSharedPointer<CDBClassName> data = data_list.at(i).dynamicCast<CDBClassName>();	\
			if (data->getId() == id) {	\
				return data->clone();	\
			}	\
		}	\
		return CDBClassName();	\
	}	\
	QStringList CDBChildContainerName::getData##CDBPartName##AllId(){	\
		QList<C_ContainerDataBasePtr> data_list = this->getDataListByClass(#CDBClassName);	\
		QStringList result_list = QStringList();	\
		for (int i = 0; i < data_list.count(); i++){	\
			C_ContainerDataBasePtr data = data_list.at(i);	\
			result_list.append(data->getId());	\
		}	\
		return result_list;	\
	}
	//（最后一行不要加"\"，会出现错误"无法识别的标记"）

	

