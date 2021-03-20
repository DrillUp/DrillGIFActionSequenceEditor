#ifndef I_PPaScene_H
#define I_PPaScene_H

#include <QGraphicsScene>
#include "Source/GraphModule/item/maskBackgroundItem/i_MaskBackgroundItem.h"
#include "Source/GraphModule/item/gridLineItem/p_GridLineItem.h"

/*
-----==========================================================-----
		�ࣺ		�и�֡ѡ���� ����.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		ʹ�ÿ����ڳ����е����ѡ�и�֡��
					����ϸ��.cpp��
-----==========================================================-----
*/
class I_PPaScene : public QGraphicsScene
{
    Q_OBJECT
	
	public:
		I_PPaScene(QWidget *parent = 0);		//���캯��
		~I_PPaScene();							//��������
		void init();							//��ʼ��
		
		
	//-----------------------------------
	//----����¼�
	private:
										//�����¼� - ��갴��
		void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
										//�����¼� - ���̧��
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
										//�����¼� - ����ƶ�
		void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
										//�����¼� - ���˫��
		void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;


	//-----------------------------------
	//----����
	private:
		int m_canvasWidth;			//�������
		int m_canvasHeight;			//�����߶�
		int m_canvasThickness;		//������Ѫ�ߺ��
		int m_pixelWidth;			//ͼ����
		int m_pixelHeight;			//ͼ��߶�
		
	//-----------------------------------
	//----��������
	private:
		QColor m_gridLineColor;						//�����ߵ�ɫ
		QColor m_backgroundColor;					//������ɫ
		P_GridLineItem* m_P_GridLineItem;			//������
		I_MaskBackgroundItem* m_maskBackground;		//����
	public:
									//���� - ����������
		void setGridLine(int column, int row);
									//���� - ���������
		void clearGridLine();
	private:
									//���� - ˢ�±���
		void refreshBackground();
		
	//-----------------------------------
	//----��Դ
	public:
		QPixmap m_bitmap;
	public:
									//��Դ - ������Դ
		void setSource(QPixmap bitmap);
									//��Դ - �����Դ
		void clearSource();

	protected:
									//��Դ - �ؽ�����
		void rebuildScene();

	//-----------------------------------
	//----ͼƬ
	private:
		QGraphicsPixmapItem* m_bitmapItem;
	public:
									//ͼƬ - ���߶�
		int getMaxHeight();
									//ͼƬ - �����
		int getMaxWidth();

};

#endif // I_PPaScene_H
