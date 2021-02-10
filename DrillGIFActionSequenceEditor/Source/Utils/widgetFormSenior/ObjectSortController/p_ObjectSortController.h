#ifndef W_ObjectSortList_H
#define W_ObjectSortList_H

#include "c_ObjectSortData.h"

/*
-----==========================================================-----
		类：		对象排序 控制器.h
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		能将指定输入的列表进行排序，返回排序坐标、也可提供分页功能。
					设置数据（setData）时，【控制器才排序一次】，会重刷控制器的所有数据。
					（详细见cpp）
-----==========================================================-----
*/
class P_ObjectSortController : public QObject
{
	Q_OBJECT

	public:
		P_ObjectSortController(QWidget *parent = 0);
		~P_ObjectSortController();
		
	//-----------------------------------
	//----数据
	protected:
		QList<QJsonObject>* local_org_objList;						//载入时的obj
		QList<C_ObjectSortData*> local_org_dataList;				//载入时的排序数据列表
		QString m_symbol_id;										//标识名id
		QString m_symbol_name;										//标识名name
		QString m_symbol_type;										//标识名type
	public:
												//数据 - 设置数据（对象列表）
												//		【参数1】obj对象列表（默认会搜索 id, name, type）
												//		【参数2】id标识名（特殊称呼用，比如放的是"sid"）
												//		【参数3】name标识名（特殊称呼用）
												//		【参数4】type标识名（特殊称呼用）
												//		【说明】设置了对象列表，就能从 JsonObject获取函数 中获取到对象。
		void setData_FromObject(QList<QJsonObject>* obj_list);
		void setData_FromObject(QList<QJsonObject>* obj_list, QString id_symbol, QString name_symbol, QString type_symbol);
												//数据 - 设置数据（排序数据列表）
												//		【参数】id列表
												//		【说明】arrIndex可以不填，赋值时用不上。
		void setData_FromSortData(QList<C_ObjectSortData> sort_data_list);
												//数据 - 设置数据（基本列表）
												//		【参数1】id列表
												//		【参数2】name列表
												//		【参数3】type列表
												//		【说明】参数的列表长度必须一致。若设置为基本列表，则 JsonObject获取函数 只能为空。
		void setData_FromBasic(QList<int> id_list, QList<QString> name_list);
		void setData_FromBasic(QList<int> id_list, QList<QString> name_list, QList<QString> type_list);

												//数据 - 清除当前全部缓存
		void clearAll();
												//数据 - 获取数据数量
		int getDataCount();
												//数据JsonObject - 设置的数据是否为JsonObject对象
		bool isObjectData();
												//数据JsonObject - 修改标识名
		void modifySymbol(QString id_symbol, QString name_symbol, QString type_symbol);
												//数据JsonObject - 获取标识名
		QString getSymbolId();
		QString getSymbolName();
		QString getSymbolType();

	//-----------------------------------
	//----排序结果
	public:
												//排序结果 - 根据索引序列，获取到数据
		C_ObjectSortData* getSortData_ByIndex(int arrIndex);
		QList<C_ObjectSortData*> getSortDataList_ByIndex(QList<int> arrIndex_list);
												//排序结果 - 根据索引序列，获取到数据（注意，如果赋值时不是FromObject，则返回空）
		QJsonObject getObjectData_ByIndex(int arrIndex);
		QList<QJsonObject> getObjectDataList_ByIndex(QList<int> arrIndex_list);

	//-----------------------------------
	//----ID排序
	protected:
		QList<C_ObjectSortData*> local_dataList_orderBy_IdInc;		//按ID递增的列表
		QList<C_ObjectSortData*> local_dataList_orderBy_IdDec;		//按ID递减的列表
	public:
												//ID - 初始化（自动）
		void initSort_Id_IfRequire();
												//ID - 获取索引序列（升序）
		QList<int> get_IdInc_IndexSeq();
		QList<int> get_IdInc_IndexSeq(int bottom, int top);
												//ID - 获取指定页的索引序列（升序）
		QList<int> get_IdInc_PageIndexList(int page_index, int page_per_count);

												//ID - 获取索引序列（降序）
		QList<int> get_IdDec_IndexSeq();
		QList<int> get_IdDec_IndexSeq(int bottom, int top);
												//ID - 获取指定页的索引序列（降序）
		QList<int> get_IdDec_PageIndexList(int page_index, int page_per_count);
		
	//-----------------------------------
	//----名称排序
	protected:
		QList<C_ObjectSortData*> local_dataList_orderBy_NameInc;	//按名称递增的列表
		QList<C_ObjectSortData*> local_dataList_orderBy_NameDec;	//按名称递减的列表
		QString m_firstCharSeq;										//首字母序列
	public:
												//名称 - 初始化（自动）
		void initSort_Name_IfRequire();
												//名称 - 获取索引序列（升序）
		QList<int> get_NameInc_IndexSeq();
		QList<int> get_NameInc_IndexSeq(int bottom, int top);
												//名称 - 获取索引序列（降序）
		QList<int> get_NameDec_IndexSeq();
		QList<int> get_NameDec_IndexSeq(int bottom, int top);
												//名称 - 判断是否含指定首字母的数据
		bool hasAnyData_Name_ByCharacter(QChar ch);
												//名称 - 获取指定首字母的索引序列
		QList<int> get_Name_IndexListByCharacter(QChar ch);
												//名称 - 获取名称为空的索引序列
		QList<int> get_Name_IndexListByEmptyName();
		

	//-----------------------------------
	//----类型排序
	protected:
		QList<C_ObjectSortData*> local_dataList_orderBy_TypeAndId;			//按 类型+ID 排序的列表
		QList<C_ObjectSortData*> local_dataList_orderBy_TypeAndName;		//按 类型+名称 排序的列表
		QStringList m_distinguishedType;
	public:
												//类型 - 初始化（自动）
		void initSort_Type_IfRequire();
												//类型 - 判断是否含类型分组的数据
		bool hasAnyData_Type();
												//类型 - 获取所有类型
		QStringList get_Type_DistinguishedList();
												//类型 - 获取索引序列（类型+ID 顺序）
		QList<int> get_TypeAndId_IndexSeq();
		QList<int> get_TypeAndId_IndexSeq(int bottom, int top);
												//类型 - 获取指定类型的索引列（类型+ID 顺序）
		QList<int> get_TypeAndId_IndexListByType(QString type);
												//类型 - 获取索引序列（类型+名称 顺序）
		QList<int> get_TypeAndName_IndexSeq();
		QList<int> get_TypeAndName_IndexSeq(int bottom, int top);
												//类型 - 获取指定类型的索引列（类型+名称 顺序）
		QList<int> get_TypeAndName_IndexListByType(QString type);

		
	//-----------------------------------
	//----静态方法
	public:
		static QList<QJsonObject> _to_objectList_(QList<int> id_list, QList<QString> name_list);
		static QList<QJsonObject> _to_objectList_(QList<int> id_list, QList<QString> name_list, QList<QString> type_list);
};

#endif // W_ObjectSortList_H
