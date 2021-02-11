#include "stdafx.h"
#include "p_RmmvCaller.h"

#include "Source/MainModule/drillgifactionsequenceeditor.h"

#include "Source/Utils/manager/s_CallManager.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		rmmv交互者（父类）.cpp
		版本：		v1.00
		所属模块：	交互模块
		功能：		与rmmv软件交互的单例，只包含一些基本独立的操作。

		说明：		为了保证接口唯一性，
					与rmmv软件/文件的所有相关操作都在该交互模块中。
					其他模块都不能直接与rmmv交互，必须经过该 父类 进行交互。

		目标：		-> 打开rmmv
					-> 保存rmmv
					-> 运行rmmv
					
		使用方法：
				>继承该类
					
-----==========================================================-----
*/
P_RmmvCaller::P_RmmvCaller(){
}
P_RmmvCaller::~P_RmmvCaller() {
}

/*-------------------------------------------------
		rmmv - 打开rmmv
*/
C_RmmvProjectData P_RmmvCaller::callRmmvOpen(){
	QFileDialog fd;
	fd.setWindowTitle("打开rmmv工程");
	fd.setAcceptMode(QFileDialog::AcceptOpen);		//对话框类型（打开/保存）（保存会有文件覆盖提示）
	fd.setDirectory(".");							//默认目录
	fd.setNameFilters(QStringList()					//文件格式
		<< "rmmv工程文件(*.rpgproject)"
		);
	fd.setViewMode(QFileDialog::Detail);

	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()) {
			//（没有选择文件进入的情况）
			return C_RmmvProjectData();
		}
		QString file_path = fd.selectedFiles().at(0);

		C_RmmvProjectData data = C_RmmvProjectData();
		data.path = QFileInfo(file_path).absolutePath();	//获取父目录

		// > 读取工程文件
		QFile project_file(file_path);
		if (!project_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QMessageBox::warning( DrillGIFActionSequenceEditor::getInstance(), "错误", "未找到Game.rpgproject文件。", QMessageBox::Yes);
			return C_RmmvProjectData();
		}
		data.version = project_file.readAll();
		project_file.close();

		// > 读取index文件
		QFile index_file(data.path + "/index.html");
		if (!index_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QMessageBox::warning(DrillGIFActionSequenceEditor::getInstance(), "错误", "未找到index.html文件。", QMessageBox::Yes);
			return C_RmmvProjectData();
		}
		QString index_context = index_file.readAll();
		index_file.close();

		// > 解析出名称
		int i_a = index_context.indexOf("<title>") + 7;
		int i_b = index_context.indexOf("</title>", i_a);
		data.name = index_context.mid(i_a,i_b-1);

		return data;
	}
	else {
		//（点击关闭或者取消操作的情况）
		return C_RmmvProjectData();
	}
}

/*-------------------------------------------------
		rmmv - 保存rmmv
*/
void P_RmmvCaller::callRmmvSave(C_RmmvProjectData rmmvProjectData){
	QString rmmv_window_name = rmmvProjectData.name + " - RPG Maker MV";

	// > rmmv数量检查
	int num = S_CallManager::getInstance()->getWindowNum(rmmv_window_name);
	if (num > 1){
		QMessageBox::warning( nullptr, "错误", "你同时开了" + TTool::_auto_(num) + "个一模一样的rmmv工程窗口，请确保只有一个rmmv工程窗口。", QMessageBox::Yes);
		return;
	}else if (num == 0){
		this->saveDataToRmmv(rmmvProjectData);
		return;
	}

	// > rmmv保存
	HWND hwnd = S_CallManager::getInstance()->getWindow(rmmv_window_name);
	int success = S_CallManager::getInstance()->callSave(hwnd);

	// > 工程覆盖保存
	this->saveDataToRmmv(rmmvProjectData);

	// > 返回界面
	S_CallManager::getInstance()->activeLocalWindow( DrillGIFActionSequenceEditor::getInstance() );
	if (!success){
		QMessageBox::warning(DrillGIFActionSequenceEditor::getInstance(), "错误", "保存失败，请先关闭你在rmmv中正在编辑的窗口。", QMessageBox::Yes);
		return;
	}
}

/*-------------------------------------------------
		rmmv - 运行rmmv
*/
void P_RmmvCaller::callRmmvRun(C_RmmvProjectData rmmvProjectData){
	QString rmmv_window_name = rmmvProjectData.name + " - RPG Maker MV";

	//>工程数量检查
	int num = S_CallManager::getInstance()->getWindowNum(rmmv_window_name);
	if (num > 1){
		QMessageBox::warning(DrillGIFActionSequenceEditor::getInstance(), "错误", "你同时开了" + TTool::_auto_(num) + "个一模一样的rmmv工程窗口，请确保只有一个rmmv工程窗口。", QMessageBox::Yes);
		return;
	}else if (num == 0){

		// ...（需要找到一个合适的方法，在没有rmmv窗口时，启动rmmv）

		QMessageBox::warning(DrillGIFActionSequenceEditor::getInstance(), "错误", "你没有打开rmmv软件。", QMessageBox::Yes);
		return;
	}

	//>工程运行
	HWND hwnd = S_CallManager::getInstance()->getWindow(rmmv_window_name);
	int success = S_CallManager::getInstance()->callSave(hwnd);

	//>返回界面
	S_CallManager::getInstance()->activeLocalWindow(DrillGIFActionSequenceEditor::getInstance());
	if (!success){
		QMessageBox::warning(DrillGIFActionSequenceEditor::getInstance(), "错误", "运行失败，请先关闭你在rmmv中正在编辑的窗口。", QMessageBox::Yes);
		return;
	}
}
