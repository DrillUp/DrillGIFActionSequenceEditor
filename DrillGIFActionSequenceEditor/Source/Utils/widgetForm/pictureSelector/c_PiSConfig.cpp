#include "stdafx.h"
#include "c_PiSConfig.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ���� ������.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		ͼƬ���� �������ࡣ
					
		�ӹ��ܣ�
					->�ն���
					
		˵����		height�߶��ǿ��Ե����ģ�
					��������ÿ���޸�config������Ҫ�ؽ���ͼ��bitmap��ϵͳ�ٶȻ������
					
-----==========================================================-----
*/
C_PiSConfig::C_PiSConfig(){

	this->zeroFill = false;
	this->zeroFillCount = 3;
	this->zeroFillChar = '0';

	this->height = 140;				//�߶ȣ��ؼ����ã�
	this->isMultiSelect = false;	//��ѡ����
}
C_PiSConfig::~C_PiSConfig(){
}


/*-------------------------------------------------
		��ȡͼƬ�߶�
*/
int C_PiSConfig::getItemHeight(){
	return this->height - 28;		//���̶�28���صľ��룩
}
/*-------------------------------------------------
		��ȡ�����ַ�����\n �ſ�ѡ�е� ��ɫ���飩
*/
QString C_PiSConfig::getLineExpand(){
	QString result = "��";
	int count = qFloor(this->getItemHeight() / 16);
	for (int i = 0; i < count; i++){
		result += "\n��";
	}
	return result;
}
/*-------------------------------------------------
		ʵ���� -> QJsonObject
*/
QJsonObject C_PiSConfig::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("zeroFill", this->zeroFill);
	obj.insert("zeroFillCount", this->zeroFillCount);
	obj.insert("zeroFillChar", QString(this->zeroFillChar));
	//������ �߶� �� ��ѡ���أ�
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_PiSConfig::setJsonObject(QJsonObject obj){

	if (obj.value("zeroFill").isUndefined() == false){ this->zeroFill = obj.value("zeroFill").toBool(); }
	if (obj.value("zeroFillCount").isUndefined() == false){ this->zeroFillCount = obj.value("zeroFillCount").toInt(); }
	if (obj.value("zeroFillChar").isUndefined() == false){ this->zeroFillChar = obj.value("zeroFillChar").toString().at(0); }
	//������ �߶� �� ��ѡ���أ�
}