#include "stdafx.h"
#include "C_UniqueTempFile.h"
#include "s_TempFileManager.h"

/*
-----==========================================================-----
		�ࣺ		Ψһ�ļ� ʵ����.h
		�汾��		v1.17
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		�����ļ�ΪΨһ�ļ���ʵ���ࡣ
					
		˵����		1.��temp�ļ�����ȫ��ר�á�
					2.ֻ��һ���ļ�ָ�룬���Ҫ�洢����Ҫ���ʵ���࣬ͬʱ��Ҫ��ԭ�ļ�ת�ơ�
					3.ֻ��temp�ļ���һ��Ŀ¼��ʹ�ã����ܶ༶�ļ��С�

		�ṹ��		��OriginalFile ԭ�ļ�����ID����
								�̶� + Ψһ + �����޸� ��ԭ�ļ�
					��CommonFile ��ǰ�ļ�����name����
								������ʱ�����ǡ��޸ģ����߻ָ�Ϊԭ�ļ���

		ע�����	1.C_UniqueFile��·����temp�ļ��е�·����
					  �����ļ�����temp�ļ����еġ�
					2.����㱣���ļ���������ʹ�ø��ࡣ
					3.���������Ψһ�ļ�id���㽨������Ҫ������ļ�������
					4.���� ��ϵ�����ݿ��ԭ�� ��������ڷǳ���Ҫ�������û������ֶ��޸��ļ��������ֶ��޸��ļ�����ʱ���ã�
					5.������ļ����������ġ�����ô��ǿ��ʹ��id�ļ���

		ʹ�÷�����	
				>��ʼ������ǰ�ļ���
					C_UniqueTempFile uf = C_UniqueTempFile();
					uf.initFormRealFile(S_TempFileManager::getInstance()->getTempFileUrl() + "/temp.xds_ptn");
					uf.setName(file_default_name);
					if ( !uf.exists() ){
						QMessageBox::warning(this, "����", "û���ҵ�.xds_ptn����ļ���");
					}
				>��ȡ��ǰ�ļ�·��
					uf.getCommonFileFullPath();				//"F:/../temp/workspace/filename.facet"����·��
					uf.getCommonFileName();					//"filename.facet" ֻ�ļ���
				>��ȡ���޸�����
					QString name = uf.getName();			//����
					QString name = uf.getCommonFileName();	//����+��׺������ʵ�ļ�����
				>ʹ��ԭ�ļ�
					this->m_realFile.recoverCommonFile();	//�Ȼ�ԭ ԭ�ļ�
					this->m_realFile.getCommonFile();		//��ִ�в���

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
		��ʼ�� - ���Ƶ�ǰ�ļ��ã��Զ�����temp�ļ����У�
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
		��ʼ�� - �������ļ���
*/
void C_UniqueTempFile::initFormData(QString id, QString filename, QString suffix){

	// > ����ԭ�ļ�
	if (this->id != id){
		this->deleteOriginalFile();
		this->deleteCommonFile();
	}

	// > �����ļ�
	this->id = id;
	this->name = filename;
	this->suffix = suffix;
	this->checkAndBuildOriginalFile();
}


/*-------------------------------------------------
		���� - ��ȡΨһID
*/
QString C_UniqueTempFile::getID(){
	return this->id;
}
/*-------------------------------------------------
		���� - ��ȡ��׺
*/
QString C_UniqueTempFile::getSuffix(){
	return this->suffix;
}
/*-------------------------------------------------
		���� - ��ȡ�ַ���
*/
QString C_UniqueTempFile::getName(){
	return this->name;
}
/*-------------------------------------------------
		���� - �޸��ַ���
*/
void C_UniqueTempFile::setName(QString filename){
	this->name = filename;
	this->checkAndBuildOriginalFile();
}
/*-------------------------------------------------
		���� - �޸��ַ���
*/
void C_UniqueTempFile::setNameAndSuffix(QString name, QString suffix){
	this->name = name;
	this->suffix = suffix;
	this->checkAndBuildOriginalFile();
}
/*-------------------------------------------------
		���� - �����Ƿ�ȱ��
*/
bool C_UniqueTempFile::isAnyDataEmpty(){
	if (this->id == ""){ return true; }
	if (this->suffix == ""){ return true; }
	if (this->name == ""){ return true; }
	return false;
}


/*-------------------------------------------------
		ԭ�ļ� - ��ȡԭ�ļ�
*/
QFileInfo C_UniqueTempFile::getOriginalFile(){
	return QFileInfo(this->getOriginalFileParentPath() + this->getOriginalFileName());
}
/*-------------------------------------------------
		ԭ�ļ� - ��·��
*/
QString C_UniqueTempFile::getOriginalFileParentPath(){
	return S_TempFileManager::getInstance()->getTempFileUrl() + "/" + "unique" + "/";
}
/*-------------------------------------------------
		ԭ�ļ� - ��ȡԭ�ļ���������׺��
*/
QString C_UniqueTempFile::getOriginalFileName(){
	return this->getID() + "." + this->getSuffix();
}
/*-------------------------------------------------
		ԭ�ļ� - ��鲢����ԭ�ļ�
*/
void C_UniqueTempFile::checkAndBuildOriginalFile(){

	// > �����ļ���·��
	QDir dir(this->getOriginalFileParentPath());
	dir.mkdir(this->getOriginalFileParentPath());

	// > ���ԭ�ļ���û��id�ļ�ʱ��ʹ�õ�ǰ�ļ������ɣ�
	if (this->exists() && this->getOriginalFile().exists() == false){
		S_TempFileManager::getInstance()->copyTempToTemp_FileWithNewName(this->getCommonFileName(), "unique/" + this->getOriginalFileName());
	}
}
/*-------------------------------------------------
		ԭ�ļ� - ɾ��ԭ�ļ�
*/
void C_UniqueTempFile::deleteOriginalFile(){
	if (this->exists()) {
		QFile::remove(this->getOriginalFile().absoluteFilePath());
	};
}



/*-------------------------------------------------
		��ǰ�ļ� - ��ȡ��ǰ�ļ�
*/
QFileInfo C_UniqueTempFile::getCommonFile(){
	return QFileInfo(S_TempFileManager::getInstance()->getTempFileUrl() + "/" + this->getName() + "." + this->getSuffix());
}
/*-------------------------------------------------
		��ǰ�ļ� - ��ȡ�ļ���������׺��
*/
QString C_UniqueTempFile::getCommonFileName(){
	if (this->isAnyDataEmpty()){ return ""; }
	return this->getName() + "." + this->getSuffix();
}

/*-------------------------------------------------
		��ǰ�ļ� - ��ȡ��ǰ�ļ�����·��
*/
QString C_UniqueTempFile::getCommonFileFullPath(){
	if (this->isNull()){ return ""; }
	this->checkAndBuildCommonFile();
	return S_TempFileManager::getInstance()->getTempFileUrl() + "/" + this->getCommonFileName();
}
/*-------------------------------------------------
		��ǰ�ļ� - �ָ�Ϊԭ�ļ�
*/
void C_UniqueTempFile::recoverCommonFile() {
	S_TempFileManager::getInstance()->copyTempToTemp_FileWithNewName("unique/" + this->getOriginalFileName(), this->getCommonFileName());
}
/*-------------------------------------------------
		��ǰ�ļ� - ɾ����ǰ�ļ�
*/
void C_UniqueTempFile::deleteCommonFile() {
	if (this->exists()) {
		QFile::remove(this->getCommonFileFullPath());
	};
}
/*-------------------------------------------------
		��ǰ�ļ� - ��鲢������ǰ�ļ�
*/
void C_UniqueTempFile::checkAndBuildCommonFile() {

	// > ���ԭ�ļ���û��name�ļ�ʱ��ʹ�õ�ǰ�ļ������ɣ�
	if (this->exists() && this->getCommonFile().exists() == false){
		S_TempFileManager::getInstance()->copyTempToTemp_FileWithNewName("unique/" + this->getOriginalFileName(), this->getCommonFileName());
	}
}



/*-------------------------------------------------
		�ļ� - ��ǰ�ļ��Ƿ����
*/
bool C_UniqueTempFile::exists(){
	if (this->isAnyDataEmpty()){ return false; }
	QString original_name = this->id + "." + this->suffix;			//id�����ļ���
	QString common_name = this->name + "." + this->suffix;			//ԭ�ļ���
	if (QFileInfo(this->getOriginalFileParentPath()+original_name).exists()){ return true; }
	if (S_TempFileManager::getInstance()->hasTempFile(common_name)){ return true; }
	return false;
}
/*-------------------------------------------------
		�ļ� - �Ƿ�Ϊ��
*/
bool C_UniqueTempFile::isNull(){
	if (this->isAnyDataEmpty()){ return true; }		//���ݿ�
	if (!this->exists()){ return true; }			//��ǰ�ļ���
	return false;			
}
/*-------------------------------------------------
		�ļ� - �����Ϣ������ǰ�ļ�
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
		���������
*/
const bool C_UniqueTempFile::operator== (const C_UniqueTempFile& a)const {
	return this->id == a.id;
}
/*-------------------------------------------------
		ʵ���� -> QJsonObject
*/
QJsonObject C_UniqueTempFile::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("id", this->id);
	obj.insert("name", this->name);
	obj.insert("suffix", this->suffix);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_UniqueTempFile::setJsonObject(QJsonObject obj){

	this->id = obj.value("id").toString();
	this->name = obj.value("name").toString();
	this->suffix = obj.value("suffix").toString();
	this->checkAndBuildOriginalFile();
}
