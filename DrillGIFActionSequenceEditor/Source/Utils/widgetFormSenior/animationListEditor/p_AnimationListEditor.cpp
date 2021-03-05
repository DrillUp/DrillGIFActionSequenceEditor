#include "stdafx.h"
#include "p_AnimationListEditor.h"

#include "private/p_ALEBlock.h"
#include "private/w_ALEDataEdit.h"
#include "private/w_ALEConfigEdit.h"

#include "Source/Utils/widgetForm/pictureSelector/p_PictureSelector.h"
#include "Source/Utils/manager/GIFManager/s_GIFManager.h"
#include "Source/Utils/common/p_FileOperater.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		����֡�༭��.cpp
		�汾��		v1.02
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
					-> ����
						-> ��ͼ
						-> ��ͼ
						-> GIF
					-> ����
						-> ��ͼ
						-> ��ͼ
						-> GIF
					-> �༭֡
						-> ��֡�༭��ͼ��֡��
						-> ��֡�༭��֡��
					-> �༭UI����
					-> ��ݼ�
					
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
					
				> ��ݼ�������widget��дkeyPressEvent������
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
	//----������ʼ��
	this->m_iconSrcPath = ":/DrillGIFActionSequenceEditor/Resources/icons";
	this->m_copyedList = QList<QFileInfo>();
	this->m_unit = C_ALEData::FrameUnit;

	// > ����
	this->setConfigParam(C_ALEConfig());
	this->m_exportName = "����";

	// > ��ݼ�������
	this->m_listWidget->installEventFilter(this);
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
	this->m_data.setData_IntervalDefault(config.m_defaultInterval);
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
	d.setDataInModifyMode(this->m_config_ALE, this->m_unit);
	if (d.exec() == QDialog::Accepted){
		C_ALEConfig config = d.getData();

		// > ���ú�ǿ�Ʊ仯ͳһĬ��֡���
		this->m_data.setIntervalDefaultWithFit(config.m_defaultInterval);

		// > ��λ���
		this->m_unit = (C_ALEData::DATA_UNIT)d.getDataUnit();
		this->m_data.setData_Unit(this->m_unit);
		
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
		
		QMenu* menu_add = new QMenu("���֡", menu);
		menu_add->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));

			action = new QAction("���ͼƬ", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
			action->setData(0);
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertInAction);
			menu_add->addAction(action);

			action = new QAction("���GIF", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
			action->setData(0);
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertGIFInAction);
			menu_add->addAction(action);
		menu->addMenu(menu_add);

		if (this->m_config_ALE.m_isMultiSelect == true){
			QMenu* menu_select = new QMenu("ѡ��֡", menu);
			menu_select->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));
			menu_select->setEnabled(false);
			menu->addMenu(menu_select);
		}

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

		if (this->m_config_ALE.m_isMultiSelect == true){
			menu->addSeparator();

			QMenu* menu_export = new QMenu("����", menu);
			menu_export->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Export.png"));
			menu_export->setEnabled(false);
			menu->addMenu(menu_export);
		}

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
		
		QMenu* menu_add = new QMenu("���֡", menu);
		menu_add->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));

			action = new QAction("���ͼƬ", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
			action->setData(pos);
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertInAction);
			menu_add->addAction(action);

			action = new QAction("���GIF", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
			action->setData(pos);
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertGIFInAction);
			menu_add->addAction(action);
		menu->addMenu(menu_add);

		if (this->m_config_ALE.m_isMultiSelect == true){
			QMenu* menu_select = new QMenu("ѡ��֡", menu);
			menu_select->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));

			action = new QAction("ȫѡ", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));
			action->setShortcut(QKeySequence::SelectAll);
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_selectAllInAction);
			menu_select->addAction(action);
			action = new QAction("ѡ������֡", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_selectOddInAction);
			menu_select->addAction(action);
			action = new QAction("ѡ��ż��֡", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_selectEvenInAction);
			menu_select->addAction(action);

			menu->addMenu(menu_select);
		}

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

		if (this->m_config_ALE.m_isMultiSelect == true){
			menu->addSeparator();
			QMenu* menu_export = new QMenu("����", menu);
			menu_export->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Export.png"));

			action = new QAction("����PNG - ��ͼ", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/ExportImage.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_exportSingle_PicInAction);
			menu_export->addAction(action);
			action = new QAction("����PNG - ȫ��֡", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/ExportImage.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_exportAll_PicInAction);
			menu_export->addAction(action);
			action = new QAction("����GIF - ��֡", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/ExportGIF.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_exportSelected_GIFInAction);
			menu_export->addAction(action);
			action = new QAction("����GIF - ȫ��֡", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/ExportGIF.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_exportAll_GIFInAction);
			menu_export->addAction(action);

			menu->addMenu(menu_export);
		}

		menu->exec(QCursor::pos());
		
	/*-------------------------������-----------------------------*/
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
		qSort(pos_list);		//�����򣬷�ֹɾ��ʱ˳�����ˣ�
		QList<QString> pos_list_str = QList<QString>();
		for (int i = 0; i < pos_list.count(); i++){
			pos_list_str.append(QString::number(pos_list.at(i)));
		}

		QMenu* menu_add = new QMenu("���֡", menu);
		menu_add->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));

			action = new QAction("���ͼƬ", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
			action->setData(front_pos);
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertInAction);
			menu_add->addAction(action);

			action = new QAction("���GIF", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Add.png"));
			action->setData(front_pos);
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_insertGIFInAction);
			menu_add->addAction(action);
		menu->addMenu(menu_add);

		if (this->m_config_ALE.m_isMultiSelect == true){
			QMenu* menu_select = new QMenu("ѡ��֡", menu);
			menu_select->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));

			action = new QAction("ȫѡ", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));
			action->setShortcut(QKeySequence::SelectAll);
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_selectAllInAction);
			menu_select->addAction(action);
			action = new QAction("ѡ������֡", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_selectOddInAction);
			menu_select->addAction(action);
			action = new QAction("ѡ��ż��֡", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_SelectAll.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_selectEvenInAction);
			menu_select->addAction(action);

			menu->addMenu(menu_select);
		}

		action = new QAction("����֡", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Copy.png"));
		action->setData(pos_list_str.join(","));
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
		action->setData(pos_list_str.join(","));
		action->setShortcut(QKeySequence::Delete);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_removeInAction);
		menu->addAction(action);

		menu->addSeparator();

		action = new QAction("�༭֡ʱ��", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Edit.png"));
		action->setData(pos_list_str.join(","));
		action->setShortcut(QKeySequence::Delete);
		connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_editMultiInAction);
		menu->addAction(action);

		if (this->m_config_ALE.m_isMultiSelect == true){
			menu->addSeparator();
			QMenu* menu_export = new QMenu("����", menu);
			menu_export->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Export.png"));

			action = new QAction("����PNG - ѡ��֡", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/ExportImage.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_exportSelected_PicInAction);
			menu_export->addAction(action);
			action = new QAction("����PNG - ȫ��֡", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/ExportImage.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_exportAll_PicInAction);
			menu_export->addAction(action);
			action = new QAction("����GIF - ѡ��֡", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/ExportGIF.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_exportSelected_GIFInAction);
			menu_export->addAction(action);
			action = new QAction("����GIF - ȫ��֡", this);
			action->setIcon(QIcon(this->m_iconSrcPath + "/menu/ExportGIF.png"));
			connect(action, &QAction::triggered, this, &P_AnimationListEditor::op_exportAll_GIFInAction);
			menu_export->addAction(action);

			menu->addMenu(menu_export);
		}

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
	this->m_data.setData_Unit(this->m_unit);	//����λ��ǣ�
	this->setSource(this->m_data.getAllFile());
}
/*-------------------------------------------------
		��Դ���� - ��ȡ���ݣ���Դ���ݻᱻ�ÿ��޸ģ���Ҫ��ʱ��ȡ���仯��
*/
C_ALEData P_AnimationListEditor::getSource(){
	this->m_data.setData_Unit(this->m_unit);	//����λ��ǣ�
	return this->m_data;
}
/*-------------------------------------------------
		��Դ���� - ���õ�λ
*/
void P_AnimationListEditor::setUnit(C_ALEData::DATA_UNIT unit){
	this->m_unit = unit;
}
/*-------------------------------------------------
		��Դ���� - ��ȡ��λ
*/
C_ALEData::DATA_UNIT P_AnimationListEditor::getUnit(){
	return this->m_unit;
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
void P_AnimationListEditor::op_remove(QList<int> index_list){
	if (this->m_data.isNull()){ return; }
	qSort(index_list);	//������

	// > ����ɾ��
	int index = 0;
	for (int i = index_list.count() - 1; i >= 0; i--){
		index = index_list.at(i);

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
	}

	// > ���������
	this->m_copyedList.clear();

	// > ȡ��ѡ��
	if (this->m_itemTank.count() == 0){
		emit allFrameDeleted();
	}else{
		this->selectIndex(index - 1);
	}

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
	if (index < 0){ return; }
	if (index >= this->m_widgetTank.count() ){ return; }
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

	bool cover_all = false;
	QStringList file_list = this->openWindow_getPicFileList();	//���򿪴���ѡ�����ͼƬ��
	for (int i = 0; i < file_list.count(); i++){
		QFileInfo fileinfo = QFileInfo(file_list.at(i));
		if (fileinfo.exists() == false){ continue; }
		QString file_name = fileinfo.completeBaseName();
		
		// > ��ͬһ���ļ�����
		if (this->m_data.getFileRoot() == fileinfo.absolutePath()){
			this->op_append(file_name);
			continue;
		}

		// > ���ظ��ļ�
		if ( this->m_data.hasFileName(file_name) == false ){
			C_ALEData::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
			this->op_append(file_name);
			continue;
		}

		// > ȫ������
		if (cover_all == true){
			C_ALEData::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
			this->op_append(file_name);
			continue;
		}

		// > �ظ��ļ�����
		switch (QMessageBox::question(this->m_listWidget, "��ʾ", "�����ͼƬ\"" + file_name + "\"���������Ѵ��ڣ��Ƿ񸲸�ԭ�ļ���", "ȫ������", "����", "����ԭͼ���", 0)) {

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
		action - ���֡�����֡ʱ��
*/
void P_AnimationListEditor::op_insertInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	int pos = cur_action->data().value<int>();

	bool cover_all = false;
	QStringList file_list = this->openWindow_getPicFileList();	//���򿪴���ѡ�����ͼƬ��
	for (int i = 0; i < file_list.count(); i++){
		QFileInfo fileinfo = QFileInfo(file_list.at(i));
		if (fileinfo.exists() == false){ continue; }
		QString file_name = fileinfo.completeBaseName();

		// > ��ͬһ���ļ�����
		if (this->m_data.getFileRoot() == fileinfo.absolutePath()){
			this->op_insert(pos + i, QStringList() << file_name);
			continue;
		}

		// > ���ظ��ļ�
		if (this->m_data.hasFileName(file_name) == false){
			C_ALEData::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
			this->op_insert(pos + i, QStringList() << file_name);
			continue;
		}

		// > ȫ������
		if (cover_all == true){
			C_ALEData::copyFile(fileinfo.absoluteFilePath(), this->m_data.getFileRoot() + "/" + fileinfo.fileName());
			this->op_insert(pos + i, QStringList() << file_name);
			continue;
		}

		// > �ظ��ļ�����
		switch (QMessageBox::question(this->m_listWidget, "��ʾ", "�����ͼƬ\"" + file_name + "\"���������Ѵ��ڣ��Ƿ񸲸�ԭ�ļ���", "ȫ������", "����", "����ԭͼ���", 0)) {

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
	QStringList index_str_list = index_str.split(",");

	QList<int> index_list = QList<int>();
	for (int i = 0; i < index_str_list.count(); i++){
		index_list.append(index_str_list.at(i).toInt());
	}

	this->op_remove(index_list);
}
/*-------------------------------------------------
		action - ȫѡ�������Ͷ����
*/
void P_AnimationListEditor::op_selectAllInAction(){
	this->m_listWidget->selectAll();
}
/*-------------------------------------------------
		action - ѡ����
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
		action - ѡż��
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

	this->m_copyedList = this->m_data.getFile_Multi(index_list);
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
		���� - ���õ�������
*/
void P_AnimationListEditor::setExportName(QString name){
	if (name == ""){ name = "����"; }
	this->m_exportName = name;
}
/*-------------------------------------------------
		���� - ������ͼ
*/
void P_AnimationListEditor::op_exportPic(int index, QFileInfo target_file){
	QFileInfo file_from = this->m_data.getFile(index);
	if (file_from.exists() == false){ return; }

	P_FileOperater op = P_FileOperater();
	op.copy_File(file_from, target_file);
}
/*-------------------------------------------------
		���� - ������ͼ
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
		���� - ����GIF��ָ���
*/
void P_AnimationListEditor::op_exportGIF_Multi(QList<int> index_list, QFileInfo target_file){

	// > �ļ��б�
	QList<QFileInfo> fileInfo_list = this->m_data.getFile_Multi(index_list);

	// > ֡����б���ת��λ��
	double interval_default = this->m_data.getIntervalDefaultWithUnit();
	QList<double> interval_list = this->m_data.getIntervalWithUnit_Multi(index_list);

	// > ����GIF
	S_GIFManager::getInstance()->generateGIF(
		fileInfo_list,
		target_file.absoluteFilePath(),
		interval_default,
		TTool::_QList_DoubleToInt_floor_(interval_list)
	);
}
/*-------------------------------------------------
		action - ������ͼ
*/
void P_AnimationListEditor::op_exportSingle_PicInAction(){
	QList<int> selected_list = this->getSelectedIndex_Multi();
	if (selected_list.count() == 0){ return; }
	QString path = this->openWindow_exportPNGFile();
	if (path == ""){ return; }

	this->op_exportPic(selected_list.at(0), QFileInfo(path));
	QMessageBox::information(this->m_listWidget, "��ʾ", "ͼƬ�����ɹ���", QMessageBox::Yes);
}
/*-------------------------------------------------
		action - ������ͼ
*/
void P_AnimationListEditor::op_exportSelected_PicInAction(){
	QList<int> selected_list = this->getSelectedIndex_Multi();
	if (selected_list.count() == 0){ return; }
	QString path = this->openWindow_exportDir();
	if (path == ""){ return; }

	this->op_exportPic_Multi(selected_list, QDir(path));
	QMessageBox::information(this->m_listWidget, "��ʾ", "ͼƬ�������ɹ���", QMessageBox::Yes);
}
/*-------------------------------------------------
		action - ����ȫ��ͼ
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
	QMessageBox::information(this->m_listWidget, "��ʾ", "ͼƬ�������ɹ���", QMessageBox::Yes);
}
/*-------------------------------------------------
		action - ����GIF - ѡ����
*/
void P_AnimationListEditor::op_exportSelected_GIFInAction(){
	QList<int> selected_list = this->getSelectedIndex_Multi();
	if (selected_list.count() == 0){ QMessageBox::warning(this->m_listWidget, "��ʾ", "��Ҫѡ������һ������֡���ܵ�����", QMessageBox::Yes); return; }
	QString path = this->openWindow_exportGIFFile();
	if (path == ""){ return; }

	this->op_exportGIF_Multi(selected_list, QFileInfo(path));
	QMessageBox::information(this->m_listWidget, "��ʾ", "GIF�����ɹ���", QMessageBox::Yes);
}
/*-------------------------------------------------
		action - ����GIF - ȫ����
*/
void P_AnimationListEditor::op_exportAll_GIFInAction(){
	QList<int> selected_list = QList<int>();
	for (int i = 0; i < this->m_itemTank.count(); i++){
		selected_list.append(i);
	}
	if (selected_list.count() == 0){ QMessageBox::warning(this->m_listWidget, "��ʾ", "��Ҫѡ������һ������֡���ܵ�����", QMessageBox::Yes); return; }
	QString path = this->openWindow_exportGIFFile();
	if (path == ""){ return; }

	this->op_exportGIF_Multi(selected_list,QFileInfo(path));
	QMessageBox::information(this->m_listWidget, "��ʾ", "GIF�����ɹ���", QMessageBox::Yes);
}


/*-------------------------------------------------
		�༭���� - ѡ�����ͼƬ�����룩
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
		�༭���� - ѡ��GIF�����룩
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
/*-------------------------------------------------
		�༭���� - ѡ��GIF��������
*/
QString P_AnimationListEditor::openWindow_exportGIFFile(){
	QString target_file = "";
	QFileDialog fd;
	fd.setWindowTitle("����GIF");
	fd.setAcceptMode(QFileDialog::AcceptSave);
	fd.setDirectory(".");
	fd.setNameFilters(QStringList() << "��ͼ(*.gif)");
	fd.setViewMode(QFileDialog::Detail);
	fd.setFileMode(QFileDialog::ExistingFile);		//�����ļ�
	fd.selectFile(this->m_exportName+"_��ͼ.gif");
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()){ return ""; }
		target_file = fd.selectedFiles().at(0);
	}else {
		return "";
	}
	return target_file;
}
/*-------------------------------------------------
		�༭���� - ѡ��PNG��������
*/
QString P_AnimationListEditor::openWindow_exportPNGFile(){
	QString target_file = "";
	QFileDialog fd;
	fd.setWindowTitle("����PNG");
	fd.setAcceptMode(QFileDialog::AcceptSave);
	fd.setDirectory(".");
	fd.setNameFilters(QStringList() << "����֡(*.png)");
	fd.setViewMode(QFileDialog::Detail);
	fd.setFileMode(QFileDialog::ExistingFile);		//�����ļ�
	fd.selectFile(this->m_exportName+"_����֡.png");
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()){ return ""; }
		target_file = fd.selectedFiles().at(0);
	}else {
		return "";
	}
	return target_file;
}
/*-------------------------------------------------
		�༭���� - ѡ���ļ��У�������
*/
QString P_AnimationListEditor::openWindow_exportDir(){
	QString target_dir = "";
	QFileDialog fd;
	target_dir = fd.getExistingDirectory(this->m_listWidget, "�������ļ���");
	return target_dir;
}


/*-------------------------------------------------
		��ݼ� - ȫѡ
*/
void P_AnimationListEditor::shortcut_selectAll(){
	if (this->m_listWidget->hasFocus() == false){ return; }
	this->m_listWidget->selectAll();

}
/*-------------------------------------------------
		��ݼ� - ����
*/
void P_AnimationListEditor::shortcut_copy(){
	if (this->m_listWidget->hasFocus() == false){ return; }

	QList<int> index_list = this->getSelectedIndex_Multi();
	this->m_copyedList = this->m_data.getFile_Multi(index_list);
}
/*-------------------------------------------------
		��ݼ� - ճ��
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
		��ݼ� - ɾ��
*/
void P_AnimationListEditor::shortcut_delete(){
	if (this->m_listWidget->hasFocus() == false){ return; }
	QList<int> index_list = this->getSelectedIndex_Multi();
	this->op_remove(index_list);
}