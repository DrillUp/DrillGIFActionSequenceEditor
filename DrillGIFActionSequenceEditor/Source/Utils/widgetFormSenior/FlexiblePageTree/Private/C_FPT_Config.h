#pragma once


/*
-----==========================================================-----
		类：		树设置 数据类.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		灵活分类树的 树设置 数据类。
					（详细见cpp）
-----==========================================================-----
*/
class P_FlexiblePageTree;
class C_FPT_Config{

	public:
		C_FPT_Config();
		~C_FPT_Config();
		
	//-----------------------------------
	//----显示设置
	public:
		int rowHeight;				//行高
		bool zeroFill;				//零填充
		int zeroFillCount;			//填充位数
		QChar zeroFillChar;			//填充字符

	
	//-----------------------------------
	//----分支模式
	protected:
		QString m_mode;				//当前模式
		QStringList m_modeList;		//模式列表
	public:
									//分支模式 - 设置当前模式
		void setCurrentMode(QString sortMode);
									//分支模式 - 获取当前模式
		QString getCurrentMode();
									//分支模式 - 设置模式列表
		void setModeList(QStringList sortModeList);
									//分支模式 - 获取模式列表
		QStringList getModeList();
									//分支模式 - 判断 ID分支 模式
		bool is_id_Mode();
									//分支模式 - 判断 名称分支 模式
		bool is_name_Mode();
		

	//-----------------------------------
	//----ID页
	public:
		int pagePerNum;				//ID分支每页条数
	protected:
		int m_id_maxCount;			//最大值（私有临时对象）
	public:
									//ID页 - 设置最大值
									//		【说明】：注意，必须要先赋值，在执行后面的获取函数，不然都会得到-1和空字符串。
		void set_id_MaxCount(int count);
									//ID页 - 获取最大值
		int get_id_MaxCount();
	public:
									//ID页 - 获取页数
		int get_id_PageCount();
									//ID页 - 获取底
		int get_id_Bottom(int page_index);
									//ID页 - 获取顶
		int get_id_Top(int page_index);
									//ID页 - 获取页名称（根据页索引）
		QString get_id_PageNameByIndex(int page_index);
									//ID页 - 获取格式化的ID
		QString get_id_FormatedId(int id);
		
	//-----------------------------------
	//----名称页
	protected:
		QStringList m_name_textList;	//名称列表
		QStringList m_name_symbolList;	//标识列表
	public:
									//名称页 - 获取页数
		int get_name_PageCount();
									//名称页 - 获取名称列表
		QStringList get_name_PageNameList();
									//名称页 - 获取标识列表
		QStringList get_name_PageSymbolList();
		
	//-----------------------------------
	//----自定义数据
	protected:
		QJsonObject data;
	public:
									//自定义数据 - 获取数据
		QJsonObject getCustomData();
		
	//-----------------------------------
	//----类属性
	public:
									//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject();
									//QJsonObject -> 实体类
		virtual void setJsonObject(QJsonObject obj, P_FlexiblePageTree* parent_obj);
};