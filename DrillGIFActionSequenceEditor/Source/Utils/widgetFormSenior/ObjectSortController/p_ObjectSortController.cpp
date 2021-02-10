#include "stdafx.h"
#include "p_ObjectSortController.h"

#include "Source/Utils/manager/chineseManager/s_ChineseManager.h"

/*
-----==========================================================-----
		类：		对象排序 控制器.cpp
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		能将指定输入的列表进行排序，返回排序坐标、也可提供分页功能。
					设置数据（setData）时，【控制器才排序一次】，会重刷控制器的所有数据。
					
		结构：		> 分页处理
					> 添加type类，可根据分类来选择

		使用方法：
				> 初始化
					this->m_p_ObjectSortController = new P_ObjectSortController();
					this->m_p_ObjectSortController->setData_FromObject(obj_list, id_symbol, name_symbol, type_symbol);
				> 获取id分页
					index_list = this->m_p_ObjectSortController->get_IdInc_PageIndexList( this->cur_selectIndex_page, 30 );
					
-----==========================================================-----
*/

P_ObjectSortController::P_ObjectSortController(QWidget *parent)
	: QObject(parent)
{

	//-----------------------------------
	//----初始化参数
	this->local_org_objList = nullptr;
	this->local_org_dataList = QList<C_ObjectSortData*>();
	this->m_symbol_id = "id";	//（默认小写，setData时会统一）
	this->m_symbol_name = "name";
	this->m_symbol_type = "type";

	this->local_dataList_orderBy_IdInc = QList<C_ObjectSortData*>();
	this->local_dataList_orderBy_IdDec = QList<C_ObjectSortData*>();
	this->local_dataList_orderBy_NameInc = QList<C_ObjectSortData*>();
	this->local_dataList_orderBy_NameDec = QList<C_ObjectSortData*>();
	this->local_dataList_orderBy_TypeAndId = QList<C_ObjectSortData*>();
	this->local_dataList_orderBy_TypeAndName = QList<C_ObjectSortData*>();
	
}

P_ObjectSortController::~P_ObjectSortController(){
}



/*-------------------------------------------------
		数据 - 设置数据（对象列表）
*/
void P_ObjectSortController::setData_FromObject(QList<QJsonObject>* obj_list) {
	this->local_org_objList = obj_list;
	if (this->local_org_objList->count() == 0){ return; }

	// > 确定第一个obj的标识名
	QJsonObject first_obj = obj_list->at(0);
	if (first_obj.isEmpty()){ return; }
	QJsonValue v_id = first_obj.value("id");
	if (v_id.isUndefined()){ this->m_symbol_id = "Id"; v_id = first_obj.value("Id"); }
	if (v_id.isUndefined()){ this->m_symbol_id = "ID"; v_id = first_obj.value("ID"); }
	if (v_id.isUndefined()){ this->m_symbol_id = ""; return; }
	QString name = first_obj.value("name").toString();
	if (name == ""){ this->m_symbol_name = "Name"; name = first_obj.value("Name").toString(); }
	if (name == ""){ this->m_symbol_name = "NAME"; name = first_obj.value("NAME").toString(); }
	if (name == ""){ this->m_symbol_name = "name"; }	//（保持原标识）
	QString type = first_obj.value("type").toString();
	if (type == ""){ this->m_symbol_type = "Type"; type = first_obj.value("Type").toString(); }
	if (type == ""){ this->m_symbol_type = "TYPE"; type = first_obj.value("TYPE").toString(); }
	if (type == ""){ this->m_symbol_type = "type"; }	//（保持原标识）

	// > 准备列表值
	QList<int> id_list = QList<int>();
	QList<QString> name_list = QList<QString>();
	QList<QString> type_list = QList<QString>();
	for (int i = 0; i < obj_list->count(); i++){
		QJsonObject obj = obj_list->at(i);
		if (obj.isEmpty()){ continue; }

		QJsonValue v_id = obj.value(this->m_symbol_id);
		if (v_id.isUndefined()){ continue; }
		int id = 0;
		if (v_id.isString()){ id = v_id.toString().toInt(); }
		else{ id = v_id.toInt(); }
		id_list.push_back(id);
		
		QString name = obj.value(this->m_symbol_name).toString();
		name_list.push_back(name);

		QString type = obj.value(this->m_symbol_type).toString();
		type_list.push_back(type);
	}

	// > 设置（基本列表）
	this->setData_FromBasic(id_list, name_list, type_list);
}
void P_ObjectSortController::setData_FromObject(QList<QJsonObject>* obj_list, QString id_symbol, QString name_symbol, QString type_symbol){
	this->local_org_objList = obj_list;
	if (this->local_org_objList->count() == 0){ return; }

	// > 确定标识名
	this->m_symbol_id = id_symbol;
	this->m_symbol_name = name_symbol;
	this->m_symbol_type = type_symbol;

	// > 准备列表值
	QList<int> id_list = QList<int>();
	QList<QString> name_list = QList<QString>();
	QList<QString> type_list = QList<QString>();
	for (int i = 0; i < obj_list->count(); i++){
		QJsonObject obj = obj_list->at(i);
		if (obj.isEmpty()){ continue; }

		QJsonValue v_id = obj.value(this->m_symbol_id);
		if (v_id.isUndefined()){ continue; }
		int id = 0;
		if (v_id.isString()){ id = v_id.toString().toInt(); }
		else{ id = v_id.toInt(); }
		id_list.push_back(id);

		QString name = obj.value(this->m_symbol_name).toString();
		name_list.push_back(name);

		QString type = obj.value(this->m_symbol_type).toString();
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
	QList<int> id_list = QList<int>();
	QList<QString> name_list = QList<QString>();
	QList<QString> type_list = QList<QString>();
	for (int i = 0; i < sort_data_list.count(); i++){
		C_ObjectSortData data = sort_data_list.at(i);
		id_list.append(data.id);
		name_list.append(data.name);
		type_list.append(data.type);
	}

	// > 设置（基本列表）
	this->setData_FromBasic(id_list, name_list, type_list);
}
/*-------------------------------------------------
		数据 - 设置数据（基本列表）
*/
void P_ObjectSortController::setData_FromBasic(QList<int> id_list, QList<QString> name_list) {
	if (id_list.count() != name_list.count()){ return; }
	QStringList type_list = QStringList();
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
		C_ObjectSortData* data = new C_ObjectSortData(i, id, name, type);
		this->local_org_dataList.append(data);
	}
}
/*-------------------------------------------------
		数据 - 清理数据
*/
void P_ObjectSortController::clearAll() {

	this->local_dataList_orderBy_IdInc.clear();
	this->local_dataList_orderBy_IdDec.clear();
	this->local_dataList_orderBy_NameInc.clear();
	this->local_dataList_orderBy_NameDec.clear();
	this->local_dataList_orderBy_TypeAndId.clear();
	this->local_dataList_orderBy_TypeAndName.clear();
	
	for (int i = 0; i < this->local_org_dataList.count(); i++){
		delete this->local_org_dataList.at(i);
	}
	this->local_org_dataList.clear();

}
/*-------------------------------------------------
		数据 - 获取数据数量
*/
int P_ObjectSortController::getDataCount(){
	return this->local_org_dataList.count();
}
/*-------------------------------------------------
		数据 - 修改标识名
*/
void P_ObjectSortController::modifySymbol(QString id_symbol, QString name_symbol, QString type_symbol){
	this->m_symbol_id = id_symbol;
	this->m_symbol_name = name_symbol;
	this->m_symbol_type = type_symbol;
}
/*-------------------------------------------------
		数据 - 获取标识名
*/
QString P_ObjectSortController::getSymbolId(){
	return this->m_symbol_id;
}
QString P_ObjectSortController::getSymbolName(){
	return this->m_symbol_name;
}
QString P_ObjectSortController::getSymbolType(){
	return this->m_symbol_type;
}
/*-------------------------------------------------
		数据 - 设置的数据是否为JsonObject对象
*/
bool P_ObjectSortController::isObjectData(){
	return this->local_org_objList != nullptr;
}
/*-------------------------------------------------
		数据 - 根据索引序列，获取到数据
*/
C_ObjectSortData* P_ObjectSortController::getSortData_ByIndex(int arrIndex){
	return this->local_org_dataList.at(arrIndex);
}
QList<C_ObjectSortData*> P_ObjectSortController::getSortDataList_ByIndex(QList<int> arrIndex_list){
	QList<C_ObjectSortData*> result_list = QList<C_ObjectSortData*>();
	for (int i = 0; i < arrIndex_list.count(); i++){
		int index = arrIndex_list.at(i);
		result_list.push_back(this->local_org_dataList.at(index));
	}
	return result_list;
}
QJsonObject P_ObjectSortController::getObjectData_ByIndex(int arrIndex){
	return this->local_org_objList->at(arrIndex);
}
QList<QJsonObject> P_ObjectSortController::getObjectDataList_ByIndex(QList<int> arrIndex_list){
	if (this->local_org_objList == nullptr){ return QList<QJsonObject>(); }
	QList<QJsonObject> result_list = QList<QJsonObject>();
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
	return a->id < b->id;
}
bool P_ObjectSortController_sortBy_IdDec(const C_ObjectSortData* a, const C_ObjectSortData* b){
	return a->id > b->id;
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

	QList<int> index_list = QList<int>();
	for (int i = bottom; i <= top; i++){
		int index = this->local_dataList_orderBy_IdInc.at(i)->arrIndex;
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

	QList<int> index_list = QList<int>();
	for (int i = bottom; i <= top; i++){
		int index = this->local_dataList_orderBy_IdDec.at(i)->arrIndex;
		index_list.push_back(index);
	}
	return index_list;
}
/*-------------------------------------------------
		ID - 获取指定页的索引序列（升序）
*/
QList<int> P_ObjectSortController::get_IdInc_PageIndexList(int page_index, int page_per_count){
	this->initSort_Id_IfRequire();
	QList<int> result_list = QList<int>();
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
	QList<int> result_list = QList<int>();
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
	if (a->name == ""){ return false; }
	if (b->name == ""){ return true; }
	return a->name.toLocal8Bit() < b->name.toLocal8Bit();
}
bool P_ObjectSortController_sortBy_NameDec(const C_ObjectSortData* a, const C_ObjectSortData* b){
	if (a->name == ""){ return true; }
	if (b->name == ""){ return false; }
	return a->name.toLocal8Bit() > b->name.toLocal8Bit();
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
	this->m_firstCharSeq = "";
	for (int i = 0; i < this->local_dataList_orderBy_NameInc.count(); i++){
		QString name = this->local_dataList_orderBy_NameInc.at(i)->name;
		if (name.count() > 0){
			this->m_firstCharSeq.push_back(S_ChineseManager::getInstance()->getChineseFirstSpell(name.at(0)));
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

	QList<int> index_list = QList<int>();
	for (int i = bottom; i <= top; i++){
		int index = this->local_dataList_orderBy_NameInc.at(i)->arrIndex;
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

	QList<int> index_list = QList<int>();
	for (int i = bottom; i <= top; i++){
		int index = this->local_dataList_orderBy_NameDec.at(i)->arrIndex;
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
	QList<int> result_list = QList<int>();
	ch = ch.toLower();

	int bottom = this->m_firstCharSeq.indexOf(ch);		//（首字母序列是递增的，与递增名称序列对应）
	int top = this->m_firstCharSeq.lastIndexOf(ch);
	if (bottom == -1){ return QList<int>(); }
	if (top == -1){ return QList<int>(); }
	for (int i = bottom; i <= top; i++){
		int arr_index = this->local_dataList_orderBy_NameInc.at(i)->arrIndex;
		result_list.append(arr_index);
	}
	return result_list;
}
/*-------------------------------------------------
		名称 - 获取名称为空的索引序列
*/
QList<int> P_ObjectSortController::get_Name_IndexListByEmptyName(){
	this->initSort_Name_IfRequire();
	QList<int> result_list = QList<int>();
	int ch_count = this->m_firstCharSeq.count();		//（名称为空的对象排在末尾，且没有首字母）
	
	for (int i = ch_count; i < this->local_dataList_orderBy_NameInc.count(); i++){
		int arr_index = this->local_dataList_orderBy_NameInc.at(i)->arrIndex;
		result_list.append(arr_index);
	}
	return result_list;
}



/*-------------------------------------------------
		类型+id 排序
*/
bool P_ObjectSortController_sortBy_TypeAndId(const C_ObjectSortData* a, const C_ObjectSortData* b){
	if (a->type == b->type){
		return a->id < b->id;
	}
	return a->type.toLocal8Bit() < b->type.toLocal8Bit();
}
/*-------------------------------------------------
		类型+名称 排序
*/
bool P_ObjectSortController_sortBy_TypeAndName(const C_ObjectSortData* a, const C_ObjectSortData* b){
	if (a->type == b->type){
		if (a->name == ""){ return false; }
		if (b->name == ""){ return true; }
		return a->name.toLocal8Bit() < b->name.toLocal8Bit();
	}
	return a->type.toLocal8Bit() < b->type.toLocal8Bit();
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
	this->m_distinguishedType = QStringList();
	for (int i = 0; i < this->local_org_dataList.count(); i++){
		QString type = this->local_org_dataList.at(i)->type;
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
		if (data->type.isEmpty() == false){
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

	QList<int> index_list = QList<int>();
	for (int i = bottom; i <= top; i++){
		int index = this->local_dataList_orderBy_TypeAndId.at(i)->arrIndex;
		index_list.push_back(index);
	}
	return index_list;
}
/*-------------------------------------------------
		类型 - 获取指定类型的索引列（类型+ID 顺序）
*/
QList<int> P_ObjectSortController::get_TypeAndId_IndexListByType(QString type){
	this->initSort_Type_IfRequire();

	QList<int> index_list = QList<int>();
	for (int i = 0; i < this->local_dataList_orderBy_TypeAndId.count(); i++){
		if (this->local_dataList_orderBy_TypeAndId.at(i)->type != type){
			continue;
		}
		int index = this->local_dataList_orderBy_TypeAndId.at(i)->arrIndex;
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

	QList<int> index_list = QList<int>();
	for (int i = bottom; i <= top; i++){
		int index = this->local_dataList_orderBy_TypeAndName.at(i)->arrIndex;
		index_list.push_back(index);
	}
	return index_list;
}
/*-------------------------------------------------
		类型 - 获取指定类型的索引列（类型+名称 顺序）
*/
QList<int> P_ObjectSortController::get_TypeAndName_IndexListByType(QString type){
	this->initSort_Type_IfRequire();

	QList<int> index_list = QList<int>();
	for (int i = 0; i < this->local_dataList_orderBy_TypeAndName.count(); i++){
		if (this->local_dataList_orderBy_TypeAndName.at(i)->type != type){
			continue;
		}
		int index = this->local_dataList_orderBy_TypeAndName.at(i)->arrIndex;
		index_list.push_back(index);
	}
	return index_list;
}


/*-------------------------------------------------
		静态方法
*/
QList<QJsonObject> P_ObjectSortController::_to_objectList_(QList<int> id_list, QList<QString> name_list){
	QStringList type_list = QStringList();
	for (int i = 0; i < id_list.count(); i++){
		type_list.append("");
	}
	return P_ObjectSortController::_to_objectList_(id_list, name_list, type_list);
}
QList<QJsonObject> P_ObjectSortController::_to_objectList_(QList<int> id_list, QList<QString> name_list, QList<QString> type_list){
	if (id_list.count() != name_list.count()){ return QList<QJsonObject>(); }
	if (id_list.count() != type_list.count()){ return QList<QJsonObject>(); }

	QList<QJsonObject> result_list = QList<QJsonObject>();
	for (int i = 0; i < id_list.count(); i++){
		QJsonObject object = QJsonObject();
		object.insert("id", id_list.at(i));
		object.insert("name", name_list.at(i));
		object.insert("type", type_list.at(i));
		result_list.append(object);
	}
	return result_list;
}