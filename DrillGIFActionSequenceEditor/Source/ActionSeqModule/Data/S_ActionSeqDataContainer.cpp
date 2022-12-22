#include "stdafx.h"
#include "S_ActionSeqDataContainer.h"

#include "DrillGIFActionSequenceEditor.h"
#include "Source/ProjectModule/File/S_TempFileManager.h"
#include "Source/PluginModule/StorageData/S_PluginDataContainer.h"
#include "Source/PluginModule/LengthEditor/S_LEAnnotationReader.h"
#include "Source/RmmvInteractiveModule/Base/S_RmmvDataContainer.h"

/*
-----==========================================================-----
		类：		动画序列 数据容器.cpp
		所属模块：	动画序列模块
		功能：		动画序列的数据容器。
					会主动使用插件容器中的数据。
			
		使用方法：
				>初始化：
					S_ActionSeqDataContainer::getInstance();
-----==========================================================-----
*/
S_ActionSeqDataContainer::S_ActionSeqDataContainer(){
	this->clearAllData();

	//-----------------------------------
	//----事件绑定
	connect(S_PluginDataContainer::getInstance(), &S_PluginDataContainer::pluginDataReloaded, this, &S_ActionSeqDataContainer::resetPluginData);

}
S_ActionSeqDataContainer::~S_ActionSeqDataContainer() {
}

/* --------------------------------------------------------------
----------PluginDataManager 单例
*/
S_ActionSeqDataContainer* S_ActionSeqDataContainer::cur_manager = NULL;
S_ActionSeqDataContainer* S_ActionSeqDataContainer::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_ActionSeqDataContainer();
	}
	return cur_manager;
}


/*-------------------------------------------------
		插件数据 - 重设
*/
void S_ActionSeqDataContainer::resetPluginData() {
	this->clearAllData();

	// > 插件数据
	this->data_ActionSeqPlugin = S_PluginDataContainer::getInstance()->getPluginData("Drill_CoreOfActionSequence");

	// > GIF动画序列核心
	if (this->data_ActionSeqPlugin != nullptr){	

		// > 插件的长度数据
		this->setActionSeqLength(this->getPluginData_ActionSeqLength());
		// > 插件配置的数据
		this->setActionSeqData_Object( this->data_ActionSeqPlugin->parameters );
	}

	// > 工程中没有插件数据
	if (this->data_ActionSeqPlugin == nullptr){
		if (QMessageBox::information(nullptr, "提示", "该工程中，没有找到\"GIF动画序列核心\"的插件配置，是否添加？", "添加", "取消", 0, 1) == 0){

			DrillGIFActionSequenceEditor::getInstance()->rebuildActionSeqData();
			return;
		}
	}

	emit dataAllReloaded();
}
/*-------------------------------------------------
		插件数据 - 获取（插件数据）
*/
C_PluginData* S_ActionSeqDataContainer::getPluginData_ActionSeq() {
	return this->data_ActionSeqPlugin;
}
/*-------------------------------------------------
		插件数据 - 获取（插件文件）
*/
QFileInfo S_ActionSeqDataContainer::getPluginFile_ActionSeq() {
	return S_RmmvDataContainer::getInstance()->getRmmvFile_Plugin("Drill_CoreOfActionSequence");
}
/*-------------------------------------------------
		插件数据 - 获取长度
*/
C_COAS_Length S_ActionSeqDataContainer::getPluginData_ActionSeqLength() {
	C_COAS_Length result = C_COAS_Length();
	QFileInfo plugin_file = this->getPluginFile_ActionSeq();
	if (plugin_file.exists() == false){ return result; }

	C_LEAnnotation* c_le = S_LEAnnotationReader::getInstance()->readPlugin(plugin_file);
	result.realLen_actionSeq = c_le->getParamByKey("动画序列-%d").getRealLen();
	if (result.realLen_actionSeq == 0){
		result.realLen_actionSeq = c_le->getParamByKey("动作序列-%d").getRealLen();
	}
	result.realLen_action = c_le->getParamByKey("动作元-%d").getRealLen();
	result.realLen_state = c_le->getParamByKey("状态元-%d").getRealLen();
	return result;
}



/*-------------------------------------------------
		数据 - 设置
*/
void S_ActionSeqDataContainer::setActionSeqData(QList<C_COAS_DataPtr> data_list){
	this->data_ActionSeqData = data_list;
}
void S_ActionSeqDataContainer::setActionSeqData_Object(QJsonObject data_obj){

	// > 删除全部数据
	for (int i = 0; i < this->data_ActionSeqData.count(); i++){
		C_COAS_DataPtr data = this->data_ActionSeqData.at(i);
		data.clear();
	}
	this->data_ActionSeqData.clear();

	// > 填充数据
	for (int i = 0; i < this->data_ActionSeqLength.realLen_actionSeq; i++){

		// > 旧名字修改（动作序列 -> 动画序列）
		QJsonValue v = data_obj.value("动作序列-" + QString::number(i + 1));
		if (v.isUndefined() == false){
			data_obj.insert("动画序列-" + QString::number(i + 1), v);
			data_obj.remove("动作序列-" + QString::number(i + 1));
		}

		// > 添加内容
		QJsonValue value = data_obj.value("动画序列-" + QString::number(i + 1));
		if (value.isUndefined() == false){
			QJsonObject obj = TTool::_JSON_parse_To_Obj_(value.toString());

			// > 新建对象
			C_COAS_DataPtr data = C_COAS_DataPtr::create();
			data->setJsonObject_Chinese(obj);

			// > 树数据
			data->m_COAS_id = i + 1;
			//m_COAS_name
			//m_COAS_type

			this->data_ActionSeqData.append(data);

		}else{
			QJsonObject obj = QJsonObject();

			// > 新建对象
			C_COAS_DataPtr data = C_COAS_DataPtr::create();
			data->setJsonObject_Chinese(obj);

			// > 树数据
			data->m_COAS_id = i + 1;
			//m_COAS_name
			//m_COAS_type

			this->data_ActionSeqData.append(data);
		}
	}

}
/*-------------------------------------------------
		数据 - 获取
*/
QList<C_COAS_DataPtr> S_ActionSeqDataContainer::getActionSeqData(){
	return this->data_ActionSeqData;	//（注意，从rmmv中读取的数据 和 编辑器配置的数据 不一样）
}
QJsonObject S_ActionSeqDataContainer::getActionSeqData_Object(){
	this->refreshActionSeqLength();

	QJsonObject result_obj;
	for (int i = 0; i < this->data_ActionSeqData.count(); i++){
		C_COAS_DataPtr data = this->data_ActionSeqData.at(i);
		QJsonObject data_obj = data->getJsonObject_Chinese();

		// > 树数据（合并到动画序列）
		data_obj.remove("COAS_id");
		data_obj.remove("COAS_name");	//（去除id和name，type保留）

		QString data_str = TTool::_JSON_stringify_(data_obj);
		result_obj.insert("动画序列-" + QString::number(i + 1), data_str);
	}
	return result_obj;
}
/*-------------------------------------------------
		数据 - 设置长度
*/
void S_ActionSeqDataContainer::setActionSeqLength(C_COAS_Length data){
	this->data_ActionSeqLength = data;
	this->refreshActionSeqLength();

}
/*-------------------------------------------------
		数据 - 获取长度
*/
C_COAS_Length S_ActionSeqDataContainer::getActionSeqLength(){
	return this->data_ActionSeqLength;	//（注意，插件读取的长度 和 编辑器配置的长度 不一样）
}
/*-------------------------------------------------
		数据 - 重刷长度
*/
void S_ActionSeqDataContainer::refreshActionSeqLength(){
	
	// > 少了，要加上
	if (this->data_ActionSeqLength.realLen_actionSeq > this->data_ActionSeqData.count()){
		for (int i = this->data_ActionSeqData.count(); i < this->data_ActionSeqLength.realLen_actionSeq; i++){
			QJsonObject obj = QJsonObject();

			// > 新建对象
			C_COAS_DataPtr data = C_COAS_DataPtr::create();
			data->setJsonObject_Chinese(obj);

			// > 树数据
			data->m_COAS_id = i + 1;
			//m_COAS_name
			//m_COAS_type

			this->data_ActionSeqData.append(data);
		}
	}

	// > 多了，要去掉
	if (this->data_ActionSeqLength.realLen_actionSeq < this->data_ActionSeqData.count()){
		for (int i = this->data_ActionSeqData.count()-1; i >= this->data_ActionSeqLength.realLen_actionSeq; i--){
			C_COAS_DataPtr data = this->data_ActionSeqData.at(i);
			data.clear();
			this->data_ActionSeqData.removeAt(i);
		}
	}

	// > 刷新动画序列中 状态元、状态节点、动作元 内容
	for (int i = 0; i < this->data_ActionSeqData.count(); i++){
		C_COAS_DataPtr data = this->data_ActionSeqData.at(i);
		data->refreshTankLength(this->data_ActionSeqLength);
	}
}
/*-------------------------------------------------
		数据 - 获取全部关联的文件
*/
QList<QFileInfo> S_ActionSeqDataContainer::getAllRelatedFile(){

	QStringList fileName_list = QStringList();
	for (int i = 0; i < this->data_ActionSeqData.count(); i++){
		C_COAS_DataPtr data = this->data_ActionSeqData.at(i);
		if (data == nullptr){ continue; }
		if (data->isNull()){ continue; }

		// > 动作元资源
		for (int j = 0; j < data->m_act_tank.count(); j++){
			C_COAS_ActionPtr action_ptr = data->m_act_tank.at(j);
			fileName_list.append(action_ptr->gif_src);
		}

		// > 状态元资源
		for (int j = 0; j < data->m_state_tank.count(); j++){
			C_COAS_StatePtr state_ptr = data->m_state_tank.at(j);
			fileName_list.append(state_ptr->gif_src);
		}
	}

	/*
	QStringList fileName_list = QStringList();
	for (int i = 0; i < this->data_ActionSeqLength.realLen_actionSeq; i++){

		// > 一层字符串解封
		QString str_actionSeq = this->data_ActionSeqData.value("动画序列-" + QString::number(i + 1)).toString();
		if (str_actionSeq == ""){ continue; }
		QJsonObject obj_actionSeq = TTool::_JSON_parse_To_Obj_(str_actionSeq);

		// > 动作元资源
		for (int j = 0; j < this->data_ActionSeqLength.realLen_action; j++){

			// > 二层字符串解封
			QString str_action = obj_actionSeq.value("动作元-" + QString::number(j + 1)).toString();
			if (str_action == ""){ continue; }
			QJsonObject obj_action = TTool::_JSON_parse_To_Obj_(str_action);

			// > 三层字符串解封
			QString str_src = obj_action.value("资源-动作元").toString();
			QStringList src_list = TTool::_JSON_parse_To_QListQString_(str_src);

			fileName_list.append(src_list);
		}

		// > 状态元资源
		for (int j = 0; j < this->data_ActionSeqLength.realLen_state; j++){

			// > 二层字符串解封
			QString str_action = obj_actionSeq.value("状态元-" + QString::number(j + 1)).toString();
			if (str_action == ""){ continue; }
			QJsonObject obj_action = TTool::_JSON_parse_To_Obj_(str_action);

			// > 三层字符串解封
			QString str_src = obj_action.value("资源-状态元").toString();
			QStringList src_list = TTool::_JSON_parse_To_QListQString_(str_src);

			fileName_list.append(src_list);
		}
	}
	*/

	// > 去重
	fileName_list = fileName_list.toSet().toList();

	// > 列出资源
	QList<QFileInfo> result_list = QList<QFileInfo>();
	for (int i = 0; i < fileName_list.count(); i++){
		QString file_path = this->getActionSeqDir() + "/" + fileName_list.at(i) + ".png";
		result_list.append(QFileInfo(file_path));
	}
	return result_list;
}


/*-------------------------------------------------
		常量 - 获取软件保存图片路径
*/
QString S_ActionSeqDataContainer::getActionSeqDir(){
	QString dir_path = S_TempFileManager::getInstance()->getTempFileUrl() +"/Special__actionSeq";
	QDir dir(dir_path);
	if (dir.exists() == false){ dir.mkdir(dir_path); }
	return dir_path;
}


/*-------------------------------------------------
		树配置 - 修改配置
*/
void S_ActionSeqDataContainer::modifyTreeConfig(QJsonObject tree){
	this->data_treeConfig = tree;
}
/*-------------------------------------------------
		树配置 - 获取配置
*/
QJsonObject S_ActionSeqDataContainer::getTreeConfig(){
	return this->data_treeConfig;
}
/*-------------------------------------------------
		树配置 - 获取树数据
*/
QList<QJsonObject> S_ActionSeqDataContainer::getTreeData(){
	//（此数据存于 动画序列数据 中）
	QList<QJsonObject> result_list;
	for (int i = 0; i < this->data_ActionSeqData.count(); i++){
		C_COAS_DataPtr data = this->data_ActionSeqData.at(i);
		QJsonObject obj;
		obj.insert("COAS_id", data->m_COAS_id);
		obj.insert("COAS_name", data->m_COAS_name);
		obj.insert("COAS_type", data->m_COAS_type);
		result_list.append(obj);
	}
	return result_list;
}

/*-----------------------------------
		数据 - 获取存储的名称
*/
QString S_ActionSeqDataContainer::getSaveName() {
	return "S_ActionSeqDataContainer";
}
/*-----------------------------------
		数据 - 清除当前管理器数据
*/
void S_ActionSeqDataContainer::clearAllData() {
	this->data_ActionSeqPlugin = nullptr;
	this->data_treeConfig = QJsonObject();
	this->data_ActionSeqLength = C_COAS_Length();
	this->data_ActionSeqData.clear();
}
/*-----------------------------------
		数据 - 全部激励源数据 -> QJsonObject
*/
QJsonObject S_ActionSeqDataContainer::getAllDataOfJsonObject(){
	QJsonObject obj_all = QJsonObject();

	obj_all.insert("data_treeConfig", this->data_treeConfig);								//树配置数据
	obj_all.insert("data_ActionSeqLength", this->data_ActionSeqLength.getJsonObject());		//动画序列长度
	obj_all.insert("data_ActionSeqData", this->getActionSeqData_Object());					//动画序列数据
	
	return obj_all;
}

/*-----------------------------------
		数据 - QJsonObject -> 全部激励源数据
*/
void S_ActionSeqDataContainer::setAllDataFromJsonObject(QJsonObject obj_all){
	this->clearAllData();

	this->data_treeConfig = obj_all.value("data_treeConfig").toObject();							//树配置数据
	this->data_ActionSeqLength.setJsonObject( obj_all.value("data_ActionSeqLength").toObject() );	//动画序列长度
	this->setActionSeqData_Object( obj_all.value("data_ActionSeqData").toObject() );				//动画序列数据

	emit dataAllReloaded();
}
