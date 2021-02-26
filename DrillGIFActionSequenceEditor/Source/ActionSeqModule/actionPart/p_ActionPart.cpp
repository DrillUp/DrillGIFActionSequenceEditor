#include "stdafx.h"
#include "p_ActionPart.h"

#include "../actionSeqData/s_ActionSeqDataDefiner.h"
#include "../actionSeqData/s_ActionSeqDataContainer.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		动作元块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		该部分提供动作元编辑功能。
		
		目标：		1.

		使用方法：
				>初始化

-----==========================================================-----
*/
P_ActionPart::P_ActionPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数

	//-----------------------------------
	//----初始化ui
	this->m_last_index = -1;
	
	// > 编辑树
	this->m_table = new P_RadioTable(ui.tableWidget);
	QJsonObject obj_config = QJsonObject();
	obj_config.insert("zeroFillCount", 2);
	obj_config.insert("rowHeight", 22);
	this->m_table->setConfigParam(obj_config);	//固定参数

	// > 快速表单
	C_FastClass c_class = S_ActionSeqDataDefiner::getInstance()->getTable_Action();
	this->m_FastForm = new P_FastForm(ui.widget_form);
	this->m_FastForm->prepareFastClass(c_class);
	this->m_FastForm->rebuildUI();

	// > 动画帧
	this->m_p_AnimationListEditor = new P_AnimationListEditor(ui.listWidget);
	this->m_p_AnimationListEditor->setSource(C_ALEData());

	C_ALEConfig config = C_ALEConfig();
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
	connect(this->m_table, &P_RadioTable::currentIndexChanged, this, &P_ActionPart::currentIndexChanged);
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::selectedIndexChanged_Multi, this, &P_ActionPart::tableChanged_Multi);

	// > 图片查看块 - 连接帧切换
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::currentIndexChanged, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::setAnimFrame);
	// > 图片查看块 - 连接资源切换
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::animBitmapChanged, this, &P_ActionPart::bitmapChanged);
	// > 图片查看块 - 缩放
	connect(ui.toolButton_zoom_in, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomIn);
	connect(ui.toolButton_zoom_out, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomOut);
	connect(ui.toolButton_zoom_regular, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomReset);
	connect(this->m_p_AnimPictureViewer, &P_AnimPictureViewer::scaleChanged, this, &P_ActionPart::zoomValueChanged);

}

P_ActionPart::~P_ActionPart(){
}

/*-------------------------------------------------
		动画帧 - 选项变化
*/
void P_ActionPart::tableChanged_Multi(QList<int> index_list){
	//（暂无操作）
}
/*-------------------------------------------------
		动画帧 - 资源切换
*/
void P_ActionPart::bitmapChanged(){
	C_ALEData data = this->m_p_AnimationListEditor->getSource();
	this->m_p_AnimPictureViewer->setSource(data.getAllFile());
}
/*-------------------------------------------------
		动画帧 - 缩放比例切换
*/
void P_ActionPart::zoomValueChanged(double value){
	ui.label_zoomValue->setText( QString::number(value * 100)+"%" );
}

/*-------------------------------------------------
		控件 - 获取动作元名称
*/
QStringList P_ActionPart::getNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_actionDataList.count(); i++){
		result.append(this->local_actionDataList.at(i).value("动作元名称").toString());
	}
	return result;
}

/*-------------------------------------------------
		控件 - 动作元切换
*/
void P_ActionPart::currentIndexChanged(int index){

	// > 旧的内容存储
	this->local_saveCurIndexData();

	// > 填入新的内容
	this->local_loadIndexData(index);
}


/*-------------------------------------------------
		快捷键 - 事件
*/
void P_ActionPart::keyPressEvent(QKeyEvent *event){
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
void P_ActionPart::local_saveCurIndexData(){
	if (this->m_last_index < 0){ return; }
	if (this->m_last_index > this->local_actionDataList.count()){ return; }

	// > 表单数据
	QJsonObject obj_edit = this->m_FastForm->getJsonObject();
	QJsonObject obj_org = this->local_actionDataList.at(this->m_last_index);
	TTool::_QJsonObject_put_(&obj_org, obj_edit);

	// > 图片数据
	QJsonObject obj_org2 = QJsonObject();
	//....

	TTool::_QJsonObject_put_(&obj_org, obj_edit);

	this->local_actionDataList.replace(this->m_last_index, obj_org);
}
/*-------------------------------------------------
		数据 - 读取本地数据
*/
void P_ActionPart::local_loadIndexData(int index){
	if (index < 0){ return; }
	if (index > this->local_actionDataList.count()){ return; }

	// > 表单数据
	QJsonObject obj_data = this->local_actionDataList.at(index);
	this->m_FastForm->setJsonObjectAutoDefault(obj_data);
	qDebug() << obj_data;

	// > 图片数据
		int gif_interval = obj_data.value("帧间隔").toString().toInt();					//帧间隔
		QString gif_src_file = S_ActionSeqDataContainer::getInstance()->getActionSeqDir();				//资源文件夹
	
		QString gif_src_str = obj_data.value("资源-动作元").toString();
		QList<QString> gif_src = TTool::_QJsonArrayString_To_QListQString_(gif_src_str);	//资源文件名
		QString gif_intervalTank_str = obj_data.value("帧间隔-明细表").toString();
		QList<int> gif_intervalTank = TTool::_QJsonArrayString_To_QListInt_(gif_intervalTank_str);;		//帧间隔-明细表

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
void P_ActionPart::setData(QList<QJsonObject> actionData) {
	this->local_actionDataList = actionData;
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
QList<QJsonObject> P_ActionPart::getData(){
	this->putUiToData();
	return this->local_actionDataList;
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_ActionPart::putDataToUi() {
	
	// > 名称列表
	this->m_table->setSource(this->getNameList());

	// > 当前选中的数据
	this->local_loadIndexData(this->m_last_index);
}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_ActionPart::putUiToData() {

	// > 保存当前数据
	this->local_saveCurIndexData();

}
