#ifndef P_PPaViewer_H
#define P_PPaViewer_H

#include <QtGui/QMouseEvent>
#include "i_PPaView.h"

/*
-----==========================================================-----
		�ࣺ		�и�֡ѡ���� ��װ��.h
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		ͨ��new���Զ���һ��QWidget�����ٳ� �и�֡ѡ���� ��QGraphView��
					����ϸ��.cpp��
-----==========================================================-----
*/
class P_MouseResizeController;
class P_PPaViewer : public QObject
{
	Q_OBJECT

	public:
		P_PPaViewer(QWidget* _parent);		//���캯��
		~P_PPaViewer();						//��������
		
	//-----------------------------------
	//----���ؼ�
	private:
		QWidget* m_parent;				//����
		QVBoxLayout* m_layout;			//������
	private:
										//���ؼ���ʼ��
		void initWidgetAndLayout(QWidget* _parent);

	//-----------------------------------
	//----�ؼ�
	private:
		I_PPaView* m_GraphView;			//��ͼ
	public:
										//�ؼ� - �ؽ��ؼ�
		void rebuildUI();
										//�ؼ� - ����UI
		void clearUI();

	//-----------------------------------
	//----ͼƬ
	public slots:
										//ͼƬ - ���� ͼƬ��Դ
		void setSource(QFileInfo file);
		void setSource(QPixmap pixmap);


	//-----------------------------------
	//----����
	public slots:
										//���� - ���������ߺͷ��黮��
		void setGridLineAndBlockMatrix(int column, int row);
										//���� - ���������
		void clearGridLine();
										//���� - ��ȡ�и��ͼƬ
		QList<QPixmap> getCutBitmap();
		
	//-----------------------------------
	//----���š����� ������ſ����� ���ܡ�
	public slots:
										//���� - ��ȡ������
		P_MouseResizeController* getResizeController();
										//���� - ��С
		void zoomIn();
										//���� - �Ŵ�
		void zoomOut();
										//���� - ��С����
		void zoomReset();
										//���� - ��ȡ����ֵ
		double getScale();
										//���� - ���ù����������η�
										//		��˵��������"ctrl"��"alt"�����ַ�����ʾ������ֱ�����š�
		void setScaleWheelModifier(QString charModifier);
	signals:
										//���� - ����ֵ�ı�
		void scaleChanged(double scale);
	private slots:
										//���� - ����ֵ�ı䣨��view������
		void scaleChanged_view(double scale);

};

#endif // P_PPaViewer_H