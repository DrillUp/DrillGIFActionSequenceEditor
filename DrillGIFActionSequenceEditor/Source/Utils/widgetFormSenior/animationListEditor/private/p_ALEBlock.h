#ifndef P_ALEBlock_H
#define P_ALEBlock_H

#include "Source/Utils/widgetForm/pictureSelector/private/p_PiSBlock.h"

/*
-----==========================================================-----
		�ࣺ		����֡��.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		һ��������ͼƬ�飬���Լ̳и��࣬�������QWidget��
					����ϸ��cpp��
-----==========================================================-----
*/
class P_ALEBlock : public P_PiSBlock
{
	Q_OBJECT

	public:
		P_ALEBlock(int width, int height, QWidget* parent = 0);	//���캯��
		~P_ALEBlock();											//��������
		void _init2();											//��ʼ��

	//-----------------------------------
	//----�ؼ�
	private:
		QLabel* m_FrameLabel;
	public:
							//�ؼ� - ����֡�ı�
		void setFrameText(QString name);
		
};

#endif // P_ALEBlock_H
