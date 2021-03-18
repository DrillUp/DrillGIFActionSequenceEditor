#ifndef I_MaskBackgroundItem_H
#define I_MaskBackgroundItem_H

#include <QGraphicsPixmapItem>

/*
-----==========================================================-----
		�ࣺ		�����˱���.h
		�汾��		v1.04
		���ߣ�		drill_up
		����ģ�飺	ͼ��ģ��
		���ܣ�		�ṩһ��ֱ�����ɱ�����item����
					����ϸ��.cpp��
-----==========================================================-----
*/
class I_MaskBackgroundItem : public QGraphicsPixmapItem
{
	
	public:
		I_MaskBackgroundItem();				//���캯��
		~I_MaskBackgroundItem();			//��������
		void init();						//��ʼ��

	//-----------------------------------
	//----����
	protected:
		int m_blockWidth;
		int m_blockHeight;
	public:
											//���� - ���ñ�������ɫ��
		void setBackground_oneColor(int width, int height, int block_width, int block_height, QColor color = QColor(255, 255, 255), int opacity = 30);
											//���� - ���ñ�����˫ɫ��
		void setBackground_twoColor(int width, int height, int block_width, int block_height, QColor color1, QColor color2);

											//���� - ��ȡͼ�񣨵�ɫ��
		QPixmap getBackgroundPixmap_oneColor(int width, int height, int block_width, int block_height, QColor color, int opacity = 30);
											//���� - ��ȡͼ��˫ɫ��
		QPixmap getBackgroundPixmap_twoColor(int width, int height, int block_width, int block_height, QColor color1, QColor color2);
		
											//���� - ��ȡ������
		int getBlockWidth();
											//���� - ��ȡ����߶�
		int getBlockHeight();

};

#endif // I_MaskBackgroundItem_H
