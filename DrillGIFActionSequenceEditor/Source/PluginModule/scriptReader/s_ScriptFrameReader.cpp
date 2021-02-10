#include "stdafx.h"
#include "S_ScriptFrameReader.h"

#include "Source/Utils/manager/txtRead/p_TxtFastReader.h"

/*
-----==========================================================-----
		类：		脚本参数解析器.cpp
		所属模块：	插件模块
		功能：		读取js文件内的所有数据框架内容。

		使用方法：
				>初始化：
				
-----==========================================================-----
*/

S_ScriptFrameReader::S_ScriptFrameReader(){
}
S_ScriptFrameReader::~S_ScriptFrameReader() {
}

/* --------------------------------------------------------------
----------PluginFrameManager 单例
*/
S_ScriptFrameReader* S_ScriptFrameReader::cur_manager = NULL;
S_ScriptFrameReader* S_ScriptFrameReader::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_ScriptFrameReader();
	}
	return cur_manager;
}


/*-------------------------------------------------
		读取 - 插件参数（简要）
*/
C_ScriptFrame S_ScriptFrameReader::readPlugin_simple(QString plugin_context) {
	C_ScriptFrame c_ScriptFrame = C_ScriptFrame();
	c_ScriptFrame.isComplete = false;

	// > 读取基本信息
	P_TxtFastReader reader = P_TxtFastReader(plugin_context);
	reader.prepare_trimAllRows();									// 去除空格
	reader.prepare_replaceInStrings(QRegExp("^\\*[ ]?"), "");		// 去除注释的前缀内容

	// > 第一篇注释
	int i_comment_start = reader.d_indexOf("/*:");
	int i_comment_end = reader.d_indexOf("*/", i_comment_start);
	if (i_comment_start != -1) {

		// > 获取 描述
		int i_plugindesc = reader.d_indexOf("@plugindesc", i_comment_start);
		if (i_plugindesc != -1 && i_plugindesc <= i_comment_end) {
			QString s_rows = reader.d_getRows(i_plugindesc,2).join(" ");
			QStringList list_plugindesc = s_rows.split("@");
			if (list_plugindesc.size() >= 2) {
				QString s = list_plugindesc.at(1);
				c_ScriptFrame.plugindesc = s.replace("plugindesc", "").trimmed();
			}
		}

		// > 获取 作者
		int i_author = reader.d_indexOf("@author", i_comment_start);
		if (i_author != -1 && i_author <= i_comment_end) {
			QStringList list_author = reader.d_rowAt(i_author).split("@author");
			if (list_author.size() >= 2) {
				c_ScriptFrame.author = list_author.at(1).trimmed();
			}
		}

	}

	return c_ScriptFrame;
}
/*-------------------------------------------------
		读取 - 插件参数（完整）
*/
C_ScriptFrame S_ScriptFrameReader::readPlugin_complete(QString plugin_context) {
	C_ScriptFrame c_ScriptFrame = C_ScriptFrame();
	c_ScriptFrame.isComplete = true;

	// > 读取基本信息
	P_TxtFastReader reader = P_TxtFastReader(plugin_context);
	reader.prepare_trimAllRows();									// 去除空格
	reader.prepare_replaceInStrings(QRegExp("^\\*[ ]?"), "");		// 去除注释的前缀内容

	// > 第一篇注释
	int i_comment_start = reader.d_indexOf("/*:");
	int i_comment_end = reader.d_indexOf("*/", i_comment_start);
	if (i_comment_start != -1) {

		// > 获取 描述
		int i_plugindesc = reader.d_indexOf("@plugindesc", i_comment_start);
		if (i_plugindesc != -1 && i_plugindesc <= i_comment_end ) {
			QString s_rows = reader.d_getRows(i_plugindesc,2).join(" ");
			QStringList list_plugindesc = s_rows.split("@");
			if (list_plugindesc.size() >= 2) {
				QString s = list_plugindesc.at(1);
				c_ScriptFrame.plugindesc = s.replace("plugindesc", "").trimmed();
			}
		}

		// > 获取 作者
		int i_author = reader.d_indexOf("@author", i_comment_start);
		if (i_author != -1 && i_author <= i_comment_end){
			QStringList list_author = reader.d_rowAt(i_author).split("@author");
			if (list_author.size() >= 2) {
				c_ScriptFrame.author = list_author.at(1).trimmed();
			}
		}

		// > 获取 帮助信息
		int i_help = reader.d_indexOf("@help", i_comment_start, i_comment_end);
		if (i_help != -1 && i_help <= i_comment_end) {
			QStringList list_help = reader.d_getRows(i_help , i_comment_end-i_help).join("\n").split("@help");	// 从@help开始到注释结束查找"@"
			if (list_help.size() >= 2) {
				QString help_text = list_help.at(1);
				help_text = help_text.replace("@help", "");
				int index = help_text.indexOf("@");
				help_text = help_text.mid(0, index);
				help_text = help_text.trimmed();	// 去除开头的结尾的"\n"和空格
				c_ScriptFrame.help = help_text;
			}
		}
		
		// > 获取 参数
		int i_param = reader.d_indexOf("@param", i_comment_start, i_comment_end);

	}

	// > 结构体
	int i_comment_struct = reader.d_indexOf(QRegExp("/\\*~struct~[\\S]+:"));
	//...

	return c_ScriptFrame;
}