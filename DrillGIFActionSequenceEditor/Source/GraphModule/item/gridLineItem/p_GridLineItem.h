#ifndef P_GridLineItem_H
#define P_GridLineItem_H

#include <QGraphicsLineItem>

/*
-----==========================================================-----
		�ࣺ		������.h
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	ͼ��ģ��
		���ܣ�		�ܹ������������ã������߶����顣
					����ϸ��.cpp��
-----==========================================================-----
*/
class P_GridLineItem : public QObject
{
	Q_OBJECT

	public:
		P_GridLineItem(QGraphicsScene* scene);		//���캯��
		~P_GridLineItem();							//��������
		
	//-----------------------------------
	//----������
	private:
		QGraphicsScene* m_scene;				//���ؼ�
		QList<QGraphicsLineItem*> m_itemTank;	//�����б�
	public:
										//������ - ��������
		void rebuildGrid(int width, int height, int column, int row, QColor color);
										//������ - ��ȡ������
		QList<QGraphicsLineItem*> getItems();
										//������ - ��ӵ�����
		void addItemsToScene();
										//������ - ��ն���
		void clearAllItem();

};

#endif // P_GridLineItem_H