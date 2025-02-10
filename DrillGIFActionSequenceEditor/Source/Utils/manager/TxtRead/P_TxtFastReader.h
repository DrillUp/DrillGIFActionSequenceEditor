#pragma once
#include "stdafx.h"

#include <QObject>

/*
-----==========================================================-----
		类：		文本快速读取器.h
		版本：		v1.13
		作者：		drill_up
		所属模块：	工具模块
		功能：		读取 文本文件的内容，批量操作。
					【可对多篇文本解析，速度快（优先考虑性能）】
					（详细见cpp）
-----==========================================================-----
*/

class P_TxtFastReader
{

	//-----------------------------------
	//----基础结构
	public:
		P_TxtFastReader(QString context);
		P_TxtFastReader(QString context, QRegExp re);
		P_TxtFastReader(QStringList context_list);
		~P_TxtFastReader();
		
	//-----------------------------------
	//----数据
	protected:
		QStringList log_context_list;	//log行字符（处理后的）
	public:
										//数据 - 获取全部行
		QStringList getAllRows();

	//-----------------------------------
	//----全行
	protected:
		bool m_trim;
		QStringList m_replace_strFrom;
		QStringList m_replace_strTo;
		QList<QRegExp> m_replace_expFrom;
		QStringList m_replace_expTo;
	public:
										//全行 - 清理空格（预备）
		void prepare_trimAllRows();
										//全行 - 替换字符（预备）
		void prepare_replaceInStrings(QString re_str, QString str);
		void prepare_replaceInStrings(QRegExp re, QString str);
										//全行 - 清理空行
		void clearEmptyRows();
										//全行 - 去除注释行（根据首字符）
		void clearComment(QString prefix);

	protected:
										//全行 - 执行预备操作
		QString doPrepare(QString str);

	//-----------------------------------
	//----直接操作（directly）
	public:
										//直接 - 第i行
		virtual QString d_rowAt(int i);
										//直接 - 获取全部行数
		virtual int d_rowCount();
										//直接 - 获取包含行
										//		【参数1】：包含的索引字符串
										//		【返回】：索引值（未找到返回-1）
		virtual int d_indexOf(QString contains);
		virtual int d_indexOf(QRegExp re_contains);
										//直接 - 获取包含行（从末尾找）
										//		【参数1】：包含的索引字符串
										//		【返回】：索引值（未找到返回-1）
		virtual int d_lastIndexOf(QString contains);
		virtual int d_lastIndexOf(QRegExp re_contains);
										//直接 - 获取包含行（指定范围）
										//		【参数1】起始行
										//		【参数2】终止行
										//		【返回】索引值（未找到返回-1）
		virtual int d_indexOf(QString contains, int n, int m = -1);
		virtual int d_indexOf(QRegExp re_contains, int n, int m = -1);
										//直接 - 获取包含行下的所有行
										//		【参数1】起始行
										//		【参数2】长度
										//		【返回】行列表（未找到/光标超过总行数 返回空列表）
		virtual QStringList d_getRows(int n, int row_count = -1);
										//直接 - 获取包含的所有行
										//		【参数1】包含的索引字符串
										//		【返回】所有包含的行（未找到返回空列表）
		virtual QList<int> d_getAllRowIndexsContains(QString contains);
		virtual QList<int> d_getAllRowIndexsContains(QRegExp re_contains);
		virtual QStringList d_getAllRowsContains(QString contains);
		virtual QStringList d_getAllRowsContains(QRegExp re_contains);
		
		
	//-----------------------------------
	//----字符串
	public:
										//字符串 - 获取浮点数（非榨取， 1.0e3 浮点数两边有空格才被获取）
										//		【参数1】单行字符串
										//		【返回】数列表
		QList<double> s_getDoubleInRow(QString row);
										//字符串 - 获取浮点数（榨取）
		QList<double> s_getDoubleForceInRow(QString row);
										//字符串 - 获取整数（非榨取， 23 整数数两边有空格才被获取）
										//		【参数1】单行字符串
										//		【返回】数列表
		QList<int> s_getIntInRow(QString row);
										//字符串 - 获取整数（榨取）
		QList<int> s_getIntForceInRow(QString row);

};

