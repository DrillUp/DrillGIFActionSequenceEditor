#ifndef W_RmmvOperateBoard_H
#define W_RmmvOperateBoard_H

#include <QtWidgets/QDialog>
#include "ui_W_RmmvOperateBoard.h"
#include "p_RmmvOperateBoard.h"

/*
-----==========================================================-----
		�ࣺ		������壨���ڣ�.h
		����ģ�飺	����ģ��
		���ܣ�		�������Ĵ��ڡ�
					����ϸ��cpp��
-----==========================================================-----
*/
class W_RmmvOperateBoard : public QDialog
{
	Q_OBJECT

	public:
		W_RmmvOperateBoard(QWidget *parent = 0);		//���캯��
		~W_RmmvOperateBoard();							//��������

	//-----------------------------------
	//----�ؼ�
	public:
		P_RmmvOperateBoard* m_P_RmmvOperateBoard;

	//-----------------------------------
	//----����
									//���� - ��������
									//���� - ��ȡ����
									//���� - �������� -> ui����
									//���� - ui���� -> ��������
									//���� - �ύ���ݣ�У�飩
	private:
		Ui::W_RmmvOperateBoard ui;
		
};

#endif // W_RmmvOperateBoard_H
