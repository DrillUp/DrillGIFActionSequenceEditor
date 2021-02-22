#ifndef P_PiSBlock_H
#define P_PiSBlock_H

#include <QtWidgets>
#include "ui_P_PiSBlock.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ��.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		һ��������ͼƬ�飬���Լ̳и��࣬�������QWidget��
					����ϸ��cpp��
-----==========================================================-----
*/
class I_PiSPicture;
class P_PiSBlock : public QWidget
{
	Q_OBJECT

	public:
		P_PiSBlock(int width, int height, QWidget* parent = 0);	//���캯��
		~P_PiSBlock();											//��������
		void _init();											//��ʼ�����ú��� ����̳�û�ã�

	//-----------------------------------
	//----�ؼ�
	protected:
		int m_width;
		int m_height;
		I_PiSPicture* m_i_PiSPicture;
	public:
							//�ؼ� - �����ı�
		void setCountText(QString name);
							//�ؼ� - ��ȡ�ı�
		QString getCountText();
							//�ؼ� - ����ͼƬ
		void setPixmap(QPixmap pixmap);
							//�ؼ� - ���������˱���
		void setMaskEnabled(bool enabled);
		
	//-----------------------------------
	//----����
	protected:
		Ui::P_PiSBlock ui;
};

#endif // P_PiSBlock_H
