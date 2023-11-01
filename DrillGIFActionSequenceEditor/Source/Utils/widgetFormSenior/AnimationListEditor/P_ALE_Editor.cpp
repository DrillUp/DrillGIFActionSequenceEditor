#include "stdafx.h"
#include "P_ALE_Editor.h"

#include "Private/I_ALE_Cell.h"
#include "Private/W_ALE_Config.h"
#include "Private/W_ALE_CellDataSingle.h"
#include "Private/W_ALE_CellDataMultiple.h"
#include "Private/W_ALE_CellDataGlobal.h"
#include "Private/S_ALE_FileManager.h"

#include "Source/Utils/WidgetFormSenior/AnimationBlock/P_AnimationBlock.h"
#include "Source/GraphModule/Widget/PictureCombiner/W_PictureCombiner.h"
#include "Source/GraphModule/Widget/PicturePartitioner/W_PicturePartitioner.h"

#include "Source/DllModule/Cximagecrt_drill/Src_header/S_cximageManager.h"
#include "Source/Utils/Manager/GIFManager/S_GIFManager.h"

#include "Source/Utils/Common/P_FileOperater.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		动画帧编辑块.cpp
		版本：		v1.10
		作者：		drill_up
		所属模块：	工具模块
		功能：		将图片全部显示，能多选、编辑图片。（PLE全称：Picture_List_Editor）
					【底层基于QListWidget的item和widget，与QGraphics无关】
					【编辑块固定为 多选 】
					
		使用方法：
				> 初始化
					
-----==========================================================-----
*/

P_ALE_Editor::P_ALE_Editor(QListWidget *parent)
	: P_PLE_Editor(parent)
{

	//-----------------------------------
	//----参数初始化

	// > 数据
	this->setConfigParam(C_PLE_Config());

}
P_ALE_Editor::~P_ALE_Editor(){
}


/*-------------------------------------------------
		工厂 - 建立一个元胞数据（覆写）
*/
C_PictureBlockDataPtr P_ALE_Editor::createPictureCellData(){
	C_AnimationBlockDataPtr new_dataPtr = C_AnimationBlockDataPtr::create();
	new_dataPtr->m_intervalValue = this->m_dataSet_defaultInterval;		//（赋值当前控件的默认帧间隔）
	return new_dataPtr;
}
/*-------------------------------------------------
		工厂 - 建立一个元胞（覆写）
*/
I_PiS_Cell* P_ALE_Editor::createPictureCell(C_PictureBlockDataPtr data){

	// > 控件 - 建立动画帧单块
	P_AnimationBlock* widget = new P_AnimationBlock(this->m_parent);
	widget->setDataPtr(data);

	// > 项 - 建立项
	QListWidgetItem* item = new QListWidgetItem(this->m_parent);


	// > 元胞
	I_ALE_Cell* cell = new I_ALE_Cell();
	cell->item = item;
	cell->widget = widget;

	return cell;
}
/*-------------------------------------------------
		工厂 - 刷新元胞（覆写）
*/
void P_ALE_Editor::refreshPictureCell(int index){
	P_PLE_Editor::refreshPictureCell(index);
	if (index < 0){ return; }
	if (index >= this->m_cellTank.length()){ return; }
	I_PiS_Cell* cell_PiS = this->m_cellTank.at(index);
	I_ALE_Cell* cell = dynamic_cast<I_ALE_Cell*>(cell_PiS);

	// > 控件
	P_AnimationBlock* widget = cell->getBlock_ALE();
	C_AnimationBlockDataPtr data = cell->getBlockDataPtr_ALE();
	if (widget == nullptr){ return; }
	if (data.isNull()){ return; }

	// > 控件 - 刷新帧文本
	QString frame_text = C_ALE_DataSet::converterUnit_getDescriptionString(data->m_intervalValue, this->m_dataSet_unit);
	widget->setFrameLabel_Text(frame_text);

	// > 控件 - 刷新配置
	widget->setFrameLabel_Bold(this->m_config_ALE.m_frameText_isBold);

}


/*-------------------------------------------------
		控件 - 重建UI
*/
void P_ALE_Editor::rebuildUI_ALE(QStringList bitmapPath_list, QList<int> interval_list){
	P_PLE_Editor::rebuildUI(bitmapPath_list);

	Q_ASSERT(bitmapPath_list.count() == interval_list.count());

	// > 初始化 帧间隔值
	for (int i = 0; i < this->m_cellTank.count(); i++){
		I_PiS_Cell* cell_PiS = this->m_cellTank.at(i);
		I_ALE_Cell* cell = dynamic_cast<I_ALE_Cell*>(cell_PiS);
		C_AnimationBlockDataPtr cellData = cell->getBlockDataPtr_ALE();
		cellData->m_intervalValue = interval_list.at(i);
	}
}
void P_ALE_Editor::rebuildUI(QStringList bitmapPath_list){
	P_PLE_Editor::rebuildUI(bitmapPath_list);
	//（暂无，特殊操作在 rebuildUI_ALE 中执行）
}
/*-------------------------------------------------
		控件 - 刷新UI
*/
void P_ALE_Editor::refreshUI(){
	P_PLE_Editor::refreshUI();
	//（暂无）
}
/*-------------------------------------------------
		控件 - 清理控件
*/
void P_ALE_Editor::clearCells(){
	P_PLE_Editor::clearCells();
	//（暂无）
}
/*-------------------------------------------------
		控件 - 清理全部
*/
void P_ALE_Editor::clearAll(){
	P_PLE_Editor::clearAll();
	//（暂无）
}



/*-------------------------------------------------
		块数据 - 设置数据
*/
void P_ALE_Editor::setDataSet_ALE(C_ALE_DataSet data) {

	// > 父路径（全局数据）
	this->m_dataSet_parentDir = data.getData_ParentDir();
	
	// > 单位（全局数据）
	this->m_dataSet_unit = data.getData_Unit();

	// > 默认帧间隔（全局数据）
	this->m_dataSet_defaultInterval = data.getData_IntervalDefault();

	// > 资源文件
	QStringList pic_list = data.getData_PicList();
	QList<int> interval_list = data.getData_IntervalTank_WithFit();
	this->rebuildUI_ALE(pic_list, interval_list);	//（重建UI）
}
/*-------------------------------------------------
		块数据 - 获取数据
*/
C_ALE_DataSet P_ALE_Editor::getDataSet_ALE(){
	if (this->m_cellTank.count() == 0){ return C_ALE_DataSet(); }

	QStringList pic_list;
	QList<int> interval_list;
	for (int i = 0; i < this->m_cellTank.count(); i++){
		I_PiS_Cell* cell_PiS = this->m_cellTank.at(i);
		I_ALE_Cell* cell = dynamic_cast<I_ALE_Cell*>(cell_PiS);
		C_AnimationBlockDataPtr cellData = cell->getBlockDataPtr_ALE();
		pic_list.append(cellData->m_bitmapPath);
		interval_list.append(cellData->m_intervalValue);
	}

	C_ALE_DataSet dataSet;
	dataSet.setData_PicList(pic_list);
	dataSet.setData_IntervalTank(interval_list);

	dataSet.setData_ParentDir(this->m_dataSet_parentDir);
	dataSet.setData_Unit(this->m_dataSet_unit);
	dataSet.setData_IntervalDefault(this->m_dataSet_defaultInterval);

	return dataSet;
}
/*-------------------------------------------------
		块数据 - 获取当前数据 - 单位
*/
C_ALE_DataSet::DATA_UNIT P_ALE_Editor::getCurrentData_Unit(){
	return this->m_dataSet_unit;
}
/*-------------------------------------------------
		块数据 - 获取当前数据 - 帧列表
*/
QList<int> P_ALE_Editor::getCurrentData_IntervalList(){
	QList<int> interval_list;
	for (int i = 0; i < this->m_cellTank.count(); i++){
		I_PiS_Cell* cell_PiS = this->m_cellTank.at(i);
		I_ALE_Cell* cell = dynamic_cast<I_ALE_Cell*>(cell_PiS);
		C_AnimationBlockDataPtr cellData = cell->getBlockDataPtr_ALE();
		interval_list.append(cellData->m_intervalValue);
	}
	return interval_list;
}
/*-------------------------------------------------
		块数据 - 设置数据（不开放）
*/
void P_ALE_Editor::setDataSet_PLE(C_PLE_DataSet data) {
	P_PLE_Editor::setDataSet_PLE(data);
}
/*-------------------------------------------------
		块数据 - 获取数据（不开放）
*/
C_PLE_DataSet P_ALE_Editor::getDataSet_PLE(){
	return P_PLE_Editor::getDataSet_PLE();
}


/*-------------------------------------------------
		块配置 - 设置参数
*/
void P_ALE_Editor::setConfigParam_ALE(C_ALE_Config config){
	this->m_config_ALE = config;
	P_PLE_Editor::setConfigParam(config);
}
/*-------------------------------------------------
		块配置 - 取出参数
*/
C_ALE_Config P_ALE_Editor::getConfigParam_ALE(){
	return this->m_config_ALE;
}
/*-------------------------------------------------
		块配置 - 设置参数（不开放）
*/
void P_ALE_Editor::setConfigParam_PLE(C_PLE_Config config){
	P_PLE_Editor::setConfigParam_PLE(config);
}
/*-------------------------------------------------
		块配置 - 取出参数（不开放）
*/
C_PLE_Config P_ALE_Editor::getConfigParam_PLE(){
	return P_PLE_Editor::getConfigParam_PLE();
}
/*-------------------------------------------------
		块配置 - 设置参数（不开放）
*/
void P_ALE_Editor::setConfigParam(C_PiS_Config config){
	P_PLE_Editor::setConfigParam(config);
}
/*-------------------------------------------------
		块配置 - 取出参数（不开放）
*/
C_PiS_Config P_ALE_Editor::getConfigParam(){
	return P_PLE_Editor::getConfigParam();
}
/*-------------------------------------------------
		块配置 - 窗口编辑ui设置
*/
void P_ALE_Editor::openWindow_setConfigParam_ALE(){

	W_ALE_Config d(this->m_parent);
	d.setDataInModifyMode(this->m_config_ALE);
	if (d.exec() == QDialog::Accepted){
		C_ALE_Config config = d.getData();
		this->setConfigParam_ALE(config);
	}
}



/*-------------------------------------------------
		鼠标事件 - 圈选变化事件（单个、多个）
*/
void P_ALE_Editor::event_itemSelectionChanged(QList<QListWidgetItem*> selected_item_list){
	P_PLE_Editor::event_itemSelectionChanged(selected_item_list);
}
/*-------------------------------------------------
		鼠标事件 - 右键事件（零个、单个、多个）
*/
void P_ALE_Editor::event_itemRightClicked(QList<QListWidgetItem*> item_list){
	P_PLE_Editor::event_itemRightClicked(item_list);
}

/*-------------------------------------------------
		右键菜单 - 零个（覆写）（点击空白处）
*/
QMenu* P_ALE_Editor::createMenu_selectNone(){
	//（完全覆写，父对象的函数不执行）
	QAction* action;
	QMenu* menu = new QMenu();


	// > 零个 - 刷新编号
	action = new QAction("刷新编号", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Refresh.png"));
	action->setEnabled(this->action_isCellTitleNeedRefresh());
	connect(action, &QAction::triggered, this, &P_ALE_Editor::action_refreshCellTitle);
	menu->addAction(action);

	menu->addSeparator();


	// > 零个 - 添加帧
	QMenu* menu_add = new QMenu("添加帧");
	menu_add->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));

		action = new QAction("添加图片", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_append);
		menu_add->addAction(action);

		action = new QAction("添加GIF", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_appendGIF_ALE);
		menu_add->addAction(action);

		action = new QAction("添加序列大图", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_appendSeq_ALE);
		menu_add->addAction(action);

	menu->addMenu(menu_add);


	// > 零个 - 选择帧
	QMenu* menu_select = new QMenu("选择帧", menu);
	menu_select->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_SelectAll.png"));
	menu_select->setEnabled(false);
	menu->addMenu(menu_select);


	// > 零个 - 复制/粘贴/删除
	action = new QAction("复制帧", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Copy.png"));
	action->setShortcut(QKeySequence::Copy);
	action->setEnabled(false);
	menu->addAction(action);

	action = new QAction("粘贴帧", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Paste.png"));
	action->setData(this->m_cellTank.count());	//最末尾的位置
	action->setShortcut(QKeySequence::Paste);
	action->setEnabled(this->action_isEnablePaste_PLE());
	connect(action, &QAction::triggered, this, &P_ALE_Editor::action_paste);
	menu->addAction(action);

	action = new QAction("删除帧", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Delete.png"));
	action->setShortcut(QKeySequence::Delete);
	action->setEnabled(false);
	menu->addAction(action);

	menu->addSeparator();


	// > 零个 - 导出
	QMenu* menu_export = new QMenu("导出", menu);
	menu_export->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Export.png"));
	menu_export->setEnabled(false);
	menu->addMenu(menu_export);

	return menu;
}
/*-------------------------------------------------
		右键菜单 - 单个（覆写）
*/
QMenu* P_ALE_Editor::createMenu_selectSingle(int pos){
	//（完全覆写，父对象的函数不执行）
	if (pos < 0){ return nullptr; }
	QAction* action;
	QMenu* menu = new QMenu();
	QString pos_str = QString::number(pos);


	// > 单个 - 刷新编号
	action = new QAction("刷新编号", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Refresh.png"));
	action->setEnabled(this->action_isCellTitleNeedRefresh());
	connect(action, &QAction::triggered, this, &P_ALE_Editor::action_refreshCellTitle);
	menu->addAction(action);

	// > 单个 - 左移右移
	action = new QAction("左移", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Left.png"));
	action->setData(pos_str);
	connect(action, &QAction::triggered, this, &P_ALE_Editor::action_moveLeft);
	menu->addAction(action);
	if (pos - 1 < 0){ action->setEnabled(false); }

	action = new QAction("右移", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Right.png"));
	action->setData(pos_str);
	connect(action, &QAction::triggered, this, &P_ALE_Editor::action_moveRight);
	menu->addAction(action);
	if (pos + 1 >= this->m_cellTank.count()){ action->setEnabled(false); }

	menu->addSeparator();


	// > 单个 - 添加帧
	QMenu* menu_add = new QMenu("添加帧", menu);
	menu_add->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));

		action = new QAction("添加图片", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos_str);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_insert);
		menu_add->addAction(action);

		action = new QAction("添加GIF", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos_str);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_insertGIF_ALE);
		menu_add->addAction(action);

		action = new QAction("添加序列大图", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos_str);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_insertSeq_ALE);
		menu_add->addAction(action);

	menu->addMenu(menu_add);


	// > 单个 - 选择帧
	QMenu* menu_select = this->createMenu_changeSelection();
	menu->addMenu(menu_select);


	// > 单个 - 复制/粘贴/删除
	action = new QAction("复制帧", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Copy.png"));
	action->setData(pos_str);
	action->setShortcut(QKeySequence::Copy);
	connect(action, &QAction::triggered, this, &P_ALE_Editor::action_copy);
	menu->addAction(action);

	action = new QAction("粘贴帧", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Paste.png"));
	action->setData(pos_str);
	action->setShortcut(QKeySequence::Paste);
	action->setEnabled(this->action_isEnablePaste_PLE());
	connect(action, &QAction::triggered, this, &P_ALE_Editor::action_paste);
	menu->addAction(action);

	action = new QAction("删除帧", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Delete.png"));
	action->setData(pos_str);
	action->setShortcut(QKeySequence::Delete);
	connect(action, &QAction::triggered, this, &P_ALE_Editor::action_remove);
	menu->addAction(action);

	menu->addSeparator();


	// > 单个 - 选中帧
	action = new QAction("编辑选中帧", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Edit.png"));
	action->setData(pos_str);
	connect(action, &QAction::triggered, this, &P_ALE_Editor::action_openWindow_CellDataSingle_ALE);
	menu->addAction(action);

	// > 单个 - 帧设置
	action = new QAction("编辑帧设置", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Edit.png"));
	action->setData(pos_str);
	connect(action, &QAction::triggered, this, &P_ALE_Editor::action_openWindow_CellDataGlobal_ALE);
	menu->addAction(action);

	// > 单个 - UI配置
	action = new QAction("UI配置", this); 
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Setting.png"));
	connect(action, &QAction::triggered, this, &P_ALE_Editor::openWindow_setConfigParam_ALE);
	menu->addAction(action);


	menu->addSeparator();

	// > 单个 - 导出
	QMenu* menu_export = new QMenu("导出", menu);
	menu_export->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Export.png"));

		action = new QAction("导出 - 单图", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos_str);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_exportPic_Single);
		menu_export->addAction(action);

		action = new QAction("导出 - 全部帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos_str);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_exportPic_All);
		menu_export->addAction(action);

		action = new QAction("生成GIF - 单帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos_str);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_exportGIF_Single);
		menu_export->addAction(action);

		action = new QAction("生成GIF - 全部帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos_str);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_exportGIF_All);
		menu_export->addAction(action);

		action = new QAction("生成序列大图 - 单帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos_str);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_exportSeq_Single);
		menu_export->addAction(action);

		action = new QAction("生成序列大图 - 全部帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos_str);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_exportSeq_All);
		menu_export->addAction(action);

	menu->addMenu(menu_export);

	return menu;
}
/*-------------------------------------------------
		右键菜单 - 多个（覆写）
*/
QMenu* P_ALE_Editor::createMenu_selectMultiple(QList<int> pos_list){
	//（完全覆写，父对象的函数不执行）
	if (pos_list.count() <= 0){ return nullptr; }
	QAction* action;
	QMenu* menu = new QMenu();
	QList<QString> pos_list_str = TTool::_QList_IntToQString_(pos_list);
	QString pos_frist_str = QString::number(pos_list.first());


	// > 多个 - 刷新编号
	action = new QAction("刷新编号", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Refresh.png"));
	action->setEnabled(this->action_isCellTitleNeedRefresh());
	connect(action, &QAction::triggered, this, &P_ALE_Editor::action_refreshCellTitle);
	menu->addAction(action);

	menu->addSeparator();

	// > 多个 - 添加帧
	QMenu* menu_add = new QMenu("添加帧", menu);
	menu_add->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));

		action = new QAction("添加图片", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos_frist_str);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_insert);
		menu_add->addAction(action);

		action = new QAction("添加GIF", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos_frist_str);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_insertGIF_ALE);
		menu_add->addAction(action);

		action = new QAction("添加序列大图", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos_frist_str);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_insertSeq_ALE);
		menu_add->addAction(action);

	menu->addMenu(menu_add);


	// > 多个 - 选择帧
	QMenu* menu_select = this->createMenu_changeSelection();
	menu->addMenu(menu_select);


	// > 多个 - 复制/粘贴/删除
	action = new QAction("复制帧", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Copy.png"));
	action->setData(pos_list_str.join(","));
	action->setShortcut(QKeySequence::Copy);
	connect(action, &QAction::triggered, this, &P_ALE_Editor::action_copy);
	menu->addAction(action);

	action = new QAction("粘贴帧", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Paste.png"));
	action->setData(pos_frist_str);
	action->setShortcut(QKeySequence::Paste);
	action->setEnabled(this->action_isEnablePaste_PLE());
	connect(action, &QAction::triggered, this, &P_ALE_Editor::action_paste);
	menu->addAction(action);

	action = new QAction("删除帧", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Delete.png"));
	action->setData(pos_list_str.join(","));
	action->setShortcut(QKeySequence::Delete);
	connect(action, &QAction::triggered, this, &P_ALE_Editor::action_remove);
	menu->addAction(action);

	menu->addSeparator();


	// > 多个 - 选中帧
	action = new QAction("编辑选中帧(多个)", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Edit.png"));
	action->setData(pos_list_str.join(","));
	connect(action, &QAction::triggered, this, &P_ALE_Editor::action_openWindow_CellDataMultiple_ALE);
	menu->addAction(action);

	// > 多个 - 帧设置
	action = new QAction("编辑帧设置", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Edit.png"));
	action->setData(pos_list_str.join(","));
	connect(action, &QAction::triggered, this, &P_ALE_Editor::action_openWindow_CellDataGlobal_ALE);
	menu->addAction(action);

	// > 多个 - UI配置
	action = new QAction("UI配置", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Setting.png"));
	connect(action, &QAction::triggered, this, &P_ALE_Editor::openWindow_setConfigParam_ALE);
	menu->addAction(action);


	menu->addSeparator();

	// > 多个 - 导出
	QMenu* menu_export = new QMenu("导出", menu);
	menu_export->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Export.png"));

		action = new QAction("导出 - 选中帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos_list_str.join(","));
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_exportPic_Multiple);
		menu_export->addAction(action);

		action = new QAction("导出 - 全部帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos_list_str.join(","));
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_exportPic_All);
		menu_export->addAction(action);

		action = new QAction("生成GIF - 选中帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos_list_str.join(","));
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_exportGIF_Multiple);
		menu_export->addAction(action);

		action = new QAction("生成GIF - 全部帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos_list_str.join(","));
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_exportGIF_All);
		menu_export->addAction(action);

		action = new QAction("生成序列大图 - 选中帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos_list_str.join(","));
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_exportSeq_Multiple);
		menu_export->addAction(action);

		action = new QAction("生成序列大图 - 全部帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos_list_str.join(","));
		connect(action, &QAction::triggered, this, &P_ALE_Editor::action_exportSeq_All);
		menu_export->addAction(action);

	menu->addMenu(menu_export);

	return menu;
}
/*-------------------------------------------------
		右键菜单 - action - 编辑数据-单选
*/
void P_ALE_Editor::action_openWindow_CellDataSingle_ALE(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString pos_str = cur_action->data().value<QString>();
	int index = pos_str.toInt();
	I_PiS_Cell* cell_PiS = this->m_cellTank.at(index);
	I_ALE_Cell* cell = dynamic_cast<I_ALE_Cell*>(cell_PiS);

	W_ALE_CellDataSingle d(this->m_parent);
	d.setDataInModifyMode(cell->getBlockDataPtr_ALE(), this->m_dataSet_unit, this->m_dataSet_parentDir);
	if (d.exec() == QDialog::Accepted){
		//（直接修改指针数据）
		this->refreshPictureCell(index);
	}
}
/*-------------------------------------------------
		右键菜单 - action - 编辑数据-多选
*/
void P_ALE_Editor::action_openWindow_CellDataMultiple_ALE(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString pos_list_str = cur_action->data().value<QString>();
	QList<int> index_list = TTool::_QList_QStringToInt_(pos_list_str.split(","));
	QList<C_AnimationBlockDataPtr> data_list;
	for (int i = 0; i < index_list.count(); i++){
		I_PiS_Cell* cell_PiS = this->m_cellTank.at(index_list.at(i));
		I_ALE_Cell* cell = dynamic_cast<I_ALE_Cell*>(cell_PiS);
		data_list.append(cell->getBlockDataPtr_ALE());
	}

	W_ALE_CellDataMultiple d(this->m_parent);
	d.setDataInModifyMode(data_list, this->m_dataSet_unit, this->m_dataSet_parentDir, this->m_dataSet_defaultInterval);
	if (d.exec() == QDialog::Accepted){
		//（直接修改指针数据）
		for (int i = 0; i < index_list.count(); i++){
			this->refreshPictureCell(index_list.at(i));
		}
	}
}
/*-------------------------------------------------
		右键菜单 - action - 编辑数据-全局
*/
void P_ALE_Editor::action_openWindow_CellDataGlobal_ALE(){

	W_ALE_CellDataGlobal d(this->m_parent);
	d.setDataInModifyMode(this->m_dataSet_defaultInterval, this->m_dataSet_unit);
	if (d.exec() == QDialog::Accepted){
		this->setIntervalDefaultInAll(d.getData_DefaultInterval());
		this->m_dataSet_unit = d.getData_Unit();
		this->refreshUI();
	}
}
/*-------------------------------------------------
		右键菜单 - 操作 - 修改默认帧间隔
*/
void P_ALE_Editor::setIntervalDefaultInAll(int gif_interval){
	if (gif_interval == 0){ return; }		//（不允许赋值零）
	int old_interval = this->m_dataSet_defaultInterval;
	this->m_dataSet_defaultInterval = gif_interval;
	for (int i = 0; i < this->m_cellTank.count(); i++){	//（替换掉所有默认的旧帧间隔）
		I_PiS_Cell* cell_PiS = this->m_cellTank.at(i);
		I_ALE_Cell* cell = dynamic_cast<I_ALE_Cell*>(cell_PiS);
		C_AnimationBlockDataPtr dataPtr = cell->getBlockDataPtr_ALE();
		if (dataPtr->m_intervalValue == old_interval){
			dataPtr->m_intervalValue = gif_interval;
		}
	}
}


/*-------------------------------------------------
		导入图片 - 复制文件到文件夹
*/
QStringList P_ALE_Editor::importPic_addToParentDir(QStringList file_list){
	//P_PLE_Editor::importPic_addToParentDir(file_list);	（覆写）

	QStringList parentDirFile_list;
	for (int i = 0; i < file_list.count(); i++){
		QString cur_filePath = file_list.at(i);

		// > 不存在的文件，跳过
		if (QFileInfo(cur_filePath).exists() == false){
			continue;
		}

		// > 未添加成功的文件，跳过
		QString resultPath = S_ALE_FileManager::getInstance()->addPicFile_ToDir(cur_filePath, this->m_dataSet_parentDir);
		if (resultPath == ""){
			continue;
		}
		parentDirFile_list.append(resultPath);
	}
	
	// > 发出信号
	emit signal_picListChanged();

	return parentDirFile_list;
};



/*-------------------------------------------------
		基函数（复制/粘贴） - 复制（覆写）
*/
void P_ALE_Editor::base_copy(QList<int> index_list){
	//P_PLE_Editor::base_copy(index_list);	（覆写）
	if (index_list.count() == 0){ return; }

	// > 取出数据
	QList<C_AnimationBlockDataPtr> data_list;
	for (int i = 0; i < index_list.count(); i++){
		int index = index_list.at(i);
		I_PiS_Cell* cell_PiS = this->m_cellTank.at(index);
		I_ALE_Cell* cell = dynamic_cast<I_ALE_Cell*>(cell_PiS);
		data_list.append(cell->getBlockDataPtr_ALE());
	}

	// > 生成JSON字节流
	QJsonObject data_all;
	QJsonArray data_array;
	for (int i = 0; i < data_list.count(); i++){
		C_PictureBlockDataPtr dataPtr = data_list.at(i);
		data_array.append(dataPtr->getJsonObject());
	}
	data_all.insert("data_list", data_array);
	data_all.insert("parentDir", this->m_dataSet_parentDir);
	QJsonDocument document;
	document.setObject(data_all);

	// > 剪贴板 - 清除
	QClipboard* clipboard = QApplication::clipboard();
	clipboard->clear();

	// > 剪贴板 - 赋值
	QMimeData* mimeData = new QMimeData();
	mimeData->setData("application/drill-P_ALE_Editor", document.toJson());
	clipboard->setMimeData(mimeData);
}
/*-------------------------------------------------
		基函数（复制/粘贴） - 粘贴（覆写）
*/
void P_ALE_Editor::base_paste(int index){
	//P_PLE_Editor::base_paste(index);	（覆写）

	// > 剪贴板 - 获取
	QClipboard* clipboard = QApplication::clipboard();
	const QMimeData* mimeData = clipboard->mimeData();
	QByteArray data = mimeData->data("application/drill-P_ALE_Editor");
	if (data.isEmpty()){
		data = mimeData->data("application/drill-P_PLE_Editor");	//（兼容PLE）
		if (data.isEmpty()){
			return;
		}
	}

	// > 转换JSON字节流
	QJsonDocument jsonDocument = QJsonDocument::fromJson(data);
	QJsonObject data_all = jsonDocument.object();

	// > 建立元胞数据
	int item_height = this->m_config.getSize_ItemHeight();
	int cell_count = this->m_cellTank.count();
	QList<C_PictureBlockDataPtr> cellData_list;
	QJsonArray data_array = data_all.value("data_list").toArray();
	for (int i = 0; i < data_array.count(); i++){
		QJsonObject obj = data_array.at(i).toObject();
		C_PictureBlockDataPtr dataPtr = this->createPictureCellData();
		dataPtr->setJsonObject(obj);

		// > 画布与粘贴的位置一直
		dataPtr->m_width = item_height;
		dataPtr->m_height = item_height;

		// > 重新编号
		dataPtr->m_titleNum = cell_count + i;

		cellData_list.append(dataPtr);
	}

	// > 检查父路径
	QString parentDir = data_all.value("parentDir").toString();
	if (parentDir != this->m_dataSet_parentDir){
		//...
	}
	
	if (index == cell_count - 1){	//（如果选择的最后一个，则在末尾添加）
		this->base_append(cellData_list);
	}else{
		this->base_insert(index, cellData_list);
	}
}
/*-------------------------------------------------
		action - 是否可以粘贴（不开放）
*/
bool P_ALE_Editor::action_isEnablePaste_PLE(){
	//P_PLE_Editor::action_isEnablePaste_PLE();	（覆写）
	return this->action_isEnablePaste_ALE();
}
/*-------------------------------------------------
		action - 是否可以粘贴
*/
bool P_ALE_Editor::action_isEnablePaste_ALE(){
	QClipboard* clipboard = QApplication::clipboard();
	const QMimeData* mimeData = clipboard->mimeData();
	QByteArray data = mimeData->data("application/drill-P_ALE_Editor");
	if (data.isEmpty()){
		data = mimeData->data("application/drill-P_PLE_Editor");	//（兼容PLE）
		if (data.isEmpty()){
			return false;
		}
	}
	return true;
}



/*-------------------------------------------------
		action - 添加GIF（append）
*/
void P_ALE_Editor::action_appendGIF_ALE(){
	
	// > 选择导入方式
	int readerMethod = 0;
	W_ALE_GIFReader_TypeChoose d(this->m_parent);
	if (d.exec() == QDialog::Accepted){
		readerMethod = d.getData();
	}else{
		return;
	}

	// > 导入GIF - 选择文件
	QString file_gif = this->importGIF_openQFileDialog();
	if (file_gif == ""){ return; }

	// > 导入GIF - 复制文件到文件夹
	//		（GIF包含了帧间隔和图片，所以需要分别获取）
	bool success = S_ALE_FileManager::getInstance()->addGIFFile_ToDir(file_gif, readerMethod, this->m_dataSet_parentDir);
	if ( success == false){ return; }
	QList<QFileInfo> file_list = S_ALE_FileManager::getInstance()->getGIF_LastFileList();
	QList<int> interval_list = S_ALE_FileManager::getInstance()->getGIF_LastIntervalList();

	// > 导入GIF - 复制文件到文件夹（发出信号）
	emit signal_picListChanged();

	// > 导入GIF - 文件 转 图片单块
	QList<C_PictureBlockDataPtr> cellData_list = this->importGIF_getCellListByFileList(file_list, interval_list);

	this->base_append(cellData_list);
}
/*-------------------------------------------------
		action - 添加GIF（insert）
*/
void P_ALE_Editor::action_insertGIF_ALE(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString pos_str = cur_action->data().value<QString>();
	int pos = pos_str.toInt();
	
	// > 选择导入方式
	int readerMethod = 0;
	W_ALE_GIFReader_TypeChoose d(this->m_parent);
	if (d.exec() == QDialog::Accepted){
		readerMethod = d.getData();
	}else{
		return;
	}

	// > 导入GIF - 选择文件
	QString file_gif = this->importGIF_openQFileDialog();
	if (file_gif == ""){ return; }

	// > 导入GIF - 复制文件到文件夹
	//		（GIF包含了帧间隔和图片，所以需要分别获取）
	bool success = S_ALE_FileManager::getInstance()->addGIFFile_ToDir(file_gif, readerMethod, this->m_dataSet_parentDir);
	if (success == false){ return; }
	QList<QFileInfo> file_list = S_ALE_FileManager::getInstance()->getGIF_LastFileList();
	QList<int> interval_list = S_ALE_FileManager::getInstance()->getGIF_LastIntervalList();

	// > 导入GIF - 复制文件到文件夹（发出信号）
	emit signal_picListChanged();

	// > 导入GIF - 文件 转 图片单块
	QList<C_PictureBlockDataPtr> cellData_list = this->importGIF_getCellListByFileList(file_list, interval_list);
	
	if (pos == this->m_cellTank.count() - 1){	//（如果选择的最后一个，则在末尾添加）
		this->base_append(cellData_list);
	}else{
		this->base_insert(pos, cellData_list);
	}
}
/*-------------------------------------------------
		导入GIF - 选择文件
*/
QString P_ALE_Editor::importGIF_openQFileDialog(){
	QString result = "";
	QFileDialog fd;
	fd.setWindowTitle("导入GIF");
	fd.setAcceptMode(QFileDialog::AcceptOpen);
	fd.setNameFilters(QStringList() << "动图(*.gif)");
	fd.setViewMode(QFileDialog::Detail);
	fd.setFileMode(QFileDialog::ExistingFile);		//单个文件
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()){ return ""; }
		result = fd.selectedFiles().at(0);
		return result;
	}else {
		return "";
	}
}
/*-------------------------------------------------
		导入GIF - 文件 转 图片单块
*/
QList<C_PictureBlockDataPtr> P_ALE_Editor::importGIF_getCellListByFileList(QList<QFileInfo> file_list, QList<int> interval_list){
	int item_height = this->m_config.getSize_ItemHeight();
	int cell_count = this->m_cellTank.count();
	QList<C_PictureBlockDataPtr> cellData_list;
	for (int i = 0; i < file_list.count(); i++){
		QFileInfo file_info = file_list.at(i);	//文件
		int interval = interval_list.at(i);		//帧数

		C_PictureBlockDataPtr cellData_pic = this->createPictureCellData();
		C_AnimationBlockDataPtr cellData = cellData_pic.dynamicCast<C_AnimationBlockData>();
		cellData->init(
			item_height,
			item_height,
			cell_count + i,
			file_info.absoluteFilePath()
			);
		cellData->m_intervalValue = interval;	//帧数赋值
		cellData_list.append(cellData);
	}
	return cellData_list;
}

/*-------------------------------------------------
		action - 导出GIF（单张）
*/
void P_ALE_Editor::action_exportGIF_Single(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString pos_str = cur_action->data().value<QString>();
	int pos = pos_str.toInt();
	
	// > 单帧提示
	if (QMessageBox::question(nullptr, "提示", "你只选择了一帧，确定要导出GIF吗？", "执行导出", "取消", 0) == 1) {
		return;
	}

	// > 选择导出方式
	int writerMethod = 0;
	W_ALE_GIFWriter_TypeChoose d(this->m_parent);
	if (d.exec() == QDialog::Accepted){
		writerMethod = d.getData();
	}else{
		return;
	}

	// > 导出GIF - 选择文件
	QString exportName = "动图";
	if (this->m_customExportName != ""){ exportName = this->m_customExportName + "_动图"; }
	QString tar_file = this->exportGIF_openQFileDialog_File(exportName);
	if (tar_file == ""){ return; }

	// > 导出GIF - 执行导出
	I_PiS_Cell* cell_PiS = this->m_cellTank.at(pos);
	I_ALE_Cell* cell = dynamic_cast<I_ALE_Cell*>(cell_PiS);
	QList<C_AnimationBlockDataPtr> cellData_list;
	cellData_list.append(cell->getBlockDataPtr_ALE());
	this->exportGIF_exportToFile(QFileInfo(tar_file), writerMethod, cellData_list);
}
/*-------------------------------------------------
		action - 导出GIF（多张）
*/
void P_ALE_Editor::action_exportGIF_Multiple(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString pos_list_str = cur_action->data().value<QString>();
	QList<int> pos_list = TTool::_QList_QStringToInt_(pos_list_str.split(","));

	// > 单帧提示
	if (pos_list.count() == 1){
		if (QMessageBox::question(nullptr, "提示", "你只选择了一帧，确定要导出GIF吗？", "执行导出", "取消", 0) == 1) {
			return;
		}
	}

	// > 选择导出方式
	int writerMethod = 0;
	W_ALE_GIFWriter_TypeChoose d(this->m_parent);
	if (d.exec() == QDialog::Accepted){
		writerMethod = d.getData();
	}else{
		return;
	}

	// > 导出GIF - 选择文件
	QString exportName = "动图";
	if (this->m_customExportName != ""){ exportName = this->m_customExportName + "_动图"; }
	QString tar_file = this->exportGIF_openQFileDialog_File(exportName);
	if (tar_file == ""){ return; }

	// > 导出GIF - 执行导出
	QList<C_AnimationBlockDataPtr> cellData_list;
	for (int i = 0; i < pos_list.count(); i++){
		int pos = pos_list.at(i);
		I_PiS_Cell* cell_PiS = this->m_cellTank.at(i);
		I_ALE_Cell* cell = dynamic_cast<I_ALE_Cell*>(cell_PiS);
		cellData_list.append(cell->getBlockDataPtr_ALE());
	}
	this->exportGIF_exportToFile(QFileInfo(tar_file), writerMethod, cellData_list);
}
/*-------------------------------------------------
		action - 导出GIF（全部）
*/
void P_ALE_Editor::action_exportGIF_All(){
	QList<int> pos_list;
	for (int i = 0; i < this->m_cellTank.count(); i++){ pos_list.append(i); }
	if (pos_list.count() == 0){
		QMessageBox::warning(this->m_parent, "提示", "需要至少一帧才能生成GIF。", QMessageBox::Yes);
		return;
	}

	// > 单帧提示
	if (pos_list.count() == 1){
		if (QMessageBox::question(nullptr, "提示", "你只选择了一帧，确定要导出GIF吗？", "执行导出", "取消", 0) == 1) {
			return;
		}
	}

	// > 选择导出方式
	int writerMethod = 0;
	W_ALE_GIFWriter_TypeChoose d(this->m_parent);
	if (d.exec() == QDialog::Accepted){
		writerMethod = d.getData();
	}else{
		return;
	}

	// > 导出GIF - 选择文件
	QString exportName = "动图";
	if (this->m_customExportName != ""){ exportName = this->m_customExportName + "_动图"; }
	QString tar_file = this->exportGIF_openQFileDialog_File(exportName);
	if (tar_file == ""){ return; }

	// > 导出GIF - 执行导出
	QList<C_AnimationBlockDataPtr> cellData_list;
	for (int i = 0; i < pos_list.count(); i++){
		int pos = pos_list.at(i);
		I_PiS_Cell* cell_PiS = this->m_cellTank.at(i);
		I_ALE_Cell* cell = dynamic_cast<I_ALE_Cell*>(cell_PiS);
		cellData_list.append(cell->getBlockDataPtr_ALE());
	}
	this->exportGIF_exportToFile(QFileInfo(tar_file), writerMethod, cellData_list);
}
/*-------------------------------------------------
		导出GIF - 选择文件
*/
QString P_ALE_Editor::exportGIF_openQFileDialog_File(QString defaultFile_name){
	QString target_file = "";
	QFileDialog fd;
	fd.setWindowTitle("导出GIF");
	fd.setAcceptMode(QFileDialog::AcceptSave);
	fd.setDirectory(".");
	fd.setNameFilters(QStringList() << "动图(*.gif)");
	fd.setViewMode(QFileDialog::Detail);
	fd.setFileMode(QFileDialog::ExistingFile);		//单个文件
	fd.selectFile(defaultFile_name + ".gif");		//默认导出的文件名
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()){ return ""; }
		target_file = fd.selectedFiles().at(0);
	}else {
		return "";
	}
	return target_file;
}
/*-------------------------------------------------
		导出GIF - 执行导出
*/
void P_ALE_Editor::exportGIF_exportToFile(QFileInfo tar_file, int writerMethod, QList<C_AnimationBlockDataPtr> cellData_list){
	double interval_default = this->m_dataSet_defaultInterval;
	QList<QFileInfo> fileInfo_list;
	QList<double> interval_list;
	for (int i = 0; i < cellData_list.count(); i++){
		C_AnimationBlockDataPtr cellData = cellData_list.at(i);
		fileInfo_list.append(QFileInfo(cellData->m_bitmapPath));
		interval_list.append(cellData->m_intervalValue);
	}

	// > gif.h生成
	if (writerMethod == 0){
		
		S_GIFManager::getInstance()->generateGIF(
			fileInfo_list,
			tar_file.absoluteFilePath(),
			interval_default,
			TTool::_QList_DoubleToInt_floor_(interval_list)
		);

	// > cximage生成
	}else{

		S_cximageManager::getInstance()->generateGIF(
			fileInfo_list,
			tar_file.absoluteFilePath(),
			interval_default,
			TTool::_QList_DoubleToInt_floor_(interval_list)
		);
	}

	QMessageBox::information(this->m_parent, "提示", "GIF导出成功。", QMessageBox::Yes);
}



/*-------------------------------------------------
		action - 添加序列大图（append）
*/
void P_ALE_Editor::action_appendSeq_ALE(){

	// > 导入序列大图 - 选择文件
	QString file_seq = this->importSeq_openQFileDialog();
	if (file_seq == ""){ return; }

	// > 图片分割器窗口
	QList<QPixmap> bitmap_list;
	W_PicturePartitioner d(this->m_parent);
	d.setData(QFileInfo(file_seq));
	if (d.exec() == QDialog::Accepted){
		bitmap_list = d.getData();
	}else{
		return;
	}

	// > 导入序列大图 - 复制文件到文件夹
	QStringList file_list = S_ALE_FileManager::getInstance()->addSeqFile_ToDir(file_seq, bitmap_list, this->m_dataSet_parentDir);
	if (file_list.count() == 0){ return; }

	// > 导入序列大图 - 复制文件到文件夹（发出信号）
	emit signal_picListChanged();

	// > 导入序列大图 - 文件 转 图片单块
	QList<C_PictureBlockDataPtr> cellData_list = this->importSeq_getCellListByFileList(file_list);

	this->base_append(cellData_list);
}
/*-------------------------------------------------
		action - 添加序列大图（insert）
*/
void P_ALE_Editor::action_insertSeq_ALE(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString pos_str = cur_action->data().value<QString>();
	int pos = pos_str.toInt();

	// > 导入序列大图 - 选择文件
	QString file_seq = this->importSeq_openQFileDialog();
	if (file_seq == ""){ return; }
	
	// > 图片分割器窗口
	QList<QPixmap> bitmap_list;
	W_PicturePartitioner d(this->m_parent);
	d.setData(QFileInfo(file_seq));
	if (d.exec() == QDialog::Accepted){
		bitmap_list = d.getData();
	}else{
		return;
	}

	// > 导入序列大图 - 复制文件到文件夹
	QStringList file_list = S_ALE_FileManager::getInstance()->addSeqFile_ToDir(file_seq, bitmap_list, this->m_dataSet_parentDir);
	if (file_list.count() == 0){ return; }

	// > 导入序列大图 - 复制文件到文件夹（发出信号）
	emit signal_picListChanged();

	// > 导入序列大图 - 文件 转 图片单块
	QList<C_PictureBlockDataPtr> cellData_list = this->importSeq_getCellListByFileList(file_list);

	if (pos == this->m_cellTank.count() - 1){	//（如果选择的最后一个，则在末尾添加）
		this->base_append(cellData_list);
	}else{
		this->base_insert(pos, cellData_list);
	}
}
/*-------------------------------------------------
		导入序列大图 - 选择文件
*/
QString P_ALE_Editor::importSeq_openQFileDialog(){
	QString result = "";
	QFileDialog fd;
	fd.setWindowTitle("导入序列大图");
	fd.setAcceptMode(QFileDialog::AcceptOpen);	
	fd.setNameFilters(QStringList() << "序列大图(*.png)");
	fd.setViewMode(QFileDialog::Detail);
	fd.setFileMode(QFileDialog::ExistingFile);		//单个文件
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()){ return ""; }
		result = fd.selectedFiles().at(0);
		return result;
	}else {
		return "";
	}
}
/*-------------------------------------------------
		导入序列大图 - 文件 转 图片单块
*/
QList<C_PictureBlockDataPtr> P_ALE_Editor::importSeq_getCellListByFileList(QStringList file_list){
	return this->importPic_getCellListByFileList(file_list);	//（与 导入图片 的一样）
}

/*-------------------------------------------------
		action - 导出序列大图（单张）
*/
void P_ALE_Editor::action_exportSeq_Single(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString pos_str = cur_action->data().value<QString>();
	int pos = pos_str.toInt();

	// > 单帧提示
	if (QMessageBox::question(nullptr, "提示", "你只选择了一帧，确定要导出序列大图吗？", "执行导出", "取消", 0) == 1) {
		//（取消单帧导出）
		return;
	}else{
		//（单帧导出）

		// > 导出序列大图 - 选择文件
		QString exportName = "序列大图";
		if (this->m_customExportName != ""){ exportName = this->m_customExportName + "_序列大图"; }
		QString tar_file = this->exportSeq_openQFileDialog_File(exportName);
		if (tar_file == ""){ return; }

		// > 获取 图片单块 的文件路径
		I_PiS_Cell* cell = this->m_cellTank.at(pos);
		QString from_file = cell->getBlockDataPtr()->m_bitmapPath;

		// > 导出文件
		P_FileOperater op;
		op.copy_File(QFileInfo(from_file), QFileInfo(tar_file));
		QMessageBox::information(this->m_parent, "提示", "序列大图导出成功。", QMessageBox::Yes);
		return;
	}
}
/*-------------------------------------------------
		action - 导出序列大图（多张）
*/
void P_ALE_Editor::action_exportSeq_Multiple(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString pos_list_str = cur_action->data().value<QString>();
	QList<int> pos_list = TTool::_QList_QStringToInt_(pos_list_str.split(","));

	// > 单帧提示
	if (pos_list.count() == 1){
		if (QMessageBox::question(nullptr, "提示", "你只选择了一帧，确定要导出序列大图吗？", "执行导出", "取消", 0) == 1) {
			//（取消单帧导出）
			return;
		}else{
			//（单帧导出）

			// > 导出序列大图 - 选择文件
			QString exportName = "序列大图";
			if (this->m_customExportName != ""){ exportName = this->m_customExportName + "_序列大图"; }
			QString tar_file = this->exportSeq_openQFileDialog_File(exportName);
			if (tar_file == ""){ return; }

			// > 获取 图片单块 的文件路径
			I_PiS_Cell* cell = this->m_cellTank.at(pos_list.first());
			QString from_file = cell->getBlockDataPtr()->m_bitmapPath;

			// > 导出文件
			P_FileOperater op;
			op.copy_File(QFileInfo(from_file), QFileInfo(tar_file));
			QMessageBox::information(this->m_parent, "提示", "序列大图导出成功。", QMessageBox::Yes);
			return;
		}
	}
	
	// > 图片合成器 - 准备内容
	QPixmap tar_bitmap;
	QList<QFileInfo> fileInfo_list;
	for (int i = 0; i < pos_list.count(); i++){
		int pos = pos_list.at(i);
		I_PiS_Cell* cell = this->m_cellTank.at(i);
		fileInfo_list.append(QFileInfo(cell->getBlockDataPtr()->m_bitmapPath));
	}

	// > 图片合成器 - 打开
	W_PictureCombiner d(this->m_parent);
	d.setData(fileInfo_list);
	if (d.exec() == QDialog::Accepted){
		tar_bitmap = d.getData();
	}else{
		return;
	}

	// > 导出序列大图 - 选择文件
	QString exportName = "序列大图";
	if (this->m_customExportName != ""){ exportName = this->m_customExportName + "_序列大图"; }
	QString tar_file = this->exportSeq_openQFileDialog_File(exportName);
	if (tar_file == ""){ return; }

	// > 导出文件
	tar_bitmap.toImage().save(tar_file, "png");
}
/*-------------------------------------------------
		action - 导出序列大图（全部）
*/
void P_ALE_Editor::action_exportSeq_All(){
	QList<int> pos_list;
	for (int i = 0; i < this->m_cellTank.count(); i++){ pos_list.append(i); }
	if (pos_list.count() == 0){
		QMessageBox::warning(this->m_parent, "提示", "需要至少一帧才能生成序列大图。", QMessageBox::Yes);
		return;
	}

	// > 单帧提示
	if (pos_list.count() == 1){
		if (QMessageBox::question(nullptr, "提示", "你只选择了一帧，确定要导出序列大图吗？", "执行导出", "取消", 0) == 1) {
			//（取消单帧导出）
			return;
		}else{
			//（单帧导出）

			// > 导出序列大图 - 选择文件
			QString exportName = "序列大图";
			if (this->m_customExportName != ""){ exportName = this->m_customExportName + "_序列大图"; }
			QString tar_file = this->exportSeq_openQFileDialog_File(exportName);
			if (tar_file == ""){ return; }

			// > 获取 图片单块 的文件路径
			I_PiS_Cell* cell = this->m_cellTank.at(pos_list.first());
			QString from_file = cell->getBlockDataPtr()->m_bitmapPath;

			// > 导出文件
			P_FileOperater op;
			op.copy_File(QFileInfo(from_file), QFileInfo(tar_file));
			QMessageBox::information(this->m_parent, "提示", "序列大图导出成功。", QMessageBox::Yes);
			return;
		}
	}

	// > 图片合成器 - 准备内容
	QPixmap tar_bitmap;
	QList<QFileInfo> fileInfo_list;
	for (int i = 0; i < pos_list.count(); i++){
		int pos = pos_list.at(i);
		I_PiS_Cell* cell = this->m_cellTank.at(i);
		fileInfo_list.append(QFileInfo(cell->getBlockDataPtr()->m_bitmapPath));
	}

	// > 图片合成器 - 打开
	W_PictureCombiner d(this->m_parent);
	d.setData(fileInfo_list);
	if (d.exec() == QDialog::Accepted){
		tar_bitmap = d.getData();
	}else{
		return;
	}

	// > 导出序列大图 - 选择文件
	QString exportName = "序列大图";
	if (this->m_customExportName != ""){ exportName = this->m_customExportName + "_序列大图"; }
	QString tar_file = this->exportSeq_openQFileDialog_File(exportName);
	if (tar_file == ""){ return; }

	// > 导出文件
	tar_bitmap.toImage().save(tar_file, "png");
}
/*-------------------------------------------------
		导出序列大图 - 选择文件
*/
QString P_ALE_Editor::exportSeq_openQFileDialog_File(QString defaultFile_name){
	QString target_file = "";
	QFileDialog fd;
	fd.setWindowTitle("导出序列大图");
	fd.setAcceptMode(QFileDialog::AcceptSave);
	fd.setDirectory(".");
	fd.setNameFilters(QStringList() << "序列大图(*.png)");
	fd.setViewMode(QFileDialog::Detail);
	fd.setFileMode(QFileDialog::ExistingFile);		//单个文件
	fd.selectFile(defaultFile_name + ".png");		//默认导出的文件名
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()){ return ""; }
		target_file = fd.selectedFiles().at(0);
	}else {
		return "";
	}
	return target_file;
}