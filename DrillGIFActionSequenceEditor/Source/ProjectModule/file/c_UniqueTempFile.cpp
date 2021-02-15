#include "stdafx.h"
#include "C_UniqueTempFile.h"
#include "s_TempFileManager.h"

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

		注意事项：	1.C_UniqueFile的路径是temp文件夹的路径。
					  所有文件都是temp文件夹中的。
					2.如果你保存文件名，尽量使用该类。
					3.该类会生成唯一文件id，你建立后，需要给你的文件改名。
					4.根据 关系型数据库的原则 ，该类存在非常必要，并且用户可能手动修改文件名。（手动修改文件名暂时禁用）
					5.【如果文件名存在中文】，那么将强制使用id文件。

		使用方法：	
				>初始化（当前文件）
					C_UniqueTempFile uf = C_UniqueTempFile();
					uf.initFormRealFile(S_TempFileManager::getInstance()->getTempFileUrl() + "/temp.xds_ptn");
					uf.setName(file_default_name);
					if ( !uf.exists() ){
						QMessageBox::warning(this, "错误", "没有找到.xds_ptn结果文件。");
					}
				>获取当前文件路径
					uf.getCommonFileFullPath();				//"F:/../temp/workspace/filename.facet"完整路径
					uf.getCommonFileName();					//"filename.facet" 只文件名
				>获取可修改名字
					QString name = uf.getName();			//名字
					QString name = uf.getCommonFileName();	//名字+后缀（非真实文件名）
				>使用原文件
					this->m_realFile.recoverCommonFile();	//先还原 原文件
					this->m_realFile.getCommonFile();		//再执行操作

-----==========================================================-----
*/
C_UniqueTempFile::C_UniqueTempFile(){
	this->id = QUuid::createUuid().toString();
	this->id = this->id.replace("{", "");
	this->id = this->id.replace("}", "");
	this->id = this->id.replace("-", "");
	this->id = "unique_" + this->id;
	this->name = "";
	this->suffix = "";
}

C_UniqueTempFile::~C_UniqueTempFile(){
}

/*-------------------------------------------------
		初始化 - 复制当前文件用（自动存入temp文件夹中）
*/
void C_UniqueTempFile::initFormRealFile(QString fileurl){
	S_TempFileManager::getInstance()->copyResourceToTemp_File(fileurl);

	QFileInfo temp_info(fileurl);
	if (temp_info.exists()){
		this->setNameAndSuffix(temp_info.completeBaseName(), temp_info.suffix());
	}
	this->checkAndBuildOriginalFile();
}
/*-------------------------------------------------
		初始化 - 生成新文件用
*/
void C_UniqueTempFile::initFormData(QString id, QString filename, QString suffix){

	// > 销毁原文件
	if (this->id != id){
		this->deleteOriginalFile();
		this->deleteCommonFile();
	}

	// > 生成文件
	this->id = id;
	this->name = filename;
	this->suffix = suffix;
	this->checkAndBuildOriginalFile();
}


/*-------------------------------------------------
		属性 - 获取唯一ID
*/
QString C_UniqueTempFile::getID(){
	return this->id;
}
/*-------------------------------------------------
		属性 - 获取后缀
*/
QString C_UniqueTempFile::getSuffix(){
	return this->suffix;
}
/*-------------------------------------------------
		属性 - 获取字符串
*/
QString C_UniqueTempFile::getName(){
	return this->name;
}
/*-------------------------------------------------
		属性 - 修改字符串
*/
void C_UniqueTempFile::setName(QString filename){
	this->name = filename;
	this->checkAndBuildOriginalFile();
}
/*-------------------------------------------------
		属性 - 修改字符串
*/
void C_UniqueTempFile::setNameAndSuffix(QString name, QString suffix){
	this->name = name;
	this->suffix = suffix;
	this->checkAndBuildOriginalFile();
}
/*-------------------------------------------------
		属性 - 属性是否缺项
*/
bool C_UniqueTempFile::isAnyDataEmpty(){
	if (this->id == ""){ return true; }
	if (this->suffix == ""){ return true; }
	if (this->name == ""){ return true; }
	return false;
}


/*-------------------------------------------------
		原文件 - 获取原文件
*/
QFileInfo C_UniqueTempFile::getOriginalFile(){
	return QFileInfo(this->getOriginalFileParentPath() + this->getOriginalFileName());
}
/*-------------------------------------------------
		原文件 - 父路径
*/
QString C_UniqueTempFile::getOriginalFileParentPath(){
	return S_TempFileManager::getInstance()->getTempFileUrl() + "/" + "unique" + "/";
}
/*-------------------------------------------------
		原文件 - 获取原文件名（含后缀）
*/
QString C_UniqueTempFile::getOriginalFileName(){
	return this->getID() + "." + this->getSuffix();
}
/*-------------------------------------------------
		原文件 - 检查并建立原文件
*/
void C_UniqueTempFile::checkAndBuildOriginalFile(){

	// > 建立文件夹路径
	QDir dir(this->getOriginalFileParentPath());
	dir.mkdir(this->getOriginalFileParentPath());

	// > 检查原文件（没有id文件时，使用当前文件的生成）
	if (this->exists() && this->getOriginalFile().exists() == false){
		S_TempFileManager::getInstance()->copyTempToTemp_FileWithNewName(this->getCommonFileName(), "unique/" + this->getOriginalFileName());
	}
}
/*-------------------------------------------------
		原文件 - 删除原文件
*/
void C_UniqueTempFile::deleteOriginalFile(){
	if (this->exists()) {
		QFile::remove(this->getOriginalFile().absoluteFilePath());
	};
}



/*-------------------------------------------------
		当前文件 - 获取当前文件
*/
QFileInfo C_UniqueTempFile::getCommonFile(){
	return QFileInfo(S_TempFileManager::getInstance()->getTempFileUrl() + "/" + this->getName() + "." + this->getSuffix());
}
/*-------------------------------------------------
		当前文件 - 获取文件名（含后缀）
*/
QString C_UniqueTempFile::getCommonFileName(){
	if (this->isAnyDataEmpty()){ return ""; }
	return this->getName() + "." + this->getSuffix();
}

/*-------------------------------------------------
		当前文件 - 获取当前文件完整路径
*/
QString C_UniqueTempFile::getCommonFileFullPath(){
	if (this->isNull()){ return ""; }
	this->checkAndBuildCommonFile();
	return S_TempFileManager::getInstance()->getTempFileUrl() + "/" + this->getCommonFileName();
}
/*-------------------------------------------------
		当前文件 - 恢复为原文件
*/
void C_UniqueTempFile::recoverCommonFile() {
	S_TempFileManager::getInstance()->copyTempToTemp_FileWithNewName("unique/" + this->getOriginalFileName(), this->getCommonFileName());
}
/*-------------------------------------------------
		当前文件 - 删除当前文件
*/
void C_UniqueTempFile::deleteCommonFile() {
	if (this->exists()) {
		QFile::remove(this->getCommonFileFullPath());
	};
}
/*-------------------------------------------------
		当前文件 - 检查并建立当前文件
*/
void C_UniqueTempFile::checkAndBuildCommonFile() {

	// > 检查原文件（没有name文件时，使用当前文件的生成）
	if (this->exists() && this->getCommonFile().exists() == false){
		S_TempFileManager::getInstance()->copyTempToTemp_FileWithNewName("unique/" + this->getOriginalFileName(), this->getCommonFileName());
	}
}



/*-------------------------------------------------
		文件 - 当前文件是否存在
*/
bool C_UniqueTempFile::exists(){
	if (this->isAnyDataEmpty()){ return false; }
	QString original_name = this->id + "." + this->suffix;			//id加密文件名
	QString common_name = this->name + "." + this->suffix;			//原文件名
	if (QFileInfo(this->getOriginalFileParentPath()+original_name).exists()){ return true; }
	if (S_TempFileManager::getInstance()->hasTempFile(common_name)){ return true; }
	return false;
}
/*-------------------------------------------------
		文件 - 是否为空
*/
bool C_UniqueTempFile::isNull(){
	if (this->isAnyDataEmpty()){ return true; }		//数据空
	if (!this->exists()){ return true; }			//当前文件空
	return false;			
}
/*-------------------------------------------------
		文件 - 清空信息包括当前文件
*/
void C_UniqueTempFile::clearAll(){
	this->deleteOriginalFile();
	this->deleteCommonFile();
	this->name = "";
	this->suffix = "";
}
void C_UniqueTempFile::deleteAll(){
	this->clearAll();
}


/*-------------------------------------------------
		运算符重载
*/
const bool C_UniqueTempFile::operator== (const C_UniqueTempFile& a)const {
	return this->id == a.id;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_UniqueTempFile::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("id", this->id);
	obj.insert("name", this->name);
	obj.insert("suffix", this->suffix);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_UniqueTempFile::setJsonObject(QJsonObject obj){

	this->id = obj.value("id").toString();
	this->name = obj.value("name").toString();
	this->suffix = obj.value("suffix").toString();
	this->checkAndBuildOriginalFile();
}
