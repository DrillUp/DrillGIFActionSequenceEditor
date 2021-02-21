#ifndef I_PiSPicture_H
#define I_PiSPicture_H

#include <QWidget>

/*
-----==========================================================-----
		�ࣺ		ͼƬ���ؼ�.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		ʹ�ñ�ǩ���ж����ť����Щ��ť�൱�ڹ�ѡ��
					����ϸ��cpp��
-----==========================================================-----
*/

class I_PiSPicture : public QWidget
{
	Q_OBJECT

	public:
		I_PiSPicture(int width, int height, QWidget* parent = 0);	//���캯��
		~I_PiSPicture();											//��������

	//-----------------------------------
	//----�ؼ�
	private:
		int m_pixmapWidth;
		int m_pixmapHeight;
		QPixmap m_pixmap;
	public:
									//�ؼ� - ����ͼƬ
		void setPixmap(QPixmap pixmap);
									//�ؼ� - ����������
		void setMaskEnabled(bool maskEnabled);
		
	//-----------------------------------
	//----����
	protected:
		bool m_maskEnabled;			//������
		int m_maskWidth;			//������ ���
		int m_maskHeight;			//������ ���
		QColor m_maskColor;			//������ ��ɫ
	protected:
									//���� - ����ͼƬ
		void paintEvent(QPaintEvent*);
									//���� - ���������˷���
		QPixmap getBitmapMaskBackground();

};

#endif // I_PiSPicture_H
