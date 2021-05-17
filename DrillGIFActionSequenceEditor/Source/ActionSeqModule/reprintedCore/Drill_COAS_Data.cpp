#include "stdafx.h"
#include "Drill_COAS_Data.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		�����������ݡ������ࡿ.cpp
		���ߣ�		drill_up
		����ģ�飺	������������
		���ܣ�		�������к��Ĳ���ĸ����ࡣ
					
		˵����		��ǰ���̰汾��[v1.3]
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
	if (data["waitForPreload"].isUndefined() == true){ data["waitForPreload"] = false; }							//Ԥ���صȴ����Ӳ���ò�����
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
	this->_drill_state_lastAnnotation = "";							//״̬Ԫ - ��һ��ע����
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
	if (this->_drill_state_curTime >= data_state["gif_intervalRealTank_total"].toInt()){
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
	if (this->_drill_act_curTime > data_act["gif_intervalRealTank_total"].toInt()){
		this->_drill_act_curCom = "";
		this->_drill_act_curTime = 0;
	}

}


/*-------------------------------------------------
		���� - �������Ԫ��

		��c++�����Զ���飩
*/

/*-------------------------------------------------
		���� - ״̬Ԫ - �������У��ӿڣ�

				˵����	�����д�� ״̬���� �����飬״̬Ԫ��������ʶ��Ķ���
*/
void Drill_COAS_Data::drill_COAS_setSequence(QStringList seq){
	this->_drill_state_curSeq = seq;
}
/*-------------------------------------------------
		���� - ״̬Ԫ - �������У����̸ı䣨�ӿڣ�
*/
void Drill_COAS_Data::drill_COAS_setSequenceImmediate(QStringList seq){
	this->drill_COAS_setSequence( seq );
	this->_drill_state_curCom = "";
	this->_drill_state_curTime = 0;
}
/*-------------------------------------------------
		���� - ״̬Ԫ - ����״̬Ԫ����[ע��ģʽ]���ӿڣ�

				˵����	״̬Ԫ�����к����ض�ע��ģ��ᱻ���������û���񵽣�����ʧ�ܣ�false����
*/
bool Drill_COAS_Data::drill_COAS_setSequenceByAnnotation(QString annotation){
	if (this->_drill_state_lastAnnotation == annotation){ return true; }	//���ظ�ע��ʱ������
	this->_drill_state_lastAnnotation = annotation;

	// > ��ȡע���б�
	QStringList annotation_list = QStringList();
	QStringList temp_list = annotation.split("@");
	for (int i = 0; i < temp_list.count(); i++){
		QString temp = temp_list[i];
		if (temp == ""){ continue; }
		annotation_list.push_back("@" + temp);
	}

	// > �ҵ�����ע����������״̬Ԫ��
	QStringList name_list = this->drill_COAS_getAllStateName();
	int max_fit_count = 0;									//��������������
	QList<QJsonObject> tag_seq = QList<QJsonObject>();		//�������ϵ������б�
	for (int i = 0; i < name_list.count(); i++){
		QString name = name_list[i];

		// > ��ȡ@��������
		int char_count = 0;
		for (int j = 0; j < name.count(); j++){
			if (name[j] == '@'){
				char_count += 1;
			}
		}
		if (char_count == 0){ continue; }

		// > ��¼ע���������
		int fit_count = 0;
		for (int j = 0; j < annotation_list.count(); j++){
			QString annotation = annotation_list[j];
			if (name.contains(annotation) == true){
				fit_count += 1;
			}
		}

		// > ���в�ƥ���@����ʱ������
		if (char_count > fit_count){
			continue;
		}

		// > ������������ʱ��������У��������
		if (fit_count > max_fit_count){
			tag_seq = QList<QJsonObject>();
			max_fit_count = fit_count;

			QJsonObject tag = QJsonObject();
			tag["index"] = i;
			tag["count"] = fit_count;
			tag["name"] = name;
			tag_seq.push_back(tag);

		// > ����������ȣ��ۼ�
		}else if (fit_count == max_fit_count){
			QJsonObject tag = QJsonObject();
			tag["index"] = i;
			tag["count"] = fit_count;
			tag["name"] = name;
			tag_seq.push_back(tag);

		// > �����������ˣ�����
		}else{
			continue;
		}
	}
	if (tag_seq.count() == 0){ return false; }

	// > �������ֵ���±�ȡ�����ϵ�����
	QStringList seq = QStringList();
	for (int i = 0; i < tag_seq.count(); i++){
		seq.push_back(tag_seq[i]["name"].toString());
	}

	this->_drill_state_curSeq = seq;
	return true;
}

/*-------------------------------------------------
		���� - ״̬Ԫ - ����״̬Ԫ����[ע��ģʽ]-���̸ı䣨�ӿڣ�
*/
bool Drill_COAS_Data::drill_COAS_setSequenceImmediateByAnnotation(QString annotation){
	if (this->_drill_state_lastAnnotation == annotation){ return true; }	//���ظ�ע��ʱ������

	bool success = this->drill_COAS_setSequenceByAnnotation(annotation);
	if (success){
		this->_drill_state_curCom = "";
		this->_drill_state_curTime = 0;
	}
	return success;
}
/*-------------------------------------------------
		���� - ״̬Ԫ - ��ȡĬ��״̬Ԫ���ϣ��ӿڣ�
*/
QStringList Drill_COAS_Data::drill_COAS_getDefaultStateGroup(){
	return TTool::_QJsonArray_AToQString_(this->_drill_data["state_default_randomSeq"].toArray());
}
/*-------------------------------------------------
		���� - ״̬Ԫ - ��ȡ��ǰ״̬Ԫ���ƣ��ӿڣ�
*/
QString Drill_COAS_Data::drill_COAS_getCurrentStateName(){
	return this->_drill_state_curCom;
}
/*-------------------------------------------------
		���� - ״̬Ԫ - ��ȡ��ǰ״̬Ԫ�������ƣ��ӿڣ�
*/
QStringList Drill_COAS_Data::drill_COAS_getCurrentStateSeqName(){
	return this->_drill_state_curSeq;
}
/*-------------------------------------------------
		���� - ״̬Ԫ - ��ȡȫ��״̬Ԫ���ƣ��ӿڣ�
*/
QStringList Drill_COAS_Data::drill_COAS_getAllStateName(){
	QJsonObject data = this->_drill_data;
	QStringList result_list = QStringList();
	QJsonArray state_tank = data["state_tank"].toArray();
	for (int i = 0; i < state_tank.count(); i++){
		QJsonObject data_state = state_tank[i].toObject();
		if (data_state.isEmpty() == false && data_state["name"].toString() != ""){
			result_list.push_back(data_state["name"].toString());
		}
	}
	return result_list;
}
/*-------------------------------------------------
		���� - ״̬Ԫ - ��ȡ���� ��������
*/
QJsonObject Drill_COAS_Data::drill_COAS_getDataState(QString state_name){
	QJsonObject data = this->_drill_data;
	QJsonArray state_tank = data["state_tank"].toArray();
	for (int i = 0; i < state_tank.count(); i++){
		QJsonObject data_state = state_tank[i].toObject();
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
		
		if (cur_state["priority"].toInt() > data_act["priority"].toInt()){	//��ͬ���Ķ���Ԫ���Ը���״̬Ԫ��
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
		���� - ����Ԫ - ��ȡ��ǰ����Ԫ���ƣ��ӿڣ�
*/
QString Drill_COAS_Data::drill_COAS_getCurrentActName(){
	return this->_drill_act_curCom;
}
/*-------------------------------------------------
		���� - ����Ԫ - ��ȡȫ������Ԫ���ƣ��ӿڣ�
*/
QStringList Drill_COAS_Data::drill_COAS_getAllActName(){
	QJsonObject data = this->_drill_data;
	QStringList result_list = QStringList();
	QJsonArray act_tank = data["act_tank"].toArray();
	for (int i = 0; i < act_tank.count(); i++){
		QJsonObject data_act = act_tank[i].toObject();
		if (data_act.isEmpty() == false && data_act["name"].toString() != ""){
			result_list.push_back(data_act["name"].toString());
		}
	}
	return result_list;
}
/*-------------------------------------------------
		���� - ����Ԫ - ��ȡ���� ��������
*/
QJsonObject Drill_COAS_Data::drill_COAS_getDataAct(QString act_name){
	QJsonObject data = this->_drill_data;
	QJsonArray act_tank = data["act_tank"].toArray();
	for (int i = 0; i < act_tank.count(); i++){
		QJsonObject data_act = act_tank[i].toObject();
		if (data_act.isEmpty() == false && data_act["name"].toString() == act_name){
			return data_act;
		}
	}
	return QJsonObject();
}