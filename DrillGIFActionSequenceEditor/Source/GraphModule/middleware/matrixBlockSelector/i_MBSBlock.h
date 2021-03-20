#pragma once
#include <QGraphicsRectItem>

/*
-----==========================================================-----
		�ࣺ		���з���.h
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		������󣬳��ֲ�ͬ��ʾ״̬�ķ��顣
					����������x��y��w��h����ȫ������
					����ϸ��cpp��
-----==========================================================-----
*/
class I_MBSBlock : public QGraphicsRectItem{

	public:
		I_MBSBlock();
		~I_MBSBlock();
		void _init();

	//-----------------------------------
	//----״̬λ
	protected:
		int m_state;					//״̬λ
		QList<QColor> m_blockColorTank;	//����ͬ״̬λ���ò�ͬ��ɫ��
	public:
								//״̬λ - ���÷�����ɫ����
		void setColorTank(QList<QColor> color_tank);
								//״̬λ - ���õ�ǰ״̬
		void setState(int state);
								//״̬λ - ��ȡ��ǰ״̬
		int getState();
		
	//-----------------------------------
	//----����
	protected:
		int m_width;
		int m_height;
		int m_matrixX;
		int m_matrixY;
	public:
								//���� - ���÷����С
		void setBlockSize(int width, int height);
								//���� - ����λ�ã���λͼ�飩
		void setMatrixPosition(int x,int y);
								//���� - ��ȡ����λ��X
		int getMatrixX();
								//���� - ��ȡ����λ��Y
		int getMatrixY();
	protected:
								//���� - ˢ�·������
		void refreshRect();
		
	//-----------------------------------
	//----������
	public:
								//���������
		const bool operator== (const I_MBSBlock& a)const;

};