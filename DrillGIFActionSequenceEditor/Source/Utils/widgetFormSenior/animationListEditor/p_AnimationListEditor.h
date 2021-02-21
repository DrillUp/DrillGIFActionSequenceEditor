#pragma once
#include "Source/Utils/widgetForm/pictureSelector/p_PictureSelector.h"
#include "c_ALEConfig.h"
#include "c_ALEData.h"

/*
-----==========================================================-----
		�ࣺ		����֡�༭��.cpp
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��ͼƬȫ����ʾ�����ܵ�ѡ/��ѡ��
					����ϸ��cpp��
-----==========================================================-----
*/
class P_AnimationListEditor : public P_PictureSelector
{
	Q_OBJECT

	public:
		P_AnimationListEditor(QListWidget *parent);		//���캯��
		~P_AnimationListEditor();						//��������
		
	//-----------------------------------
	//----�ؼ�
	protected:
		QString m_iconSrcPath;
	public:
									//�ؼ� - ÿ����ؼ�����д��
		virtual QWidget* createPictureWidget(int i, QPixmap pixmap);
									//�ؼ� - ����ȫ��
		virtual void clearAll();

	signals:
									//�ؼ� - ȫ��֡��ɾ�����źţ�
		void allFrameDeleted();
		
	//-----------------------------------
	//----����֡����
	protected:
		C_ALEConfig m_ALE_config;			//����֡������
	public:
											//����֡���� - ���ò���
		void setConfigParam_ALE(C_ALEConfig config);
											//����֡���� - ȡ������
		C_ALEConfig getConfigParam_ALE();
		
	//-----------------------------------
	//----�¼�
	protected:
											//�¼� - �Ҽ��¼�������������������
		virtual void event_itemRightClicked(QList<QListWidgetItem*> item_list);
											//�¼� - Ȧѡ�仯�¼��������������
		virtual void event_itemSelectionChanged(QList<QListWidgetItem*> selected_item_list);


	//-----------------------------------
	//----��Դ����
	protected:
		C_ALEData m_data;						//����
	protected:
									//��Դ���� - �������ݣ������ţ�
		virtual void setSource(QList<QFileInfo> file_list);
	public:
									//��Դ���� - �������ݣ�ע�⣬�����е�idҪ��ֵ��
		void setSource(C_ALEData data);
									//��Դ���� - ��ȡ����
		C_ALEData getSource();
		
	//-----------------------------------
	//----��Դ���ݣ�������
	protected:
		QList<QFileInfo> m_copyedList;
	public:
									//���� - ��ӡ��˴� �ƿ��� insertItem ��bug������ṹ��Ƚϸ��ӡ�
		void op_append(QString gif_src);
		void op_insert(int index, QStringList gif_src_list, QList<int> interval_list = QList<int>() );
									//���� - �Ƴ�
		void op_remove(int index);
									//���� - ����λ��
		void op_swap(int index_a, int index_b);
									//���� - ˢ����ͼ����
		void op_refresh(int index);
		void op_refreshAll(int startAt = 0);
		
	public slots:
									//action - ���֡������հ״���
		void op_appendInAction();
									//action - ���֡�����֡ʱ��
		void op_insertInAction();
									//action - ���֡��GIF��
		void op_insertGIFInAction();
									//action - ɾ��֡�������Ͷ����
		void op_removeInAction();
									//action - ���ƣ������Ͷ����
		void op_copyInAction();
									//action - ճ���������Ͷ����
		void op_pasteInAction();
									//action - ����
		void op_moveLeftInAction();
									//action - ����
		void op_moveRightInAction();
									//action - �༭֡
		void op_editOneInAction();
									//action - �༭֡ʱ��
		void op_editMultiInAction();

		
	//-----------------------------------
	//----�༭����
	public:
									//�༭���� - ѡ�����ͼƬ
		QStringList openWindow_getPicFileList();
									//�༭���� - ѡ��GIF
		QString openWindow_getGIFFile();
	
	//-----------------------------------
	//----������
	public:


};
