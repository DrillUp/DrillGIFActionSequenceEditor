#ifndef I_FCTLeaf_H
#define I_FCTLeaf_H

#include "Source/Utils/common/TTool.h"
#include "c_FPT_Config.h"

/*
-----==========================================================-----
		类：		叶子.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		叶子控件类。
-----==========================================================-----
*/
class I_FPT_Leaf : public QTreeWidgetItem{

	public:
		I_FPT_Leaf(C_FPT_Config* config, QTreeWidgetItem *parent = nullptr);
		~I_FPT_Leaf();

	//-----------------------------------
	//----数据
		TTOOL_DATA_PROPERTY(int, Id);							//标识
		TTOOL_DATA_PROPERTY(QString, Name);						//名称
		TTOOL_DATA_PROPERTY(QString, Type);						//所属类型

		TTOOL_DATA_PROPERTY(QString, Leaf_name_Symbol);			//名称分支 - 首字母标识
	
	protected:
		C_FPT_Config* m_config;		//树设置

	//-----------------------------------
	//----控件
	public:
										//控件 - 获取显示文本
		QString getShowingText();
										//控件 - 根据数据自我刷新
		void refreshItemSelf();	

};

#endif // I_FCTLeaf_H
