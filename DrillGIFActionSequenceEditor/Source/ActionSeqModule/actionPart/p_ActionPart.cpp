#include "stdafx.h"
#include "p_ActionPart.h"

/*
-----==========================================================-----
		�ࣺ		����Ԫ��.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ò����ṩ����Ԫ�༭���ܡ�
		
		Ŀ�꣺		1.

		ʹ�÷�����
				>��ʼ��

-----==========================================================-----
*/
P_ActionPart::P_ActionPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������

	//-----------------------------------
	//----��ʼ��ui
	
	// > �༭��
	this->m_table = new P_RadioTable(ui.tableWidget);
	QJsonObject obj_config = QJsonObject();
	obj_config.insert("zeroFillCount", 2);
	obj_config.insert("rowHeight", 22);
	this->m_table->setConfigParam(obj_config);	//�̶�����

	// > ����֡
	C_ALEData data = C_ALEData();
	data.setId(10);
	data.setSource("F:/rpg mv��/mog�������ȫ����(Drill_up)v2.41/�������ʾ��/img/enemies/", QList<QString>()
		<< "С����˿001"
		<< "С����˿002"
		<< "С����˿003"
		<< "С����˿004"
		<< "С����˿005"
		<< "С����˿006"
		<< "С����˿004"
		<< "С����˿006");
	data.setInterval(3, QList<int>() << 6 << 5 << 4 << 3 << 2 << 1);

	this->m_p_AnimationListEditor = new P_AnimationListEditor(ui.listWidget);
	this->m_p_AnimationListEditor->setSource(data);

	C_ALEConfig config = C_ALEConfig();
	this->m_p_AnimationListEditor->setConfigParam_ALE(config);

	// > ����֡������
	this->m_p_AnimationListPlayer = new P_AnimationListPlayer();
	this->m_p_AnimationListPlayer->setAnimationListEditor(m_p_AnimationListEditor);
	this->m_p_AnimationListPlayer->setPlayType(QStringList() << "����һ��" << "��������" << "��������");
	QHBoxLayout* layout = new QHBoxLayout(ui.groupBox_animPlayer);
	ui.groupBox_animPlayer->setLayout(layout);
	layout->setMargin(3);
	layout->addWidget(this->m_p_AnimationListPlayer);

	// > ͼƬ�鿴��
	this->m_p_AnimPictureViewer = new P_AnimPictureViewer(ui.widget_view);
	this->m_p_AnimPictureViewer->rebuildUI();
	this->m_p_AnimPictureViewer->setSource(data.getAllFile());

	//-----------------------------------
	//----�¼���
	connect(this->m_table, &P_RadioTable::currentIndexChanged, this, &P_ActionPart::currentIndexChanged);
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::selectedIndexChanged_Multi, this, &P_ActionPart::tableChanged_Multi);

	// > ͼƬ�鿴�� - ����֡�л�
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::currentIndexChanged, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::setAnimFrame);
	// > ͼƬ�鿴�� - ������Դ�л�
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::animBitmapChanged, this, &P_ActionPart::bitmapChanged);
	// > ͼƬ�鿴�� - ����
	connect(ui.toolButton_zoom_in, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomIn);
	connect(ui.toolButton_zoom_out, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomOut);
	connect(ui.toolButton_zoom_regular, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomReset);
	connect(this->m_p_AnimPictureViewer, &P_AnimPictureViewer::scaleChanged, this, &P_ActionPart::zoomValueChanged);

}

P_ActionPart::~P_ActionPart(){
}

/*-------------------------------------------------
		����֡ - ѡ��仯
*/
void P_ActionPart::tableChanged_Multi(QList<int> index_list){
	QString text = "";
	for (int i = 0; i < index_list.count(); i++){
		text += QString::number( index_list.at(i) + 1 )+ "/";
	}
	//ui.label->setText("��ѡ���ˣ�" + text);
}
/*-------------------------------------------------
		����֡ - ��Դ�л�
*/
void P_ActionPart::bitmapChanged(){
	C_ALEData data = this->m_p_AnimationListEditor->getSource();
	this->m_p_AnimPictureViewer->setSource(data.getAllFile());
}
/*-------------------------------------------------
		����֡ - ���ű����л�
*/
void P_ActionPart::zoomValueChanged(double value){
	ui.label_zoomValue->setText( QString::number(value * 100)+"%" );
}

/*-------------------------------------------------
		�ؼ� - ��ȡ����Ԫ����
*/
QStringList P_ActionPart::getNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_actionDataList.count(); i++){
		result.append(this->local_actionDataList.at(i).value("����Ԫ����").toString());
	}
	return result;
}

/*-------------------------------------------------
		�ؼ� - ����Ԫ�л�
*/
void P_ActionPart::currentIndexChanged(int index){

	// > �ɵ����ݴ洢


	// > �����µ�����
	ui.lineEdit_name->setText(this->getNameList().at(index));
}


/*-------------------------------------------------
		��ݼ� - �¼�
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
		���� - ��������
*/
void P_ActionPart::setData(QList<QJsonObject> actionData) {
	this->local_actionDataList = actionData;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ȡ������
*/
QList<QJsonObject> P_ActionPart::getData(){
	this->putUiToData();

	// > У��
	//...
	
	return this->local_actionDataList;
}
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void P_ActionPart::putDataToUi() {
	
	this->m_table->setSource(this->getNameList());

}
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_ActionPart::putUiToData() {
	

}
