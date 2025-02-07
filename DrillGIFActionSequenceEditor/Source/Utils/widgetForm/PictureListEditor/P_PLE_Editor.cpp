#include "stdafx.h"
#include "P_PLE_Editor.h"

#include "Private/S_PLE_FileManager.h"
#include "Source/Utils/WidgetForm/PictureBitmapCache/S_PictureBitmapCache.h"
#include "Source/Utils/WidgetForm/PictureSelector/P_PiS_Selector.h"

#include "Source/Utils/Common/P_FileOperater.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		图片编辑块.cpp
		版本：		v1.10
		作者：		drill_up
		所属模块：	工具模块
		功能：		将图片全部显示，能多选、编辑图片。（PLE全称：Picture_List_Editor）
					【底层基于QListWidget的item和widget，与QGraphics无关】
					【编辑块固定为 多选 】

		子功能：	-> 文本选择
						-> 多选
						-> 自动编号
					
		使用方法：
				> 初始化
					
-----==========================================================-----
*/
P_PLE_Editor::P_PLE_Editor(QListWidget *parent)
	: P_PiS_Selector(parent)
{

	//-----------------------------------
	//----参数初始化

	// > 数据
	this->setConfigParam(C_PLE_Config());

}
P_PLE_Editor::~P_PLE_Editor(){
}


/*-------------------------------------------------
		工厂 - 建立一个元胞数据（覆写）
*/
C_PictureBlockDataPtr P_PLE_Editor::createPictureCellData(){
	return P_PiS_Selector::createPictureCellData();		//（不改变数据）
}
/*-------------------------------------------------
		工厂 - 建立一个元胞（覆写）
*/
I_PiS_Cell* P_PLE_Editor::createPictureCell(C_PictureBlockDataPtr data){

	// > 控件 - 建立图片块
	P_PictureBlock* widget = new P_PictureBlock(this->m_parent);
	widget->setDataPtr(data);

	// > 项 - 建立项
	QListWidgetItem* item = new QListWidgetItem(this->m_parent);


	// > 元胞
	I_PiS_Cell* cell = new I_PiS_Cell();
	cell->item = item;
	cell->widget = widget;

	return cell;
}
/*-------------------------------------------------
		工厂 - 刷新元胞（覆写）
*/
void P_PLE_Editor::refreshPictureCell(int index){
	P_PiS_Selector::refreshPictureCell(index);
}


/*-------------------------------------------------
		控件 - 重建UI
*/
void P_PLE_Editor::rebuildUI(QStringList bitmapPath_list){
	P_PiS_Selector::rebuildUI(bitmapPath_list);
	//（暂无）
}
/*-------------------------------------------------
		控件 - 刷新UI
*/
void P_PLE_Editor::refreshUI(){
	P_PiS_Selector::refreshUI();
	//（暂无）
}
/*-------------------------------------------------
		控件 - 清理控件
*/
void P_PLE_Editor::clearCells(){
	P_PiS_Selector::clearCells();
	//（暂无）
}
/*-------------------------------------------------
		控件 - 清理全部
*/
void P_PLE_Editor::clearAll(){
	P_PiS_Selector::clearAll();
	//（暂无）
}



/*-------------------------------------------------
		块数据 - 设置数据
*/
void P_PLE_Editor::setDataSet_PLE(C_PLE_DataSet data) {

	// > 父路径（全局数据）
	this->m_dataSet_parentDir = data.getData_ParentDir();

	// > 资源文件
	QStringList pic_list = data.getData_PicList();
	this->rebuildUI(pic_list);	//（重建UI）

	// > 数据重置信号
	emit signal_dataSetReloaded();
}
/*-------------------------------------------------
		块数据 - 获取数据
*/
C_PLE_DataSet P_PLE_Editor::getDataSet_PLE(){
	if (this->m_cellTank.count() == 0){ return C_PLE_DataSet(); }

	QStringList pic_list;
	for (int i = 0; i < this->m_cellTank.count(); i++){
		I_PiS_Cell* cell = this->m_cellTank.at(i);
		C_PictureBlockDataPtr cellData = cell->getBlockDataPtr();
		pic_list.append(cellData->m_bitmapPath);
	}

	C_PLE_DataSet dataSet;
	dataSet.setData_PicList(pic_list);

	dataSet.setData_ParentDir(this->m_dataSet_parentDir);
	return dataSet;
}
/*-------------------------------------------------
		块数据 - 获取当前数据 - 图片列表
*/
QStringList P_PLE_Editor::getCurrentData_PicList(){
	QStringList result_list;
	for (int i = 0; i < this->m_cellTank.count(); i++){
		I_PiS_Cell* cell = this->m_cellTank.at(i);
		result_list.append(cell->getBlockDataPtr()->m_bitmapPath);
	}
	return result_list;
}


/*-------------------------------------------------
		块配置 - 设置参数
*/
void P_PLE_Editor::setConfigParam_PLE(C_PLE_Config config){
	this->m_config_PLE = config;
	P_PiS_Selector::setConfigParam(config);
}
/*-------------------------------------------------
		块配置 - 取出参数
*/
C_PLE_Config P_PLE_Editor::getConfigParam_PLE(){
	return this->m_config_PLE;
}
/*-------------------------------------------------
		块配置 - 设置参数（不开放）
*/
void P_PLE_Editor::setConfigParam(C_PiS_Config config){
	P_PiS_Selector::setConfigParam(config);
}
/*-------------------------------------------------
		块配置 - 取出参数（不开放）
*/
C_PiS_Config P_PLE_Editor::getConfigParam(){
	return P_PiS_Selector::getConfigParam();
}
/*-------------------------------------------------
		块配置 - 窗口编辑ui设置
*/
void P_PLE_Editor::openWindow_setConfigParam_PLE(){
	//（暂无）
}



/*-------------------------------------------------
		鼠标事件 - 圈选变化事件（单个、多个）
*/
void P_PLE_Editor::event_itemSelectionChanged(QList<QListWidgetItem*> selected_item_list){
	P_PiS_Selector::event_itemSelectionChanged(selected_item_list);
}
/*-------------------------------------------------
		鼠标事件 - 右键事件（零个、单个、多个）
*/
void P_PLE_Editor::event_itemRightClicked(QList<QListWidgetItem*> item_list){
	P_PiS_Selector::event_itemRightClicked(item_list);
}

/*-------------------------------------------------
		右键菜单 - 零个（覆写）（点击空白处）
*/
QMenu* P_PLE_Editor::createMenu_selectNone(){
	QAction* action;
	QMenu* menu = new QMenu();


	// > 零个 - 刷新编号
	action = new QAction("刷新编号", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Refresh.png"));
	action->setEnabled(this->action_isCellTitleNeedRefresh());
	connect(action, &QAction::triggered, this, &P_PLE_Editor::action_refreshCellTitle);
	menu->addAction(action);

	menu->addSeparator();


	// > 零个 - 添加图片
	QMenu* menu_add = new QMenu("添加图片");
	menu_add->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));

	action = new QAction("添加图片", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
	connect(action, &QAction::triggered, this, &P_PLE_Editor::action_append);
	menu_add->addAction(action);

	menu->addMenu(menu_add);


	// > 零个 - 选择图片
	QMenu* menu_select = new QMenu("选择图片", menu);
	menu_select->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_SelectAll.png"));
	menu_select->setEnabled(false);
	menu->addMenu(menu_select);


	// > 零个 - 复制/粘贴/删除
	action = new QAction("复制图片", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Copy.png"));
	action->setShortcut(QKeySequence::Copy);
	action->setEnabled(false);
	menu->addAction(action);

	action = new QAction("粘贴图片", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Paste.png"));
	action->setData(this->m_cellTank.count());	//最末尾的位置
	action->setShortcut(QKeySequence::Paste);
	action->setEnabled(this->action_isEnablePaste_PLE());
	connect(action, &QAction::triggered, this, &P_PLE_Editor::action_paste);
	menu->addAction(action);

	action = new QAction("删除图片", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Delete.png"));
	action->setShortcut(QKeySequence::Delete);
	action->setEnabled(false);
	menu->addAction(action);

	menu->addSeparator();


	// > 零个 - 导出
	QMenu* menu_export = new QMenu("导出图片", menu);
	menu_export->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Export.png"));
	menu_export->setEnabled(false);
	menu->addMenu(menu_export);

	return menu;
}
/*-------------------------------------------------
		右键菜单 - 单个（覆写）
*/
QMenu* P_PLE_Editor::createMenu_selectSingle(int pos){
	if (pos < 0){ return nullptr; }
	QAction* action;
	QMenu* menu = new QMenu();
	QString pos_str = QString::number(pos);


	// > 单个 - 刷新编号
	action = new QAction("刷新编号", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Refresh.png"));
	action->setEnabled(this->action_isCellTitleNeedRefresh());
	connect(action, &QAction::triggered, this, &P_PLE_Editor::action_refreshCellTitle);
	menu->addAction(action);

	// > 单个 - 左移右移
	action = new QAction("左移", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Left.png"));
	action->setData(pos_str);
	connect(action, &QAction::triggered, this, &P_PLE_Editor::action_moveLeft);
	menu->addAction(action);
	if (pos - 1 < 0){ action->setEnabled(false); }

	action = new QAction("右移", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Right.png"));
	action->setData(pos_str);
	connect(action, &QAction::triggered, this, &P_PLE_Editor::action_moveRight);
	menu->addAction(action);
	if (pos + 1 >= this->m_cellTank.count()){ action->setEnabled(false); }

	menu->addSeparator();


	// > 单个 - 添加图片
	action = new QAction("添加图片", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
	action->setData(pos_str);
	connect(action, &QAction::triggered, this, &P_PLE_Editor::action_insert);
	menu->addAction(action);


	// > 单个 - 选择图片
	QMenu* menu_select = this->createMenu_changeSelection();
	menu->addMenu(menu_select);


	// > 单个 - 复制/粘贴/删除
	action = new QAction("复制图片", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Copy.png"));
	action->setData(pos_str);
	action->setShortcut(QKeySequence::Copy);
	connect(action, &QAction::triggered, this, &P_PLE_Editor::action_copy);
	menu->addAction(action);

	action = new QAction("粘贴图片", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Paste.png"));
	action->setData(pos_str);
	action->setShortcut(QKeySequence::Paste);
	action->setEnabled(this->action_isEnablePaste_PLE());
	connect(action, &QAction::triggered, this, &P_PLE_Editor::action_paste);
	menu->addAction(action);

	action = new QAction("删除图片", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Delete.png"));
	action->setData(pos_str);
	action->setShortcut(QKeySequence::Delete);
	connect(action, &QAction::triggered, this, &P_PLE_Editor::action_remove);
	menu->addAction(action);

	//menu->addSeparator();
	//
	// > 单个 - 编辑图片
	//	（无）


	menu->addSeparator();

	// > 单个 - 导出图片
	action = new QAction("导出图片", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Export.png"));
	action->setData(pos_str);
	connect(action, &QAction::triggered, this, &P_PLE_Editor::action_exportPic_Single);
	menu->addAction(action);

	return menu;
}
/*-------------------------------------------------
		右键菜单 - 多个（覆写）
*/
QMenu* P_PLE_Editor::createMenu_selectMultiple(QList<int> pos_list){
	if (pos_list.count() <= 0){ return nullptr; }
	QAction* action;
	QMenu* menu = new QMenu();
	QList<QString> pos_list_str = TTool::_QList_IntToQString_(pos_list);
	QString pos_frist_str = QString::number(pos_list.first());


	// > 多个 - 刷新编号
	action = new QAction("刷新编号", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Refresh.png"));
	action->setEnabled(this->action_isCellTitleNeedRefresh());
	connect(action, &QAction::triggered, this, &P_PLE_Editor::action_refreshCellTitle);
	menu->addAction(action);

	menu->addSeparator();

	// > 多个 - 添加图片
	action = new QAction("添加图片", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
	action->setData(pos_frist_str);
	connect(action, &QAction::triggered, this, &P_PLE_Editor::action_insert);
	menu->addAction(action);


	// > 多个 - 选择图片
	QMenu* menu_select = this->createMenu_changeSelection();
	menu->addMenu(menu_select);


	// > 多个 - 复制/粘贴/删除
	action = new QAction("复制图片", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Copy.png"));
	action->setData(pos_list_str.join(","));
	action->setShortcut(QKeySequence::Copy);
	connect(action, &QAction::triggered, this, &P_PLE_Editor::action_copy);
	menu->addAction(action);

	action = new QAction("粘贴图片", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Paste.png"));
	action->setData(pos_frist_str);
	action->setShortcut(QKeySequence::Paste);
	action->setEnabled(this->action_isEnablePaste_PLE());
	connect(action, &QAction::triggered, this, &P_PLE_Editor::action_paste);
	menu->addAction(action);

	action = new QAction("删除图片", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Delete.png"));
	action->setData(pos_list_str.join(","));
	action->setShortcut(QKeySequence::Delete);
	connect(action, &QAction::triggered, this, &P_PLE_Editor::action_remove);
	menu->addAction(action);

	//menu->addSeparator();
	//
	// > 多个 - 编辑图片
	//	（无）


	menu->addSeparator();

	// > 多个 - 导出图片
	action = new QAction("导出图片(多张)", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Export.png"));
	action->setData(pos_list_str.join(","));
	connect(action, &QAction::triggered, this, &P_PLE_Editor::action_exportPic_Multiple);
	menu->addAction(action);

	return menu;
}



/*-------------------------------------------------
		基函数（数据交换） - 一对一交换
*/
void P_PLE_Editor::base_swap(int index_a, int index_b){
	
	// > 内容交换
	I_PiS_Cell* cell_a = this->m_cellTank.at(index_a);
	I_PiS_Cell* cell_b = this->m_cellTank.at(index_b);
	C_PictureBlockDataPtr cellData_a = cell_a->getBlockDataPtr();
	C_PictureBlockDataPtr cellData_b = cell_b->getBlockDataPtr();
	cell_a->widget->setDataPtr(cellData_b);
	cell_b->widget->setDataPtr(cellData_a);

	// > 刷新元胞
	this->refreshPictureCell(index_a);
	this->refreshPictureCell(index_b);

	// > 修改状态
	cell_a->widget->setTitleColorType("Changed");
	cell_b->widget->setTitleColorType("Changed");

	emit signal_cellChanged();
}
/*-------------------------------------------------
		基函数（数据交换） - 取出多个然后放在指定位置
*/
void P_PLE_Editor::base_swapList(int org_index, QList<int> org_index_list){
	qSort(org_index_list);

	// > 计算取出后的索引位置
	int cur_index = org_index;
	for (int i = 0; i < org_index_list.count(); i++){
		int index = org_index_list.at(i);
		if (index < cur_index){		//取出对象在索引前面，则-1
			cur_index -= 1;
		}
	}

	// > 数据初始化
	QList<C_PictureBlockDataPtr> data_all;
	for (int i = 0; i < this->m_cellTank.count(); i++){
		I_PiS_Cell* cell = this->m_cellTank.at(i);
		data_all.append(cell->getBlockDataPtr());
	}

	// > 取出数据
	QList<C_PictureBlockDataPtr> data_out;
	for (int i = org_index_list.count()-1; i >= 0; i--){
		int index = org_index_list.at(i);
		C_PictureBlockDataPtr data = data_all.at(index);

		// > 修改状态
		data->m_titleColorType = "Changed";

		data_out.append(data);
		data_all.removeAt(index);
	}

	// > 插入数据
	for (int i = 0; i < data_out.count(); i++){
		data_all.insert(cur_index, data_out.at(i));
		//data_all.insert(cur_index + i, data_out.at(i));	//（反序）
	}

	// > 数据全部重置
	for (int i = 0; i < this->m_cellTank.count(); i++){
		I_PiS_Cell* cell = this->m_cellTank.at(i);
		cell->widget->setDataPtr(data_all.at(i));

		// > 刷新元胞
		this->refreshPictureCell(i);
	}

	emit signal_cellChanged();
}
/*-------------------------------------------------
		action - 左移
*/
void P_PLE_Editor::action_moveLeft(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString pos_str = cur_action->data().value<QString>();
	int pos = pos_str.toInt();

	if (pos - 1 < 0){ return; }
	this->base_swap(pos, pos - 1);
	this->selectIndex_Single(pos - 1);	//（选中）
}
/*-------------------------------------------------
		action - 右移
*/
void P_PLE_Editor::action_moveRight(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString pos_str = cur_action->data().value<QString>();
	int pos = pos_str.toInt();

	if (pos + 1 >= this->m_cellTank.count()){ return; }
	this->base_swap(pos, pos + 1);
	this->selectIndex_Single(pos + 1);	//（选中）
}
/*-------------------------------------------------
		action - 刷新编号
*/
void P_PLE_Editor::action_refreshCellTitle(){
	this->refreshUI();	//（直接重刷全部）
}
/*-------------------------------------------------
		action - 是否需要刷新编号
*/
bool P_PLE_Editor::action_isCellTitleNeedRefresh(){
	
	for (int i = 0; i < this->m_cellTank.count(); i++){
		I_PiS_Cell* cell = this->m_cellTank.at(i);
		C_PictureBlockDataPtr cellData = cell->getBlockDataPtr();
		
		// > 有修改状态时
		if (cellData->m_titleColorType != "Normal"){ return true; }

		// > 编号不一致时
		if (cellData->m_titleNum != i){ return true; }

	}
	return false;
}



/*-------------------------------------------------
		基函数（添加/删除） - 添加(append)
*/
void P_PLE_Editor::base_append(QList<C_PictureBlockDataPtr> cellData_list){
	if (cellData_list.count() == 0){ return; }

	// > 执行添加
	for (int i = 0; i < cellData_list.count(); i++){
		C_PictureBlockDataPtr cell_data = cellData_list.at(i);

		// > 加入缓存容器
		S_PictureBitmapCache::getInstance()->addPath(cell_data->m_bitmapPath);

		// > 索引标记
		int new_index = this->m_cellTank.count();

		// > 建立元胞
		I_PiS_Cell* cell = this->createPictureCell(cell_data);
		this->m_cellTank.append(cell);

		// > 添加 - 项
		this->m_parent->addItem(cell->item);
		// > 添加 - 控件
		this->m_parent->setItemWidget(cell->item, cell->widget);

		// > 修改状态
		cell->widget->setTitleColorType("Changed");
		// > 刷新元胞
		this->refreshPictureCell(new_index);
	}

	emit signal_cellChanged();
}
/*-------------------------------------------------
		基函数（添加/删除） - 添加(insert)
*/
void P_PLE_Editor::base_insert(int index, QList<C_PictureBlockDataPtr> cellData_list){
	if (cellData_list.count() == 0){ return; }

	// > 空数组时，走 append 流程
	if (this->m_cellTank.count() == 0){
		this->base_append(cellData_list); 
		return; 
	}

	// > 执行添加
	QList<int> new_index_list;
	for (int i = 0; i < cellData_list.count(); i++){
		C_PictureBlockDataPtr cell_data = cellData_list.at(i);

		// > 加入缓存容器
		S_PictureBitmapCache::getInstance()->addPath(cell_data->m_bitmapPath);

		// > 索引标记
		int new_index = this->m_cellTank.count();
		new_index_list.append(new_index);

		// > 建立元胞
		//		（insertItem有bug，与addItem的效果是一样的，所以干脆用addItem，然后刷新所有cell。）
		//		（注意，item和cell深度绑定，cell的数据能改变，但是控件无法改变，所以顺序也是固定的。）
		I_PiS_Cell* cell = this->createPictureCell(cell_data);
		this->m_cellTank.append(cell);

		// > 添加 - 项
		this->m_parent->addItem(cell->item);
		// > 添加 - 控件
		this->m_parent->setItemWidget(cell->item, cell->widget);
	}

	// > 数据交换
	this->base_swapList(index, new_index_list);
	//emit signal_cellChanged();	//（不需要发两次信号）
}
/*-------------------------------------------------
		基函数（添加/删除） - 移除
*/
void P_PLE_Editor::base_remove(QList<int> index_list){
	if (this->m_cellTank.count() == 0){ return; }
	if (index_list.count() == 0){ return; }
	qSort(index_list);	//（排序）

	// > 倒序删除
	int index = 0;
	for (int i = index_list.count() - 1; i >= 0; i--){
		index = index_list.at(i);

		// > 获取元胞
		I_PiS_Cell* cell = this->m_cellTank.at(index);
		this->m_cellTank.removeAt(index);

		// > 控件移除
		this->m_parent->takeItem(index);

		delete cell->item;
		delete cell->widget;
		delete cell;
	}

	// > 取消选择
	if (this->m_cellTank.count() == 0){
		emit signal_allCellDeleted();
	}else{
		this->selectIndex_Single(index - 1);
	}

	emit signal_cellChanged();
}


/*-------------------------------------------------
		action - 添加图片（append）
*/
void P_PLE_Editor::action_append(){

	// > 导入图片 - 选择文件（可选多张）
	QStringList file_list = this->importPic_openQFileDialog();
	if (file_list.count() == 0){ return; }

	// > 导入图片 - 复制文件到文件夹
	file_list = this->importPic_addToParentDir(file_list);

	// > 导入图片 - 文件 转 图片单块
	QList<C_PictureBlockDataPtr> cellData_list = this->importPic_getCellListByFileList(file_list);

	this->base_append(cellData_list);
}
/*-------------------------------------------------
		action - 添加图片（insert）
*/
void P_PLE_Editor::action_insert(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString pos_str = cur_action->data().value<QString>();
	int pos = pos_str.toInt();

	// > 导入图片 - 选择文件（可选多张）
	QStringList file_list = this->importPic_openQFileDialog();
	if (file_list.count() == 0){ return; }

	// > 导入图片 - 复制文件到文件夹
	file_list = this->importPic_addToParentDir(file_list);

	// > 导入图片 - 文件 转 图片单块
	QList<C_PictureBlockDataPtr> cellData_list = this->importPic_getCellListByFileList(file_list);

	if (pos == this->m_cellTank.count() - 1){	//（如果选择的最后一个，则在末尾添加）
		this->base_append(cellData_list);
	}else{
		this->base_insert(pos, cellData_list);
	}
}
/*-------------------------------------------------
		action - 删除图片
*/
void P_PLE_Editor::action_remove(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString pos_list_str = cur_action->data().value<QString>();
	QList<int> pos_list = TTool::_QList_QStringToInt_(pos_list_str.split(","));
	this->base_remove(pos_list);
}

/*-------------------------------------------------
		导入图片 - 选择文件（可选多张）
*/
QStringList P_PLE_Editor::importPic_openQFileDialog(){
	QStringList result_list;
	QFileDialog fd;
	fd.setWindowTitle("导入图片（可选多张）");
	fd.setAcceptMode(QFileDialog::AcceptOpen);	
	fd.setNameFilters(QStringList() << "图片(*.png)");
	fd.setViewMode(QFileDialog::Detail);
	fd.setFileMode(QFileDialog::ExistingFiles);		//多个文件
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()){ return result_list; }
		result_list = fd.selectedFiles();
		return result_list;
	}else {
		return result_list;
	}
}
/*-------------------------------------------------
		导入图片 - 复制文件到文件夹
*/
QStringList P_PLE_Editor::importPic_addToParentDir(QStringList file_list){
	QStringList parentDirFile_list;
	for (int i = 0; i < file_list.count(); i++){
		QString cur_filePath = file_list.at(i);

		// > 不存在的文件，跳过
		if (QFileInfo(cur_filePath).exists() == false){
			continue;
		}

		// > 未添加成功的文件，跳过
		QString resultPath = S_PLE_FileManager::getInstance()->addPicFile_ToDir(cur_filePath, this->m_dataSet_parentDir);
		if (resultPath == ""){
			continue;
		}
		parentDirFile_list.append(resultPath);
	}

	// > 发出信号
	emit signal_picListChanged();

	return parentDirFile_list;
}
/*-------------------------------------------------
		导入图片 - 文件 转 图片单块
*/
QList<C_PictureBlockDataPtr> P_PLE_Editor::importPic_getCellListByFileList(QStringList file_list){
	int item_width = this->m_config.getSize_ItemWidth();
	int item_height = this->m_config.getSize_ItemHeight();
	int cell_count = this->m_cellTank.count();
	QList<C_PictureBlockDataPtr> cellData_list;
	for (int i = 0; i < file_list.count(); i++){
		QString bitmapPath = file_list.at(i);

		C_PictureBlockDataPtr cellData = this->createPictureCellData();
		cellData->init(
			item_width,
			item_height,
			cell_count + i,
			bitmapPath
			);
		cellData_list.append(cellData);
	}
	return cellData_list;
}



/*-------------------------------------------------
		导出图片 - 设置自定义名称
*/
void P_PLE_Editor::setCustomExportName(QString name){
	this->m_customExportName = name;
}
QString P_PLE_Editor::getCustomExportName(){
	return this->m_customExportName;
}
/*-------------------------------------------------
		action - 导出图片（单张）
*/
void P_PLE_Editor::action_exportPic_Single(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString pos_str = cur_action->data().value<QString>();
	int pos = pos_str.toInt();

	// > 导出图片 - 选择文件（单张导出）
	QString exportName = "图片";
	if (this->m_customExportName != ""){ exportName = this->m_customExportName; }
	QString tar_file = this->exportPic_openQFileDialog_File(exportName);
	if (tar_file == ""){ return; }

	// > 获取 图片单块 的文件路径
	I_PiS_Cell* cell = this->m_cellTank.at(pos);
	QString from_file = cell->getBlockDataPtr()->m_bitmapPath;

	// > 复制文件
	P_FileOperater op;
	op.copy_File(QFileInfo(from_file), QFileInfo(tar_file));
	QMessageBox::information(this->m_parent, "提示", "图片导出成功。", QMessageBox::Yes);
}
/*-------------------------------------------------
		action - 导出图片（多张）
*/
void P_PLE_Editor::action_exportPic_Multiple(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString pos_list_str = cur_action->data().value<QString>();
	QList<int> pos_list = TTool::_QList_QStringToInt_(pos_list_str.split(","));

	// > 导出图片 - 选择文件夹（多张导出）
	QString tar_dir = this->exportPic_openQFileDialog_Dir();
	if (tar_dir == ""){ return; }

	// > 获取 图片单块 的文件路径
	QStringList from_file_list;
	for (int i = 0; i < pos_list.count(); i++){
		int pos = pos_list.at(i);
		I_PiS_Cell* cell = this->m_cellTank.at(pos);
		QString from_file = cell->getBlockDataPtr()->m_bitmapPath;
		if (from_file == ""){ continue; }
		from_file_list.append(from_file);
	}

	// > 复制文件
	P_FileOperater op;
	for (int i = 0; i < from_file_list.count(); i++){
		QString from_file = from_file_list.at(i);
		QString tar_file = QDir(tar_dir).absoluteFilePath("图片_" + QString::number(i + 1) + ".png");
		op.copy_File(QFileInfo(from_file), QFileInfo(tar_file));
	}
	QMessageBox::information(this->m_parent, "提示", "图片集导出成功。", QMessageBox::Yes);
}
/*-------------------------------------------------
		action - 导出图片（全部）
*/
void P_PLE_Editor::action_exportPic_All(){

	// > 导出图片 - 选择文件夹（多张导出）
	QString tar_dir = this->exportPic_openQFileDialog_Dir();
	if (tar_dir == ""){ return; }

	// > 获取 图片单块 的文件路径
	QStringList from_file_list;
	for (int i = 0; i < this->m_cellTank.count(); i++){
		I_PiS_Cell* cell = this->m_cellTank.at(i);
		QString from_file = cell->getBlockDataPtr()->m_bitmapPath;
		if (from_file == ""){ continue; }
		from_file_list.append(from_file);
	}

	// > 复制文件
	P_FileOperater op;
	for (int i = 0; i < from_file_list.count(); i++){
		QString from_file = from_file_list.at(i);
		QString tar_file = QDir(tar_dir).absoluteFilePath("图片_" + QString::number(i + 1) + ".png");
		op.copy_File(QFileInfo(from_file), QFileInfo(tar_file));
	}
	QMessageBox::information(this->m_parent, "提示", "图片集导出成功。", QMessageBox::Yes);
}
/*-------------------------------------------------
		导出图片 - 选择文件（单张导出）
*/
QString P_PLE_Editor::exportPic_openQFileDialog_File(QString defaultFile_name){
	QString target_file = "";
	QFileDialog fd;
	fd.setWindowTitle("导出图片");
	fd.setAcceptMode(QFileDialog::AcceptSave);
	fd.setDirectory(".");
	fd.setNameFilters(QStringList() << "图片(*.png)");
	fd.setViewMode(QFileDialog::Detail);
	fd.setFileMode(QFileDialog::ExistingFile);		//单个文件
	fd.selectFile(defaultFile_name + ".png");		//默认导出的文件名
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()){ return ""; }
		target_file = fd.selectedFiles().at(0);
	}
	else {
		return "";
	}
	return target_file;
}
/*-------------------------------------------------
		导出图片 - 选择文件夹（多张导出）
*/
QString P_PLE_Editor::exportPic_openQFileDialog_Dir(){
	QString target_dir = "";
	QFileDialog fd;
	target_dir = fd.getExistingDirectory(this->m_parent, "导出到文件夹");
	return target_dir;
}



/*-------------------------------------------------
		基函数（复制/粘贴） - 复制
*/
void P_PLE_Editor::base_copy(QList<int> index_list){
	if (index_list.count() == 0){ return; }

	// > 取出数据
	QList<C_PictureBlockDataPtr> data_list;
	for (int i = 0; i < index_list.count(); i++){
		int index = index_list.at(i);
		I_PiS_Cell* cell = this->m_cellTank.at(index);
		data_list.append(cell->getBlockDataPtr());
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
	mimeData->setData("application/drill-P_PLE_Editor", document.toJson());
	clipboard->setMimeData(mimeData);
}
/*-------------------------------------------------
		基函数（复制/粘贴） - 粘贴
*/
void P_PLE_Editor::base_paste(int index){

	// > 剪贴板 - 获取
	QClipboard* clipboard = QApplication::clipboard();
	const QMimeData* mimeData = clipboard->mimeData();
	QByteArray data = mimeData->data("application/drill-P_PLE_Editor");
	if (data.isEmpty()){ return; }	//（注意，粘贴不支持ALE，因为不考虑子类兼容）

	// > 转换JSON字节流
	QJsonDocument jsonDocument = QJsonDocument::fromJson(data);
	QJsonObject data_all = jsonDocument.object();

	// > 建立元胞数据
	int item_width = this->m_config.getSize_ItemWidth();
	int item_height = this->m_config.getSize_ItemHeight();
	int cell_count = this->m_cellTank.count();
	QList<C_PictureBlockDataPtr> cellData_list;
	QJsonArray data_array = data_all.value("data_list").toArray();
	for (int i = 0; i < data_array.count(); i++){
		QJsonObject obj = data_array.at(i).toObject();
		C_PictureBlockDataPtr dataPtr = this->createPictureCellData();
		dataPtr->setJsonObject(obj);

		// > 画布与粘贴的位置一直
		dataPtr->m_width = item_width;
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
		action - 复制
*/
void P_PLE_Editor::action_copy(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString pos_list_str = cur_action->data().value<QString>();
	QList<int> pos_list = TTool::_QList_QStringToInt_(pos_list_str.split(","));
	this->base_copy(pos_list);
}
/*-------------------------------------------------
		action - 粘贴
*/
void P_PLE_Editor::action_paste(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString pos_str = cur_action->data().value<QString>();
	int pos = pos_str.toInt();
	this->base_paste(pos);
}
/*-------------------------------------------------
		action - 是否可以粘贴
*/
bool P_PLE_Editor::action_isEnablePaste_PLE(){
	QClipboard* clipboard = QApplication::clipboard();
	const QMimeData* mimeData = clipboard->mimeData();
	QByteArray data = mimeData->data("application/drill-P_PLE_Editor");
	if (data.isEmpty()){ return false; }
	return true;
}



/*-------------------------------------------------
		快捷键 - 事件绑定
*/
bool P_PLE_Editor::event_shortcut_keyPress(QKeyEvent *event){

	// > 父控件没有焦点则直接跳出
	if (this->m_parent->hasFocus() == false){ return false; }

	// > 键位监听
	if (event->modifiers() & Qt::ControlModifier){
		if (event->key() == Qt::Key_C){
			this->shortcut_copy();
			return true;
		}
		if (event->key() == Qt::Key_V){
			this->shortcut_paste();
			return true;
		}
		if (event->key() == Qt::Key_A){
			this->shortcut_selectAll();
			return true;
		}
	}
	if (event->key() == Qt::Key_Delete){
		this->shortcut_delete();
		return true;
	}

	return false;
}
/*-------------------------------------------------
		快捷键 - 全选
*/
void P_PLE_Editor::shortcut_selectAll(){
	this->m_parent->selectAll();
}
/*-------------------------------------------------
		快捷键 - 复制
*/
void P_PLE_Editor::shortcut_copy(){
	QList<int> pos_list;
	QList<QListWidgetItem*> item_list = this->m_parent->selectedItems();
	for (int i = 0; i < item_list.count(); i++){
		QListWidgetItem* item = item_list.at(i);
		int pos = this->getCellIndexByItem(item);
		if (pos == -1){ continue; }
		pos_list.append(pos);
	}
	qSort(pos_list);		//（排序）
	if (pos_list.count() == 0){ return; }
	this->base_copy(pos_list);
}
/*-------------------------------------------------
		快捷键 - 粘贴
*/
void P_PLE_Editor::shortcut_paste(){
	QList<QListWidgetItem*> item_list = this->m_parent->selectedItems();
	if (item_list.count() == 0){ return; }
	int pos = this->getCellIndexByItem(item_list.first());
	this->base_paste(pos);
}
/*-------------------------------------------------
		快捷键 - 删除
*/
void P_PLE_Editor::shortcut_delete(){
	QList<int> index_list = this->getSelectedIndex_Multiple();
	this->base_remove(index_list);
}

