#ifndef I_PPaView_H
#define I_PPaView_H

#include <QGraphicsView>
#include "i_PPaScene.h"

#include "Source/GraphModule/middleware/mouseResizeController/p_MouseResizeController.h"
#include "Source/GraphModule/middleware/matrixBlockSelector/p_MatrixBlockSelector.h"

/*
-----==========================================================-----
		�ࣺ		�и�֡ѡ���� ��ͼ.h
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���ӿؼ��볡�����м��࣬���ƹ����������Ź��ܡ�
					����ϸ��.cpp��
-----==========================================================-----
*/
class I_PPaView : public QGraphicsView
{
    Q_OBJECT
	
	public:
		I_PPaView(QWidget *parent = 0);			//���캯��
		~I_PPaView();							//��������
		void init();							//��ʼ��
		
		
	//-----------------------------------
	//----����
	private:
		I_PPaScene* m_scene;
		P_MouseResizeController* m_p_MouseResizeController;
		P_MatrixBlockSelector* m_p_MatrixBlockSelector;		//�������е�ѡ��
	public:
										//���� - ��ȡ����
		I_PPaScene* getScene();
										//���� - ��ȡ������ſ�����
		P_MouseResizeController* getMouseResizeController();
										//���� - �������е�ѡ��
		P_MatrixBlockSelector* getMatrixBlockSelector();
		
	//-----------------------------------
	//----��ѡ��
	public:
		int m_block_width;	//�����ؼ���ֵ��
		int m_block_height;
	public:
										//��ѡ�� - ��ȡ�иͼƬ
		QList<QPixmap> getCutBitmap();
	private slots:
										//��ѡ�� - �����ѡ�����
		void blockAdded(I_MBSBlock* block, QGraphicsSvgItem* item);
										//��ѡ�� - �����ѡ�����
		void blockClear(QList<I_MBSBlock*> block_list, QList<QGraphicsSvgItem*> item_list);


	//-----------------------------------
	//----������
	private slots:
										//������ - ����������
		void scrollValueChanged();

	//-----------------------------------
	//----�����¼�
	private:
										//�����¼� - ��갴��
		void I_PPaView::mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//�����¼� - ���̧��
		void I_PPaView::mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//�����¼� - ����ƶ�
		void I_PPaView::mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//�����¼� - ������
		void I_PPaView::wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

};

#endif // I_PPaView_H
