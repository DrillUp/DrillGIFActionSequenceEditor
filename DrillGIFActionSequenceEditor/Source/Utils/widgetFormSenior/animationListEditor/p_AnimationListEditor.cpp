#include "stdafx.h"
#include "p_AnimationListEditor.h"

#include "private/p_ALEBlock.h"
#include "private/w_ALEDataEdit.h"
#include "private/w_ALEConfigEdit.h"
#include "private/w_ALEGIFReaderType.h"
#include "private/w_ALEGIFWriterType.h"

#include "Source/DllModule/cximagecrt_drill/src_header/s_cximageManager.h"
#include "Source/Utils/manager/GIFManager/s_GIFManager.h"

#include "Source/Utils/widgetForm/pictureSelector/p_PictureSelector.h"
#include "Source/Utils/common/p_FileOperater.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		动画帧编辑块.cpp
		版本：		v1.03
		作者：		drill_up
		所属模块：	工具模块
		功能：		将图片全部显示，并能单选/多选。
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
					-> 导出
						-> 单图
						-> 多图
						-> GIF
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
					C_ALEData data = C_ALEData();
					data.setId(10);
					data.setSource(	"F:/rpg mv箱/mog插件中文全翻译(Drill_up)v2.41 - 副本/插件集合示例/img/enemies/", 
									QList<QString>() << "小爱丽丝001" << "小爱丽丝002" << "小爱丽丝003" << "小爱丽丝004" << "小爱丽丝005" << "小爱丽丝006" << "小爱丽丝004" << "小爱丽丝006");
					data.setInterval( 3, QList<int>() << 6 << 5 << 4 << 3 << 2 << 1);

				> 修改配置
					C_ALEConfig config = C_ALEConfig();		//注意，C_PiSConfig也可以当配置，但是要开启多选功能。
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
P_AnimationListEditor::P_AnimationListEditor(QListWidget *parent)
	: P_PictureSelector(parent)
{

	//-----------------------------------
	//----参数初始化
	this->m_iconSrcPath = ":/DrillGIFActionSequenceEditor/Resources/icons";
	this->m_copyedList = QList<QFileInfo>();
	this->m_unit = C_ALEData::FrameUnit;

	// > 数据
	this->setConfigParam(C_ALEConfig());
	this->m_exportName = "动画";

	// > 快捷键过滤器
	this->m_listWidget->installEventFilter(this);
}
P_AnimationListEditor::~P_AnimationListEditor(){
}


/*-------------------------------------------------
		控件 - 每项建立控件（覆写）
*/
QWidget* P_AnimationListEditor::createPictureWidget(int i, QPixmap pixmap){
	int item_height = this->m_config.getItemHeight();

	// > 建立图片块
	P_ALEBlock* widget = new P_ALEBlock(item_height, item_height, this->m_listWidget);
	if (this->m_config.m_zeroFill == true){
		widget->setCountText(TTool::_zeroFill_(i + 1, this->m_config.m_zeroFillCount, QLatin1Char(this->m_config.m_zeroFillChar.toLatin1())));
	}else{
		widget->setCountText(QString::number(i + 1));
	}

	// > 绘制图片
	widget->setPixmap(pixmap);
	widget->setMaskEnabled(this->m_config.m_isMaskEnabled);

	// > 绘制帧数
	widget->setFrameText(this->m_data.getIntervalString(i));
	
	return widget;
}
/*-------------------------------------------------
		控件 - 清理全部
*/
void P_AnimationListEditor::clearAll(){
	P_PictureSelector::clearAll();
	//（暂无）
}


/*-------------------------------------------------
		动画帧设置 - 设置参数
*/
void P_AnimationListEditor::setConfigParam_ALE(C_ALEConfig config){
	this->m_config_ALE = config;
	this->m_data.setData_IntervalDefault(config.m_defaultInterval);
	P_PictureSelector::setConfigParam(config);
}
/*-------------------------------------------------
		动画帧设置 - 取出参数
*/
C_ALEConfig P_AnimationListEditor::getConfigParam_ALE(){
	return this->m_config_ALE;
}
/*-------------------------------------------------
		动画帧设置 - 设置参数（不开放）
*/
void P_AnimationListEditor::setConfigParam(C_PiSConfig config){
	P_PictureSelector::setConfigParam(config);
}
/*-------------------------------------------------
		动画帧设置 - 取出参数（不开放）
*/
C_PiSConfig P_AnimationListEditor::getConfigParam(){
	return P_PictureSelector::getConfigParam();
}
/*-------------------------------------------------
		动画帧设置 - 窗口编辑ui设置
*/
void P_AnimationListEditor::openWindow_setConfigParam(){

	// > 弹出ui编辑框
	W_ALEConfigEdit d(this->m_listWidget);
	d.setDataInModifyMode(this->m_config_ALE, this->m_unit);
	if (d.exec() == QDialog::Accepted){
		C_ALEConfig config = d.getData();

		// > 设置后，强制变化统一默认帧间隔
		this->m_data.setIntervalDefaultWithFit(config.m_defaultInterval);

		// > 单位标记
		this->m_unit = (C_ALEData::DATA_UNIT)d.getDataUnit();
		this->m_data.setData_Unit(this->m_unit);
		
		// > 设置参数
		this->setConfigParam_ALE(config);
	}
}



/*-------------------------------------------------
		事件 - 右键事件（零个、单个、多个）
*/
void P_AnimationListEditor::event_itemRightClicked(QList<QListWidgetItem*> item_list){
	P_PictureSelector::event_itemRightClicked(item_list);
	QAction* action;
	QMenu* menu = new QMenu();

	/*-------------------------点击空白处-----------------------------*/
	if (item_list.count() == 0){
		
		QMenu* menu_add = new QMenu("添加帧", menu);
		menu_add->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));

			action = new QAction("添加图片", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
			action->setData(0);
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertInAction);
			menu_add->addAction(action);

			action = new QAction("添加GIF", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
			action->setData(0);
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertGIFInAction);
			menu_add->addAction(action);
		menu->addMenu(menu_add);

		if (this->m_config_ALE.m_isMultiSelect == true){
			QMenu* menu_select = new QMenu("选择帧", menu);
			menu_select->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));
			menu_select->setEnabled(false);
			menu->addMenu(menu_select);
		}

		action = new QAction("复制帧", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Copy.png"));
		action->setShortcut(QKeySequence::Copy);
		action->setEnabled(false);
		menu->addAction(action);

		action = new QAction("粘贴帧", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Paste.png"));
		action->setShortcut(QKeySequence::Paste);
		action->setEnabled(false);
		menu->addAction(action);

		action = new QAction("删除帧", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Delete.png"));
		action->setShortcut(QKeySequence::Delete);
		action->setEnabled(false);
		menu->addAction(action);

		if (this->m_config_ALE.m_isMultiSelect == true){
			menu->addSeparator();

			QMenu* menu_export = new QMenu("导出", menu);
			menu_export->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Export.png"));
			menu_export->setEnabled(false);
			menu->addMenu(menu_export);
		}

		menu->exec(QCursor::pos());
		
	/*-------------------------点击单个-----------------------------*/
	}else if (item_list.count() == 1){
		int pos = this->m_itemTank.indexOf( item_list.at(0) );
		if (pos == -1){ return; }

		action = new QAction("左移", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Left.png"));
		action->setData(pos);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_moveLeftInAction);
		menu->addAction(action);
		if (pos -1 < 0 ){ action->setEnabled(false); }

		action = new QAction("右移", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Right.png"));
		action->setData(pos);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_moveRightInAction);
		menu->addAction(action);
		if (pos + 1 >= this->m_itemTank.count()){ action->setEnabled(false); }

		menu->addSeparator();
		
		QMenu* menu_add = new QMenu("添加帧", menu);
		menu_add->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));

			action = new QAction("添加图片", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
			action->setData(pos);
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertInAction);
			menu_add->addAction(action);

			action = new QAction("添加GIF", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
			action->setData(pos);
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertGIFInAction);
			menu_add->addAction(action);
		menu->addMenu(menu_add);

		if (this->m_config_ALE.m_isMultiSelect == true){
			QMenu* menu_select = new QMenu("选择帧", menu);
			menu_select->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));

			action = new QAction("全选", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));
			action->setShortcut(QKeySequence::SelectAll);
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_selectAllInAction);
			menu_select->addAction(action);
			action = new QAction("选择奇数帧", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_selectOddInAction);
			menu_select->addAction(action);
			action = new QAction("选择偶数帧", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_selectEvenInAction);
			menu_select->addAction(action);

			menu->addMenu(menu_select);
		}

		action = new QAction("复制帧", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Copy.png"));
		action->setData( QString::number(pos) );
		action->setShortcut(QKeySequence::Copy);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_copyInAction);
		menu->addAction(action);

		action = new QAction("粘贴帧", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Paste.png"));
		action->setData( pos );
		action->setShortcut(QKeySequence::Paste);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_pasteInAction);
		menu->addAction(action);
		if (this->m_copyedList.count() == 0){ action->setEnabled(false); }

		action = new QAction("删除帧", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Delete.png"));
		action->setData(pos);
		action->setShortcut(QKeySequence::Delete);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_removeInAction);
		menu->addAction(action);

		menu->addSeparator();

		action = new QAction("编辑帧", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Edit.png"));
		action->setData(pos);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_editOneInAction);
		menu->addAction(action);

		if (this->m_config_ALE.m_isMultiSelect == true){
			menu->addSeparator();
			QMenu* menu_export = new QMenu("导出", menu);
			menu_export->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Export.png"));

			action = new QAction("导出 - 单图", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/ExportImage.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_exportSingle_PicInAction);
			menu_export->addAction(action);
			action = new QAction("导出 - 全部帧", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/ExportImage.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_exportAll_PicInAction);
			menu_export->addAction(action);
			action = new QAction("生成GIF - 单帧", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/ExportGIF.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_exportSelected_GIFInAction);
			menu_export->addAction(action);
			action = new QAction("生成GIF - 全部帧", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/ExportGIF.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_exportAll_GIFInAction);
			menu_export->addAction(action);

			menu->addMenu(menu_export);
		}

		menu->exec(QCursor::pos());
		
	/*-------------------------点击多个-----------------------------*/
	}else{
		int front_pos = 1000;
		QList<int> pos_list = QList<int>();
		for (int i = 0; i < item_list.count(); i++){
			QListWidgetItem* item = item_list.at(i);
			int pos = this->m_itemTank.indexOf(item);
			if (pos == -1){ continue; }
			if (pos < front_pos){ front_pos = pos; }
			pos_list.append( pos );
		}
		qSort(pos_list);		//（排序，防止删除时顺序乱了）
		QList<QString> pos_list_str = QList<QString>();
		for (int i = 0; i < pos_list.count(); i++){
			pos_list_str.append(QString::number(pos_list.at(i)));
		}

		QMenu* menu_add = new QMenu("添加帧", menu);
		menu_add->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));

			action = new QAction("添加图片", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
			action->setData(front_pos);
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertInAction);
			menu_add->addAction(action);

			action = new QAction("添加GIF", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
			action->setData(front_pos);
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertGIFInAction);
			menu_add->addAction(action);
		menu->addMenu(menu_add);

		if (this->m_config_ALE.m_isMultiSelect == true){
			QMenu* menu_select = new QMenu("选择帧", menu);
			menu_select->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));

			action = new QAction("全选", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));
			action->setShortcut(QKeySequence::SelectAll);
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_selectAllInAction);
			menu_select->addAction(action);
			action = new QAction("选择奇数帧", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_selectOddInAction);
			menu_select->addAction(action);
			action = new QAction("选择偶数帧", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_selectEvenInAction);
			menu_select->addAction(action);

			menu->addMenu(menu_select);
		}

		action = new QAction("复制帧", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Copy.png"));
		action->setData(pos_list_str.join(","));
		action->setShortcut(QKeySequence::Copy);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_copyInAction);
		menu->addAction(action);

		action = new QAction("粘贴帧", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Paste.png"));
		action->setData(front_pos);
		action->setShortcut(QKeySequence::Paste);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_pasteInAction);
		menu->addAction(action);
		if (this->m_copyedList.count() == 0){ action->setEnabled(false); }

		action = new QAction("删除帧", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Delete.png"));
		action->setData(pos_list_str.join(","));
		action->setShortcut(QKeySequence::Delete);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_removeInAction);
		menu->addAction(action);

		menu->addSeparator();

		action = new QAction("编辑帧时间", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Edit.png"));
		action->setData(pos_list_str.join(","));
		action->setShortcut(QKeySequence::Delete);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_editMultiInAction);
		menu->addAction(action);

		if (this->m_config_ALE.m_isMultiSelect == true){
			menu->addSeparator();
			QMenu* menu_export = new QMenu("导出", menu);
			menu_export->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Export.png"));

			action = new QAction("导出 - 选中帧", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/ExportImage.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_exportSelected_PicInAction);
			menu_export->addAction(action);
			action = new QAction("导出 - 全部帧", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/ExportImage.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_exportAll_PicInAction);
			menu_export->addAction(action);
			action = new QAction("生成GIF - 选中帧", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/ExportGIF.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_exportSelected_GIFInAction);
			menu_export->addAction(action);
			action = new QAction("生成GIF - 全部帧", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/ExportGIF.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_exportAll_GIFInAction);
			menu_export->addAction(action);

			menu->addMenu(menu_export);
		}

		menu->exec(QCursor::pos());
	}

}
/*-------------------------------------------------
		事件 - 圈选变化事件（单个、多个）
*/
void P_AnimationListEditor::event_itemSelectionChanged(QList<QListWidgetItem*> selected_item_list){
	P_PictureSelector::event_itemSelectionChanged(selected_item_list);
}


/*-------------------------------------------------
		资源数据 - 设置数据（继承）
*/
void P_AnimationListEditor::setSource(QList<QFileInfo> file_list) {
	P_PictureSelector::setSource(file_list);
}
void P_AnimationListEditor::setSource(QList<QPixmap> bitmap_list) {
	P_PictureSelector::setSource(bitmap_list);
}
/*-------------------------------------------------
		资源数据 - 设置数据
*/
void P_AnimationListEditor::setSource(C_ALEData data) {
	this->m_data = data;
	this->m_data.setData_Unit(this->m_unit);	//（单位标记）
	this->setSource(this->m_data.getAllFile());
}
/*-------------------------------------------------
		资源数据 - 获取数据（资源数据会被该块修改，需要随时获取并变化）
*/
C_ALEData P_AnimationListEditor::getSource(){
	this->m_data.setData_Unit(this->m_unit);	//（单位标记）
	return this->m_data;
}
/*-------------------------------------------------
		资源数据 - 设置单位
*/
void P_AnimationListEditor::setUnit(C_ALEData::DATA_UNIT unit){
	this->m_unit = unit;
}
/*-------------------------------------------------
		资源数据 - 获取单位
*/
C_ALEData::DATA_UNIT P_AnimationListEditor::getUnit(){
	return this->m_unit;
}


/*-------------------------------------------------
		操作 - 添加资源(append)
*/
void P_AnimationListEditor::op_append(QString gif_src){
	if (this->m_data.isNull()){ return; }

	// > 数据添加
	this->m_data.op_append(gif_src);
	int index = this->m_data.getFileCount() - 1;

	// > 图形添加
	QFileInfo info = this->m_data.getFile(index);
	QImage image = QImage(info.absoluteFilePath());
	QPixmap pixmap = QPixmap::fromImage(image);
	this->m_org_bitmapList.append(pixmap);

	// > 控件添加
	QListWidgetItem* item = this->createPictureItem();
	this->m_listWidget->addItem(item);
	this->m_itemTank.append(item);

	// > 建立控件
	QWidget* widget = this->createPictureWidget(index, pixmap);
	this->m_listWidget->setItemWidget(item, widget);
	this->m_widgetTank.append(widget);

	//（不需要刷）

	emit animBitmapChanged();
}
/*-------------------------------------------------
		操作 - 添加资源(insert)【此处 绕开了 insertItem 的bug，代码结构会比较复杂】
*/
void P_AnimationListEditor::op_insert(int index, QStringList gif_src_list, QList<int> interval_list){
	if (this->m_data.isNull()){ return; }
	if (gif_src_list.count() == 0){ return; }

	// > 数据添加
	this->m_data.op_insert(index, gif_src_list, interval_list);

	for (int i = 0; i < gif_src_list.count(); i++){

		// > 图形添加
		QFileInfo info = this->m_data.getFile(index + i);
		QImage image = QImage(info.absoluteFilePath());
		QPixmap pixmap = QPixmap::fromImage(image);
		this->m_org_bitmapList.insert(index + i, pixmap);

		// > 控件添加
		QListWidgetItem* item = this->createPictureItem();
		this->m_listWidget->addItem(item);		//【insertItem有bug，只能add】
		this->m_itemTank.append(item);

		// > 建立控件
		QWidget* widget = this->createPictureWidget(index, pixmap);
		this->m_listWidget->setItemWidget(item, widget);
		this->m_widgetTank.append(widget);
	}

	// > 强制全刷
	this->op_refreshAll(index);

	emit animBitmapChanged();
}
/*-------------------------------------------------
		操作 - 移除
*/
void P_AnimationListEditor::op_remove(QList<int> index_list){
	if (this->m_data.isNull()){ return; }
	qSort(index_list);	//（排序）

	// > 倒序删除
	int index = 0;
	for (int i = index_list.count() - 1; i >= 0; i--){
		index = index_list.at(i);

		// > 数据移除
		this->m_data.op_remove(index);

		// > 图形移除
		this->m_org_bitmapList.removeAt(index);

		// > 控件移除
		this->m_listWidget->takeItem(index);
		QListWidgetItem* item = this->m_itemTank.at(index);
		QWidget* widget = this->m_widgetTank.at(index);
		this->m_itemTank.removeAt(index);
		this->m_widgetTank.removeAt(index);
		delete item;
		delete widget;
	}

	// > 清除复制项
	this->m_copyedList.clear();

	// > 取消选择
	if (this->m_itemTank.count() == 0){
		emit allFrameDeleted();
	}else{
		this->selectIndex(index - 1);
	}

	emit animBitmapChanged();
}
/*-------------------------------------------------
		操作 - 交换位置
*/
void P_AnimationListEditor::op_swap(int index_a, int index_b){
	if (this->m_data.isNull()){ return; }
	if (index_a == index_b){ return; }

	// > 数据交换
	this->m_data.op_swap(index_a, index_b);

	// > 图形交换
	this->m_org_bitmapList.swap(index_a, index_b);
	
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

	emit animBitmapChanged();
}
/*-------------------------------------------------
		操作 - 刷新贴图内容
*/
void P_AnimationListEditor::op_refresh(int index){
	if (index < 0){ return; }
	if (index >= this->m_widgetTank.count() ){ return; }
	P_ALEBlock* widget = dynamic_cast<P_ALEBlock*>(this->m_widgetTank.at(index));
	if (this->m_config.m_zeroFill == true){		//（刷新计数）
		widget->setCountText(TTool::_zeroFill_(index + 1, this->m_config.m_zeroFillCount, QLatin1Char(this->m_config.m_zeroFillChar.toLatin1())));
	}else{
		widget->setCountText(QString::number(index + 1));
	}
	widget->setPixmap(this->m_org_bitmapList.at(index));	//（刷新图片）
	widget->setFrameText(this->m_data.getIntervalString(index));	//（刷新帧）
}
void P_AnimationListEditor::op_refreshAll(int startAt){
	for (int i = startAt; i < this->m_itemTank.count(); i++){
		this->op_refresh(i);
	}
}




/*-------------------------------------------------
		action - 添加帧（点击空白处）
*/
void P_AnimationListEditor::op_appendInAction(){

	bool cover_all = false;
	QStringList file_list = this->openWindow_getPicFileList();	//（打开窗口选择多张图片）
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
			C_ALEData::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
			this->op_append(file_name);
			continue;
		}

		// > 全部覆盖
		if (cover_all == true){
			C_ALEData::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
			this->op_append(file_name);
			continue;
		}

		// > 重复文件处理
		switch (QMessageBox::question(this->m_listWidget, "提示", "要导入的图片：\"" + file_name + "\"在配置中已存在，是否覆盖原文件？", "全部覆盖", "覆盖", "保持原图添加", 0)) {

			case 0:
				cover_all = true;
			case 1:
				C_ALEData::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
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
void P_AnimationListEditor::op_insertInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int pos = cur_action->data().value<int>();

	bool cover_all = false;
	QStringList file_list = this->openWindow_getPicFileList();	//（打开窗口选择多张图片）
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
			C_ALEData::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
			this->op_insert(pos + i, QStringList() << file_name);
			continue;
		}

		// > 全部覆盖
		if (cover_all == true){
			C_ALEData::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
			this->op_insert(pos + i, QStringList() << file_name);
			continue;
		}

		// > 重复文件处理
		switch (QMessageBox::question(this->m_listWidget, "提示", "要导入的图片：\"" + file_name + "\"在配置中已存在，是否覆盖原文件？", "全部覆盖", "覆盖", "保持原图添加", 0)) {

			case 0:
				cover_all = true;
			case 1:
				C_ALEData::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
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
void P_AnimationListEditor::op_insertGIFInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int pos = cur_action->data().value<int>();

	int readerMethod = 0;
	W_ALEGIFReaderType d(this->m_listWidget);
	if (d.exec() == QDialog::Accepted){
		readerMethod = d.getData();
	}else{
		return;
	}

	QStringList file_name_list = QStringList();
	QList<int> interval_list = QList<int>();
	QString file_src = this->openWindow_getGIFFile();
	if (file_src == ""){ return; }
	QFileInfo fileinfo_src = QFileInfo(file_src);

	// > 重复文件处理
	QFileInfo repeat_file = QFileInfo(this->m_data.getFileRoot() + "/" + fileinfo_src.completeBaseName() + "_0" + ".png");
	if (repeat_file.exists() == true){
		if( QMessageBox::question(this->m_listWidget, "提示", "要导入的GIF：\"" + fileinfo_src.completeBaseName() + "\"在配置中已存在，是否覆盖原文件？", "覆盖", "取消导入", 0) == 1) {
			return;
		}
	}

	// > QImageReader读取
	if (readerMethod == 0){

		// > 拆解GIF到目录
		bool successed = S_GIFManager::getInstance()->dismantlingGIF(fileinfo_src, QDir(this->m_data.getFileRoot() + "/"), "png", "%2_%1");
		if ( successed == false){
			QMessageBox::warning(this->m_listWidget, "错误", "GIF解析失败。", QMessageBox::Yes);
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
			QMessageBox::warning(this->m_listWidget, "错误", "GIF解析失败。", QMessageBox::Yes);
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
		action - 删除帧（单个和多个）
*/
void P_AnimationListEditor::op_removeInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString index_str = cur_action->data().value<QString>();
	QStringList index_str_list = index_str.split(",");

	QList<int> index_list = QList<int>();
	for (int i = 0; i < index_str_list.count(); i++){
		index_list.append(index_str_list.at(i).toInt());
	}

	this->op_remove(index_list);
}
/*-------------------------------------------------
		action - 全选（单个和多个）
*/
void P_AnimationListEditor::op_selectAllInAction(){
	this->m_listWidget->selectAll();
}
/*-------------------------------------------------
		action - 选奇数
*/
void P_AnimationListEditor::op_selectOddInAction(){
	QList<int> index_list = QList<int>();
	for (int i = 0; i < this->m_itemTank.count(); i++){
		if (i%2 == 0){
			index_list.append(i);
		}
	}
	this->selectIndex_Multi(index_list);
}
/*-------------------------------------------------
		action - 选偶数
*/
void P_AnimationListEditor::op_selectEvenInAction(){
	QList<int> index_list = QList<int>();
	for (int i = 0; i < this->m_itemTank.count(); i++){
		if (i % 2 == 1){
			index_list.append(i);
		}
	}
	this->selectIndex_Multi(index_list);
}
/*-------------------------------------------------
		action - 复制（单个和多个）
*/
void P_AnimationListEditor::op_copyInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString index_str = cur_action->data().value<QString>();

	QStringList index_list_str = index_str.split(",");
	QList<int> index_list = QList<int>();
	for (int i = 0; i < index_list_str.count(); i++ ){
		index_list.append( index_list_str.at(i).toInt() );
	}

	this->m_copyedList = this->m_data.getFile_Multi(index_list);
}
/*-------------------------------------------------
		action - 粘贴（单个和多个）
*/
void P_AnimationListEditor::op_pasteInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int pos = cur_action->data().value<int>();
	if (this->m_copyedList.count() == 0){ return; }

	QStringList file_name_list = QStringList();
	for (int i = 0; i < this->m_copyedList.count(); i++){
		file_name_list.append(this->m_copyedList.at(i).completeBaseName());
	}

	this->op_insert(pos, file_name_list);
}
/*-------------------------------------------------
		action - 左移
*/
void P_AnimationListEditor::op_moveLeftInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int index = cur_action->data().value<int>();
	if (index - 1 < 0){ return; }
	this->op_swap(index, index - 1);
	this->selectIndex(index - 1);	//（选中）
}
/*-------------------------------------------------
		action - 右移
*/
void P_AnimationListEditor::op_moveRightInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int index = cur_action->data().value<int>();
	if (index + 1 >= this->m_itemTank.count() ){ return; }
	this->op_swap(index, index + 1);
	this->selectIndex(index+1);	//（选中）
}
/*-------------------------------------------------
		action - 编辑帧
*/
void P_AnimationListEditor::op_editOneInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int index = cur_action->data().value<int>();

	// > 弹出对话框
	W_ALEDataEdit d(this->m_listWidget);
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
void P_AnimationListEditor::op_editMultiInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString index_str = cur_action->data().value<QString>();

	QStringList index_list_str = index_str.split(",");
	QList<int> index_list = QList<int>();
	for (int i = 0; i < index_list_str.count(); i++){
		index_list.append(index_list_str.at(i).toInt());
	}

	// > 弹出对话框
	W_ALEDataEdit d(this->m_listWidget);
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
void P_AnimationListEditor::setExportName(QString name){
	if (name == ""){ name = "动画"; }
	this->m_exportName = name;
}
/*-------------------------------------------------
		导出 - 导出单图
*/
void P_AnimationListEditor::op_exportPic(int index, QFileInfo target_file){
	QFileInfo file_from = this->m_data.getFile(index);
	if (file_from.exists() == false){ return; }

	P_FileOperater op = P_FileOperater();
	op.copy_File(file_from, target_file);
}
/*-------------------------------------------------
		导出 - 导出多图
*/
void P_AnimationListEditor::op_exportPic_Multi(QList<int> index_list, QDir target_dir){
	if (target_dir.exists() == false){ return; }
	P_FileOperater op = P_FileOperater();

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
void P_AnimationListEditor::op_exportGIF_Multi(QList<int> index_list, QFileInfo target_file, int writerMethod){

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

	QMessageBox::information(this->m_listWidget, "提示", "GIF导出成功。", QMessageBox::Yes);
}
/*-------------------------------------------------
		action - 导出单图
*/
void P_AnimationListEditor::op_exportSingle_PicInAction(){
	QList<int> selected_list = this->getSelectedIndex_Multi();
	if (selected_list.count() == 0){ return; }
	QString path = this->openWindow_exportPNGFile();
	if (path == ""){ return; }

	this->op_exportPic(selected_list.at(0), QFileInfo(path));
	QMessageBox::information(this->m_listWidget, "提示", "图片导出成功。", QMessageBox::Yes);
}
/*-------------------------------------------------
		action - 导出多图
*/
void P_AnimationListEditor::op_exportSelected_PicInAction(){
	QList<int> selected_list = this->getSelectedIndex_Multi();
	if (selected_list.count() == 0){ return; }
	QString path = this->openWindow_exportDir();
	if (path == ""){ return; }

	this->op_exportPic_Multi(selected_list, QDir(path));
	QMessageBox::information(this->m_listWidget, "提示", "图片集导出成功。", QMessageBox::Yes);
}
/*-------------------------------------------------
		action - 导出全部图
*/
void P_AnimationListEditor::op_exportAll_PicInAction(){
	QList<int> selected_list = QList<int>();
	for (int i = 0; i < this->m_itemTank.count(); i++){
		selected_list.append(i);
	}
	if (selected_list.count() == 0){ return; }
	QString path = this->openWindow_exportDir();
	if (path == ""){ return; }

	this->op_exportPic_Multi(selected_list, QDir(path));
	QMessageBox::information(this->m_listWidget, "提示", "图片集导出成功。", QMessageBox::Yes);
}
/*-------------------------------------------------
		action - 导出GIF - 选中项
*/
void P_AnimationListEditor::op_exportSelected_GIFInAction(){

	// > 选择项
	QList<int> selected_list = this->getSelectedIndex_Multi();
	if (selected_list.count() == 0){ QMessageBox::warning(this->m_listWidget, "提示", "需要选择至少一个动画帧才能导出。", QMessageBox::Yes); return; }

	// > 选择方式
	int writerMethod = 0;
	W_ALEGIFWriterType d(this->m_listWidget);
	if (d.exec() == QDialog::Accepted){
		writerMethod = d.getData();
	}else{
		return;
	}

	// > 选择存储文件
	QString path = this->openWindow_exportGIFFile();
	if (path == ""){ return; }

	this->op_exportGIF_Multi(selected_list, QFileInfo(path), writerMethod);
}
/*-------------------------------------------------
		action - 导出GIF - 全部项
*/
void P_AnimationListEditor::op_exportAll_GIFInAction(){

	// > 选择项
	QList<int> selected_list = QList<int>();
	for (int i = 0; i < this->m_itemTank.count(); i++){
		selected_list.append(i);
	}
	if (selected_list.count() == 0){ QMessageBox::warning(this->m_listWidget, "提示", "需要选择至少一个动画帧才能导出。", QMessageBox::Yes); return; }

	// > 选择方式
	int writerMethod = 0;
	W_ALEGIFWriterType d(this->m_listWidget);
	if (d.exec() == QDialog::Accepted){
		writerMethod = d.getData();
	}else{
		return;
	}

	// > 选择存储文件
	QString path = this->openWindow_exportGIFFile();
	if (path == ""){ return; }

	this->op_exportGIF_Multi(selected_list, QFileInfo(path), writerMethod);
}


/*-------------------------------------------------
		编辑窗口 - 选择多张图片（导入）
*/
QStringList P_AnimationListEditor::openWindow_getPicFileList(){
	QStringList result_list = QStringList();
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
		编辑窗口 - 选择GIF（导入）
*/
QString P_AnimationListEditor::openWindow_getGIFFile(){
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
		编辑窗口 - 选择GIF（导出）
*/
QString P_AnimationListEditor::openWindow_exportGIFFile(){
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
		编辑窗口 - 选择PNG（导出）
*/
QString P_AnimationListEditor::openWindow_exportPNGFile(){
	QString target_file = "";
	QFileDialog fd;
	fd.setWindowTitle("导出PNG");
	fd.setAcceptMode(QFileDialog::AcceptSave);
	fd.setDirectory(".");
	fd.setNameFilters(QStringList() << "动画帧(*.png)");
	fd.setViewMode(QFileDialog::Detail);
	fd.setFileMode(QFileDialog::ExistingFile);		//单个文件
	fd.selectFile(this->m_exportName+"_动画帧.png");
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()){ return ""; }
		target_file = fd.selectedFiles().at(0);
	}else {
		return "";
	}
	return target_file;
}
/*-------------------------------------------------
		编辑窗口 - 选择文件夹（导出）
*/
QString P_AnimationListEditor::openWindow_exportDir(){
	QString target_dir = "";
	QFileDialog fd;
	target_dir = fd.getExistingDirectory(this->m_listWidget, "导出到文件夹");
	return target_dir;
}


/*-------------------------------------------------
		快捷键 - 全选
*/
void P_AnimationListEditor::shortcut_selectAll(){
	if (this->m_listWidget->hasFocus() == false){ return; }
	this->m_listWidget->selectAll();

}
/*-------------------------------------------------
		快捷键 - 复制
*/
void P_AnimationListEditor::shortcut_copy(){
	if (this->m_listWidget->hasFocus() == false){ return; }

	QList<int> index_list = this->getSelectedIndex_Multi();
	this->m_copyedList = this->m_data.getFile_Multi(index_list);
}
/*-------------------------------------------------
		快捷键 - 粘贴
*/
void P_AnimationListEditor::shortcut_paste(){
	if (this->m_listWidget->hasFocus() == false){ return; }
	if (this->m_copyedList.count() == 0){ return; }

	QStringList file_name_list = QStringList();
	for (int i = 0; i < this->m_copyedList.count(); i++){
		file_name_list.append(this->m_copyedList.at(i).completeBaseName());
	}
	int pos = this->getSelectedIndex();
	this->op_insert(pos, file_name_list);
	
}
/*-------------------------------------------------
		快捷键 - 删除
*/
void P_AnimationListEditor::shortcut_delete(){
	if (this->m_listWidget->hasFocus() == false){ return; }
	QList<int> index_list = this->getSelectedIndex_Multi();
	this->op_remove(index_list);
}