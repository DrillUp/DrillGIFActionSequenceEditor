#include "stdafx.h"
#include "Drill_COAS_Data.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		�����������ݡ������ࡿ.cpp
		���ߣ�		drill_up
		����ģ�飺	������������
		���ܣ�		�������к��Ĳ���ĸ����ࡣ
		
		Ŀ�꣺		->�ؼ�
						->��ѡ���
						->���ٱ�

		ʹ�÷�����
				>��ʼ��

-----==========================================================-----
*/
Drill_COAS_Data::Drill_COAS_Data(){
	this->_drill_data = QJsonObject();	//�������
	this->drill_initData();				//��ʼ������
}
Drill_COAS_Data::Drill_COAS_Data(QJsonObject data){
	this->_drill_data = data;			//�������
	this->drill_initData();				//��ʼ������
}
Drill_COAS_Data::~Drill_COAS_Data(){
}

/*-------------------------------------------------
		���� - ���ж�
*/
bool Drill_COAS_Data::isNull(){
	return this->_drill_data.isEmpty();
}
/*-------------------------------------------------
		���� - ��ʼ��
*/
void Drill_COAS_Data::drill_initData(){
	QJsonObject data = this->_drill_data;
	
	// > Ĭ��ֵ
	if (data["state_default_randomSeq"].isUndefined() == true){ data["state_default_randomSeq"] = QJsonArray(); }	//Ĭ��״̬Ԫ����
	if (data["state_tank"].isUndefined() == true){ data["state_tank"] = QJsonArray(); }								//״̬Ԫ ����
	if (data["act_tank"].isUndefined() == true){ data["act_tank"] = QJsonArray(); }									//����Ԫ ����
	
	// > ˽�б�����ʼ��
	this->_drill_time = 0;											//����ʱ��
	this->_drill_arrayCheck = true;									//�������Ԫ��
	this->_drill_bitmapName = "";									//��ǰ��bitmap������
	this->_drill_bitmapPath = "";									//��ǰ��bitmap·��
	this->_drill_bitmapTint = 0;									//��ǰ��bitmapɫ��
	this->_drill_bitmapSmooth = false;								//��ǰ��bitmapģ��
	this->_drill_state_curCom = "";									//״̬Ԫ - ��ǰ״̬
	this->_drill_state_curTime = 0;									//״̬Ԫ - ��ǰʱ��
	this->_drill_state_curSeq = TTool::_QJsonArray_AToQString_(data["state_default_randomSeq"].toArray());		//״̬Ԫ - ��ǰ����
	this->_drill_act_curCom = "";									//����Ԫ - ��ǰ����
	this->_drill_act_curTime = 0;									//����Ԫ - ��ǰʱ��
}
/*-------------------------------------------------
		���� - ֡ˢ�£���Ҫ�����ֶ�ִ�У�
*/
void Drill_COAS_Data::update(){
	if (this->isNull()){ return; }
	this->_drill_time += 1;				//ʱ��+1
	this->drill_COAS_updateState();		//ˢ��״̬Ԫ
	this->drill_COAS_updateAct();		//ˢ�¶���Ԫ
}
/*-------------------------------------------------
		֡ˢ�� - ˢ��״̬Ԫ
*/
void Drill_COAS_Data::drill_COAS_updateState(){
	if (this->drill_COAS_isPlayingAct() == true){ return; }		//��������ʱ��������״̬Ԫ
	if (this->_drill_state_curSeq.count() == 0){ return; }		//״̬Ԫ���� Ϊ��ʱ��������

	// > �����ȡ
	if (this->_drill_state_curCom == ""){
		if (this->_drill_state_curSeq.count() == 1){
			this->_drill_state_curCom = this->_drill_state_curSeq[0];
		}
		int index = rand() % this->_drill_state_curSeq.count();
		this->_drill_state_curCom = this->_drill_state_curSeq[index];
	}

	QJsonObject data = this->_drill_data;
	QJsonObject data_state = this->drill_COAS_getDataState(this->_drill_state_curCom);

	// > û�и�״̬Ԫʱ
	if (data_state["gif_src"].isUndefined() == true){
		this->_drill_state_curCom = "";
		return;
	}

	// > ״̬����
	//if (this->_drill_state_curTime == 0){
	//	QJsonObject se = {};
	//	se["name"] = data_state["se"];
	//	se["pitch"] = 100;
	//	se["volume"] = 100;
	//	AudioManager.playSe(se);
	//}

	// > gif����
	QJsonArray gif_src = data_state["gif_src"].toArray();
	if (gif_src.count() != 0){
		
		int index = 0;
		int inter_time = this->_drill_state_curTime;
		if( data_state["gif_back_run"].toBool() == false){

			// > ���򲥷�
			QJsonArray gif_intervalRealTank = data_state["gif_intervalRealTank"].toArray();
			for (int i = 0; i < gif_intervalRealTank.count(); i++){
				int i_time = gif_intervalRealTank[i].toInt();
				if( inter_time < i_time ){
					index = i;
					break;
				}
				inter_time -= i_time;
			}
		}else{

			// > ����
			QJsonArray gif_intervalRealTank = data_state["gif_intervalRealTank"].toArray();
			for (int i = gif_intervalRealTank.count() - 1; i >= 0; i--){
				int i_time = gif_intervalRealTank[i].toInt();
				if( inter_time < i_time ){
					index = i;
					break;
				}
				inter_time -= i_time;
			}
		}
		this->_drill_bitmapName = gif_src[index].toString();
		this->_drill_bitmapPath = data_state["gif_src_file"].toString();
		this->_drill_bitmapTint = data_state["gif_tint"].toInt();
		this->_drill_bitmapSmooth = data_state["gif_smooth"].toBool();
	}

	// > ʱ��+1
	this->_drill_state_curTime += 1;
	if (this->_drill_state_curTime >= gif_src.count() * data_state["gif_interval"].toInt()){
		this->_drill_state_curCom = "";
		this->_drill_state_curTime = 0;
	}
}
/*-------------------------------------------------
		֡ˢ�� - ˢ�¶���Ԫ
*/
void Drill_COAS_Data::drill_COAS_updateAct(){
	if (this->drill_COAS_isPlayingAct() == false){ return; }		//����δ����ʱ��������

	QJsonObject data = this->_drill_data;
	QJsonObject data_act = this->drill_COAS_getDataAct(this->_drill_act_curCom);

	// > û�иö���Ԫʱ
	if (data_act["gif_src"].isUndefined() == true){
		this->_drill_act_curCom = "";
		return;
	}

	// > ��������
	//if (this->_drill_state_curTime == 0){
	//	QJsonObject se = {};
	//	se["name"] = data_state["se"];
	//	se["pitch"] = 100;
	//	se["volume"] = 100;
	//	AudioManager.playSe(se);
	//}

	// > gif���ţ�һ��ֻ�ܲ���һ����Ϊ��
	QJsonArray gif_src = data_act["gif_src"].toArray();
	if (gif_src.count() != 0){
		
		int index = 0;
		int inter_time = this->_drill_state_curTime;
		if (data_act["gif_back_run"].toBool() == false){

			// > ���򲥷�
			QJsonArray gif_intervalRealTank = data_act["gif_intervalRealTank"].toArray();
			for (int i = 0; i < gif_intervalRealTank.count(); i++){
				int i_time = gif_intervalRealTank[i].toInt();
				if( inter_time < i_time ){
					index = i;
					break;
				}
				inter_time -= i_time;
			}
		}else{

			// > ����
			QJsonArray gif_intervalRealTank = data_act["gif_intervalRealTank"].toArray();
			for (int i = gif_intervalRealTank.count() - 1; i >= 0; i--){
				int i_time = gif_intervalRealTank[i].toInt();
				if( inter_time < i_time ){
					index = i;
					break;
				}
				inter_time -= i_time;
			}
		}
		this->_drill_bitmapName = gif_src[index].toString();
		this->_drill_bitmapPath = data_act["gif_src_file"].toString();
		this->_drill_bitmapTint = data_act["gif_tint"].toInt();
		this->_drill_bitmapSmooth = data_act["gif_smooth"].toBool();
	}

	// > ʱ��+1
	this->_drill_act_curTime += 1;
	if (this->_drill_act_curTime > gif_src.count() * data_act["gif_interval"].toInt()){
		this->_drill_act_curCom = "";
		this->_drill_act_curTime = 0;
	}

}


/*-------------------------------------------------
		���� - ״̬Ԫ - �������У��ӿڣ�
*/
void Drill_COAS_Data::drill_COAS_setSequence(QStringList seq){
	this->_drill_state_curSeq = seq;
}
/*-------------------------------------------------
		���� - ״̬Ԫ - �������У����̸ı䣨�ӿڣ�
*/
void Drill_COAS_Data::drill_COAS_setSequenceImmediate(QStringList seq){
	this->_drill_state_curSeq = seq;
	this->_drill_state_curCom = "";
	this->_drill_state_curTime = 0;
}
/*-------------------------------------------------
		���� - ״̬Ԫ - ��ȡ���� ��������
*/
QJsonObject Drill_COAS_Data::drill_COAS_getDataState(QString state_name){
	QJsonObject data = this->_drill_data;
	QJsonArray arr = data["state_tank"].toArray();
	for (int i = 0; i < arr.count(); i++){
		QJsonObject data_state = arr[i].toObject();
		if (data_state.isEmpty() == false && data_state["name"].toString() == state_name){
			return data_state;
		}
	}
	return QJsonObject();
}
/*-------------------------------------------------
		���� - ����Ԫ - �жϲ���
*/
bool Drill_COAS_Data::drill_COAS_isPlayingAct(){
	return this->_drill_act_curCom != "";
}
/*-------------------------------------------------
		���� - ����Ԫ - ��Ӷ������ӿڣ�
*/
void Drill_COAS_Data::drill_COAS_setAct(QString act_name){
	if( this->_drill_act_curCom == act_name ){ return; }
	
	// > �������ȼ�״̬Ԫ
	if( this->_drill_act_curCom == "" ){
		QJsonObject data_act = this->drill_COAS_getDataAct(act_name);
		QJsonObject cur_state = this->drill_COAS_getDataState(this->_drill_state_curCom);
		
		if (cur_state["priority"].toInt() >= data_act["priority"].toInt()){	//��ֻ�ܸ���С�����ȼ���������ͬ����
			return;
		}
	}
		
	// > �������ڲ���ʱ
	if( this->_drill_act_curCom != "" ){
		QJsonObject data_act = this->drill_COAS_getDataAct(act_name);
		QJsonObject cur_act = this->drill_COAS_getDataAct(this->_drill_act_curCom);
		
		if (cur_act["priority"].toInt() >= data_act["priority"].toInt()){	//��ֻ�ܸ���С�����ȼ���������ͬ����
			return;
		}
	}
	
	this->_drill_act_curCom = act_name;
}
/*-------------------------------------------------
		���� - ����Ԫ - ������ֹ�������ӿڣ�
*/
void Drill_COAS_Data::drill_COAS_stopAct(){
	this->_drill_act_curCom = "";
	this->_drill_act_curTime = 0;
}
/*-------------------------------------------------
		���� - ����Ԫ - ��ȡ���� ��������
*/
QJsonObject Drill_COAS_Data::drill_COAS_getDataAct(QString act_name){
	QJsonObject data = this->_drill_data;
	QJsonArray arr = data["act_tank"].toArray();
	for (int i = 0; i < arr.count(); i++){
		QJsonObject data_act = arr[i].toObject();
		if (data_act.isEmpty() == false && data_act["name"].toString() == act_name){
			return data_act;
		}
	}
	return QJsonObject();
}