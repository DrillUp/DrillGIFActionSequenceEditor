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
		�ࣺ		����֡�༭��.cpp
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��ͼƬȫ����ʾ�����ܵ�ѡ/��ѡ��
					��ע��һ��������źţ�allFrameDeleted��ȫ��֡��ɾ�������

		�ӹ��ܣ�	-> �ı�ѡ��
						-> ��ѡ
						-> ��ѡ
						-> �Զ����
					-> ����֡����
						-> ���óߴ�ģʽ
						-> ����Ҷ����ʾ�ı�
					-> ��Դ�������
						-> �Ҽ��˵�
						-> ��ӡ��滻��ɾ��
						-> ���ơ�ճ��
					-> �༭֡
						-> ��֡�༭��ͼ��֡��
						-> ��֡�༭��֡��
					-> �༭UI����
					
		ʹ�÷�����
				> ��ʼ������Ҫ���ݣ�
					this->m_p_AnimationListEditor = new P_AnimationListEditor(ui.listWidget);
					this->m_p_AnimationListEditor->setSource(data);

				> ���ݳ�ʼ��
					C_ALEData data = C_ALEData();
					data.setId(10);
					data.setSource(	"F:/rpg mv��/mog�������ȫ����(Drill_up)v2.41 - ����/�������ʾ��/img/enemies/", 
									QList<QString>() << "С����˿001" << "С����˿002" << "С����˿003" << "С����˿004" << "С����˿005" << "С����˿006" << "С����˿004" << "С����˿006");
					data.setInterval( 3, QList<int>() << 6 << 5 << 4 << 3 << 2 << 1);

				> �޸�����
					C_ALEConfig config = C_ALEConfig();		//ע�⣬C_PiSConfigҲ���Ե����ã�����Ҫ������ѡ���ܡ�
					config ...
					this->m_p_AnimationListEditor->setConfigParam(config);
					
-----==========================================================-----
*/
P_AnimationListEditor::P_AnimationListEditor(QListWidget *parent)
	: P_PictureSelector(parent)
{

	//-----------------------------------
	//----������ʼ��
	this->m_iconSrcPath = ":/DrillGIFActionSequenceEditor/Resources/icons";
	
	// > ����
	this->setConfigParam(C_ALEConfig());

}
P_AnimationListEditor::~P_AnimationListEditor(){
}


/*-------------------------------------------------
		�ؼ� - ÿ����ؼ�����д��
*/
QWidget* P_AnimationListEditor::createPictureWidget(int i, QPixmap pixmap){
	int item_height = this->m_config.getItemHeight();

	// > ����ͼƬ��
	P_ALEBlock* widget = new P_ALEBlock(item_height, item_height, this->m_listWidget);
	if (this->m_config.m_zeroFill == true){
		widget->setCountText(TTool::_zeroFill_(i + 1, this->m_config.m_zeroFillCount, QLatin1Char(this->m_config.m_zeroFillChar.toLatin1())));
	}else{
		widget->setCountText(QString::number(i + 1));
	}

	// > ����ͼƬ
	widget->setPixmap(pixmap);
	widget->setMaskEnabled(this->m_config.m_isMaskEnabled);

	// > ����֡��
	widget->setFrameText(this->m_data.getIntervalString(i));
	
	return widget;
}
/*-------------------------------------------------
		�ؼ� - ����ȫ��
*/
void P_AnimationListEditor::clearAll(){
	P_PictureSelector::clearAll();
	//�����ޣ�
}


/*-------------------------------------------------
		����֡���� - ���ò���
*/
void P_AnimationListEditor::setConfigParam_ALE(C_ALEConfig config){
	this->m_config_ALE = config;
	this->m_data.setIntervalDefault(config.m_defaultInterval);
	P_PictureSelector::setConfigParam(config);
}
/*-------------------------------------------------
		����֡���� - ȡ������
*/
C_ALEConfig P_AnimationListEditor::getConfigParam_ALE(){
	return this->m_config_ALE;
}
/*-------------------------------------------------
		����֡���� - ���ò����������ţ�
*/
void P_AnimationListEditor::setConfigParam(C_PiSConfig config){
	P_PictureSelector::setConfigParam(config);
}
/*-------------------------------------------------
		����֡���� - ȡ�������������ţ�
*/
C_PiSConfig P_AnimationListEditor::getConfigParam(){
	return P_PictureSelector::getConfigParam();
}
/*-------------------------------------------------
		����֡���� - ���ڱ༭ui����
*/
void P_AnimationListEditor::openWindow_setConfigParam(){

	// > ����ui�༭��
	W_ALEConfigEdit d(this->m_listWidget);
	d.setDataInModifyMode(this->m_config_ALE);
	if (d.exec() == QDialog::Accepted){
		C_ALEConfig config = d.getData();

		// > ���ú�ǿ�Ʊ仯ͳһĬ��֡���
		this->m_data.setIntervalDefaultAndChange(config.m_defaultInterval);
		
		// > ���ò���
		this->setConfigParam_ALE(config);
	}
}



/*-------------------------------------------------
		�¼� - �Ҽ��¼�������������������
*/
void P_AnimationListEditor::event_itemRightClicked(QList<QListWidgetItem*> item_list){
	P_PictureSelector::event_itemRightClicked(item_list);
	QAction* action;
	QMenu* menu = new QMenu();

	/*-------------------------����հ״�-----------------------------*/
	if (item_list.count() == 0){

		action = new QAction("���֡", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_appendInAction);
		menu->addAction(action);

		action = new QAction("���֡(GIF)", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
		action->setData(0);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertGIFInAction);
		menu->addAction(action);

		action = new QAction("����֡", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Copy.png"));
		action->setShortcut(QKeySequence::Copy);
		action->setEnabled(false);
		menu->addAction(action);

		action = new QAction("ճ��֡", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Paste.png"));
		action->setShortcut(QKeySequence::Paste);
		action->setEnabled(false);
		menu->addAction(action);

		action = new QAction("ɾ��֡", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Delete.png"));
		action->setShortcut(QKeySequence::Delete);
		action->setEnabled(false);
		menu->addAction(action);

		menu->exec(QCursor::pos());
		
	/*-------------------------�������-----------------------------*/
	}else if (item_list.count() == 1){
		int pos = this->m_itemTank.indexOf( item_list.at(0) );
		if (pos == -1){ return; }

		action = new QAction("����", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Left.png"));
		action->setData(pos);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_moveLeftInAction);
		menu->addAction(action);
		if (pos -1 < 0 ){ action->setEnabled(false); }

		action = new QAction("����", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Right.png"));
		action->setData(pos);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_moveRightInAction);
		menu->addAction(action);
		if (pos + 1 >= this->m_itemTank.count()){ action->setEnabled(false); }

		menu->addSeparator();

		action = new QAction("���֡", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertInAction);
		menu->addAction(action);

		action = new QAction("���֡(GIF)", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
		action->setData(pos);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertGIFInAction);
		menu->addAction(action);

		action = new QAction("����֡", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Copy.png"));
		action->setData( QString::number(pos) );
		action->setShortcut(QKeySequence::Copy);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_copyInAction);
		menu->addAction(action);

		action = new QAction("ճ��֡", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Paste.png"));
		action->setData( pos );
		action->setShortcut(QKeySequence::Paste);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_pasteInAction);
		menu->addAction(action);
		if (this->m_copyedList.count() == 0){ action->setEnabled(false); }

		action = new QAction("ɾ��֡", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Delete.png"));
		action->setData(pos);
		action->setShortcut(QKeySequence::Delete);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_removeInAction);
		menu->addAction(action);

		menu->addSeparator();

		action = new QAction("�༭֡", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Edit.png"));
		action->setData(pos);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_editOneInAction);
		menu->addAction(action);

		menu->exec(QCursor::pos());
		
	/*-------------------------������-----------------------------*/
	}else{
		int front_pos = 1000;
		QList<QString> pos_list = QList<QString>();

		for (int i = 0; i < item_list.count(); i++){
			QListWidgetItem* item = item_list.at(i);
			int pos = this->m_itemTank.indexOf(item);
			if (pos == -1){ continue; }
			if (pos < front_pos){ front_pos = pos; }
			pos_list.append( QString::number(pos) );
		}
		qSort(pos_list);		//�����򣬷�ֹɾ��ʱ˳�����ˣ�

		action = new QAction("���֡", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
		action->setData(front_pos);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertInAction);
		menu->addAction(action);

		action = new QAction("���֡(GIF)", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
		action->setData(front_pos);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertGIFInAction);
		menu->addAction(action);

		action = new QAction("����֡", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Copy.png"));
		action->setData(pos_list.join(","));
		action->setShortcut(QKeySequence::Copy);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_copyInAction);
		menu->addAction(action);

		action = new QAction("ճ��֡", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Paste.png"));
		action->setData(front_pos);
		action->setShortcut(QKeySequence::Paste);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_pasteInAction);
		menu->addAction(action);
		if (this->m_copyedList.count() == 0){ action->setEnabled(false); }

		action = new QAction("ɾ��֡", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Delete.png"));
		action->setData(pos_list.join(","));
		action->setShortcut(QKeySequence::Delete);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_removeInAction);
		menu->addAction(action);

		menu->addSeparator();

		action = new QAction("�༭֡ʱ��", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Edit.png"));
		action->setData(pos_list.join(","));
		action->setShortcut(QKeySequence::Delete);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_editMultiInAction);
		menu->addAction(action);

		menu->exec(QCursor::pos());
	}

}
/*-------------------------------------------------
		�¼� - Ȧѡ�仯�¼��������������
*/
void P_AnimationListEditor::event_itemSelectionChanged(QList<QListWidgetItem*> selected_item_list){
	P_PictureSelector::event_itemSelectionChanged(selected_item_list);
}


/*-------------------------------------------------
		��Դ���� - �������ݣ��̳У�
*/
void P_AnimationListEditor::setSource(QList<QFileInfo> file_list) {
	P_PictureSelector::setSource(file_list);
}
void P_AnimationListEditor::setSource(QList<QPixmap> bitmap_list) {
	P_PictureSelector::setSource(bitmap_list);
}
/*-------------------------------------------------
		��Դ���� - ��������
*/
void P_AnimationListEditor::setSource(C_ALEData data) {
	this->m_data = data;
	this->setSource(this->m_data.getAllFile());
}
/*-------------------------------------------------
		��Դ���� - ��ȡ���ݣ���Դ���ݻᱻ�ÿ��޸ģ���Ҫ��ʱ��ȡ���仯��
*/
C_ALEData P_AnimationListEditor::getSource(){
	return this->m_data;
}


/*-------------------------------------------------
		���� - �����Դ(append)
*/
void P_AnimationListEditor::op_append(QString gif_src){
	if (this->m_data.isNull()){ return; }

	// > �������
	this->m_data.op_append(gif_src);
	int index = this->m_data.getFileCount() - 1;

	// > ͼ�����
	QFileInfo info = this->m_data.getFile(index);
	QImage image = QImage(info.absoluteFilePath());
	QPixmap pixmap = QPixmap::fromImage(image);
	this->m_org_bitmapList.append(pixmap);

	// > �ؼ����
	QListWidgetItem* item = this->createPictureItem();
	this->m_listWidget->addItem(item);
	this->m_itemTank.append(item);

	// > �����ؼ�
	QWidget* widget = this->createPictureWidget(index, pixmap);
	this->m_listWidget->setItemWidget(item, widget);
	this->m_widgetTank.append(widget);

	//������Ҫˢ��

	emit animBitmapChanged();
}
/*-------------------------------------------------
		���� - �����Դ(insert)���˴� �ƿ��� insertItem ��bug������ṹ��Ƚϸ��ӡ�
*/
void P_AnimationListEditor::op_insert(int index, QStringList gif_src_list, QList<int> interval_list){
	if (this->m_data.isNull()){ return; }
	if (gif_src_list.count() == 0){ return; }

	// > �������
	this->m_data.op_insert(index, gif_src_list, interval_list);

	for (int i = 0; i < gif_src_list.count(); i++){

		// > ͼ�����
		QFileInfo info = this->m_data.getFile(index + i);
		QImage image = QImage(info.absoluteFilePath());
		QPixmap pixmap = QPixmap::fromImage(image);
		this->m_org_bitmapList.insert(index + i, pixmap);

		// > �ؼ����
		QListWidgetItem* item = this->createPictureItem();
		this->m_listWidget->addItem(item);		//��insertItem��bug��ֻ��add��
		this->m_itemTank.append(item);

		// > �����ؼ�
		QWidget* widget = this->createPictureWidget(index, pixmap);
		this->m_listWidget->setItemWidget(item, widget);
		this->m_widgetTank.append(widget);
	}

	// > ǿ��ȫˢ
	this->op_refreshAll(index);

	emit animBitmapChanged();
}
/*-------------------------------------------------
		���� - �Ƴ�
*/
void P_AnimationListEditor::op_remove(int index){
	if (this->m_data.isNull()){ return; }

	// > �����Ƴ�
	this->m_data.op_remove(index);

	// > ͼ���Ƴ�
	this->m_org_bitmapList.removeAt(index);

	// > �ؼ��Ƴ�
	this->m_listWidget->takeItem(index);
	QListWidgetItem* item = this->m_itemTank.at(index);
	QWidget* widget = this->m_widgetTank.at(index);
	this->m_itemTank.removeAt(index);
	this->m_widgetTank.removeAt(index);
	delete item;
	delete widget;

	// > ���������
	this->m_copyedList.clear();

	emit animBitmapChanged();
}
/*-------------------------------------------------
		���� - ����λ��
*/
void P_AnimationListEditor::op_swap(int index_a, int index_b){
	if (this->m_data.isNull()){ return; }
	if (index_a == index_b){ return; }

	// > ���ݽ���
	this->m_data.op_swap(index_a, index_b);

	// > ͼ�ν���
	this->m_org_bitmapList.swap(index_a, index_b);
	
	// > ������uiˢ��
	this->op_refresh(index_a);
	this->op_refresh(index_b);

	// > �ؼ���������������
	//if (index_a > index_b){	//������ aС��b��
	//	int temp = index_a;
	//	index_a = index_b;
	//	index_b = temp;
	//}
	//this->m_listWidget->removeItemWidget(this->m_listWidget->item(index_a));		//��removeItem�����⣬�ܳ���ָ�����⡿
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
		���� - ˢ����ͼ����
*/
void P_AnimationListEditor::op_refresh(int index){
	P_ALEBlock* widget = dynamic_cast<P_ALEBlock*>(this->m_widgetTank.at(index));
	if (this->m_config.m_zeroFill == true){		//��ˢ�¼�����
		widget->setCountText(TTool::_zeroFill_(index + 1, this->m_config.m_zeroFillCount, QLatin1Char(this->m_config.m_zeroFillChar.toLatin1())));
	}else{
		widget->setCountText(QString::number(index + 1));
	}
	widget->setPixmap(this->m_org_bitmapList.at(index));	//��ˢ��ͼƬ��
	widget->setFrameText(this->m_data.getIntervalString(index));	//��ˢ��֡��
}
void P_AnimationListEditor::op_refreshAll(int startAt){
	for (int i = startAt; i < this->m_itemTank.count(); i++){
		this->op_refresh(i);
	}
}




/*-------------------------------------------------
		action - ���֡������հ״���
*/
void P_AnimationListEditor::op_appendInAction(){

	QStringList file_list = this->openWindow_getPicFileList();
	for (int i = 0; i < file_list.count(); i++){
		QFileInfo fileinfo = QFileInfo(file_list.at(i));
		if (fileinfo.exists() == false){ continue; }

		// > �ظ��ļ�
		QString file_name = fileinfo.completeBaseName();
		if (this->m_data.hasFileName(file_name) ||						//���������ļ� 
			this->m_data.getFileRoot() == fileinfo.absolutePath() ){	//��ͬһ���ļ�����
			this->op_append(file_name);
			continue;
		}
		
		// > ���ظ��ļ�
		C_ALEData::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
		this->op_append(file_name);
	}
}
/*-------------------------------------------------
		action - ���֡�����֡ʱ��
*/
void P_AnimationListEditor::op_insertInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	int pos = cur_action->data().value<int>();

	QStringList file_list = this->openWindow_getPicFileList();
	for (int i = 0; i < file_list.count(); i++){
		QFileInfo fileinfo = QFileInfo(file_list.at(i));
		if (fileinfo.exists() == false){ continue; }

		// > �ظ��ļ�
		QString file_name = fileinfo.completeBaseName();
		if (this->m_data.hasFileName(file_name) ||						//���������ļ� 
			this->m_data.getFileRoot() == fileinfo.absolutePath()){		//��ͬһ���ļ�����
			this->op_insert(pos + i, QStringList() << file_name);
			continue;
		}

		// > ���ظ��ļ�
		C_ALEData::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
		this->op_insert(pos + i, QStringList() << file_name);
	}
}
/*-------------------------------------------------
		action - ���֡��GIF��
*/
void P_AnimationListEditor::op_insertGIFInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	int pos = cur_action->data().value<int>();

	// > ���GIF��Ŀ¼
	QString file = this->openWindow_getGIFFile();
	bool successed = S_GIFManager::getInstance()->dismantlingGIF(QFileInfo(file), QDir(this->m_data.getFileRoot()+"/"),"png","%2_%1");
	if (successed == false){
		QMessageBox::warning(this->m_listWidget, "����", "GIF����ʧ�ܡ�", QMessageBox::Yes);
		return;
	}

	// > ��ȡ�ļ���
	QList<int> interval_list = S_GIFManager::getInstance()->getLastDismantledGIFIntervalList();
	QList<QFileInfo> file_list = S_GIFManager::getInstance()->getLastDismantledGIFFileList();
	QStringList file_name_list = QStringList();
	for (int i = 0; i <file_list.count(); i++){
		file_name_list.append(file_list.at(i).completeBaseName());
	}

	this->op_insert(pos, file_name_list, interval_list);

}
/*-------------------------------------------------
		action - ɾ��֡�������Ͷ����
*/
void P_AnimationListEditor::op_removeInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	QString index_str = cur_action->data().value<QString>();
	
	// > ����ɾ��
	int pos = 0;
	QStringList index_list = index_str.split(",");
	for (int i = index_list.count()-1; i >=0 ; i--){
		pos = index_list.at(i).toInt();
		this->op_remove(pos);
	}
	
	// > ȡ��ѡ��
	if (this->m_itemTank.count() == 0){
		emit allFrameDeleted();
	}else{
		this->selectIndex(pos - 1);
	}
}
/*-------------------------------------------------
		action - ���ƣ������Ͷ����
*/
void P_AnimationListEditor::op_copyInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	QString index_str = cur_action->data().value<QString>();

	QStringList index_list_str = index_str.split(",");
	QList<int> index_list = QList<int>();
	for (int i = 0; i < index_list_str.count(); i++ ){
		index_list.append( index_list_str.at(i).toInt() );
	}

	this->m_copyedList = this->m_data.getFileList(index_list);
}
/*-------------------------------------------------
		action - ճ���������Ͷ����
*/
void P_AnimationListEditor::op_pasteInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	int pos = cur_action->data().value<int>();
	if (this->m_copyedList.count() == 0){ return; }

	QStringList file_name_list = QStringList();
	for (int i = 0; i < this->m_copyedList.count(); i++){
		file_name_list.append(this->m_copyedList.at(i).completeBaseName());
	}

	this->op_insert(pos, file_name_list);
}
/*-------------------------------------------------
		action - ����
*/
void P_AnimationListEditor::op_moveLeftInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	int index = cur_action->data().value<int>();
	if (index - 1 < 0){ return; }
	this->op_swap(index, index - 1);
	this->selectIndex(index - 1);	//��ѡ�У�
}
/*-------------------------------------------------
		action - ����
*/
void P_AnimationListEditor::op_moveRightInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	int index = cur_action->data().value<int>();
	if (index + 1 >= this->m_itemTank.count() ){ return; }
	this->op_swap(index, index + 1);
	this->selectIndex(index+1);	//��ѡ�У�
}
/*-------------------------------------------------
		action - �༭֡
*/
void P_AnimationListEditor::op_editOneInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	int index = cur_action->data().value<int>();

	// > �����Ի���
	W_ALEDataEdit d(this->m_listWidget);
	d.setDataInModifyMode_One(this->m_data,index);
	if (d.exec() == QDialog::Accepted){
		this->m_data = d.getData();

		// > ͼ��ˢ��
		QFileInfo info = this->m_data.getFile(index);
		QImage image = QImage(info.absoluteFilePath());
		QPixmap pixmap = QPixmap::fromImage(image);
		this->m_org_bitmapList.replace(index, pixmap);

		// > �ؼ�ˢ��
		this->op_refresh(index);
	}
}
/*-------------------------------------------------
		action - �༭֡ʱ��
*/
void P_AnimationListEditor::op_editMultiInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	QString index_str = cur_action->data().value<QString>();

	QStringList index_list_str = index_str.split(",");
	QList<int> index_list = QList<int>();
	for (int i = 0; i < index_list_str.count(); i++){
		index_list.append(index_list_str.at(i).toInt());
	}

	// > �����Ի���
	W_ALEDataEdit d(this->m_listWidget);
	d.setDataInModifyMode_Multi(this->m_data, index_list);
	if (d.exec() == QDialog::Accepted){
		this->m_data = d.getData();

		//������ͼƬ��ͼ�β���Ҫˢ�£�

		for (int i = 0; i < index_list.count(); i++){
			this->op_refresh(index_list.at(i));
		}
	}
}






/*-------------------------------------------------
		�༭���� - ѡ�����ͼƬ
*/
QStringList P_AnimationListEditor::openWindow_getPicFileList(){
	QStringList result_list = QStringList();
	QFileDialog fd;
	fd.setWindowTitle("����ͼƬ����ѡ���ţ�");
	fd.setAcceptMode(QFileDialog::AcceptOpen);	
	fd.setNameFilters(QStringList() << "ͼƬ(*.png)");
	fd.setViewMode(QFileDialog::Detail);
	fd.setFileMode(QFileDialog::ExistingFiles);		//����ļ�
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()){ return result_list; }
		result_list = fd.selectedFiles();
		return result_list;
	}else {
		return result_list;
	}
}
/*-------------------------------------------------
		�༭���� - ѡ��GIF
*/
QString P_AnimationListEditor::openWindow_getGIFFile(){
	QString result = "";
	QFileDialog fd;
	fd.setWindowTitle("����GIF");
	fd.setAcceptMode(QFileDialog::AcceptOpen);	
	fd.setNameFilters(QStringList() << "��ͼ(*.gif)");
	fd.setViewMode(QFileDialog::Detail);
	fd.setFileMode(QFileDialog::ExistingFile);		//�����ļ�
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()){ return ""; }
		result = fd.selectedFiles().at(0);
		return result;
	}else {
		return "";
	}
}

