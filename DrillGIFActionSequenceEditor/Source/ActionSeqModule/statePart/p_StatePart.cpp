#include "stdafx.h"
#include "p_StatePart.h"

#include "../actionSeqData/s_ActionSeqDataDefiner.h"
#include "../actionSeqData/s_ActionSeqDataContainer.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		状态元块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		该部分提供状态元编辑功能。
		
		目标：		1.

		使用方法：
				>初始化

-----==========================================================-----
*/
P_StatePart::P_StatePart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->m_last_index = -1;
	this->m_slotBlock_source = false;

	//-----------------------------------
	//----初始化ui

	// > 编辑表格
	this->m_table = new P_RadioTable(ui.tableWidget);
	C_RaTConfig rat_config = C_RaTConfig();
	rat_config.zeroFillCount = 2;
	rat_config.rowHeight = 22;
	this->m_table->setConfigParam(rat_config);	//固定参数

	// > 快速表单
	C_FastClass c_class = S_ActionSeqDataDefiner::getInstance()->getTable_State();
	this->m_FastForm = new P_FastForm(ui.widget_form);
	this->m_FastForm->prepareFastClass(c_class);
	this->m_FastForm->rebuildUI();

	// > 动画帧
	this->m_p_AnimationListEditor = new P_AnimationListEditor(ui.listWidget);
	this->m_p_AnimationListEditor->setSource(C_ALEData());

	C_ALEConfig config = C_ALEConfig();
	this->m_p_AnimationListEditor->setConfigParam_ALE(config);

	// > 播放器
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
	connect(this->m_table, &P_RadioTable::currentIndexChanged, this, &P_StatePart::currentIndexChanged);
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::selectedIndexChanged_Multi, this, &P_StatePart::tableChanged_Multi);

	// > 表单变化绑定
	connect(this->m_FastForm->getQLineEditByName("状态元名称"), &QLineEdit::textEdited, this->m_table, &P_RadioTable::modifyText_Selected);
	connect(this->m_FastForm->getQCheckBoxByName("是否倒放"), &QCheckBox::toggled, this->m_p_AnimationListPlayer, &P_AnimationListPlayer::setPlayBackRun);

	// > 图片查看块 - 连接帧切换
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::currentIndexChanged, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::setAnimFrame);
	// > 图片查看块 - 连接资源切换
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::animBitmapChanged, this, &P_StatePart::bitmapChanged);
	// > 图片查看块 - 缩放
	connect(ui.toolButton_zoom_in, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomIn);
	connect(ui.toolButton_zoom_out, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomOut);
	connect(ui.toolButton_zoom_regular, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomReset);
	connect(this->m_p_AnimPictureViewer, &P_AnimPictureViewer::scaleChanged, this, &P_StatePart::zoomValueChanged);

}

P_StatePart::~P_StatePart(){
}

/*-------------------------------------------------
		动画帧 - 选项变化
*/
void P_StatePart::tableChanged_Multi(QList<int> index_list){
	QString text = "";
	for (int i = 0; i < index_list.count(); i++){
		text += QString::number( index_list.at(i) + 1 )+ "/";
	}
	//ui.label->setText("你选择了：" + text);
}
/*-------------------------------------------------
		动画帧 - 资源切换
*/
void P_StatePart::bitmapChanged(){
	C_ALEData data = this->m_p_AnimationListEditor->getSource();
	this->m_p_AnimPictureViewer->setSource(data.getAllFile());
}
/*-------------------------------------------------
		动画帧 - 缩放比例切换
*/
void P_StatePart::zoomValueChanged(double value){
	ui.label_zoomValue->setText( QString::number(value * 100)+"%" );
}

/*-------------------------------------------------
		控件 - 获取状态元名称
*/
QStringList P_StatePart::getNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_stateDataList.count(); i++){
		result.append(this->local_stateDataList.at(i).value("状态元名称").toString());
	}
	return result;
}

/*-------------------------------------------------
		控件 - 动作元切换
*/
void P_StatePart::currentIndexChanged(int index){
	if (this->m_slotBlock_source == true){ return; }

	// > 旧的内容存储
	this->local_saveCurIndexData();

	// > 填入新的内容
	this->local_loadIndexData(index);
}



/*-------------------------------------------------
		快捷键 - 事件
*/
void P_StatePart::keyPressEvent(QKeyEvent *event){
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
}


/*-------------------------------------------------
		数据 - 保存本地数据
*/
void P_StatePart::local_saveCurIndexData(){
	if (this->m_last_index < 0){ return; }
	if (this->m_last_index >= this->local_stateDataList.count()){ return; }

	// > 表单数据
	QJsonObject obj_edit = this->m_FastForm->getJsonObject();
	QJsonObject obj_org = this->local_stateDataList.at(this->m_last_index);
	TTool::_QJsonObject_put_(&obj_org, obj_edit);

	// > 图片数据
		C_ALEData data = this->m_p_AnimationListEditor->getSource();
		QJsonObject obj_edit2 = QJsonObject();
		obj_edit2.insert("帧间隔", QString::number( data.getIntervalDefault() ));
		//（资源文件夹不需要）
		QList<QString> gif_src = QList<QString>();
		QList<QFileInfo> info_list = data.getAllFile();
		for (int i = 0; i < info_list.count(); i++){
			gif_src.append(info_list.at(i).completeBaseName());
		}
		obj_edit2.insert("资源-状态元", TTool::_QListQString_To_QJsonArrayString_(gif_src));
		QList<int> gif_intervalTank = data.getAllInterval();
		QList<QString> gif_intervalTank_strList = TTool::_QList_IntToQString_(gif_intervalTank);
		obj_edit2.insert("帧间隔-明细表", TTool::_QListQString_To_QJsonArrayString_(gif_intervalTank_strList));
		//qDebug() << obj_edit2;
	TTool::_QJsonObject_put_(&obj_org, obj_edit2);

	this->local_stateDataList.replace(this->m_last_index, obj_org);
}
/*-------------------------------------------------
		数据 - 读取本地数据
*/
void P_StatePart::local_loadIndexData(int index){
	if (index < 0){ return; }
	if (index >= this->local_stateDataList.count()){ return; }

	// > 表单数据
	QJsonObject obj_data = this->local_stateDataList.at(index);
	this->m_FastForm->setJsonObjectAutoDefault(obj_data);
	//qDebug() << obj_data;

	// > 图片数据
		int gif_interval = obj_data.value("帧间隔").toString().toInt();								//帧间隔
		QString gif_src_file = S_ActionSeqDataContainer::getInstance()->getActionSeqDir();			//资源文件夹
	
		QString gif_src_str = obj_data.value("资源-状态元").toString();
		QList<QString> gif_src = TTool::_QJsonArrayString_To_QListQString_(gif_src_str);			//资源文件名
		QString gif_intervalTank_str = obj_data.value("帧间隔-明细表").toString();
		QList<QString> gif_intervalTank_strList = TTool::_QJsonArrayString_To_QListQString_(gif_intervalTank_str);	//帧间隔-明细表
		QList<int> gif_intervalTank = TTool::_QList_QStringToInt_(gif_intervalTank_strList);

	C_ALEData data = C_ALEData();
	data.setId(index);
	data.setSource(gif_src_file, gif_src);
	data.setInterval(gif_interval, gif_intervalTank);
	this->m_p_AnimationListEditor->setSource(data);
	this->m_p_AnimationListEditor->selectStart();

	this->m_last_index = index;
}



/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_StatePart::setData(QList<QJsonObject> state) {
	this->m_slotBlock_source = true;
	this->local_stateDataList = state;
	this->m_p_AnimationListPlayer->stopFrame();		//（设置数据时，暂停播放）
	this->putDataToUi();
	this->m_slotBlock_source = false;
	this->m_table->selectStart();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
QList<QJsonObject> P_StatePart::getData(){
	this->putUiToData();

	// > 校验
	//...
	
	return this->local_stateDataList;
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_StatePart::putDataToUi() {

	// > 名称列表
	this->m_table->setSource(this->getNameList());

	// > 当前选中的数据
	this->local_loadIndexData(this->m_last_index);
}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_StatePart::putUiToData() {

	// > 保存当前数据
	this->local_saveCurIndexData();

}
