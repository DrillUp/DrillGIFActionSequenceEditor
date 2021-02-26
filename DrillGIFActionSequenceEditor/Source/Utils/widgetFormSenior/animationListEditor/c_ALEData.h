#pragma once
#include <QList>

/*
-----==========================================================-----
		类：		动画帧 数据类.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		动画帧 的数据类。
					注意，只有五项。如果有其他数据合并，想办法另起一个类。
					（详细见cpp）
-----==========================================================-----
*/
class C_ALEData {

	public:
		C_ALEData();
		~C_ALEData();
		
	//-----------------------------------
	//----数据
	protected:
		int id;								//标识
		QList<QString> gif_src;				//资源文件名
		QString gif_src_file;				//资源文件夹
		QList<int> gif_intervalTank;		//帧间隔-明细表
		int gif_interval;					//帧间隔
		
	//-----------------------------------
	//----设置
	public:
											//设置 - 标识
		void setId(int id);

											//设置 - 资源
											//		【参数1】：文件父目录
											//		【参数2】：不带文件后缀，文件可重名
		void setSource(QString gif_src_file, QList<QString> gif_src);
											//设置 - 只设置文件父目录
		void setSourceParentFile(QString gif_src_file);

											//设置 - 帧间隔
		void setInterval(int gif_interval, QList<int> gif_intervalTank);
											//设置 - 默认帧间隔
		void setIntervalDefault(int gif_interval);
											//设置 - 默认帧间隔（统一改变）
		void setIntervalDefaultAndChange(int gif_interval);

	protected:
		void checkInterval();

	//-----------------------------------
	//----获取
	public:
											//获取 - 获取文件数量
		int getFileCount();
											//获取 - 获取文件
		QFileInfo getFile(int index);
		QList<QFileInfo> getFileList(QList<int> index_list);
		QList<QFileInfo> getAllFile();
											//获取 - 获取文件路径（F:/aaa/vvv ）
		QString getFileRoot();

											//获取 - 获取帧间隔
		QList<int> getAllInterval();
											//获取 - 获取帧间隔文本（"0.01"，实际帧为 0.01666 * n ）
		QString getIntervalString(int index);

											//获取 - 获取标识
		int getId();
											//获取 - 检查文件（不带文件后缀）
		bool hasFileName(QString file_name);
		

	//-----------------------------------
	//----操作
	public:
									//操作 - 添加
		void op_append(QString gif_src);
		void op_insert(int index, QString gif_src);
		void op_insert(int index, QStringList gif_src_list, QList<int> interval_list = QList<int>());
									//操作 - 替换
		void op_replace(int index, QString gif_src);
		void op_replace_interval(int index, int interval);
		void op_replace_interval(QList<int> index, int interval);
									//操作 - 移除
		void op_remove(int index);
									//操作 - 交换位置
		void op_swap(int index_a, int index_b);
	public:
									//复制文件
		static bool copyFile(QString filePath_from, QString filePath_to);

	//-----------------------------------
	//----类属性
	public:
									//空判断
		bool isNull();
									//实体类 -> QJsonObject
		QJsonObject getJsonObject();
									//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);
};