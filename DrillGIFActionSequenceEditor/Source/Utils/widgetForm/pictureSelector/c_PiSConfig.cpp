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

	this->m_zeroFill = false;
	this->m_zeroFillCount = 3;
	this->m_zeroFillChar = '0';
	this->m_isMaskEnabled = true;

	this->m_height = 140;				//�߶ȣ��ؼ����ã�
	this->m_isMultiSelect = false;		//��ѡ����
}
C_PiSConfig::~C_PiSConfig(){
}


/*-------------------------------------------------
		��ȡͼƬ�߶�
*/
int C_PiSConfig::getItemHeight(){
	return this->m_height - 28;		//���̶�28���صľ��룩
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
	obj.insert("m_zeroFill", this->m_zeroFill);
	obj.insert("m_zeroFillCount", this->m_zeroFillCount);
	obj.insert("m_zeroFillChar", QString(this->m_zeroFillChar));
	obj.insert("m_isMaskEnabled", this->m_isMaskEnabled);
	//������ �߶� �� ��ѡ���أ�
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_PiSConfig::setJsonObject(QJsonObject obj){

	if (obj.value("m_zeroFill").isUndefined() == false){ this->m_zeroFill = obj.value("m_zeroFill").toBool(); }
	if (obj.value("m_zeroFillCount").isUndefined() == false){ this->m_zeroFillCount = obj.value("m_zeroFillCount").toInt(); }
	if (obj.value("m_zeroFillChar").isUndefined() == false){ this->m_zeroFillChar = obj.value("m_zeroFillChar").toString().at(0); }
	if (obj.value("m_isMaskEnabled").isUndefined() == false){ this->m_isMaskEnabled = obj.value("m_isMaskEnabled").toBool(); }
	//������ �߶� �� ��ѡ���أ�
}