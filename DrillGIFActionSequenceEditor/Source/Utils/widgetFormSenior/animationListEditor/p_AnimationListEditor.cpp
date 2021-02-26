#include "stdafx.h"
#include "p_AnimationListEditor.h"

#include "private/p_ALEBlock.h"
#include "private/w_ALEDataEdit.h"
#include "private/w_ALEConfigEdit.h"

#include "Source/Utils/widgetForm/pictureSelector/p_PictureSelector.h"
#include "Source/Utils/manager/GIFManager/s_GIFManager.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		动画帧编辑块.cpp
		版本：		v1.01
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

	// > 数据
	this->setConfigParam(C_ALEConfig());

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
	this->m_data.setIntervalDefault(config.m_defaultInterval);
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
	d.setDataInModifyMode(this->m_config_ALE);
	if (d.exec() == QDialog::Accepted){
		C_ALEConfig config = d.getData();

		// > 设置后，强制变化统一默认帧间隔
		this->m_data.setIntervalDefaultAndChange(config.m_defaultInterval);
		
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

		action = new QAction("添加帧", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_appendInAction);
		menu->addAction(action);

		action = new QAction("添加帧(GIF)", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
		action->setData(0);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertGIFInAction);
		menu->addAction(action);

		action = new QAction("全选", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));
		action->setShortcut(QKeySequence::SelectAll);
		action->setEnabled(false);
		menu->addAction(action);

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

		action = new QAction("添加帧", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertInAction);
		menu->addAction(action);

		action = new QAction("添加帧(GIF)", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertGIFInAction);
		menu->addAction(action);

		action = new QAction("全选", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));
		action->setShortcut(QKeySequence::SelectAll);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_selectAllInAction);
		menu->addAction(action);
		if (this->m_config_ALE.m_isMultiSelect == false){ action->setEnabled(false); }

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

		action = new QAction("添加帧", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
		action->setData(front_pos);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertInAction);
		menu->addAction(action);

		action = new QAction("添加帧(GIF)", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
		action->setData(front_pos);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertGIFInAction);
		menu->addAction(action);

		action = new QAction("全选", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));
		action->setShortcut(QKeySequence::SelectAll);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_selectAllInAction);
		menu->addAction(action);
		if (this->m_config_ALE.m_isMultiSelect == false){ action->setEnabled(false); }

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
	this->setSource(this->m_data.getAllFile());
}
/*-------------------------------------------------
		资源数据 - 获取数据（资源数据会被该块修改，需要随时获取并变化）
*/
C_ALEData P_AnimationListEditor::getSource(){
	return this->m_data;
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

	QStringList file_list = this->openWindow_getPicFileList();
	for (int i = 0; i < file_list.count(); i++){
		QFileInfo fileinfo = QFileInfo(file_list.at(i));
		if (fileinfo.exists() == false){ continue; }

		// > 重复文件
		QString file_name = fileinfo.completeBaseName();
		if (this->m_data.hasFileName(file_name) ||						//含有重名文件 
			this->m_data.getFileRoot() == fileinfo.absolutePath() ){	//在同一个文件夹下
			this->op_append(file_name);
			continue;
		}
		
		// > 不重复文件
		C_ALEData::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
		this->op_append(file_name);
	}
}
/*-------------------------------------------------
		action - 添加帧（点击帧时）
*/
void P_AnimationListEditor::op_insertInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int pos = cur_action->data().value<int>();

	QStringList file_list = this->openWindow_getPicFileList();
	for (int i = 0; i < file_list.count(); i++){
		QFileInfo fileinfo = QFileInfo(file_list.at(i));
		if (fileinfo.exists() == false){ continue; }

		// > 重复文件
		QString file_name = fileinfo.completeBaseName();
		if (this->m_data.hasFileName(file_name) ||						//含有重名文件 
			this->m_data.getFileRoot() == fileinfo.absolutePath()){		//在同一个文件夹下
			this->op_insert(pos + i, QStringList() << file_name);
			continue;
		}

		// > 不重复文件
		C_ALEData::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
		this->op_insert(pos + i, QStringList() << file_name);
	}
}
/*-------------------------------------------------
		action - 添加帧（GIF）
*/
void P_AnimationListEditor::op_insertGIFInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int pos = cur_action->data().value<int>();

	// > 拆解GIF到目录
	QString file = this->openWindow_getGIFFile();
	bool successed = S_GIFManager::getInstance()->dismantlingGIF(QFileInfo(file), QDir(this->m_data.getFileRoot()+"/"),"png","%2_%1");
	if (successed == false){
		QMessageBox::warning(this->m_listWidget, "错误", "GIF解析失败。", QMessageBox::Yes);
		return;
	}

	// > 获取文件名
	QList<int> interval_list = S_GIFManager::getInstance()->getLastDismantledGIFIntervalList_divideTen();
	QList<QFileInfo> file_list = S_GIFManager::getInstance()->getLastDismantledGIFFileList();
	QStringList file_name_list = QStringList();
	for (int i = 0; i <file_list.count(); i++){
		file_name_list.append(file_list.at(i).completeBaseName());
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

	this->m_copyedList = this->m_data.getFileList(index_list);
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
		编辑窗口 - 选择多张图片
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
		编辑窗口 - 选择GIF
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
	this->m_copyedList = this->m_data.getFileList(index_list);
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