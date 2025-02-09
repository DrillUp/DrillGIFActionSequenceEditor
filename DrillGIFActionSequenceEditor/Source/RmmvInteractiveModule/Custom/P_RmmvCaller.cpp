#include "stdafx.h"
#include "p_RmmvCaller.h"

#include "Source/MainModule/drillgifactionsequenceeditor.h"

#include "Source/ActionSeqModule/Data/S_ActionSeqDataContainer.h"
#include "Source/Utils/manager/callManager/s_CallManager.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		rmmv交互者（父类）.cpp
		版本：		v1.02
		作者：		drill_up
		所属模块：	交互模块
		功能：		与rmmv软件交互的单例，只包含一些基本独立的操作。

		说明：		为了保证接口唯一性，
					与rmmv软件/文件的所有相关操作都在该交互模块中。
					其他模块都不能直接与rmmv交互，必须经过该 父类 进行交互。

		目标：		-> 打开rmmv
					-> 保存rmmv
					-> 运行rmmv
					
		问题：		> 必须关了rmmv编辑器，再保存才是最保险且有效的方法。
					> 虽然可以做到让rmmv保存，但是保存后再修改，这时候rmmv中的编辑器数据是不同步的。

		使用方法：
				>继承该类
					
-----==========================================================-----
*/
P_RmmvCaller::P_RmmvCaller(){
}
P_RmmvCaller::~P_RmmvCaller() {
}

/*-------------------------------------------------
		rmmv - 选择rmmv工程
*/
C_RmmvProjectData P_RmmvCaller::callRmmvSelect(){
	return C_RmmvProjectData::callRmmvSelectDialog();
}

/*-------------------------------------------------
		rmmv - 打开rmmv编辑器
*/
void P_RmmvCaller::callRmmvSoftware(C_RmmvProjectData rmmvProjectData){
	QString rmmv_window_name = rmmvProjectData.getName() + " - RPG Maker MV";

	// > 已存在时，激活窗口
	int num = S_CallManager::getInstance()->getWindowNum(rmmv_window_name);
	if (num >= 1){
		S_CallManager::getInstance()->activeWindow(rmmv_window_name);
		return;
	}
	
	// > 打开路径
	QDesktopServices::openUrl(QUrl("file:/" + rmmvProjectData.getRootPath() + "/Game.rpgproject"));
}

/*-------------------------------------------------
		rmmv - 保存rmmv（让rmmv先保存，然后自己存）
*/
void P_RmmvCaller::callRmmvSave(C_RmmvProjectData rmmvProjectData){
	QString rmmv_window_name = rmmvProjectData.getName() + " - RPG Maker MV";

	// > rmmv数量检查
	int num = S_CallManager::getInstance()->getWindowNum(rmmv_window_name);
	if (num > 1){
		QMessageBox::warning(DrillGIFActionSequenceEditor::getInstance(), "错误", "你同时开了" + TTool::_auto_(num) + "个一模一样的rmmv工程窗口，请确保只有一个rmmv工程窗口。", QMessageBox::Yes);
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
		rmmv - 保存rmmv（要求用户关闭rmmv，才能保存）
*/
void P_RmmvCaller::callRmmvSave_RequestingClose(C_RmmvProjectData rmmvProjectData){
	QString rmmv_window_name = rmmvProjectData.getName() + " - RPG Maker MV";

	// > rmmv数量检查
	int num = S_CallManager::getInstance()->getWindowNum(rmmv_window_name);
	if (num >= 1){
		QMessageBox::information(nullptr, "提示", "保存失败。\n由于更新的数据不能同步到已打开的rmmv编辑器中，\n所以必须先关闭工程\"" + rmmvProjectData.getName() + "\"，再进行保存。", QMessageBox::Yes);
		return;
	}

	// > 数组长度检查
	C_COAS_Length len = S_ActionSeqDataContainer::getInstance()->getActionSeqLength();
	if (len.isNull()){
		QMessageBox::information(nullptr, "提示", "保存失败。\n请重新检查数组长度，动画序列、状态元、状态节点、动作元长度必须要在4以上。", QMessageBox::Yes);
		return;
	}

	this->saveDataToRmmv(rmmvProjectData);
}

/*-------------------------------------------------
		rmmv - 运行rmmv
*/
void P_RmmvCaller::callRmmvRun(C_RmmvProjectData rmmvProjectData){
	QString rmmv_window_name = rmmvProjectData.getName() + " - RPG Maker MV";

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
	int success = S_CallManager::getInstance()->callRun(hwnd);

	//>返回界面
	S_CallManager::getInstance()->activeLocalWindow(DrillGIFActionSequenceEditor::getInstance());
	if (!success){
		QMessageBox::warning(DrillGIFActionSequenceEditor::getInstance(), "错误", "运行失败，请先关闭你在rmmv中正在编辑的窗口。", QMessageBox::Yes);
		return;
	}
}
