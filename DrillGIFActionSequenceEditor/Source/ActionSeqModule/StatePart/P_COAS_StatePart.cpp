#include "stdafx.h"
#include "P_COAS_StatePart.h"

#include "../Data/S_ActionSeqDataContainer.h"
#include "Source/ProjectModule/S_ProjectManager.h"
#include "Source/Utils/WidgetForm/QStringListEditor/W_QStringListEditor.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		状态元块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		该部分提供状态元编辑功能。
					【该功能块是一个大集合组件，功能交织复杂，多注意注释部分】
		
		目标：		->控件
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

		使用方法：
				>初始化

-----==========================================================-----
*/
P_COAS_StatePart::P_COAS_StatePart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->m_last_index = -1;
	this->m_slotBlock_source = false;
	this->m_curTagTank.clear();

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
	this->m_p_AnimationListEditor = new P_AnimationListEditor(ui.listWidget);
	this->m_p_AnimationListEditor->setSource(C_ALEData());

	C_ALEConfig config = C_ALEConfig();
	this->m_p_AnimationListEditor->setConfigParam_ALE(config);

	// > 动画帧播放器
	this->m_p_AnimationListPlayer = new P_AnimationListPlayer();
	this->m_p_AnimationListPlayer->setAnimationListEditor(m_p_AnimationListEditor);
	this->m_p_AnimationListPlayer->setPlayType(QStringList() << "永久循环");
	QHBoxLayout* layout = new QHBoxLayout(ui.groupBox_animPlayer);
	ui.groupBox_animPlayer->setLayout(layout);
	layout->setMargin(3);
	layout->addWidget(this->m_p_AnimationListPlayer);

	// > 图片查看块
	this->m_p_AnimPictureViewer = new P_AnimPictureViewer(ui.widget_view);
	this->m_p_AnimPictureViewer->rebuildUI();

	//-----------------------------------
	//----事件绑定
	connect(this->m_table, &P_RadioTable::currentIndexChanged, this, &P_COAS_StatePart::currentIndexChanged);
	connect(this->m_table, &P_RadioTable::menuPasteItemTriggered, this, &P_COAS_StatePart::shortcut_pasteData);
	connect(this->m_table, &P_RadioTable::menuCopyItemTriggered, this, &P_COAS_StatePart::shortcut_copyData);
	connect(this->m_table, &P_RadioTable::menuClearItemTriggered, this, &P_COAS_StatePart::shortcut_clearData);
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::selectedIndexChanged_Multi, this, &P_COAS_StatePart::tableChanged_Multi);

	// > 标签列表编辑
	connect(ui.pushButton_tagTank, &QPushButton::clicked, this, &P_COAS_StatePart::btn_editTagTank);

	// > 表单变化绑定
	connect(ui.lineEdit_name, &QLineEdit::textEdited, this->m_table, &P_RadioTable::modifyText_Selected);
	connect(ui.lineEdit_name, &QLineEdit::textChanged, this->m_p_AnimationListEditor, &P_AnimationListEditor::setExportName);
	connect(ui.checkBox_gif_back_run, &QCheckBox::toggled, this->m_p_AnimationListPlayer, &P_AnimationListPlayer::setPlayBackRun);

	// > 图片查看块 - 连接帧切换
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::currentIndexChanged, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::setAnimFrame);
	// > 图片查看块 - 连接资源切换
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::animBitmapChanged, this, &P_COAS_StatePart::bitmapChanged);
	// > 图片查看块 - 着色器
	connect(ui.horizontalSlider_tint, &QAbstractSlider::valueChanged, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::setTint );
	// > 图片查看块 - 缩放
	connect(ui.toolButton_zoom_in, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomIn);
	connect(ui.toolButton_zoom_out, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomOut);
	connect(ui.toolButton_zoom_regular, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomReset);
	connect(this->m_p_AnimPictureViewer, &P_AnimPictureViewer::scaleChanged, this, &P_COAS_StatePart::zoomValueChanged);

}

P_COAS_StatePart::~P_COAS_StatePart(){
}


/*-------------------------------------------------
		控件 - 修改标签列表
*/
void P_COAS_StatePart::btn_editTagTank(){
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
void P_COAS_StatePart::refreshTagTank(){
	QString context;
	context.append("[");
	context.append(this->m_curTagTank.join(","));
	context.append("]");
	ui.label_tagTank->setText(context);
}

/*-------------------------------------------------
		动画帧 - 选项变化
*/
void P_COAS_StatePart::tableChanged_Multi(QList<int> index_list){
	//（暂无操作）
}
/*-------------------------------------------------
		动画帧 - 资源切换
*/
void P_COAS_StatePart::bitmapChanged(){
	C_ALEData data = this->m_p_AnimationListEditor->getSource();
	this->m_p_AnimPictureViewer->setSource(data.getAllFile());
}
/*-------------------------------------------------
		动画帧 - 缩放比例切换
*/
void P_COAS_StatePart::zoomValueChanged(double value){
	ui.label_zoomValue->setText( QString::number(value * 100)+"%" );
}

/*-------------------------------------------------
		控件 - 获取状态元名称
*/
QStringList P_COAS_StatePart::getNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->m_stateDataList.count(); i++){
		result.append(this->m_stateDataList.at(i).value("状态元名称").toString());
	}
	return result;
}

/*-------------------------------------------------
		控件 - 状态元切换
*/
void P_COAS_StatePart::currentIndexChanged(int index){
	if (this->m_slotBlock_source == true){ return; }

	// > 旧的内容存储
	this->local_saveCurIndexData();

	// > 填入新的内容
	this->local_loadIndexData(index);
}



/*-------------------------------------------------
		快捷键 - 事件
*/
void P_COAS_StatePart::keyPressEvent(QKeyEvent *event){
	if (event->modifiers() & Qt::ControlModifier){
		if (event->key() == Qt::Key_C){
			this->m_p_AnimationListEditor->shortcut_copy();
			this->shortcut_copyData();
		}
		if (event->key() == Qt::Key_V){
			this->m_p_AnimationListEditor->shortcut_paste();
			this->shortcut_pasteData();
		}
		if (event->key() == Qt::Key_A){
			this->m_p_AnimationListEditor->shortcut_selectAll();
		}
	}
	if (event->key() == Qt::Key_Delete){
		this->m_p_AnimationListEditor->shortcut_delete();
		this->shortcut_clearData();
	}
}
/*-------------------------------------------------
		操作 - 替换
*/
void P_COAS_StatePart::op_replace(int index, QJsonObject state){
	if (index < 0){ return; }
	if (index >= this->m_table->count()){ return; }
	if (state.isEmpty()){ return; }

	// > 编辑标记
	S_ProjectManager::getInstance()->setDirty();

	// > 执行替换
	this->m_stateDataList.replace(index, state);
	this->local_loadIndexData(index);

	// > 更新表格的名称
	this->m_table->modifyText_Selected(ui.lineEdit_name->text());
}
/*-------------------------------------------------
		操作 - 清空
*/
void P_COAS_StatePart::op_clear(int index){
	if (index < 0){ return; }
	if (index >= this->m_table->count()){ return; }

	// > 编辑标记
	S_ProjectManager::getInstance()->setDirty();

	// > 执行替换
	this->m_stateDataList.replace(index, QJsonObject());
	this->local_loadIndexData(index);

	// > 更新表格的名称
	this->m_table->modifyText_Selected(ui.lineEdit_name->text());
}
/*-------------------------------------------------
		快捷键 - 复制
*/
void P_COAS_StatePart::shortcut_copyData(){
	if (ui.tableWidget->hasFocus() == false){ return; }
	if (this->m_last_index == -1 ){ return; }
	this->m_copyed_data = this->m_stateDataList.at(m_last_index);
	this->m_table->setItemOuterControl_PasteActive(true);		//激活粘贴
}
/*-------------------------------------------------
		快捷键 - 粘贴
*/
void P_COAS_StatePart::shortcut_pasteData(){
	if (ui.tableWidget->hasFocus() == false){ return; }
	if (this->m_last_index == -1){ return; }
	this->op_replace(this->m_last_index, this->m_copyed_data);
}
/*-------------------------------------------------
		快捷键 - 清空
*/
void P_COAS_StatePart::shortcut_clearData(){
	if (ui.tableWidget->hasFocus() == false){ return; }
	if (this->m_last_index == -1){ return; }
	this->op_clear(this->m_last_index);
}


/*-------------------------------------------------
		数据 - 保存本地数据
*/
void P_COAS_StatePart::local_saveCurIndexData(){
	if (this->m_last_index < 0){ return; }
	if (this->m_last_index >= this->m_stateDataList.count()){ return; }

	// > 表单数据
		QJsonObject obj_edit;
		obj_edit.insert("状态元名称", ui.lineEdit_name->text());
		obj_edit.insert("状态元标签", TTool::_JSON_stringify_(this->m_curTagTank));
		obj_edit.insert("状态元优先级", QString::number(ui.spinBox_priority->value()));
		obj_edit.insert("状态元权重", QString::number(ui.spinBox_proportion->value()));
		obj_edit.insert("可被动作元打断", ui.checkBox_canBeInterrupted->isChecked() ? "true" : "false");
		obj_edit.insert("是否倒放", ui.checkBox_gif_back_run->isChecked() ? "true" : "false");
		obj_edit.insert("是否预加载", ui.checkBox_gif_preload->isChecked() ? "true" : "false");
		obj_edit.insert("图像-色调值", QString::number(ui.horizontalSlider_tint->value()));
		obj_edit.insert("图像-模糊边缘", ui.checkBox_smooth->isChecked() ? "true" : "false");
		obj_edit.insert("备注", ui.plainTextEdit_note->toPlainText());
	QJsonObject obj_org = this->m_stateDataList.at(this->m_last_index);
	TTool::_QJsonObject_put_(&obj_org, obj_edit);

	// > 图片数据
		C_ALEData data = this->m_p_AnimationListEditor->getSource();
		QJsonObject obj_edit2 = QJsonObject();
		obj_edit2.insert("帧间隔", QString::number( data.getData_IntervalDefault() ));
		//（资源文件夹不需要）
		QList<QString> gif_src = QList<QString>();
		QList<QFileInfo> info_list = data.getAllFile();
		for (int i = 0; i < info_list.count(); i++){
			gif_src.append(info_list.at(i).completeBaseName());
		}
		obj_edit2.insert("资源-状态元", TTool::_JSON_stringify_(gif_src));
		QList<int> gif_intervalTank = data.getData_IntervalTank();
		QList<QString> gif_intervalTank_strList = TTool::_QList_IntToQString_(gif_intervalTank);
		obj_edit2.insert("帧间隔-明细表", TTool::_JSON_stringify_(gif_intervalTank_strList));
		//qDebug() << obj_edit2;
	TTool::_QJsonObject_put_(&obj_org, obj_edit2);

	// > 编辑标记
	S_ProjectManager::getInstance()->setDirty();

	this->m_stateDataList.replace(this->m_last_index, obj_org);
}
/*-------------------------------------------------
		数据 - 读取本地数据
*/
void P_COAS_StatePart::local_loadIndexData(int index){
	if (index < 0){ return; }
	if (index >= this->m_stateDataList.count()){ return; }

	// > 表单数据
	QJsonObject obj_data = this->m_stateDataList.at(index);
	//qDebug() << obj_data;
		ui.lineEdit_name->setText(obj_data.value("状态元名称").toString());
		this->m_curTagTank = TTool::_JSON_parse_To_QListQString_(obj_data.value("状态元标签").toString());
		this->refreshTagTank();
		ui.spinBox_priority->setValue(obj_data.value("状态元优先级").toString().toInt());
		ui.spinBox_proportion->setValue(obj_data.value("状态元权重").toString().toInt());
		ui.checkBox_canBeInterrupted->setChecked(obj_data.value("可被动作元打断").toString() == "true");
		ui.checkBox_gif_back_run->setChecked(obj_data.value("是否倒放").toString() == "true");
		ui.checkBox_gif_preload->setChecked(obj_data.value("是否预加载").toString() == "true");
		ui.horizontalSlider_tint->setValue(obj_data.value("图像-色调值").toString().toInt());
		ui.checkBox_smooth->setChecked(obj_data.value("图像-模糊边缘").toString() == "true");
		ui.plainTextEdit_note->setPlainText(obj_data.value("备注").toString());

	// > 图片数据
		int gif_interval = obj_data.value("帧间隔").toString().toInt();								//帧间隔
		QString gif_src_file = S_ActionSeqDataContainer::getInstance()->getActionSeqDir();			//资源文件夹
	
		QString gif_src_str = obj_data.value("资源-状态元").toString();
		QList<QString> gif_src = TTool::_JSON_parse_To_QListQString_(gif_src_str);			//资源文件名
		QString gif_intervalTank_str = obj_data.value("帧间隔-明细表").toString();
		QList<QString> gif_intervalTank_strList = TTool::_JSON_parse_To_QListQString_(gif_intervalTank_str);	//帧间隔-明细表
		QList<int> gif_intervalTank = TTool::_QList_QStringToInt_(gif_intervalTank_strList);

	C_ALEData data = C_ALEData();
	data.setData_Id(index);
	data.setSource(gif_src_file, gif_src);
	data.setInterval(gif_interval, gif_intervalTank);
	this->m_p_AnimationListEditor->setSource(data);
	this->m_p_AnimationListEditor->selectStart();

	this->m_last_index = index;
}



/*-------------------------------------------------
		数据检查 - 执行检查
*/
void P_COAS_StatePart::checkData_StateDataList(QList<QJsonObject> stateDataList){
	this->m_errorMessage.clear();

	// > 空校验
	// （不校验）

	// > 重名校验
	QStringList name_list;
	QStringList repeatName_list;
	for (int i = 0; i < stateDataList.count(); i++){
		QJsonObject stateData = stateDataList.at(i);
		QString name = stateData["状态元名称"].toString();
		if (name == ""){ continue; }
		if (name_list.contains(name)){
			if (repeatName_list.contains(name)){ continue; }
			repeatName_list.append(name);
		}
		name_list.append(name);
	}
	if (repeatName_list.count() > 0){
		this->m_errorMessage.append("存在重复的状态元名称：" + repeatName_list.join(","));
	}

}
/*-------------------------------------------------
		数据检查 - 获取检查信息
*/
QStringList P_COAS_StatePart::checkData_getErrorMessage(){
	return this->m_errorMessage;
}
/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_COAS_StatePart::setData(QList<QJsonObject> stateDataList) {
	this->m_slotBlock_source = true;

	// > 状态元表格
	this->m_stateDataList = stateDataList;
	if (this->m_stateDataList.count() == 0){
		int data_count = S_ActionSeqDataContainer::getInstance()->getActionSeqLength().realLen_state;
		for (int i = 0; i < data_count; i++){
			this->m_stateDataList.append(QJsonObject());
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
QList<QJsonObject> P_COAS_StatePart::getData(){
	this->putUiToData();

	// > 校验
	//...
	
	return this->m_stateDataList;
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_COAS_StatePart::putDataToUi() {

	// > 名称列表
	this->m_table->setSource(this->getNameList());

	// > 当前选中的数据
	this->local_loadIndexData(this->m_last_index);
}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_COAS_StatePart::putUiToData() {

	// > 保存当前数据
	this->local_saveCurIndexData();

}
