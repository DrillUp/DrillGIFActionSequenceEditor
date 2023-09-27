#include "stdafx.h"
#include "P_ALE_Editor.h"

#include "Private/W_ALEDataEdit.h"
#include "Private/W_ALEConfigEdit.h"
#include "Private/W_ALEGIFReaderType.h"
#include "Private/W_ALEGIFWriterType.h"
#include "Source/Utils/WidgetFormSenior/AnimationBlock/P_AnimationBlock.h"

#include "Source/DllModule/Cximagecrt_drill/Src_header/S_cximageManager.h"
#include "Source/Utils/Manager/GIFManager/S_GIFManager.h"
#include "Source/GraphModule/Widget/PictureCombiner/W_PictureCombiner.h"
#include "Source/GraphModule/Widget/PicturePartitioner/W_PicturePartitioner.h"

#include "Source/Utils/WidgetForm/PictureSelector/P_PiS_Selector.h"
#include "Source/Utils/Common/P_FileOperater.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		动画帧编辑块.cpp
		版本：		v1.04
		作者：		drill_up
		所属模块：	工具模块
		功能：		将图片全部显示，并能单选/多选。（ALE全称：Animation_List_Editor）
					【底层基于QListWidget的item和widget，与QGraphics无关】
					【注意一个特殊的信号：allFrameDeleted，全部帧被删除情况】

		子功能：	-> 文本选择
						-> 单选
						-> 多选
						-> 自动编号
					-> 动画帧配置
						-> 设置尺寸模式
						-> 设置叶子显示文本
					-> 资源数组操作
						-> 右键菜单
						-> 添加、替换、删除
						-> 复制、粘贴
					-> 导入
						-> 单图
						-> 多图
						-> GIF
						-> 序列大图
					-> 导出
						-> 单图
						-> 多图
						-> GIF
						-> 序列大图
					-> 编辑帧
						-> 单帧编辑：图像、帧数
						-> 多帧编辑：帧数
					-> 编辑UI配置
					-> 快捷键
					
		使用方法：
				> 初始化（需要数据）
					this->m_p_AnimationListEditor = new P_AnimationListEditor(ui.listWidget);
					this->m_p_AnimationListEditor->setSource(data);

				> 数据初始化
					C_ALEData data;
					data.setId(10);
					data.setSource(	"F:/rpg mv箱/mog插件中文全翻译(Drill_up)v2.41 - 副本/插件集合示例/img/enemies/", 
									QList<QString>() << "小爱丽丝001" << "小爱丽丝002" << "小爱丽丝003" << "小爱丽丝004" << "小爱丽丝005" << "小爱丽丝006" << "小爱丽丝004" << "小爱丽丝006");
					data.setInterval( 3, QList<int>() << 6 << 5 << 4 << 3 << 2 << 1);

				> 修改配置
					C_ALEConfig config;				//注意，C_PiSConfig也可以当配置，但是要开启多选功能。
					config ...
					this->m_p_AnimationListEditor->setConfigParam(config);
					
				> 快捷键（父类widget覆写keyPressEvent方法）
					if (event->modifiers() & Qt::ControlModifier){
						if (event->key() == Qt::Key_C){
							this->m_p_AnimationListEditor->shortcut_copy();
						}
						if (event->key() == Qt::Key_V){
							this->m_p_AnimationListEditor->shortcut_paste();
						}
						if (event->key() == Qt::Key_A){
							this->m_p_AnimationListEditor->shortcut_selectAll();
						}
					}
					if (event->key() == Qt::Key_Delete){
						this->m_p_AnimationListEditor->shortcut_delete();
					}
					
-----==========================================================-----
*/
P_ALE_Editor::P_ALE_Editor(QListWidget *parent)
	: P_PiS_Selector(parent)
{

	//-----------------------------------
	//----参数初始化
	this->m_copyedList.clear();
	this->m_unit = C_ALE_DataSet::FrameUnit;

	// > 数据
	this->setConfigParam(C_ALE_Config());
	this->m_exportName = "动画";

	// > 快捷键过滤器
	this->m_parent->installEventFilter(this);
}
P_ALE_Editor::~P_ALE_Editor(){
}


/*-------------------------------------------------
		工厂 - 建立一个元胞（覆写）
*/
I_PiS_Cell* P_ALE_Editor::createPictureCell(int i, QPixmap pixmap){

	// > 控件 - 建立图片块
	P_PictureBlock* widget = this->createPictureWidget(i, pixmap);

	// > 项 - 建立项
	QListWidgetItem* item = this->createPictureItem();


	// > 元胞
	I_PiS_Cell* cell = new I_PiS_Cell();
	cell->index_Title = i;
	cell->index_OrgBitmap = i;
	cell->item = item;
	cell->widget = widget;

	return cell;
}
/*-------------------------------------------------
		工厂 - 建立一个项（覆写）
*/
QListWidgetItem* P_ALE_Editor::createPictureItem(){
	return P_PiS_Selector::createPictureItem();
}
/*-------------------------------------------------
		工厂 - 建立一个控件（覆写）
*/
P_PictureBlock* P_ALE_Editor::createPictureWidget(int i, QPixmap pixmap){
	int item_height = this->m_config.getItemHeight();

	// > 建立图片块
	P_AnimationBlock* widget = new P_AnimationBlock(item_height, item_height, this->m_parent);
	if (this->m_config.m_zeroFill == true){
		widget->setTitle(TTool::_zeroFill_(i + 1, this->m_config.m_zeroFillCount, QLatin1Char(this->m_config.m_zeroFillChar.toLatin1())));
	}else{
		widget->setTitle(QString::number(i + 1));
	}

	// > 绘制图片
	widget->setPixmap(pixmap);
	widget->setMaskEnabled(this->m_config.m_isMaskEnabled);

	// > 绘制帧数
	widget->setFrameText(this->m_data.getIntervalString(i));
	
	return widget;
}
/*-------------------------------------------------
		控件 - 重建UI
*/
void P_ALE_Editor::rebuildUI(){
	P_PiS_Selector::rebuildUI();
	//（暂无）
}
/*-------------------------------------------------
		控件 - 清理控件
*/
void P_ALE_Editor::clearCells(){
	P_PiS_Selector::clearCells();
	//（暂无）
}
/*-------------------------------------------------
		控件 - 清理全部
*/
void P_ALE_Editor::clearAll(){
	P_PiS_Selector::clearAll();
	//（暂无）
}


/*-------------------------------------------------
		动画帧设置 - 设置参数
*/
void P_ALE_Editor::setConfigParam_ALE(C_ALE_Config config){
	this->m_config_ALE = config;
	this->m_data.setData_IntervalDefault(config.m_defaultInterval);
	P_PiS_Selector::setConfigParam(config);
}
/*-------------------------------------------------
		动画帧设置 - 取出参数
*/
C_ALE_Config P_ALE_Editor::getConfigParam_ALE(){
	return this->m_config_ALE;
}
/*-------------------------------------------------
		动画帧设置 - 设置参数（不开放）
*/
void P_ALE_Editor::setConfigParam(C_PiS_Config config){
	P_PiS_Selector::setConfigParam(config);
}
/*-------------------------------------------------
		动画帧设置 - 取出参数（不开放）
*/
C_PiS_Config P_ALE_Editor::getConfigParam(){
	return P_PiS_Selector::getConfigParam();
}
/*-------------------------------------------------
		动画帧设置 - 窗口编辑ui设置
*/
void P_ALE_Editor::openWindow_setConfigParam(){

	// > 弹出ui编辑框
	W_ALEConfigEdit d(this->m_parent);
	d.setDataInModifyMode(this->m_config_ALE, this->m_unit);
	if (d.exec() == QDialog::Accepted){
		C_ALE_Config config = d.getData();

		// > 设置后，强制变化统一默认帧间隔
		this->m_data.setIntervalDefaultWithFit(config.m_defaultInterval);

		// > 单位标记
		this->m_unit = (C_ALE_DataSet::DATA_UNIT)d.getDataUnit();
		this->m_data.setData_Unit(this->m_unit);
		
		// > 设置参数
		this->setConfigParam_ALE(config);
	}
}



/*-------------------------------------------------
		鼠标事件 - 右键事件（零个、单个、多个）
*/
void P_ALE_Editor::event_itemRightClicked(QList<QListWidgetItem*> item_list){
	P_PiS_Selector::event_itemRightClicked(item_list);
	QAction* action;
	QMenu* menu = new QMenu();

	/*-------------------------点击空白处-----------------------------*/
	if (item_list.count() == 0){
		
		QMenu* menu_add = new QMenu("添加帧", menu);
		menu_add->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));

			action = new QAction("添加图片", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
			action->setData(0);
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_insertInAction);
			menu_add->addAction(action);

			action = new QAction("添加GIF", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
			action->setData(0);
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_insertGIFInAction);
			menu_add->addAction(action);

			action = new QAction("添加序列大图", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
			action->setData(0);
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_insertSeqPicInAction);
			menu_add->addAction(action);
		menu->addMenu(menu_add);

		if (this->m_config_ALE.m_isMultiSelect == true){
			QMenu* menu_select = new QMenu("选择帧", menu);
			menu_select->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_SelectAll.png"));
			menu_select->setEnabled(false);
			menu->addMenu(menu_select);
		}

		action = new QAction("复制帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Copy.png"));
		action->setShortcut(QKeySequence::Copy);
		action->setEnabled(false);
		menu->addAction(action);

		action = new QAction("粘贴帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Paste.png"));
		action->setShortcut(QKeySequence::Paste);
		action->setEnabled(false);
		menu->addAction(action);

		action = new QAction("删除帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Delete.png"));
		action->setShortcut(QKeySequence::Delete);
		action->setEnabled(false);
		menu->addAction(action);

		if (this->m_config_ALE.m_isMultiSelect == true){
			menu->addSeparator();

			QMenu* menu_export = new QMenu("导出", menu);
			menu_export->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Export.png"));
			menu_export->setEnabled(false);
			menu->addMenu(menu_export);
		}

		menu->exec(QCursor::pos());
		
	/*-------------------------点击单个-----------------------------*/
	}else if (item_list.count() == 1){
		int pos = this->getCellIndexByItem( item_list.at(0) );
		if (pos == -1){ return; }

		action = new QAction("左移", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Left.png"));
		action->setData(pos);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::op_moveLeftInAction);
		menu->addAction(action);
		if (pos -1 < 0 ){ action->setEnabled(false); }

		action = new QAction("右移", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Right.png"));
		action->setData(pos);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::op_moveRightInAction);
		menu->addAction(action);
		if (pos + 1 >= this->m_cellTank.count()){ action->setEnabled(false); }

		menu->addSeparator();
		
		QMenu* menu_add = new QMenu("添加帧", menu);
		menu_add->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));

			action = new QAction("添加图片", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
			action->setData(pos);
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_insertInAction);
			menu_add->addAction(action);

			action = new QAction("添加GIF", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
			action->setData(pos);
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_insertGIFInAction);
			menu_add->addAction(action);

			action = new QAction("添加序列大图", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
			action->setData(pos);
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_insertSeqPicInAction);
			menu_add->addAction(action);
		menu->addMenu(menu_add);

		if (this->m_config_ALE.m_isMultiSelect == true){
			QMenu* menu_select = new QMenu("选择帧", menu);
			menu_select->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_SelectAll.png"));

			action = new QAction("全选", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_SelectAll.png"));
			action->setShortcut(QKeySequence::SelectAll);
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_selectAllInAction);
			menu_select->addAction(action);
			action = new QAction("选择奇数帧", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_SelectAll.png"));
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_selectOddInAction);
			menu_select->addAction(action);
			action = new QAction("选择偶数帧", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_SelectAll.png"));
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_selectEvenInAction);
			menu_select->addAction(action);

			menu->addMenu(menu_select);
		}

		action = new QAction("复制帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Copy.png"));
		action->setData( QString::number(pos) );
		action->setShortcut(QKeySequence::Copy);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::op_copyInAction);
		menu->addAction(action);

		action = new QAction("粘贴帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Paste.png"));
		action->setData( pos );
		action->setShortcut(QKeySequence::Paste);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::op_pasteInAction);
		menu->addAction(action);
		if (this->m_copyedList.count() == 0){ action->setEnabled(false); }

		action = new QAction("删除帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Delete.png"));
		action->setData(pos);
		action->setShortcut(QKeySequence::Delete);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::op_removeInAction);
		menu->addAction(action);

		menu->addSeparator();

		action = new QAction("编辑帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Edit.png"));
		action->setData(pos);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::op_editOneInAction);
		menu->addAction(action);

		if (this->m_config_ALE.m_isMultiSelect == true){
			menu->addSeparator();
			QMenu* menu_export = new QMenu("导出", menu);
			menu_export->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Export.png"));

			action = new QAction("导出 - 单图", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/ExportImage.png"));
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_exportSingle_PicInAction);
			menu_export->addAction(action);
			action = new QAction("导出 - 全部帧", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/ExportImage.png"));
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_exportAll_PicInAction);
			menu_export->addAction(action);
			action = new QAction("生成GIF - 单帧", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/ExportGIF.png"));
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_exportSelected_GIFInAction);
			menu_export->addAction(action);
			action = new QAction("生成GIF - 全部帧", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/ExportGIF.png"));
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_exportAll_GIFInAction);
			menu_export->addAction(action);
			action = new QAction("生成序列大图 - 选中帧", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/ExportImage.png"));
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_exportSelected_SeqPicInAction);
			menu_export->addAction(action);
			action = new QAction("生成序列大图 - 全部帧", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/ExportImage.png"));
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_exportAll_SeqPicInAction);
			menu_export->addAction(action);

			menu->addMenu(menu_export);
		}

		menu->exec(QCursor::pos());
		
	/*-------------------------点击多个-----------------------------*/
	}else{
		int front_pos = 1000;
		QList<int> pos_list;
		for (int i = 0; i < item_list.count(); i++){
			QListWidgetItem* item = item_list.at(i);
			int pos = this->getCellIndexByItem(item);
			if (pos == -1){ continue; }
			if (pos < front_pos){ front_pos = pos; }
			pos_list.append( pos );
		}
		qSort(pos_list);		//（排序，防止删除时顺序乱了）
		QList<QString> pos_list_str;
		for (int i = 0; i < pos_list.count(); i++){
			pos_list_str.append(QString::number(pos_list.at(i)));
		}

		QMenu* menu_add = new QMenu("添加帧", menu);
		menu_add->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));

			action = new QAction("添加图片", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
			action->setData(front_pos);
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_insertInAction);
			menu_add->addAction(action);

			action = new QAction("添加GIF", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
			action->setData(front_pos);
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_insertGIFInAction);
			menu_add->addAction(action);

			action = new QAction("序列大图", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Add.png"));
			action->setData(front_pos);
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_insertSeqPicInAction);
			menu_add->addAction(action);
		menu->addMenu(menu_add);

		if (this->m_config_ALE.m_isMultiSelect == true){
			QMenu* menu_select = new QMenu("选择帧", menu);
			menu_select->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_SelectAll.png"));

			action = new QAction("全选", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_SelectAll.png"));
			action->setShortcut(QKeySequence::SelectAll);
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_selectAllInAction);
			menu_select->addAction(action);
			action = new QAction("选择奇数帧", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_SelectAll.png"));
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_selectOddInAction);
			menu_select->addAction(action);
			action = new QAction("选择偶数帧", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_SelectAll.png"));
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_selectEvenInAction);
			menu_select->addAction(action);

			menu->addMenu(menu_select);
		}

		action = new QAction("复制帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Copy.png"));
		action->setData(pos_list_str.join(","));
		action->setShortcut(QKeySequence::Copy);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::op_copyInAction);
		menu->addAction(action);

		action = new QAction("粘贴帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Paste.png"));
		action->setData(front_pos);
		action->setShortcut(QKeySequence::Paste);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::op_pasteInAction);
		menu->addAction(action);
		if (this->m_copyedList.count() == 0){ action->setEnabled(false); }

		action = new QAction("删除帧", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Delete.png"));
		action->setData(pos_list_str.join(","));
		action->setShortcut(QKeySequence::Delete);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::op_removeInAction);
		menu->addAction(action);

		menu->addSeparator();

		action = new QAction("编辑帧时间", this);
		action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Edit.png"));
		action->setData(pos_list_str.join(","));
		action->setShortcut(QKeySequence::Delete);
		connect(action, &QAction::triggered, this, &P_ALE_Editor::op_editMultiInAction);
		menu->addAction(action);

		if (this->m_config_ALE.m_isMultiSelect == true){
			menu->addSeparator();
			QMenu* menu_export = new QMenu("导出", menu);
			menu_export->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Export.png"));

			action = new QAction("导出 - 选中帧", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/ExportImage.png"));
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_exportSelected_PicInAction);
			menu_export->addAction(action);
			action = new QAction("导出 - 全部帧", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/ExportImage.png"));
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_exportAll_PicInAction);
			menu_export->addAction(action);
			action = new QAction("生成GIF - 选中帧", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/ExportGIF.png"));
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_exportSelected_GIFInAction);
			menu_export->addAction(action);
			action = new QAction("生成GIF - 全部帧", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/ExportGIF.png"));
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_exportAll_GIFInAction);
			menu_export->addAction(action);
			action = new QAction("生成序列大图 - 选中帧", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/ExportImage.png"));
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_exportSelected_SeqPicInAction);
			menu_export->addAction(action);
			action = new QAction("生成序列大图 - 全部帧", this);
			action->setIcon(QIcon(QRC_IconSrcPath + "/menu/ExportImage.png"));
			connect(action, &QAction::triggered, this, &P_ALE_Editor::op_exportAll_SeqPicInAction);
			menu_export->addAction(action);

			menu->addMenu(menu_export);
		}

		menu->exec(QCursor::pos());
	}

}
/*-------------------------------------------------
		鼠标事件 - 圈选变化事件（单个、多个）
*/
void P_ALE_Editor::event_itemSelectionChanged(QList<QListWidgetItem*> selected_item_list){
	P_PiS_Selector::event_itemSelectionChanged(selected_item_list);
}


/*-------------------------------------------------
		资源数据 - 设置贴图（继承）（不开放）
*/
void P_ALE_Editor::setSourceBitmap(QList<QPixmap> bitmap_list) {
	P_PiS_Selector::setSourceBitmap(bitmap_list);
}
/*-------------------------------------------------
		资源数据 - 设置数据
*/
void P_ALE_Editor::setSource(C_ALE_DataSet data) {
	this->m_data = data;
	
	this->m_data.setData_Unit(this->m_unit);	//（单位标记）

	QList<QPixmap> pixmap_list;
	QList<QFileInfo> file_list = this->m_data.getAllFile();
	for (int i = 0; i < file_list.count(); i++){
		QFileInfo info = file_list.at(i);
		QImage image = QImage(info.absoluteFilePath());
		QPixmap pixmap = QPixmap::fromImage(image);
		pixmap_list.append(pixmap);
	}
	this->setSourceBitmap(pixmap_list);
}
/*-------------------------------------------------
		资源数据 - 获取数据（资源数据会被该块修改，需要随时获取并变化）
*/
C_ALE_DataSet P_ALE_Editor::getSource(){
	this->m_data.setData_Unit(this->m_unit);	//（单位标记）
	return this->m_data;
}
/*-------------------------------------------------
		资源数据 - 设置单位
*/
void P_ALE_Editor::setUnit(C_ALE_DataSet::DATA_UNIT unit){
	this->m_unit = unit;
}
/*-------------------------------------------------
		资源数据 - 获取单位
*/
C_ALE_DataSet::DATA_UNIT P_ALE_Editor::getUnit(){
	return this->m_unit;
}


/*-------------------------------------------------
		操作 - 添加资源(append)
*/
void P_ALE_Editor::op_append(QString gif_src){
	if (this->m_data.isNull()){ return; }

	// > 数据添加
	this->m_data.op_append(gif_src);
	int index = this->m_data.getFileCount() - 1;

	// > 图形添加
	QFileInfo info = this->m_data.getFile(index);
	QImage image = QImage(info.absoluteFilePath());
	QPixmap pixmap = QPixmap::fromImage(image);
	//this->m_org_bitmapList.append(pixmap);	//（暂时不改变贴图列表）


	// > 建立元胞
	I_PiS_Cell* cell = this->createPictureCell(index, pixmap);
	this->m_cellTank.append(cell);

	// > 添加 - 项
	this->m_parent->addItem(cell->item);
	// > 添加 - 控件
	this->m_parent->setItemWidget(cell->item, cell->widget);

	//（不需要刷）

	emit signal_SourceBitmapChanged();
}
/*-------------------------------------------------
		操作 - 添加资源(insert)【此处 绕开了 insertItem 的bug，代码结构会比较复杂】
*/
void P_ALE_Editor::op_insert(int index, QStringList gif_src_list, QList<int> interval_list){
	if (this->m_data.isNull()){ return; }
	if (gif_src_list.count() == 0){ return; }

	// > 数据添加
	this->m_data.op_insert(index, gif_src_list, interval_list);

	for (int i = 0; i < gif_src_list.count(); i++){

		// > 图形添加
		QFileInfo info = this->m_data.getFile(index + i);
		QImage image = QImage(info.absoluteFilePath());
		QPixmap pixmap = QPixmap::fromImage(image);
		//this->m_org_bitmapList.insert(index + i, pixmap);	//（暂时不改变贴图列表）


		// > 建立元胞
		I_PiS_Cell* cell = this->createPictureCell(index, pixmap);
		this->m_cellTank.insert(index + i, cell);

		// > 添加 - 项
		this->m_parent->addItem(cell->item);
		// > 添加 - 控件
		this->m_parent->setItemWidget(cell->item, cell->widget);
	}

	// > 强制全刷
	this->op_refreshAll(index);

	emit signal_SourceBitmapChanged();
}
/*-------------------------------------------------
		操作 - 移除
*/
void P_ALE_Editor::op_remove(QList<int> index_list){
	if (this->m_data.isNull()){ return; }
	if (index_list.count() == 0){ return; }
	qSort(index_list);	//（排序）

	// > 倒序删除
	int index = 0;
	for (int i = index_list.count() - 1; i >= 0; i--){
		index = index_list.at(i);

		// > 数据移除
		this->m_data.op_remove(index);

		// > 图形移除
		//this->m_org_bitmapList.removeAt(index);	//（暂时不改变贴图列表）

		// > 控件移除
		this->m_parent->takeItem(index);


		// > 获取元胞
		I_PiS_Cell* cell = this->m_cellTank.at(index);
		this->m_cellTank.removeAt(index);

		delete cell->item;
		delete cell->widget;
		delete cell;
	}

	// > 清除复制项
	this->m_copyedList.clear();

	// > 取消选择
	if (this->m_cellTank.count() == 0){
		emit signal_allFrameDeleted();
	}else{
		this->selectIndex_Single(index - 1);
	}

	emit signal_SourceBitmapChanged();
}
/*-------------------------------------------------
		操作 - 交换位置
*/
void P_ALE_Editor::op_swap(int index_a, int index_b){
	if (this->m_data.isNull()){ return; }
	if (index_a == index_b){ return; }

	// > 数据交换
	this->m_data.op_swap(index_a, index_b);

	// > 图形交换
	//this->m_org_bitmapList.swap(index_a, index_b);	//（暂时不改变贴图列表）
	
	// > 交换后，ui刷新
	this->op_refresh(index_a);
	this->op_refresh(index_b);

	// > 控件交换（不交换）
	//if (index_a > index_b){	//（必须 a小于b）
	//	int temp = index_a;
	//	index_a = index_b;
	//	index_b = temp;
	//}
	//this->m_listWidget->removeItemWidget(this->m_listWidget->item(index_a));		//【removeItem有问题，总出现指针问题】
	//this->m_listWidget->removeItemWidget(this->m_listWidget->item(index_b));
	//QListWidgetItem* item_a = this->m_listWidget->takeItem(index_a);
	//QListWidgetItem* item_b = this->m_listWidget->takeItem(index_b - 1);
	//this->m_listWidget->insertItem(index_a, item_b);
	//this->m_listWidget->insertItem(index_b, item_a);
	//
	//this->m_listWidget->setItemWidget(item_a, w_a);
	//this->m_listWidget->setItemWidget(item_b, w_b);
	//w_a->setPixmap(this->m_org_bitmapList.at(index_a));
	//w_b->setPixmap(this->m_org_bitmapList.at(index_b));

	//this->m_itemTank.swap(index_a, index_b);
	//this->m_widgetTank.swap(index_a, index_b);

	emit signal_SourceBitmapChanged();
}
/*-------------------------------------------------
		操作 - 刷新贴图内容
*/
void P_ALE_Editor::op_refresh(int index){
	if (index < 0){ return; }
	if (index >= this->m_cellTank.count()){ return; }
	P_AnimationBlock* widget = dynamic_cast<P_AnimationBlock*>(this->m_cellTank.at(index));
	if (this->m_config.m_zeroFill == true){		//（刷新计数）
		widget->setTitle(TTool::_zeroFill_(index + 1, this->m_config.m_zeroFillCount, QLatin1Char(this->m_config.m_zeroFillChar.toLatin1())));
	}else{
		widget->setTitle(QString::number(index + 1));
	}
	widget->setPixmap(this->m_org_bitmapList.at(index));	//（刷新图片）
	widget->setFrameText(this->m_data.getIntervalString(index));	//（刷新帧）
}
void P_ALE_Editor::op_refreshAll(int startAt){
	for (int i = startAt; i < this->m_cellTank.count(); i++){
		this->op_refresh(i);
	}
}




/*-------------------------------------------------
		action - 添加帧（点击空白处）
*/
void P_ALE_Editor::op_appendInAction(){

	bool cover_all = false;
	QStringList file_list = this->openQFileDialog_getPicFileList();	//（打开窗口选择多张图片）
	for (int i = 0; i < file_list.count(); i++){
		QFileInfo fileinfo = QFileInfo(file_list.at(i));
		if (fileinfo.exists() == false){ continue; }
		QString file_name = fileinfo.completeBaseName();
		
		// > 在同一个文件夹下
		if (this->m_data.getFileRoot() == fileinfo.absolutePath()){
			this->op_append(file_name);
			continue;
		}

		// > 不重复文件（检查文件夹）
		QFileInfo repeat_file = QFileInfo(this->m_data.getFileRoot() + "/" + fileinfo.completeBaseName() + ".png");
		if (repeat_file.exists() == false){
			C_ALE_DataSet::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
			this->op_append(file_name);
			continue;
		}

		// > 全部覆盖
		if (cover_all == true){
			C_ALE_DataSet::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
			this->op_append(file_name);
			continue;
		}

		// > 重复文件处理
		switch (QMessageBox::question(this->m_parent, "提示", "要导入的图片：\"" + file_name + "\"在配置中已存在，是否覆盖原文件？", "全部覆盖", "覆盖", "保持原图添加", 0)) {

			case 0:
				cover_all = true;
			case 1:
				C_ALE_DataSet::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
				this->op_append(file_name);
				break;

			case 2:
			default:
				this->op_append(file_name);
				break;
		}

	}
}
/*-------------------------------------------------
		action - 添加帧（点击帧时）
*/
void P_ALE_Editor::op_insertInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int pos = cur_action->data().value<int>();

	bool cover_all = false;
	QStringList file_list = this->openQFileDialog_getPicFileList();	//（打开窗口选择多张图片）
	for (int i = 0; i < file_list.count(); i++){
		QFileInfo fileinfo = QFileInfo(file_list.at(i));
		if (fileinfo.exists() == false){ continue; }
		QString file_name = fileinfo.completeBaseName();

		// > 在同一个文件夹下
		if (this->m_data.getFileRoot() == fileinfo.absolutePath()){
			this->op_insert(pos + i, QStringList() << file_name);
			continue;
		}

		// > 不重复文件（检查文件夹）
		QFileInfo repeat_file = QFileInfo(this->m_data.getFileRoot() + "/" + fileinfo.completeBaseName() + ".png");
		if (repeat_file.exists() == false){
			C_ALE_DataSet::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
			this->op_insert(pos + i, QStringList() << file_name);
			continue;
		}

		// > 全部覆盖
		if (cover_all == true){
			C_ALE_DataSet::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
			this->op_insert(pos + i, QStringList() << file_name);
			continue;
		}

		// > 重复文件处理
		switch (QMessageBox::question(this->m_parent, "提示", "要导入的图片：\"" + file_name + "\"在配置中已存在，是否覆盖原文件？", "全部覆盖", "覆盖", "保持原图添加", 0)) {

			case 0:
				cover_all = true;
			case 1:
				C_ALE_DataSet::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
				this->op_insert(pos + i, QStringList() << file_name);
				break;

			case 2:
			default:
				this->op_insert(pos + i, QStringList() << file_name);
				break;
		}
	}
}
/*-------------------------------------------------
		action - 添加帧（GIF）
*/
void P_ALE_Editor::op_insertGIFInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int pos = cur_action->data().value<int>();

	int readerMethod = 0;
	W_ALEGIFReaderType d(this->m_parent);
	if (d.exec() == QDialog::Accepted){
		readerMethod = d.getData();
	}else{
		return;
	}

	QStringList file_name_list;
	QList<int> interval_list;
	QString file_src = this->openQFileDialog_getGIFFile();
	if (file_src == ""){ return; }
	QFileInfo fileinfo_src = QFileInfo(file_src);

	// > 重复文件处理
	QFileInfo repeat_file = QFileInfo(this->m_data.getFileRoot() + "/" + fileinfo_src.completeBaseName() + "_0" + ".png");
	if (repeat_file.exists() == true){
		if( QMessageBox::question(this->m_parent, "提示", "要导入的GIF：\"" + fileinfo_src.completeBaseName() + "\"在配置中已存在，是否覆盖原文件？", "覆盖", "取消导入", 0) == 1) {
			return;
		}
	}

	// > QImageReader读取
	if (readerMethod == 0){

		// > 拆解GIF到目录
		bool successed = S_GIFManager::getInstance()->dismantlingGIF(fileinfo_src, QDir(this->m_data.getFileRoot() + "/"), "png", "%2_%1");
		if ( successed == false){
			QMessageBox::warning(this->m_parent, "错误", "GIF解析失败。", QMessageBox::Yes);
			return;
		}

		// > 获取文件名
		interval_list = S_GIFManager::getInstance()->getLastDismantledGIFIntervalList();
		QList<QFileInfo> file_list = S_GIFManager::getInstance()->getLastDismantledGIFFileList();
		for (int i = 0; i < file_list.count(); i++){
			file_name_list.append(file_list.at(i).completeBaseName());
		}


	// > cximage读取
	}else if (readerMethod == 1){
		
		// > 拆解GIF到目录
		bool successed = S_cximageManager::getInstance()->dismantlingGIF(fileinfo_src, QDir(this->m_data.getFileRoot() + "/"), "png", "%2_%1");
		if (successed == false){
			QMessageBox::warning(this->m_parent, "错误", "GIF解析失败。", QMessageBox::Yes);
			return;
		}

		// > 获取文件名
		interval_list = S_cximageManager::getInstance()->getLastDismantledGIFIntervalList();
		QList<QFileInfo> file_list = S_cximageManager::getInstance()->getLastDismantledGIFFileList();
		for (int i = 0; i < file_list.count(); i++){
			file_name_list.append(file_list.at(i).completeBaseName());
		}
	}

	this->op_insert(pos, file_name_list, interval_list);

}
/*-------------------------------------------------
		action - 添加帧（序列大图）
*/
void P_ALE_Editor::op_insertSeqPicInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int pos = cur_action->data().value<int>();

	QStringList file_name_list;
	QList<int> interval_list;
	QString file_src = this->openQFileDialog_getSeqPicFile();
	if (file_src == ""){ return; }
	QFileInfo fileinfo_src = QFileInfo(file_src);

	// > 重复文件处理
	QFileInfo repeat_file = QFileInfo(this->m_data.getFileRoot() + "/" + fileinfo_src.completeBaseName() + "_切片_0" + ".png");
	if (repeat_file.exists() == true){
		if( QMessageBox::question(this->m_parent, "提示", "要导入的GIF：\"" + fileinfo_src.completeBaseName() + "\"在配置中已存在，是否覆盖原文件？", "覆盖", "取消导入", 0) == 1) {
			return;
		}
	}

	// > 图片分割器窗口
	QList<QPixmap> bitmap_list;
	W_PicturePartitioner d(this->m_parent);
	d.setData(fileinfo_src);
	if (d.exec() == QDialog::Accepted){
		bitmap_list = d.getData();
	}else{
		return;
	}
	
	// > 保存切割的图片
	for (int i = 0; i < bitmap_list.count(); i++){
		QPixmap bitmap = bitmap_list.at(i);
		QString base_name = fileinfo_src.completeBaseName() + "_切片_" + QString::number(i) ;
		QString path = this->m_data.getFileRoot() + "/" + base_name + ".png";
		bitmap.toImage().save(path, "png");
		file_name_list.append(base_name);
	}
	
	this->op_insert(pos, file_name_list);
}
/*-------------------------------------------------
		action - 删除帧（单个和多个）
*/
void P_ALE_Editor::op_removeInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString index_str = cur_action->data().value<QString>();
	QStringList index_str_list = index_str.split(",");

	QList<int> index_list;
	for (int i = 0; i < index_str_list.count(); i++){
		index_list.append(index_str_list.at(i).toInt());
	}

	this->op_remove(index_list);
}
/*-------------------------------------------------
		action - 全选（单个和多个）
*/
void P_ALE_Editor::op_selectAllInAction(){
	this->m_parent->selectAll();
}
/*-------------------------------------------------
		action - 选奇数
*/
void P_ALE_Editor::op_selectOddInAction(){
	QList<int> index_list;
	for (int i = 0; i < this->m_cellTank.count(); i++){
		if (i%2 == 0){
			index_list.append(i);
		}
	}
	this->selectIndex_Multiple(index_list);
}
/*-------------------------------------------------
		action - 选偶数
*/
void P_ALE_Editor::op_selectEvenInAction(){
	QList<int> index_list;
	for (int i = 0; i < this->m_cellTank.count(); i++){
		if (i % 2 == 1){
			index_list.append(i);
		}
	}
	this->selectIndex_Multiple(index_list);
}
/*-------------------------------------------------
		action - 复制（单个和多个）
*/
void P_ALE_Editor::op_copyInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString index_str = cur_action->data().value<QString>();

	QStringList index_list_str = index_str.split(",");
	QList<int> index_list;
	for (int i = 0; i < index_list_str.count(); i++ ){
		index_list.append( index_list_str.at(i).toInt() );
	}

	this->m_copyedList = this->m_data.getFile_Multi(index_list);
}
/*-------------------------------------------------
		action - 粘贴（单个和多个）
*/
void P_ALE_Editor::op_pasteInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int pos = cur_action->data().value<int>();
	if (this->m_copyedList.count() == 0){ return; }

	QStringList file_name_list;
	for (int i = 0; i < this->m_copyedList.count(); i++){
		file_name_list.append(this->m_copyedList.at(i).completeBaseName());
	}

	this->op_insert(pos, file_name_list);
}
/*-------------------------------------------------
		action - 左移
*/
void P_ALE_Editor::op_moveLeftInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int index = cur_action->data().value<int>();
	if (index - 1 < 0){ return; }
	this->op_swap(index, index - 1);
	this->selectIndex_Single(index - 1);	//（选中）
}
/*-------------------------------------------------
		action - 右移
*/
void P_ALE_Editor::op_moveRightInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int index = cur_action->data().value<int>();
	if (index + 1 >= this->m_cellTank.count()){ return; }
	this->op_swap(index, index + 1);
	this->selectIndex_Single(index+1);	//（选中）
}
/*-------------------------------------------------
		action - 编辑帧
*/
void P_ALE_Editor::op_editOneInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int index = cur_action->data().value<int>();

	// > 弹出对话框
	W_ALEDataEdit d(this->m_parent);
	d.setDataInModifyMode_One(this->m_data,index);
	if (d.exec() == QDialog::Accepted){
		this->m_data = d.getData();

		// > 图形刷新
		QFileInfo info = this->m_data.getFile(index);
		QImage image = QImage(info.absoluteFilePath());
		QPixmap pixmap = QPixmap::fromImage(image);
		this->m_org_bitmapList.replace(index, pixmap);

		// > 控件刷新
		this->op_refresh(index);
	}
}
/*-------------------------------------------------
		action - 编辑帧时间
*/
void P_ALE_Editor::op_editMultiInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString index_str = cur_action->data().value<QString>();

	QStringList index_list_str = index_str.split(",");
	QList<int> index_list;
	for (int i = 0; i < index_list_str.count(); i++){
		index_list.append(index_list_str.at(i).toInt());
	}

	// > 弹出对话框
	W_ALEDataEdit d(this->m_parent);
	d.setDataInModifyMode_Multi(this->m_data, index_list);
	if (d.exec() == QDialog::Accepted){
		this->m_data = d.getData();

		//（多张图片的图形不需要刷新）

		for (int i = 0; i < index_list.count(); i++){
			this->op_refresh(index_list.at(i));
		}
	}
}

/*-------------------------------------------------
		导出 - 设置导出名称
*/
void P_ALE_Editor::setExportName(QString name){
	if (name == ""){ name = "动画"; }
	this->m_exportName = name;
}
/*-------------------------------------------------
		导出 - 导出单图
*/
void P_ALE_Editor::op_exportPic(int index, QFileInfo target_file){
	QFileInfo file_from = this->m_data.getFile(index);
	if (file_from.exists() == false){ return; }

	P_FileOperater op;
	op.copy_File(file_from, target_file);
}
/*-------------------------------------------------
		导出 - 导出多图
*/
void P_ALE_Editor::op_exportPic_Multi(QList<int> index_list, QDir target_dir){
	if (target_dir.exists() == false){ return; }
	P_FileOperater op;

	for (int i = 0; i < index_list.count(); i++){
		QFileInfo file_from = this->m_data.getFile(index_list.at(i));
		if (file_from.exists() == false){ continue; }
		QString path_to = target_dir.absolutePath()+"/" + this->m_exportName + "_" + QString::number(i + 1) + ".png";
		op.copy_File(file_from, QFileInfo(path_to));
	}
}
/*-------------------------------------------------
		导出 - 导出GIF（指定项）
*/
void P_ALE_Editor::op_exportGIF_Multi(QList<int> index_list, QFileInfo target_file, int writerMethod){

	// > 文件列表
	QList<QFileInfo> fileInfo_list = this->m_data.getFile_Multi(index_list);

	// > 帧间隔列表（已转单位）
	double interval_default = this->m_data.getIntervalDefaultWithUnit();
	QList<double> interval_list = this->m_data.getIntervalWithUnit_Multi(index_list);

	// > gif.h生成
	if (writerMethod == 0){
		
		S_GIFManager::getInstance()->generateGIF(
			fileInfo_list,
			target_file.absoluteFilePath(),
			interval_default,
			TTool::_QList_DoubleToInt_floor_(interval_list)
		);

	// > cximage生成
	}else{

		S_cximageManager::getInstance()->generateGIF(
			fileInfo_list,
			target_file.absoluteFilePath(),
			interval_default,
			TTool::_QList_DoubleToInt_floor_(interval_list)
		);
	}

	QMessageBox::information(this->m_parent, "提示", "GIF导出成功。", QMessageBox::Yes);
}
/*-------------------------------------------------
		action - 导出单图
*/
void P_ALE_Editor::op_exportSingle_PicInAction(){
	QList<int> selected_list = this->getSelectedIndex_Multiple();
	if (selected_list.count() == 0){ return; }
	QString path = this->openQFileDialog_exportPNGFile();
	if (path == ""){ return; }

	this->op_exportPic(selected_list.at(0), QFileInfo(path));
	QMessageBox::information(this->m_parent, "提示", "图片导出成功。", QMessageBox::Yes);
}
/*-------------------------------------------------
		action - 导出多图
*/
void P_ALE_Editor::op_exportSelected_PicInAction(){
	QList<int> selected_list = this->getSelectedIndex_Multiple();
	if (selected_list.count() == 0){ return; }
	QString path = this->openQFileDialog_exportDir();
	if (path == ""){ return; }

	this->op_exportPic_Multi(selected_list, QDir(path));
	QMessageBox::information(this->m_parent, "提示", "图片集导出成功。", QMessageBox::Yes);
}
/*-------------------------------------------------
		action - 导出全部图
*/
void P_ALE_Editor::op_exportAll_PicInAction(){
	QList<int> selected_list;
	for (int i = 0; i < this->m_cellTank.count(); i++){
		selected_list.append(i);
	}
	if (selected_list.count() == 0){ return; }
	QString path = this->openQFileDialog_exportDir();
	if (path == ""){ return; }

	this->op_exportPic_Multi(selected_list, QDir(path));
	QMessageBox::information(this->m_parent, "提示", "图片集导出成功。", QMessageBox::Yes);
}
/*-------------------------------------------------
		action - 导出GIF - 选中项
*/
void P_ALE_Editor::op_exportSelected_GIFInAction(){

	// > 选择项
	QList<int> selected_list = this->getSelectedIndex_Multiple();
	if (selected_list.count() == 0){ QMessageBox::warning(this->m_parent, "提示", "需要选择至少一个动画帧才能生成GIF。", QMessageBox::Yes); return; }

	// > 选择方式
	int writerMethod = 0;
	W_ALEGIFWriterType d(this->m_parent);
	if (d.exec() == QDialog::Accepted){
		writerMethod = d.getData();
	}else{
		return;
	}

	// > 选择存储文件
	QString path = this->openQFileDialog_exportGIFFile();
	if (path == ""){ return; }

	this->op_exportGIF_Multi(selected_list, QFileInfo(path), writerMethod);
}
/*-------------------------------------------------
		action - 导出GIF - 全部项
*/
void P_ALE_Editor::op_exportAll_GIFInAction(){

	// > 选择项
	QList<int> selected_list;
	for (int i = 0; i < this->m_cellTank.count(); i++){
		selected_list.append(i);
	}
	if (selected_list.count() == 0){ QMessageBox::warning(this->m_parent, "提示", "需要至少一个动画帧才能生成GIF。", QMessageBox::Yes); return; }

	// > 选择方式
	int writerMethod = 0;
	W_ALEGIFWriterType d(this->m_parent);
	if (d.exec() == QDialog::Accepted){
		writerMethod = d.getData();
	}else{
		return;
	}

	// > 选择存储文件
	QString path = this->openQFileDialog_exportGIFFile();
	if (path == ""){ return; }

	this->op_exportGIF_Multi(selected_list, QFileInfo(path), writerMethod);
}
/*-------------------------------------------------
		action - 导出序列大图 - 选中项
*/
void P_ALE_Editor::op_exportSelected_SeqPicInAction(){

	// > 选择项
	QList<int> selected_list = this->getSelectedIndex_Multiple();
	if (selected_list.count() < 2){ QMessageBox::warning(this->m_parent, "提示", "需要选择至少两个动画帧才能生成序列大图。", QMessageBox::Yes); return; }

	// > 文件列表
	QList<QFileInfo> fileInfo_list = this->m_data.getFile_Multi(selected_list);

	// > 打开图片合成器
	W_PictureCombiner d(this->m_parent);
	d.setData(fileInfo_list);
	if (d.exec() == QDialog::Accepted){
		QPixmap bitmap = d.getData();
	
		// > 选择文件存储
		QString path = this->openQFileDialog_exportPNGFile("序列大图");
		if (path == ""){ return; }
		bitmap.toImage().save(path, "png");
	}
}
/*-------------------------------------------------
		action - 导出序列大图 - 全部项
*/
void P_ALE_Editor::op_exportAll_SeqPicInAction(){

	// > 选择项
	QList<int> selected_list;
	for (int i = 0; i < this->m_cellTank.count(); i++){
		selected_list.append(i);
	}
	if (selected_list.count() < 2 ){ QMessageBox::warning(this->m_parent, "提示", "需要至少两个动画帧才能生成序列大图。", QMessageBox::Yes); return; }

	// > 文件列表
	QList<QFileInfo> fileInfo_list = this->m_data.getFile_Multi(selected_list);

	// > 打开图片合成器
	W_PictureCombiner d(this->m_parent);
	d.setData(fileInfo_list);
	if (d.exec() == QDialog::Accepted){
		QPixmap bitmap = d.getData();

		// > 选择文件存储
		QString path = this->openQFileDialog_exportPNGFile("序列大图");
		if (path == ""){ return; }
		bitmap.toImage().save(path, "png");
	}
}


/*-------------------------------------------------
		文件选择窗口 - 选择多张图片（导入）
*/
QStringList P_ALE_Editor::openQFileDialog_getPicFileList(){
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
		文件选择窗口 - 选择GIF（导入）
*/
QString P_ALE_Editor::openQFileDialog_getGIFFile(){
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
		文件选择窗口 - 选择序列大图（导入）
*/
QString P_ALE_Editor::openQFileDialog_getSeqPicFile(){
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
		文件选择窗口 - 选择GIF（导出）
*/
QString P_ALE_Editor::openQFileDialog_exportGIFFile(){
	QString target_file = "";
	QFileDialog fd;
	fd.setWindowTitle("导出GIF");
	fd.setAcceptMode(QFileDialog::AcceptSave);
	fd.setDirectory(".");
	fd.setNameFilters(QStringList() << "动图(*.gif)");
	fd.setViewMode(QFileDialog::Detail);
	fd.setFileMode(QFileDialog::ExistingFile);		//单个文件
	fd.selectFile(this->m_exportName+"_动图.gif");
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()){ return ""; }
		target_file = fd.selectedFiles().at(0);
	}else {
		return "";
	}
	return target_file;
}
/*-------------------------------------------------
		文件选择窗口 - 选择PNG（导出）
*/
QString P_ALE_Editor::openQFileDialog_exportPNGFile(QString name_suffix){
	QString target_file = "";
	QFileDialog fd;
	fd.setWindowTitle("导出PNG");
	fd.setAcceptMode(QFileDialog::AcceptSave);
	fd.setDirectory(".");
	fd.setNameFilters(QStringList() << name_suffix+"(*.png)");
	fd.setViewMode(QFileDialog::Detail);
	fd.setFileMode(QFileDialog::ExistingFile);		//单个文件
	fd.selectFile(this->m_exportName + "_" + name_suffix + ".png");
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()){ return ""; }
		target_file = fd.selectedFiles().at(0);
	}else {
		return "";
	}
	return target_file;
}
/*-------------------------------------------------
		文件选择窗口 - 选择文件夹（导出）
*/
QString P_ALE_Editor::openQFileDialog_exportDir(){
	QString target_dir = "";
	QFileDialog fd;
	target_dir = fd.getExistingDirectory(this->m_parent, "导出到文件夹");
	return target_dir;
}



/*-------------------------------------------------
		快捷键 - 事件绑定
*/
bool P_ALE_Editor::event_shortcut_keyPress(QKeyEvent *event){

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
void P_ALE_Editor::shortcut_selectAll(){
	this->m_parent->selectAll();
}
/*-------------------------------------------------
		快捷键 - 复制
*/
void P_ALE_Editor::shortcut_copy(){
	//QList<int> index_list = this->getSelectedIndex_Multiple();
	//this->m_copyedList = this->m_data.getFile_Multi(index_list);
}
/*-------------------------------------------------
		快捷键 - 粘贴
*/
void P_ALE_Editor::shortcut_paste(){
	if (this->m_copyedList.count() == 0){ return; }

	/*
	QStringList file_name_list;
	for (int i = 0; i < this->m_copyedList.count(); i++){
		file_name_list.append(this->m_copyedList.at(i).completeBaseName());
	}

	int pos = this->getSelectedIndex_Single();
	this->op_insert(pos, file_name_list);
	*/

}
/*-------------------------------------------------
		快捷键 - 删除
*/
void P_ALE_Editor::shortcut_delete(){
	QList<int> index_list = this->getSelectedIndex_Multiple();
	this->op_remove(index_list);
}