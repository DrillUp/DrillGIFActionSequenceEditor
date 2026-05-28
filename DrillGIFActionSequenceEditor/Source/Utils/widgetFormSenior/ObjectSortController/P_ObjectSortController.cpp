#include "stdafx.h"
#include "P_ObjectSortController.h"

#include "Source/Utils/Manager/ChineseManager/S_ChineseManager.h"

/*
-----==========================================================-----
		类：		对象排序 控制器.cpp
		版本：		v1.04
		作者：		drill_up
		所属模块：	工具模块
		功能：		能将指定输入的列表进行排序，返回排序坐标、也可提供分页功能。
					设置数据时（函数setData_XXXX），【控制器才排序一次】，会重刷控制器的所有数据。
					
		结构：		> 分页处理
					> 添加type类，可根据分类来选择

		使用方法：
				> 初始化
					this->m_p_ObjectSortController = new P_ObjectSortController();
					this->m_p_ObjectSortController->setData_FromObject(obj_list, key_id, key_name, key_type);
				> 获取id分页
					index_list = this->m_p_ObjectSortController->get_IdInc_PageIndexList( this->cur_selectIndex_page, 30 );
					
-----==========================================================-----
*/

P_ObjectSortController::P_ObjectSortController(QWidget *parent)
	: QObject(parent)
{

	//-----------------------------------
	//----初始化参数

	// > 数据
	this->local_org_objList = nullptr;
	this->local_org_dataList.clear();

	// > 排序结果
	//	（无）

	// > ID排序
	this->local_dataList_orderBy_IdInc.clear();
	this->local_dataList_orderBy_IdDec.clear();

	// > 名称排序
	this->local_dataList_orderBy_NameInc.clear();
	this->local_dataList_orderBy_NameDec.clear();
	this->m_firstCharSeq.clear();

	// > 类型排序
	this->local_dataList_orderBy_TypeAndId.clear();
	this->local_dataList_orderBy_TypeAndName.clear();
	this->m_distinguishedType.clear();
	
}
P_ObjectSortController::~P_ObjectSortController(){
}



/*-------------------------------------------------
		数据 - 设置数据（对象列表）
*/
void P_ObjectSortController::setData_FromObject(QList<QJsonObject>* obj_list) {
	this->local_org_objList = obj_list;
	if (this->local_org_objList->count() == 0){ return; }


	// > 标识名 - 初始化
	QString temp_key_id = "id";		//（默认小写，setData时会统一）
	QString temp_key_name = "name";
	QString temp_key_type = "type";

	// > 标识名 - 确定第一个obj的标识名
	QJsonObject first_obj = obj_list->at(0);
	if (first_obj.isEmpty()){ return; }
	QJsonValue v_id = first_obj.value("id");
	if (v_id.isUndefined()){ temp_key_id = "Id"; v_id = first_obj.value("Id"); }
	if (v_id.isUndefined()){ temp_key_id = "ID"; v_id = first_obj.value("ID"); }
	if (v_id.isUndefined()){ temp_key_id = ""; return; }
	QString name = first_obj.value("name").toString();
	if (name == ""){ temp_key_name = "Name"; name = first_obj.value("Name").toString(); }
	if (name == ""){ temp_key_name = "NAME"; name = first_obj.value("NAME").toString(); }
	if (name == ""){ temp_key_name = "name"; }	//（保持原标识）
	QString type = first_obj.value("type").toString();
	if (type == ""){ temp_key_type = "Type"; type = first_obj.value("Type").toString(); }
	if (type == ""){ temp_key_type = "TYPE"; type = first_obj.value("TYPE").toString(); }
	if (type == ""){ temp_key_type = "type"; }	//（保持原标识）


	// > 准备列表值
	QList<int> id_list;
	QList<QString> name_list;
	QList<QString> type_list;
	for (int i = 0; i < obj_list->count(); i++){
		QJsonObject obj = obj_list->at(i);
		if (obj.isEmpty()){ continue; }

		QJsonValue v_id = obj.value(temp_key_id);
		if (v_id.isUndefined()){ continue; }
		int id = 0;
		if (v_id.isString()){ id = v_id.toString().toInt(); }
		else{ id = v_id.toInt(); }
		id_list.push_back(id);
		
		QString name = obj.value(temp_key_name).toString();
		name_list.push_back(name);

		QString type = obj.value(temp_key_type).toString();
		type_list.push_back(type);
	}

	// > 设置（基本列表）
	this->setData_FromBasic(id_list, name_list, type_list);
}
void P_ObjectSortController::setData_FromObject(QList<QJsonObject>* obj_list, QString key_id, QString key_name, QString key_type){
	this->local_org_objList = obj_list;
	if (this->local_org_objList->count() == 0){ return; }


	// > 标识名 - 初始化
	QString temp_key_id = key_id;
	QString temp_key_name = key_name;
	QString temp_key_type = key_type;


	// > 准备列表值
	QList<int> id_list;
	QList<QString> name_list;
	QList<QString> type_list;
	for (int i = 0; i < obj_list->count(); i++){
		QJsonObject obj = obj_list->at(i);
		if (obj.isEmpty()){ continue; }

		QJsonValue v_id = obj.value(temp_key_id);
		if (v_id.isUndefined()){ continue; }
		int id = 0;
		if (v_id.isString()){ id = v_id.toString().toInt(); }
		else{ id = v_id.toInt(); }
		id_list.push_back(id);

		QString name = obj.value(temp_key_name).toString();
		name_list.push_back(name);

		QString type = obj.value(temp_key_type).toString();
		type_list.push_back(type);
	}

	// > 设置（基本列表）
	this->setData_FromBasic(id_list, name_list, type_list);
}
/*-------------------------------------------------
		数据 - 设置数据（排序数据列表）
*/
void P_ObjectSortController::setData_FromSortData(QList<C_ObjectSortData> sort_data_list) {

	// > 准备列表值
	QList<int> id_list;
	QList<QString> name_list;
	QList<QString> type_list;
	for (int i = 0; i < sort_data_list.count(); i++){
		C_ObjectSortData data = sort_data_list.at(i);
		id_list.append(data.getID_Org());
		name_list.append(data.getName_Org());
		type_list.append(data.getType_Org());
	}

	// > 设置（基本列表）
	this->setData_FromBasic(id_list, name_list, type_list);
}
/*-------------------------------------------------
		数据 - 设置数据（基本列表）
*/
void P_ObjectSortController::setData_FromBasic(QList<int> id_list, QList<QString> name_list) {
	if (id_list.count() != name_list.count()){ return; }
	QStringList type_list;
	for (int i = 0; i < id_list.count(); i++){
		type_list.append("");
	}
	this->setData_FromBasic(id_list, name_list, type_list);
}
void P_ObjectSortController::setData_FromBasic(QList<int> id_list, QList<QString> name_list, QList<QString> type_list) {
	if (id_list.count() != name_list.count()){ return; }
	if (id_list.count() != type_list.count()){ return; }

	// > 清理数据
	this->clearAll();

	// > 数据赋值
	for (int i = 0; i < id_list.count(); i++){
		int id = id_list.at(i);
		QString name = name_list.at(i);
		QString type = type_list.at(i);

		C_ObjectSortData* data = new C_ObjectSortData();
		data->initData(i, id, name, type);
		this->local_org_dataList.append(data);
	}
}
/*-------------------------------------------------
		数据 - 清理数据
*/
void P_ObjectSortController::clearAll() {

	// > 数据
	for (int i = 0; i < this->local_org_dataList.count(); i++){
		delete this->local_org_dataList.at(i);
	}
	this->local_org_dataList.clear();

	// > 排序结果
	//	（无）

	// > ID排序
	this->local_dataList_orderBy_IdInc.clear();
	this->local_dataList_orderBy_IdDec.clear();

	// > 名称排序
	this->local_dataList_orderBy_NameInc.clear();
	this->local_dataList_orderBy_NameDec.clear();
	this->m_firstCharSeq.clear();

	// > 类型排序
	this->local_dataList_orderBy_TypeAndId.clear();
	this->local_dataList_orderBy_TypeAndName.clear();
	this->m_distinguishedType.clear();
}
/*-------------------------------------------------
		数据 - 获取数据数量
*/
int P_ObjectSortController::getDataCount(){
	return this->local_org_dataList.count();
}
/*-------------------------------------------------
		数据 - 设置的数据是否为JsonObject对象
*/
bool P_ObjectSortController::isObjectData(){
	return this->local_org_objList != nullptr;
}


/*-------------------------------------------------
		排序结果 - 根据索引序列，获取到数据
*/
C_ObjectSortData* P_ObjectSortController::getSortData_ByIndex(int arrIndex){
	return this->local_org_dataList.at(arrIndex);
}
QList<C_ObjectSortData*> P_ObjectSortController::getSortDataList_ByIndex(QList<int> arrIndex_list){
	QList<C_ObjectSortData*> result_list;
	for (int i = 0; i < arrIndex_list.count(); i++){
		int index = arrIndex_list.at(i);
		result_list.push_back(this->local_org_dataList.at(index));
	}
	return result_list;
}
/*-------------------------------------------------
		排序结果 - 根据索引序列，获取到数据
*/
QJsonObject P_ObjectSortController::getObjectData_ByIndex(int arrIndex){
	return this->local_org_objList->at(arrIndex);
}
QList<QJsonObject> P_ObjectSortController::getObjectDataList_ByIndex(QList<int> arrIndex_list){
	if (this->local_org_objList == nullptr){ return QList<QJsonObject>(); }
	QList<QJsonObject> result_list;
	for (int i = 0; i < arrIndex_list.count(); i++){
		int index = arrIndex_list.at(i);
		result_list.push_back(this->local_org_objList->at(index));
	}
	return result_list;
}



/*-------------------------------------------------
		ID排序
*/
bool P_ObjectSortController_sortBy_IdInc(const C_ObjectSortData* a, const C_ObjectSortData* b){
	return a->m_sort_id < b->m_sort_id;
}
bool P_ObjectSortController_sortBy_IdDec(const C_ObjectSortData* a, const C_ObjectSortData* b){
	return a->m_sort_id > b->m_sort_id;
}
/*-------------------------------------------------
		ID - 初始化（自动）
*/
void P_ObjectSortController::initSort_Id_IfRequire(){
	if (this->local_dataList_orderBy_IdInc.count() != 0){ return; }
	if (this->local_dataList_orderBy_IdDec.count() != 0){ return; }

	// > id列表赋值并排序
	this->local_dataList_orderBy_IdInc = this->local_org_dataList;
	this->local_dataList_orderBy_IdDec = this->local_org_dataList;
	qSort(this->local_dataList_orderBy_IdInc.begin(), this->local_dataList_orderBy_IdInc.end(), P_ObjectSortController_sortBy_IdInc);
	qSort(this->local_dataList_orderBy_IdDec.begin(), this->local_dataList_orderBy_IdDec.end(), P_ObjectSortController_sortBy_IdDec);
}
/*-------------------------------------------------
		ID - 获取索引序列（升序）
*/
QList<int> P_ObjectSortController::get_IdInc_IndexSeq(){
	this->initSort_Id_IfRequire();
	return this->get_IdInc_IndexSeq( 0, this->local_org_dataList.count()-1 );
}
QList<int> P_ObjectSortController::get_IdInc_IndexSeq(int bottom, int top){
	this->initSort_Id_IfRequire();
	if (top < bottom){ int pos = top; top = bottom; bottom = pos; }
	if (top > this->local_org_dataList.count() - 1){ top = this->local_org_dataList.count() - 1; }

	QList<int> index_list;
	for (int i = bottom; i <= top; i++){
		int index = this->local_dataList_orderBy_IdInc.at(i)->getArrIndex();
		index_list.push_back(index);
	}
	return index_list;
}
/*-------------------------------------------------
		ID - 获取索引序列（降序）
*/
QList<int> P_ObjectSortController::get_IdDec_IndexSeq(){
	this->initSort_Id_IfRequire();
	return this->get_IdDec_IndexSeq(0, this->local_org_dataList.count() - 1);
}
QList<int> P_ObjectSortController::get_IdDec_IndexSeq(int bottom, int top){
	this->initSort_Id_IfRequire();
	if (top < bottom){ int pos = top; top = bottom; bottom = pos; }
	if (top > this->local_org_dataList.count() - 1){ top = this->local_org_dataList.count() - 1; }

	QList<int> index_list;
	for (int i = bottom; i <= top; i++){
		int index = this->local_dataList_orderBy_IdDec.at(i)->getArrIndex();
		index_list.push_back(index);
	}
	return index_list;
}
/*-------------------------------------------------
		ID - 获取指定页的索引序列（升序）
*/
QList<int> P_ObjectSortController::get_IdInc_PageIndexList(int page_index, int page_per_count){
	this->initSort_Id_IfRequire();
	QList<int> result_list;
	int all_count = this->local_org_dataList.count();
	int page_count = qCeil((double)all_count/page_per_count);

	int bottom = page_index * page_per_count + 1;
	int top = (page_index + 1) * page_per_count;
	if (top > all_count){
		top = all_count;
	}
	for (int i = bottom; i <= top; i++){
		result_list.append( i-1 );
	}
	return result_list;
}
/*-------------------------------------------------
		ID - 获取指定页的索引序列（降序）
*/
QList<int> P_ObjectSortController::get_IdDec_PageIndexList(int page_index, int page_per_count){
	this->initSort_Id_IfRequire();
	QList<int> result_list;
	int all_count = this->local_org_dataList.count();
	int page_count = qCeil((double)all_count / page_per_count);

	int index = page_count - page_index - 1;
	int bottom = index * page_per_count + 1;
	int top = (index + 1) * page_per_count;
	if (top > all_count){
		top = all_count;
	}
	for (int i = top; i >=bottom; i--){
		result_list.append( i-1 );
	}
	return result_list;
}



/*-------------------------------------------------
		名称排序
*/
bool P_ObjectSortController_sortBy_NameInc(const C_ObjectSortData* a, const C_ObjectSortData* b){
	if (a->m_sort_name == ""){ return false; }
	if (b->m_sort_name == ""){ return true; }
	return a->m_sort_name.toLocal8Bit() < b->m_sort_name.toLocal8Bit();
}
bool P_ObjectSortController_sortBy_NameDec(const C_ObjectSortData* a, const C_ObjectSortData* b){
	if (a->m_sort_name == ""){ return true; }
	if (b->m_sort_name == ""){ return false; }
	return a->m_sort_name.toLocal8Bit() > b->m_sort_name.toLocal8Bit();
}
/*-------------------------------------------------
		名称 - 初始化（自动）
*/
void P_ObjectSortController::initSort_Name_IfRequire(){
	if (this->local_dataList_orderBy_NameInc.count() != 0){ return; }
	if (this->local_dataList_orderBy_NameDec.count() != 0){ return; }

	// > 名称列表赋值并排序
	this->local_dataList_orderBy_NameInc = this->local_org_dataList;
	this->local_dataList_orderBy_NameDec = this->local_org_dataList;
	qSort(this->local_dataList_orderBy_NameInc.begin(), this->local_dataList_orderBy_NameInc.end(), P_ObjectSortController_sortBy_NameInc);
	qSort(this->local_dataList_orderBy_NameDec.begin(), this->local_dataList_orderBy_NameDec.end(), P_ObjectSortController_sortBy_NameDec);

	// > 序列位置
	this->m_firstCharSeq.clear();
	for (int i = 0; i < this->local_dataList_orderBy_NameInc.count(); i++){
		QString name = this->local_dataList_orderBy_NameInc.at(i)->getName_Sort();

		// > 获取第一个字的首字母
		if (name.count() > 0){
			this->m_firstCharSeq.push_back(name.at(0));
		
		// > 获取不到，则不操作
		}else{
			//（空字符肯定会排到后排，超出 m_firstCharSeq 的数组长度）
		}
	}

}
/*-------------------------------------------------
		名称 - 获取索引序列（升序）
*/
QList<int> P_ObjectSortController::get_NameInc_IndexSeq(){
	this->initSort_Name_IfRequire();
	return this->get_NameInc_IndexSeq(0, this->local_org_dataList.count() - 1);
}
QList<int> P_ObjectSortController::get_NameInc_IndexSeq(int bottom, int top){
	this->initSort_Name_IfRequire();
	if (top < bottom){ int pos = top; top = bottom; bottom = pos; }
	if (top > this->local_org_dataList.count() - 1){ top = this->local_org_dataList.count() - 1; }

	QList<int> index_list;
	for (int i = bottom; i <= top; i++){
		int index = this->local_dataList_orderBy_NameInc.at(i)->getArrIndex();
		index_list.push_back(index);
	}
	return index_list;
}
/*-------------------------------------------------
		名称 - 获取索引序列（降序）
*/
QList<int> P_ObjectSortController::get_NameDec_IndexSeq(){
	this->initSort_Name_IfRequire();
	return this->get_NameDec_IndexSeq(0, this->local_org_dataList.count() - 1);
}
QList<int> P_ObjectSortController::get_NameDec_IndexSeq(int bottom, int top){
	this->initSort_Name_IfRequire();
	if (top < bottom){ int pos = top; top = bottom; bottom = pos; }
	if (top > this->local_org_dataList.count() - 1){ top = this->local_org_dataList.count() - 1; }

	QList<int> index_list;
	for (int i = bottom; i <= top; i++){
		int index = this->local_dataList_orderBy_NameDec.at(i)->getArrIndex();
		index_list.push_back(index);
	}
	return index_list;
}
/*-------------------------------------------------
		名称 - 判断是否含指定首字母的数据
*/
bool P_ObjectSortController::hasAnyData_Name_ByCharacter(QChar ch){
	ch = ch.toLower();
	return this->m_firstCharSeq.contains(ch);
}
/*-------------------------------------------------
		名称 - 获取指定首字母的索引序列
*/
QList<int> P_ObjectSortController::get_Name_IndexListByCharacter(QChar ch){
	this->initSort_Name_IfRequire();
	QList<int> result_list;
	ch = ch.toLower();

	int bottom = this->m_firstCharSeq.indexOf(ch);		//（首字母序列是递增的，与递增名称序列对应）
	int top = this->m_firstCharSeq.lastIndexOf(ch);		//（比如排序为："aaaabbffzz@@"， @为生僻字）
	if (bottom == -1){ return QList<int>(); }
	if (top == -1){ return QList<int>(); }
	for (int i = bottom; i <= top; i++){
		int arr_index = this->local_dataList_orderBy_NameInc.at(i)->getArrIndex();
		result_list.append(arr_index);
	}
	return result_list;
}
/*-------------------------------------------------
		名称 - 获取非字母的索引序列
*/
QList<int> P_ObjectSortController::get_Name_IndexListByNonAlphabetic(){
	this->initSort_Name_IfRequire();
	QList<int> result_list;
	
	// > 字母前面的符号
	int index = this->m_firstCharSeq.indexOf(QRegExp("[a-z@]"));	//其他符号都会排在字母的前面
	if (index > 0){
		for (int i = 0; i < index; i++){
			int arr_index = this->local_dataList_orderBy_NameInc.at(i)->getArrIndex();
			result_list.append(arr_index);
		}
	}

	// > 后面的符号（z后面的符号： 123{  124|  125}  126~ ）
	{
		int bottom = this->m_firstCharSeq.indexOf('|');
		int top = this->m_firstCharSeq.lastIndexOf('|');
		if (bottom != -1 && top != -1){
			for (int i = bottom; i <= top; i++){
				int arr_index = this->local_dataList_orderBy_NameInc.at(i)->getArrIndex();
				result_list.append(arr_index);
			}
		}
	}
	{
		int bottom = this->m_firstCharSeq.indexOf('~');
		int top = this->m_firstCharSeq.lastIndexOf('~');
		if (bottom != -1 && top != -1){
			for (int i = bottom; i <= top; i++){
				int arr_index = this->local_dataList_orderBy_NameInc.at(i)->getArrIndex();
				result_list.append(arr_index);
			}
		}
	}

	return result_list;
}
/*-------------------------------------------------
		名称 - 获取名称为空的索引序列
*/
QList<int> P_ObjectSortController::get_Name_IndexListByEmptyName(){
	this->initSort_Name_IfRequire();
	QList<int> result_list;
	int ch_count = this->m_firstCharSeq.count();		//（名称为空的对象排在末尾，且没有首字母）
	
	for (int i = ch_count; i < this->local_dataList_orderBy_NameInc.count(); i++){
		int arr_index = this->local_dataList_orderBy_NameInc.at(i)->getArrIndex();
		result_list.append(arr_index);
	}
	return result_list;
}



/*-------------------------------------------------
		类型+id 排序
*/
bool P_ObjectSortController_sortBy_TypeAndId(const C_ObjectSortData* a, const C_ObjectSortData* b){
	if (a->m_sort_type == b->m_sort_type){
		return a->m_sort_id < b->m_sort_id;
	}
	return a->m_sort_type.toLocal8Bit() < b->m_sort_type.toLocal8Bit();
}
/*-------------------------------------------------
		类型+名称 排序
*/
bool P_ObjectSortController_sortBy_TypeAndName(const C_ObjectSortData* a, const C_ObjectSortData* b){
	if (a->m_sort_type == b->m_sort_type){
		if (a->m_sort_name == ""){ return false; }
		if (b->m_sort_name == ""){ return true; }
		return a->m_sort_name.toLocal8Bit() < b->m_sort_name.toLocal8Bit();
	}
	return a->m_sort_type.toLocal8Bit() < b->m_sort_type.toLocal8Bit();
}
/*-------------------------------------------------
		类型 - 初始化（自动）
*/
void P_ObjectSortController::initSort_Type_IfRequire(){
	if (this->local_dataList_orderBy_TypeAndId.count() != 0){ return; }
	if (this->local_dataList_orderBy_TypeAndName.count() != 0){ return; }

	// > 根据类型赋值并排序
	this->local_dataList_orderBy_TypeAndId = this->local_org_dataList;
	this->local_dataList_orderBy_TypeAndName = this->local_org_dataList;
	qSort(this->local_dataList_orderBy_TypeAndId.begin(), this->local_dataList_orderBy_TypeAndId.end(), P_ObjectSortController_sortBy_TypeAndId);
	qSort(this->local_dataList_orderBy_TypeAndName.begin(), this->local_dataList_orderBy_TypeAndName.end(), P_ObjectSortController_sortBy_TypeAndName);

	// > 获取不重复的类型
	this->m_distinguishedType.clear();
	for (int i = 0; i < this->local_org_dataList.count(); i++){
		QString type = this->local_org_dataList.at(i)->getType_Org();
		if (this->m_distinguishedType.contains(type)){
			continue;
		}
		this->m_distinguishedType.push_back(type);
	}
}
/*-------------------------------------------------
		类型 - 判断是否含类型分组的数据
*/
bool P_ObjectSortController::hasAnyData_Type(){
	this->initSort_Type_IfRequire();
	for (int i = 0; i < this->local_org_dataList.count(); i++){
		C_ObjectSortData* data = this->local_org_dataList.at(i);
		if (data->getType_Org().isEmpty() == false){
			return true;
		}
	}
	return false;
}
/*-------------------------------------------------
		类型 - 获取所有类型
*/
QStringList P_ObjectSortController::get_Type_DistinguishedList(){
	this->initSort_Type_IfRequire();
	return this->m_distinguishedType;
}
/*-------------------------------------------------
		类型 - 获取索引序列（类型+ID 顺序）
*/
QList<int> P_ObjectSortController::get_TypeAndId_IndexSeq(){
	this->initSort_Type_IfRequire();
	return this->get_TypeAndId_IndexSeq(0, this->local_org_dataList.count() - 1);
}
QList<int> P_ObjectSortController::get_TypeAndId_IndexSeq(int bottom, int top){
	this->initSort_Type_IfRequire();
	if (top < bottom){ int pos = top; top = bottom; bottom = pos; }
	if (top > this->local_org_dataList.count() - 1){ top = this->local_org_dataList.count()-1; }

	QList<int> index_list;
	for (int i = bottom; i <= top; i++){
		int index = this->local_dataList_orderBy_TypeAndId.at(i)->getArrIndex();
		index_list.push_back(index);
	}
	return index_list;
}
/*-------------------------------------------------
		类型 - 获取指定类型的索引列（类型+ID 顺序）
*/
QList<int> P_ObjectSortController::get_TypeAndId_IndexListByType(QString type){
	this->initSort_Type_IfRequire();

	QList<int> index_list;
	for (int i = 0; i < this->local_dataList_orderBy_TypeAndId.count(); i++){
		if (this->local_dataList_orderBy_TypeAndId.at(i)->getType_Org() != type){
			continue;
		}
		int index = this->local_dataList_orderBy_TypeAndId.at(i)->getArrIndex();
		index_list.push_back(index);
	}
	return index_list;
}
/*-------------------------------------------------
		类型 - 获取索引序列（类型+名称 顺序）
*/
QList<int> P_ObjectSortController::get_TypeAndName_IndexSeq(){
	this->initSort_Type_IfRequire();
	return this->get_TypeAndName_IndexSeq(0, this->local_org_dataList.count() - 1);
}
QList<int> P_ObjectSortController::get_TypeAndName_IndexSeq(int bottom, int top){
	this->initSort_Type_IfRequire();
	if (top < bottom){ int pos = top; top = bottom; bottom = pos; }
	if (top > this->local_org_dataList.count() - 1){ top = this->local_org_dataList.count()-1; }

	QList<int> index_list;
	for (int i = bottom; i <= top; i++){
		int index = this->local_dataList_orderBy_TypeAndName.at(i)->getArrIndex();
		index_list.push_back(index);
	}
	return index_list;
}
/*-------------------------------------------------
		类型 - 获取指定类型的索引列（类型+名称 顺序）
*/
QList<int> P_ObjectSortController::get_TypeAndName_IndexListByType(QString type){
	this->initSort_Type_IfRequire();

	QList<int> index_list;
	for (int i = 0; i < this->local_dataList_orderBy_TypeAndName.count(); i++){
		if (this->local_dataList_orderBy_TypeAndName.at(i)->getType_Org() != type){
			continue;
		}
		int index = this->local_dataList_orderBy_TypeAndName.at(i)->getArrIndex();
		index_list.push_back(index);
	}
	return index_list;
}


/*-------------------------------------------------
		静态方法
*/
QList<QJsonObject> P_ObjectSortController::_to_objectList_(QList<int> id_list, QList<QString> name_list){
	QStringList type_list;
	for (int i = 0; i < id_list.count(); i++){
		type_list.append("");
	}
	return P_ObjectSortController::_to_objectList_(id_list, name_list, type_list);
}
QList<QJsonObject> P_ObjectSortController::_to_objectList_(QList<int> id_list, QList<QString> name_list, QList<QString> type_list){
	if (id_list.count() != name_list.count()){ return QList<QJsonObject>(); }
	if (id_list.count() != type_list.count()){ return QList<QJsonObject>(); }

	QList<QJsonObject> result_list;
	for (int i = 0; i < id_list.count(); i++){
		QJsonObject object;
		object.insert("id", id_list.at(i));
		object.insert("name", name_list.at(i));
		object.insert("type", type_list.at(i));
		result_list.append(object);
	}
	return result_list;
}