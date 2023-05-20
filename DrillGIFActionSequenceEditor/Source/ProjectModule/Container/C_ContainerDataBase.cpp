#include "stdafx.h"
#include "C_ContainerDataBase.h"

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

C_ContainerDataBase::C_ContainerDataBase(){
	this->id = "";				//ID
	this->__CLASS_NAME__ = "";	//子类类名
}
C_ContainerDataBase::~C_ContainerDataBase(){
}
/*-------------------------------------------------
		初始化
*/
void C_ContainerDataBase::init(){
	this->generateRandomId();							//随机生成id
	this->__CLASS_NAME__ = this->get__CLASS_NAME__();	//子类类名
}

/*-------------------------------------------------
		访问器 - 设置ID
*/
void C_ContainerDataBase::setId(QString id){
	this->id = id;
}
/*-------------------------------------------------
		访问器 - 生成随机ID
*/
void C_ContainerDataBase::generateRandomId(){
	this->id = QUuid::createUuid().toString();
	this->id = this->id.replace("{", "");
	this->id = this->id.replace("}", "");
	this->id = this->id.replace("-", "");
}
/*-------------------------------------------------
		访问器 - 获取ID
*/
QString C_ContainerDataBase::getId(){
	return this->id;
}

/*-------------------------------------------------
		访问器 - 获取所有继承的类名
*/
QStringList C_ContainerDataBase::get__CLASS_INHERITED_LIST__(){
	return QStringList() << "C_ContainerDataBase";
}
/*-------------------------------------------------
		访问器 - 是否继承了某类
*/
bool C_ContainerDataBase::get__CLASS_IS_INSTANCE_OF__(QString class_name){
	return this->get__CLASS_INHERITED_LIST__().contains(class_name);
}


/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_ContainerDataBase::getJsonObject(){
	QJsonObject obj = this->getJsonObject_childData();

	// > 生成值 - ID（统一小写）
	obj.insert("id", this->id);
	
	// > 生成值 - 子类类名
	obj.insert("__CLASS_NAME__", this->get__CLASS_NAME__());
	
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_ContainerDataBase::setJsonObject(QJsonObject obj){
	this->setJsonObject_childData(obj);

	// > 获取值 - ID
	if (obj.value("iD").isUndefined() == false){ this->id = obj.value("iD").toString(); }
	if (obj.value("Id").isUndefined() == false){ this->id = obj.value("Id").toString(); }
	if (obj.value("ID").isUndefined() == false){ this->id = obj.value("ID").toString(); }
	if (obj.value("id").isUndefined() == false){ this->id = obj.value("id").toString(); }
	
	// > 获取值 - 子类类名 （__CLASS_NAME__只用于生成object时校验用，不取object的值）
	this->__CLASS_NAME__ = this->get__CLASS_NAME__();

}
/*-------------------------------------------------
		运算符重载
*/
const bool C_ContainerDataBase::operator== (const C_ContainerDataBase& a)const {
	return this->id == a.id;
}
/*-------------------------------------------------
		空判断
*/
bool C_ContainerDataBase::isNull(){
	return this->id == "";
}