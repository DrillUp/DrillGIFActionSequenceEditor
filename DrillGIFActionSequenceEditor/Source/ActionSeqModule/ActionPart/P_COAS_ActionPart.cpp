#include "stdafx.h"
#include "P_COAS_ActionPart.h"

#include "../Data/S_ActionSeqDataContainer.h"
#include "Source/ProjectModule/S_ProjectManager.h"
#include "Source/Utils/WidgetForm/QStringListEditor/W_QStringListEditor.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		动作元块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		该部分提供动作元编辑功能。
					【该功能块是一个大集合组件，功能交织复杂，多注意注释部分】
		
		子功能：	->控件
						->单选表格
						->快速表单
					->控件（动画帧）
						->动画帧编辑块
						->动画帧播放器
						->图片查看块
					->快捷键
						->复制
						->粘贴
						->清空
					->本地数据
						->列表
						->索引
						->窗口交互
						
		使用方法：	见 P_COAS_DataPart 动画序列块对此块的初始化。
-----==========================================================-----
*/
P_COAS_ActionPart::P_COAS_ActionPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->m_last_index = -1;
	this->m_slotBlock_source = false;
	this->m_curTagTank.clear();
	this->m_errorMessage.clear();

	//-----------------------------------
	//----初始化ui

	// > 编辑表格
	this->m_table = new P_RadioTable(ui.tableWidget);
	C_RaTConfig rat_config = C_RaTConfig();
	rat_config.zeroFillCount = 2;
	rat_config.rowHeight = 22;
	this->m_table->setConfigParam(rat_config);			//固定参数
	this->m_table->setItemOuterControlEnabled(true);	//开启右键菜单

	// > 动画帧
	this->m_p_AnimationListEditor = new P_ALE_Editor(ui.listWidget);

	C_ALE_Config config = C_ALE_Config();
	this->m_p_AnimationListEditor->setConfigParam_ALE(config);

	// > 动画帧播放器
	this->m_p_AnimationListPlayer = new P_AnimationListPlayer();
	this->m_p_AnimationListPlayer->setAnimationListEditor(m_p_AnimationListEditor);
	this->m_p_AnimationListPlayer->setPlayType(QStringList() << "播放一次" << "播放两次" << "播放三次");
	QHBoxLayout* layout = new QHBoxLayout(ui.groupBox_animPlayer);
	ui.groupBox_animPlayer->setLayout(layout);
	layout->setMargin(3);
	layout->addWidget(this->m_p_AnimationListPlayer);

	// > 图片查看块
	this->m_p_AnimPictureViewer = new P_AnimPictureViewer(ui.widget_view);
	this->m_p_AnimPictureViewer->rebuildUI();

	//-----------------------------------
	//----事件绑定
	connect(this->m_table, &P_RadioTable::currentIndexChanged, this, &P_COAS_ActionPart::currentIndexChanged);
	connect(this->m_table, &P_RadioTable::menuPasteItemTriggered, this, &P_COAS_ActionPart::shortcut_pasteData);
	connect(this->m_table, &P_RadioTable::menuCopyItemTriggered, this, &P_COAS_ActionPart::shortcut_copyData);
	connect(this->m_table, &P_RadioTable::menuClearItemTriggered, this, &P_COAS_ActionPart::shortcut_clearData);
	connect(this->m_p_AnimationListEditor, &P_ALE_Editor::signal_selectedIndexChanged_Multiple, this, &P_COAS_ActionPart::tableChanged_Multi);

	// > 标签列表编辑
	connect(ui.pushButton_tagTank, &QPushButton::clicked, this, &P_COAS_ActionPart::btn_editTagTank);

	// > 表单变化绑定
	connect(ui.lineEdit_name, &QLineEdit::textEdited, this, &P_COAS_ActionPart::nameEdited);
	connect(ui.lineEdit_name, &QLineEdit::textChanged, this->m_p_AnimationListEditor, &P_ALE_Editor::setCustomExportName);
	connect(ui.checkBox_gif_back_run, &QCheckBox::toggled, this->m_p_AnimationListPlayer, &P_AnimationListPlayer::setBackRun);			//正向设置
	connect(this->m_p_AnimationListPlayer, &P_AnimationListPlayer::signal_backRunChanged, this, &P_COAS_ActionPart::backRunChanged);	//反向被设置

	// > 图片查看块 - 连接帧切换
	connect(this->m_p_AnimationListEditor, &P_ALE_Editor::signal_currentIndexChanged_Single, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::setAnimFrame);
	// > 图片查看块 - 连接资源切换
	connect(this->m_p_AnimationListEditor, &P_ALE_Editor::signal_picListChanged, this, &P_COAS_ActionPart::bitmapChanged);
	connect(this->m_p_AnimationListEditor, &P_ALE_Editor::signal_dataSetReloaded, this, &P_COAS_ActionPart::bitmapChanged);
	// > 图片查看块 - 着色器
	connect(ui.horizontalSlider_tint, &QAbstractSlider::valueChanged, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::setTint);
	// > 图片查看块 - 缩放
	connect(ui.toolButton_zoom_in, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomIn);
	connect(ui.toolButton_zoom_out, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomOut);
	connect(ui.toolButton_zoom_regular, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomReset);
	connect(this->m_p_AnimPictureViewer, &P_AnimPictureViewer::scaleChanged, this, &P_COAS_ActionPart::zoomValueChanged);

}

P_COAS_ActionPart::~P_COAS_ActionPart(){
}


/*-------------------------------------------------
		控件 - 表单变化
*/
void P_COAS_ActionPart::nameEdited(QString name){

	// > 状态节点列表变化
	this->m_table->modifyText_Selected(name);

	// > 立即刷新名称到当前数据
	if (this->m_last_index != -1){
		C_COAS_ActionPtr action_ptr = this->m_actionDataList.at(this->m_last_index);
		action_ptr->name = name;
	}
}
/*-------------------------------------------------
		控件 - 倒放勾选框变化
*/
void P_COAS_ActionPart::backRunChanged(bool checked){
	if (ui.checkBox_gif_back_run->isChecked() == checked){ return; }
	ui.checkBox_gif_back_run->setChecked(checked);
}
/*-------------------------------------------------
		控件 - 修改标签列表
*/
void P_COAS_ActionPart::btn_editTagTank(){
	W_QStringListEditor d(this);
	d.setDataInModifyMode(this->m_curTagTank);
	if (d.exec() == QDialog::Accepted){
		this->m_curTagTank = d.getData();
		this->refreshTagTank();
	}
}
/*-------------------------------------------------
		控件 - 刷新标签显示
*/
void P_COAS_ActionPart::refreshTagTank(){
	QString context;
	context.append("[");
	context.append(this->m_curTagTank.join(","));
	context.append("]");
	ui.label_tagTank->setText(context);
}


/*-------------------------------------------------
		动画帧 - 选项变化
*/
void P_COAS_ActionPart::tableChanged_Multi(QList<int> index_list){
	//（暂无操作）
}
/*-------------------------------------------------
		动画帧 - 资源切换
*/
void P_COAS_ActionPart::bitmapChanged(){
	QStringList pic_list = this->m_p_AnimationListEditor->getCurrentData_PicList();
	QList<QFileInfo> file_list;
	for (int i = 0; i < pic_list.count(); i++){
		file_list.append(QFileInfo(pic_list.at(i)));
	}
	this->m_p_AnimPictureViewer->setSource(file_list);
}
/*-------------------------------------------------
		动画帧 - 缩放比例切换
*/
void P_COAS_ActionPart::zoomValueChanged(double value){
	ui.label_zoomValue->setText( QString::number(value * 100)+"%" );
}

/*-------------------------------------------------
		控件 - 获取动作元名称
*/
QStringList P_COAS_ActionPart::getNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->m_actionDataList.count(); i++){
		C_COAS_ActionPtr action_ptr = this->m_actionDataList.at(i);
		result.append(action_ptr->name);
	}
	return result;
}

/*-------------------------------------------------
		控件 - 动作元切换
*/
void P_COAS_ActionPart::currentIndexChanged(int index){
	if (this->m_slotBlock_source == true){ return; }

	// > 旧的内容存储
	this->local_saveCurIndexData();

	// > 填入新的内容
	this->local_loadIndexData(index);
}


/*-------------------------------------------------
		快捷键 - 事件
*/
void P_COAS_ActionPart::keyPressEvent(QKeyEvent *event){

	// > 动画帧编辑块 的快捷键
	bool success = this->m_p_AnimationListEditor->event_shortcut_keyPress(event);
	if (success){ return; }

	// > 当前 快捷键
	if (event->modifiers() & Qt::ControlModifier){
		if (event->key() == Qt::Key_C){
			this->shortcut_copyData();
		}
		if (event->key() == Qt::Key_V){
			this->shortcut_pasteData();
		}
		if (event->key() == Qt::Key_A){
			//..
		}
	}
	if (event->key() == Qt::Key_Delete){
		this->shortcut_clearData();
	}
}
/*-------------------------------------------------
		操作 - 替换
*/
void P_COAS_ActionPart::op_replace(int index, QJsonObject action_data){
	if (index < 0){ return; }
	if (index >= this->m_table->count()){ return; }
	if (action_data.isEmpty()){ return; }

	// > 编辑标记
	S_ProjectManager::getInstance()->setDirty();

	// > 执行替换
	C_COAS_ActionPtr action_ptr = this->m_actionDataList.at(index);
	action_ptr->setJsonObject_Chinese(action_data,index);
	this->local_loadIndexData(index);

	// > 更新表格的名称
	this->m_table->modifyText_Selected(ui.lineEdit_name->text());
}
/*-------------------------------------------------
		操作 - 清空
*/
void P_COAS_ActionPart::op_clear(int index){
	if (index < 0){ return; }
	if (index >= this->m_table->count()){ return; }

	// > 编辑标记
	S_ProjectManager::getInstance()->setDirty();

	// > 执行替换
	C_COAS_ActionPtr action_ptr = this->m_actionDataList.at(index);
	action_ptr->clearData();
	this->local_loadIndexData(index);

	// > 更新表格的名称
	this->m_table->modifyText_Selected(ui.lineEdit_name->text());
}
/*-------------------------------------------------
		快捷键 - 复制
*/
void P_COAS_ActionPart::shortcut_copyData(){
	if (ui.tableWidget->hasFocus() == false){ return; }
	if (this->m_last_index == -1 ){ return; }
	C_COAS_ActionPtr action_ptr = this->m_actionDataList.at(m_last_index);
	this->m_copyed_data = action_ptr->getJsonObject_Chinese();
	this->m_table->setItemOuterControl_PasteActive(true);		//激活粘贴
}
/*-------------------------------------------------
		快捷键 - 粘贴
*/
void P_COAS_ActionPart::shortcut_pasteData(){
	if (ui.tableWidget->hasFocus() == false){ return; }
	if (this->m_last_index == -1){ return; }
	this->op_replace(this->m_last_index, this->m_copyed_data);
}
/*-------------------------------------------------
		快捷键 - 清空
*/
void P_COAS_ActionPart::shortcut_clearData(){
	if (ui.tableWidget->hasFocus() == false){ return; }
	if (this->m_last_index == -1){ return; }
	this->op_clear(this->m_last_index);
}


/*-------------------------------------------------
		数据 - 保存本地数据
*/
void P_COAS_ActionPart::local_saveCurIndexData(){
	if (this->m_last_index < 0){ return; }
	if (this->m_last_index >= this->m_actionDataList.count()){ return; }

	// > 直接对数据指针进行赋值
	C_COAS_ActionPtr action_ptr = this->m_actionDataList.at(this->m_last_index);

	// > 常规
	action_ptr->name = ui.lineEdit_name->text();
	action_ptr->tag_tank = this->m_curTagTank;
	action_ptr->priority = ui.spinBox_priority->value();

	// > GIF
	C_ALE_DataSet ALE_data = this->m_p_AnimationListEditor->getDataSet_ALE();
	action_ptr->gif_src.clear();
	QStringList pic_list = ALE_data.getData_PicList();	//（资源名列表）
	for (int i = 0; i < pic_list.count(); i++){
		action_ptr->gif_src.append(QFileInfo(pic_list.at(i)).completeBaseName());
	}
	//		（资源文件夹，不需赋值）
	action_ptr->gif_interval = ALE_data.getData_IntervalDefault();
	action_ptr->gif_intervalTank = ALE_data.getData_IntervalTank();
	action_ptr->gif_back_run = ui.checkBox_gif_back_run->isChecked();
	action_ptr->gif_preload = ui.checkBox_gif_preload->isChecked();
	
	// > 图像
	action_ptr->tint = ui.horizontalSlider_tint->value();
	action_ptr->smooth = ui.checkBox_smooth->isChecked();

	// > 杂项
	action_ptr->note = ui.plainTextEdit_note->toPlainText();


	// > 编辑标记
	S_ProjectManager::getInstance()->setDirty();
}
/*-------------------------------------------------
		数据 - 读取本地数据
*/
void P_COAS_ActionPart::local_loadIndexData(int index){
	if (index < 0){ return; }
	if (index >= this->m_actionDataList.count()){ return; }

	// > 直接数据指针中读取值
	C_COAS_ActionPtr action_ptr = this->m_actionDataList.at(index);

	// > 常规
	ui.lineEdit_name->setText(action_ptr->name);
	this->m_curTagTank = action_ptr->tag_tank;
	this->refreshTagTank();
	ui.spinBox_priority->setValue(action_ptr->priority);

	// > GIF
	QString gif_src_file = S_ActionSeqDataContainer::getInstance()->getActionSeqDir();
	C_ALE_DataSet data = C_ALE_DataSet();
	data.setSource(gif_src_file, action_ptr->gif_src);
	data.setInterval(action_ptr->gif_interval, action_ptr->gif_intervalTank);
	this->m_p_AnimationListEditor->setDataSet_ALE(data);
	this->m_p_AnimationListEditor->selectStart();
	ui.checkBox_gif_back_run->setChecked(action_ptr->gif_back_run);
	ui.checkBox_gif_preload->setChecked(action_ptr->gif_preload);

	// > 图像
	ui.horizontalSlider_tint->setValue(action_ptr->tint);
	ui.checkBox_smooth->setChecked(action_ptr->smooth);

	// > 杂项
	ui.plainTextEdit_note->setPlainText(action_ptr->note);

	this->m_last_index = index;
}



/*-------------------------------------------------
		数据检查 - 执行检查
*/
void P_COAS_ActionPart::checkData_ActionDataList(QList<C_COAS_ActionPtr> actionDataList){
	this->m_errorMessage.clear();

	// > 空校验
	// （不校验）

	// > 重名校验
	QStringList name_list;
	QStringList repeatName_list;
	for (int i = 0; i < actionDataList.count(); i++){
		C_COAS_ActionPtr action_ptr = actionDataList.at(i);
		QString name = action_ptr->name;
		if (name == ""){ continue; }
		if (name_list.contains(name)){
			if (repeatName_list.contains(name)){ continue; }
			repeatName_list.append(name);
		}
		name_list.append(name);
	}
	if (repeatName_list.count() > 0){
		this->m_errorMessage.append("存在重复的动作元名称：" + repeatName_list.join(","));
	}

}
/*-------------------------------------------------
		数据检查 - 获取检查信息
*/
QStringList P_COAS_ActionPart::checkData_getErrorMessage(){
	return this->m_errorMessage;
}

/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_COAS_ActionPart::setData(QList<C_COAS_ActionPtr> actionDataList) {
	this->m_slotBlock_source = true;

	// > 动作元表格
	this->m_actionDataList = actionDataList;
	if (this->m_actionDataList.count() == 0){
		int data_count = S_ActionSeqDataContainer::getInstance()->getActionSeqLength().realLen_action;
		for (int i = 0; i < data_count; i++){
			this->m_actionDataList.append(C_COAS_ActionPtr::create());
		}
	}

	this->m_p_AnimationListPlayer->stopFrame();		//（设置数据时，暂停播放）
	this->putDataToUi();
	this->m_slotBlock_source = false;
	this->m_table->selectStart();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
QList<C_COAS_ActionPtr> P_COAS_ActionPart::getData(){
	this->putUiToData();
	return this->m_actionDataList;
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_COAS_ActionPart::putDataToUi() {

	// > 名称列表
	this->m_table->setSource(this->getNameList());

	// > 当前选中的数据
	this->local_loadIndexData(this->m_last_index);
}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_COAS_ActionPart::putUiToData() {

	// > 保存当前数据
	this->local_saveCurIndexData();

}
