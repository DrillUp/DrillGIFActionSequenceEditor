#pragma once
#include "stdafx.h"


/*
-----==========================================================-----
		绫伙細		琛ㄦ牸閰嶇疆 鏁版嵁绫?cpp
		浣滆€咃細		drill_up
		鎵€灞炴ā鍧楋細	宸ュ叿妯″潡
		鍔熻兘锛?	鍗曢€夎〃鏍奸厤缃?鐨勬暟鎹被銆?
					锛堣缁嗚cpp锛?
-----==========================================================-----
*/
class C_RaTConfig{

	public:
		C_RaTConfig();
		~C_RaTConfig();
		
	//-----------------------------------
	//----鏁版嵁
	public:
		bool showNumber;			//鏄剧ず搴忓彿
		bool zeroFill;				//闆跺～鍏?
		int zeroFillCount;			//濉厖浣嶆暟
		QChar zeroFillChar;			//濉厖瀛楃

		int rowHeight;				//琛岄珮
		bool isMultiSelect;			//澶氶€夊紑鍏?

	//-----------------------------------
	//----绫诲睘鎬?
	public:
									//瀹炰綋绫?-> QJsonObject
		QJsonObject getJsonObject();
									//QJsonObject -> 瀹炰綋绫?
		void setJsonObject(QJsonObject obj);
};