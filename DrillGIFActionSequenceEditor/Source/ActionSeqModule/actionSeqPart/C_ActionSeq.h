#pragma once
#include "stdafx.h"


/*
-----==========================================================-----
		绫伙細		鍔ㄧ敾搴忓垪 鏁版嵁绫?cpp
		浣滆€咃細		drill_up
		鎵€灞炴ā鍧楋細	鍔ㄧ敾搴忓垪妯″潡
		鍔熻兘锛?	鍔ㄧ敾搴忓垪 鐨勬暟鎹被銆?
					锛堣缁嗚cpp锛?
-----==========================================================-----
*/
class C_ActionSeq{

	public:
		C_ActionSeq();
		~C_ActionSeq();
		
	//-----------------------------------
	//----鏁版嵁
	public:
		QString m_name;								//鍔ㄧ敾搴忓垪鍚嶇О/鏍囩
		QStringList m_state_default_randomSeq;		//榛樿鐨勭姸鎬佸厓闆嗗悎
		QList<QJsonObject> m_act_tank;				//鍔ㄤ綔鍏冨鍣?
		QList<QJsonObject> m_state_tank;			//鐘舵€佸厓瀹瑰櫒
		QList<QJsonObject> m_stateNode_tank;		//鐘舵€佽妭鐐瑰鍣?

	//-----------------------------------
	//----绫诲睘鎬?
	public:
									//瀹炰綋绫?-> QJsonObject
		QJsonObject getJsonObject();
									//QJsonObject -> 瀹炰綋绫?
		void setJsonObject(QJsonObject obj);
};