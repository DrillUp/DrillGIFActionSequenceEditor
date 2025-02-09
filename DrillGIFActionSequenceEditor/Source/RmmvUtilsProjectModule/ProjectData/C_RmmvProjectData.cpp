#include "stdafx.h"
#include "C_RmmvProjectData.h"

/*
-----==========================================================-----
		类：		Rmmv工程数据 数据类.cpp
		版本：		v1.03
		作者：		drill_up
		所属模块：	工程模块
		功能：		工程基本信息数据。（只有工程路径的数据重要，需要存储）

		说明：		> 你可以直接用静态方法获取到该类数据：
					C_RmmvProjectData data = C_RmmvProjectData::callRmmvSelectDialog()
-----==========================================================-----
*/
C_RmmvProjectData::C_RmmvProjectData(){

	// > 数据
	this->name = "";			//工程名称
	this->version = "";			//工程版本
	this->path = "";			//工程根路径
}
C_RmmvProjectData::~C_RmmvProjectData(){
}


/*-------------------------------------------------
		数据 - 获取工程名称
*/
QString C_RmmvProjectData::getName(){
	return this->name;
}
/*-------------------------------------------------
		数据 - 获取工程版本
*/
QString C_RmmvProjectData::getVersion(){
	return this->version;
}
/*-------------------------------------------------
		数据 - 获取工程根路径
*/
QString C_RmmvProjectData::getRootPath(){
	return this->path;
}
/*-------------------------------------------------
		数据 - 判断工程是否存在
*/
bool C_RmmvProjectData::isProjectExist(){
	if (QFileInfo(this->path).exists() == false){ return false; }
	if (QFileInfo(this->path + "/index.html").exists() == false){ return false; }
	return true;
}


/*-------------------------------------------------
		读取器 - 解析工程文件
*/
void C_RmmvProjectData::init_fromFile(QString full_filePath){
	QString data_path = QFileInfo(full_filePath).absolutePath();		//获取根目录

	// > 读取文件 - Game.rpgproject
	QFile project_file(full_filePath);
	if (!project_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "错误", "未找到Game.rpgproject文件。", QMessageBox::Yes);
		return;
	}
	QString data_version = project_file.readAll();
	project_file.close();

	// > 读取文件 - index.html
	QFile index_file(data_path + "/index.html");
	if (!index_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "错误", "未找到index.html文件。", QMessageBox::Yes);
		return;
	}
	QString index_context = index_file.readAll();
	int i_a = index_context.indexOf("<title>") + 7;
	int i_b = index_context.indexOf("</title>", i_a);
	QString data_name = index_context.mid(i_a, i_b - i_a);
	index_file.close();

	// > 赋值
	this->name = data_name;
	this->version = data_version;
	this->path = data_path;
}
/*-------------------------------------------------
		读取器 - 启用文件选择框
*/
C_RmmvProjectData C_RmmvProjectData::callRmmvSelectDialog(){
	QFileDialog fd;
	fd.setWindowTitle("打开rmmv工程");
	fd.setAcceptMode(QFileDialog::AcceptOpen);		//对话框类型（打开/保存）
	fd.setDirectory(".");							//默认目录
	fd.setNameFilters(QStringList()					//文件格式
		<< "rmmv工程文件(*.rpgproject)"
	);
	fd.setViewMode(QFileDialog::Detail);

	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()) {
			//（没有选择文件进入的情况）
			return C_RmmvProjectData();
		}
		QString file_path = fd.selectedFiles().at(0);

		// > 读取成功时
		C_RmmvProjectData data = C_RmmvProjectData();
		data.init_fromFile(file_path);
		return data;
	
	}else {
		//（点击关闭或者取消操作的情况）
		return C_RmmvProjectData();
	}
}



//获取文件 - 插件
QFileInfo C_RmmvProjectData::getRmmvFile_Plugin(QString plugin_name){
	if (this->isNull()){ return QFileInfo(); }
	QString name = plugin_name.replace(".js", "");
	return QFileInfo(this->getRootPath() + "/js/plugins/" + plugin_name + ".js");
}
//获取文件 - 插件存储数据
QFileInfo C_RmmvProjectData::getRmmvFile_PluginsData(){
	if (this->isNull()){ return QFileInfo(); }
	return QFileInfo(this->getRootPath() + "/js/plugins.js");
}

//获取文件 - 地图
QFileInfo C_RmmvProjectData::getRmmvFile_Map(int i){
	if (this->isNull()){ return QFileInfo(); }
	QString name = QString("Map%1").arg(i, 3, 10, QLatin1Char('0'));
	return QFileInfo(this->getRootPath() + "/data/" + name + ".json");
}
//获取文件 - 地图信息
QFileInfo C_RmmvProjectData::getRmmvFile_MapInfos(){
	if (this->isNull()){ return QFileInfo(); }
	return QFileInfo(this->getRootPath() + "/data/MapInfos.json");
}
//获取文件 - 角色
QFileInfo C_RmmvProjectData::getRmmvFile_Actors(){
	if (this->isNull()){ return QFileInfo(); }
	return QFileInfo(this->getRootPath() + "/data/Actors.json");
}
//获取文件 - 职业
QFileInfo C_RmmvProjectData::getRmmvFile_Classes(){
	if (this->isNull()){ return QFileInfo(); }
	return QFileInfo(this->getRootPath() + "/data/Classes.json");
}
//获取文件 - 物品
QFileInfo C_RmmvProjectData::getRmmvFile_Items(){
	if (this->isNull()){ return QFileInfo(); }
	return QFileInfo(this->getRootPath() + "/data/Items.json");
}
//获取文件 - 技能
QFileInfo C_RmmvProjectData::getRmmvFile_Skills(){
	if (this->isNull()){ return QFileInfo(); }
	return QFileInfo(this->getRootPath() + "/data/Skills.json");
}
//获取文件 - 武器
QFileInfo C_RmmvProjectData::getRmmvFile_Weapons(){
	if (this->isNull()){ return QFileInfo(); }
	return QFileInfo(this->getRootPath() + "/data/Weapons.json");
}
//获取文件 - 护甲
QFileInfo C_RmmvProjectData::getRmmvFile_Armors(){
	if (this->isNull()){ return QFileInfo(); }
	return QFileInfo(this->getRootPath() + "/data/Armors.json");
}
//获取文件 - 状态
QFileInfo C_RmmvProjectData::getRmmvFile_States(){
	if (this->isNull()){ return QFileInfo(); }
	return QFileInfo(this->getRootPath() + "/data/States.json");
}
//获取文件 - 敌人
QFileInfo C_RmmvProjectData::getRmmvFile_Enemies(){
	if (this->isNull()){ return QFileInfo(); }
	return QFileInfo(this->getRootPath() + "/data/Enemies.json");
}
//获取文件 - 敌群
QFileInfo C_RmmvProjectData::getRmmvFile_Troops(){
	if (this->isNull()){ return QFileInfo(); }
	return QFileInfo(this->getRootPath() + "/data/Troops.json");
}
//获取文件 - 公共事件
QFileInfo C_RmmvProjectData::getRmmvFile_CommonEvents(){
	if (this->isNull()){ return QFileInfo(); }
	return QFileInfo(this->getRootPath() + "/data/CommonEvents.json");
}
//获取文件 - 动画
QFileInfo C_RmmvProjectData::getRmmvFile_Animations(){
	if (this->isNull()){ return QFileInfo(); }
	return QFileInfo(this->getRootPath() + "/data/Animations.json");
}
//获取文件 - 图块
QFileInfo C_RmmvProjectData::getRmmvFile_Tilesets(){
	if (this->isNull()){ return QFileInfo(); }
	return QFileInfo(this->getRootPath() + "/data/Tilesets.json");
}
//获取文件 - 系统
QFileInfo C_RmmvProjectData::getRmmvFile_System(){
	if (this->isNull()){ return QFileInfo(); }
	return QFileInfo(this->getRootPath() + "/data/System.json");
}


//获取文件夹 - 声音BGM（根路径/audio/bgm/）
QDir C_RmmvProjectData::getRmmvDir_Audio_BGM(){
	if (this->isNull()){ return QDir(); }
	return QDir(this->getRootPath() + "/audio/bgm");
}
//获取文件夹 - 声音BGS（根路径/audio/bgs/）
QDir C_RmmvProjectData::getRmmvDir_Audio_BGS(){
	if (this->isNull()){ return QDir(); }
	return QDir(this->getRootPath() + "/audio/bgs");
}
//获取文件夹 - 声音ME（根路径/audio/me/）
QDir C_RmmvProjectData::getRmmvDir_Audio_ME(){
	if (this->isNull()){ return QDir(); }
	return QDir(this->getRootPath() + "/audio/me");
}
//获取文件夹 - 声音SE（根路径/audio/se/）
QDir C_RmmvProjectData::getRmmvDir_Audio_SE(){
	if (this->isNull()){ return QDir(); }
	return QDir(this->getRootPath() + "/audio/se");
}
//获取文件夹 - 数据（根路径/data/）
QDir C_RmmvProjectData::getRmmvDir_Data(){
	if (this->isNull()){ return QDir(); }
	return QDir(this->getRootPath() + "/data");
}
//获取文件夹 - 字体（根路径/fonts/）
//【游戏中使用，编辑器用不到】
//获取文件夹 - 插件（根路径/icon/）
//【游戏中使用，编辑器用不到】
//获取文件夹 - 插件（根路径/img/）
QDir C_RmmvProjectData::getRmmvDir_Img(){
	if (this->isNull()){ return QDir(); }
	return QDir(this->getRootPath() + "/img");
}
//获取文件夹 - 插件（根路径/js/）
QDir C_RmmvProjectData::getRmmvDir_Js(){
	if (this->isNull()){ return QDir(); }
	return QDir(this->getRootPath() + "/js");
}
//获取文件夹 - 插件（根路径/js/plugins/）
QDir C_RmmvProjectData::getRmmvDir_Plugins(){
	if (this->isNull()){ return QDir(); }
	return QDir(this->getRootPath() + "/js/plugins");
}
//获取文件夹 - 插件（根路径/movies/）
QDir C_RmmvProjectData::getRmmvDir_Movies(){
	if (this->isNull()){ return QDir(); }
	return QDir(this->getRootPath() + "/movies");
}
//获取文件夹 - 存档（根路径/save/）
//【游戏中使用，编辑器用不到】



/*-------------------------------------------------
		空判断
*/
bool C_RmmvProjectData::isNull(){
	return this->name == "" || this->path == "";
}
/*-------------------------------------------------
		运算符重载
*/
const bool C_RmmvProjectData::operator== (const C_RmmvProjectData& a)const {
	return this->path == a.path;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_RmmvProjectData::getJsonObject(){
	QJsonObject obj = QJsonObject();

	// > 数据
	obj.insert("name", this->name);
	obj.insert("version", this->version);
	obj.insert("path", this->path);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_RmmvProjectData::setJsonObject(QJsonObject obj){

	// > 数据
	this->name = obj.value("name").toString();
	this->version = obj.value("version").toString();
	this->path = obj.value("path").toString();
}
