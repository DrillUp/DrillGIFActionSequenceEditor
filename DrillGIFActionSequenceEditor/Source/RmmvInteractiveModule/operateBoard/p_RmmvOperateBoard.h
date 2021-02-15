#ifndef P_OperateBoard_H
#define P_OperateBoard_H

#include <QtWidgets>
#include "ui_P_RmmvOperateBoard.h"
#include "../base/c_RmmvProjectData.h"

/*
-----==========================================================-----
		�ࣺ		�������.h
		�汾��		v1.01
		����ģ�飺	����ģ��
		���ܣ�		�ò����ṩrmmv�Ľ�����ť���ܡ�
					����ϸ��cpp��
-----==========================================================-----
*/
class P_RmmvOperateBoard : public QWidget
{
	Q_OBJECT

	public:
		P_RmmvOperateBoard(QWidget *parent = 0);
		~P_RmmvOperateBoard();
		
	//-----------------------------------
	//----�ؼ�
	public:
										//�ؼ� - ��rmmv
		void openRmmv();
										//�ؼ� - ����rmmv����
		void importRmmv();
										//�ؼ� - ���浽rmmv
		void saveToRmmv();
										//�ؼ� - ����rmmv
		void runRmmv();

	//-----------------------------------
	//----����
	public:
		C_RmmvProjectData local_rmmvData;
	public:
										//���� - ��������
		void setData(C_RmmvProjectData rmmvData);
										//���� - ȡ������
		C_RmmvProjectData getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();

	private:
		Ui::P_RmmvOperateBoard ui;

};

#endif // P_OperateBoard_H
