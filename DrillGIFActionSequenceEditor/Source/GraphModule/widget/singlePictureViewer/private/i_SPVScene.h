#ifndef I_SPVScene_H
#define I_SPVScene_H

#include <QGraphicsScene>

/*
-----==========================================================-----
		�ࣺ		ͼƬ�鿴��-��ͼ ����.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�鿴��ͼ�Ļ�����
					����ϸ��.cpp��
-----==========================================================-----
*/
class I_SPVScene : public QGraphicsScene
{
    Q_OBJECT
	
	public:
		I_SPVScene(QWidget *parent = 0);		//���캯��
		~I_SPVScene();							//��������
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
		QColor m_backgroundColor;					//��ɫ
		QGraphicsPixmapItem* m_maskBackground;		//����
	private:
									//���� - ˢ�±���
		void refreshBackground();
									//���� - ��ȡ��������ͼ
		QPixmap getBitmapMaskBackground();
		
	//-----------------------------------
	//----��Դ
	private:
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

#endif // I_SPVScene_H
