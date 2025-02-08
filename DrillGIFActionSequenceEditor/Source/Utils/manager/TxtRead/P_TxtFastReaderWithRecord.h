#pragma once
#include "stdafx.h"

#include "P_TxtFastReader.h"

/*
-----==========================================================-----
		类：		文本快速读取器（记录行）.h
		版本：		v1.10
		作者：		drill_up
		所属模块：	工具模块
		功能：		读取 文本文件的内容，批量操作。
					【可对多篇文本解析，速度快（优先考虑性能）】
					（详细见cpp）
-----==========================================================-----
*/

class P_TxtFastReaderWithRecord : public P_TxtFastReader
{

	//-----------------------------------
	//----基础结构
	public:
		P_TxtFastReaderWithRecord(QString context);
		~P_TxtFastReaderWithRecord();
		
	//-----------------------------------
	//----数据
	private:
		QSet<int> m_recorded_list;		//log行字符（处理后的）
		
	public:
										//数据 - 获取全部行（含颜色标记）
		QStringList getAllRows_FillUsedColor();

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
		
		

};

