#ifndef W_PicturePartitioner_H
#define W_PicturePartitioner_H

#include "ui_w_PicturePartitioner.h"
#include "private/p_PPaViewer.h"
#include "Source/GraphModule/widget/singlePictureViewer/p_SinglePictureViewer.h"
#include "Source/GraphModule/middleware/mouseResizeController/extendPart/p_MouseResizeButtonPart.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ�ָ��� ����.cpp
		����ģ�飺	ͼ��ģ��
		���ܣ�		���Խ�ͼƬ�ָ�ɶ��ͼƬ��
-----==========================================================-----
*/
class W_PicturePartitioner : public QDialog
{
	Q_OBJECT

	public:
		W_PicturePartitioner(QWidget* parent = 0);
		~W_PicturePartitioner();

	//-----------------------------------
	//----��ͼ
	protected:
		P_SinglePictureViewer* m_p_SinglePictureViewer;				//���и�Ԥ��ͼ��
		P_PPaViewer* m_p_PPaViewer;									//��֡ѡ��ͼ��
		P_MouseResizeButtonPart* m_p_MouseResizeButtonPart1;		//���Ű�ť��1
		P_MouseResizeButtonPart* m_p_MouseResizeButtonPart2;		//���Ű�ť��2
		
	//-----------------------------------
	//----����
	public slots:
										//���� - �����仯
		void rowChanged(int value);
		void columnChanged(int value);
										//���� - ˢ��������
		void refreshGridLine();

	//-----------------------------------
	//----����
	public slots:
										//���� - ��������1
		void toFlow_1();
										//���� - ��������2
		void toFlow_2();

	//-----------------------------------
	//----����
	protected:
		bool m_slotBlock;
		QPixmap local_bitmap;				//��ǰͼƬ
		QList<QPixmap> local_bitmapTank;	//�и���б�
	public:
										//���� - ��������
		void setData(QFileInfo file);
		void setData(QPixmap bitmap);
										//���� - ȡ������
		QList<QPixmap> getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();
	public slots:
										//���� - �ύ���ݣ�У�飩
		void acceptData();	

	private:
		Ui::W_PicturePartitioner ui;
};

#endif 