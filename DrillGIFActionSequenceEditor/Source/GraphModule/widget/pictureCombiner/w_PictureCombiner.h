#ifndef W_PictureCombiner_H
#define W_PictureCombiner_H

#include "ui_w_PictureCombiner.h"
#include "p_PictureCombiner.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ�ϳ��� ����.cpp
		����ģ�飺	ͼ��ģ��
		���ܣ�		���Խ�����ͼƬ�Ծ������ʽ�ϲ���һ��
-----==========================================================-----
*/
class W_PictureCombiner : public QDialog
{
	Q_OBJECT

	public:
		W_PictureCombiner(QWidget* parent = 0);
		~W_PictureCombiner();
		
	//-----------------------------------
	//----�ؼ�
	public:
		P_PictureCombiner* m_P_PictureCombiner;

	//-----------------------------------
	//----����
	public:
										//���� - ��������
		void setData(QList<QFileInfo> file_list);
		void setData(QList<QPixmap> bitmap_list);
										//���� - ȡ������
		QPixmap getData();
	public slots:
										//���� - �ύ���ݣ�У�飩
		void acceptData();	

	private:
		Ui::W_PictureCombiner ui;
};

#endif 