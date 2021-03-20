#ifndef P_MatrixBlockSelector_H
#define P_MatrixBlockSelector_H

#include <QtGui/QMouseEvent>
#include <QGraphicsView>
#include <QSvgRenderer>
#include <QGraphicsSvgItem>
#include "i_MBSBlock.h"

/*
-----==========================================================-----
		�ࣺ		�������е�ѡ��.h
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ÿ���������װ�� QGraphicsView ��ʹ���ܹ�֧�� �������й�ѡ�л� + ��ɫ���л�/ʸ��ͼ�л� ���ܡ�
					����ϸ��.cpp��
-----==========================================================-----
*/
class P_MatrixBlockSelector : public QObject
{
	Q_OBJECT

	public:
		P_MatrixBlockSelector();				//���캯��
		~P_MatrixBlockSelector();				//��������

	//-----------------------------------
	//----��ʼ��
	private:
		QGraphicsView* m_view;
	public:
										//��ʼ�� - ������ͼ
		void initGraphicsView(QGraphicsView* view);
										//��ʼ�� - ���÷�����ɫ
										//		��˵�������б����������״̬��������
		void initBlockColor(QList<QColor> color_list);
										//��ʼ�� - ����ʸ��ͼ�б�
										//		��˵������Ҫ�뷽����ɫ����һ�¡�
		void initBlockSvgIcon(QList<QFileInfo> svg_file_list);
										//��ʼ�� - ���÷����С
										//		��˵��������ʼ����Ҫ��ֵ�����󽫻��Դ˴�СΪ��׼�����÷��顣
		void initBlockSize(int width,int height);
										//���� - ���÷���
										//		��˵�������ò�������շ��飬Ȼ�����¹滮���÷����С��
		void rebuildBlock(int width, int height);
		void rebuildBlock_double(double width, double height);
		
	//-----------------------------------
	//----����
	private:
		double m_blockWidth;				//�����ȣ�����ֵΪint�������ų�����double�����
		double m_blockHeight;				//����߶�
		QList<QColor> m_blockColorTank;		//������ɫ
		QList<I_MBSBlock*> m_blockTank;		//��������
	public:
										//���� - ��ȡȫ������
		QList<I_MBSBlock*> getAllBlock();
										//���� - ��ȡ������󣨵�λͼ�飩
		QRect getBlockMatrix();
										//���� - ���ȫ�����顢ʸ�����
										//		��˵���������ǰ���ᷢ���ǰ�źš�
		void clearAllBlock();
	signals:
										//���� - �·��鱻��ӣ��źţ�
		void newBlockAdded(I_MBSBlock* new_block,QGraphicsSvgItem* svgItem);
										//���� - ȫ�����鱻���ǰ���źţ�
		void oldBlockClearBefore(QList<I_MBSBlock*> block_list, QList<QGraphicsSvgItem*> svgItem_list);
	private:
										//���� - ��ӷ��飨˽�У�
		void setBlock(QPoint matrix_pos, int state);
		
	//-----------------------------------
	//----ʸ�����
	private:
		QList<QSvgRenderer*> m_rendererTank;		//��Ⱦ���б�
		QList<QGraphicsSvgItem*> m_svgTank;			//ʸ��ͼ�б�
	public:
										//ʸ�� - ��ȡȫ������
		QList<QGraphicsSvgItem*> getAllSvgItem();

	//-----------------------------------
	//----��Ϳ
	private:
		QList<QPoint> m_coveredPointTank;	//��꾭���ĵ㣨ֻ�������ֵ��������
		int m_cur_mouseState;				//��ǰ��Ϳ״̬
	public:
										//��Ϳ - ��ȡָ��״̬λ��ȫ����
		QList<QPoint> getPointList(int state);
										//��Ϳ - ��Ϳȫ��״̬λ
		void fillAll(int state);


	//-----------------------------------
	//----�����¼�
	private:
		bool m_mousePressed;			//��갴�±��
	public:
										//�����¼� - ��갴�£��ӿڣ�
		void event_mousePressEvent(QMouseEvent *event);
										//�����¼� - ���̧�𣨽ӿڣ�
		void event_mouseReleaseEvent(QMouseEvent *event);
										//�����¼� - ����ƶ����ӿڣ�
		void event_mouseMoveEvent(QMouseEvent *event);


};

#endif // P_MatrixBlockSelector_H