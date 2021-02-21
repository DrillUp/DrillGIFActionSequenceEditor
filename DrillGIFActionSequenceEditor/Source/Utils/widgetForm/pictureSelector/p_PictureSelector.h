#pragma once

#include <QKeyEvent>
#include <QListWidget>

#include "C_PiSConfig.h"

/*
-----==========================================================-----
		�ࣺ		��ѡ/��ѡ ͼƬ��.cpp
		�汾��		v1.01
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��ͼƬȫ����ʾ�����ܵ�ѡ/��ѡ��
					����ϸ��cpp��
-----==========================================================-----
*/
class P_PictureSelector : public QObject
{
	Q_OBJECT

	public:
		P_PictureSelector(QListWidget *parent);		//���캯��
		~P_PictureSelector();						//��������
		
	//-----------------------------------
	//----�ؼ�
	protected:
		QListWidget* m_listWidget;					//ͼƬ�ж���
		QString m_listWidgetStyle;					//ͼƬ��Ĭ����ʽ
		QList<QListWidgetItem*> m_itemTank;			//���б�
		QList<QWidget*> m_widgetTank;				//�ؼ��б�����Ҫ�棬��Ȼȡ����Widget���Զ����١�
		int m_last_index;							//��һ��ѡ�е�������
	public:
									//�ؼ� - �ؽ�ͼƬ�У����ͼƬ�࣬�����鷴�����ã�
		void rebuildListUi();
									//�ؼ� - ����ͼƬ��
		QListWidgetItem* createPictureItem();
									//�ؼ� - ����ͼƬ�ؼ�
		virtual QWidget* createPictureWidget(int i, QPixmap pixmap);
									//�ؼ� - ����ȫ��
		virtual void clearAll();

	signals:
									//�ؼ� - ѡ��ѡ��仯���źţ�
		void currentIndexChanged(int index);
									//�ؼ� - ѡ��ѡ��仯����ѡʱ���źţ�
		void selectedIndexChanged_Multi(QList<int> index);

	//-----------------------------------
	//----�¼�
	protected:
		bool m_selectionSignalBlock_Root;	//Ȧѡ ��
	protected slots:
											//�¼� - �Ҽ��¼�
		void itemRightClicked(QPoint point);
											//�¼� - �Ҽ��¼�������������������
		virtual void event_itemRightClicked(QList<QListWidgetItem*> item_list);

											//�¼� - Ȧѡ�仯�¼�
		void itemSelectionChanged();
											//�¼� - Ȧѡ�仯�¼��������������
		virtual void event_itemSelectionChanged(QList<QListWidgetItem*> selected_item_list);
		
	//-----------------------------------
	//----ͼƬ������
	protected:
		C_PiSConfig m_config;			//ͼƬ��������
	public:
										//ͼƬ������ - ���ò���
		void setConfigParam(C_PiSConfig config);
										//ͼƬ������ - ȡ������
		C_PiSConfig getConfigParam();

	//-----------------------------------
	//----��Դ����
	protected:
		QList<QPixmap> m_org_bitmapList;		//��Դ����
	public:
									//��Դ���� - ��������
		virtual void setSource(QList<QFileInfo> file_list);
									//��Դ���� - ��������
		virtual void setSource(QList<QPixmap> bitmap_list);

	//-----------------------------------
	//----ѡ��
	public:
									//ѡ�� - ����ѡ��
		void selectIndex(int index);
									//ѡ�� - ��ȡѡ������
		int getSelectedIndex();
									//ѡ�� - ����ѡ�У���ѡʱ��
		void selectIndex_Multi(QList<int> index_list);
									//ѡ�� - ��ȡѡ�����ݣ���ѡʱ��
		QList<int> getSelectedIndex_Multi();

};
