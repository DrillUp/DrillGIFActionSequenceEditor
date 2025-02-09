#pragma once

/*
-----==========================================================-----
		类：		操作面板 数据容器.h
		作者：		drill_up
		所属模块：	交互模块
		功能：		操作面板的数据配置。
					（详细见.cpp）
-----==========================================================-----
*/
class S_RmmvOperateBoardConfig : public QObject
{
	Q_OBJECT

	public:
		S_RmmvOperateBoardConfig();
		~S_RmmvOperateBoardConfig();
		static S_RmmvOperateBoardConfig* cur_container;		//单例
		static S_RmmvOperateBoardConfig* getInstance();		//单例，获取自己
		
	//-----------------------------------
	//----数据
	protected:
		bool m_bakEnabled;			//备份设置
	public:
										//数据 - 修改备份设置
		void setBakEnabled(bool enabled);
										//数据 - 是否要备份
		bool isBakEnabled();
	
};
