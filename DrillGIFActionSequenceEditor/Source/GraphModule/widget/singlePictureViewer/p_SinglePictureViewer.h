#ifndef P_SinglePictureViewer_H
#define P_SinglePictureViewer_H

#include <QtGui/QMouseEvent>
#include "private/i_SPVView.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ�鿴��-��ͼ ��װ��.h
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		ͨ��new���Զ���һ��QWidget�����ٳ� ͼƬ�鿴�� ��QGraphView��
					����ϸ��.cpp��
-----==========================================================-----
*/
class P_SinglePictureViewer : public QObject
{
	Q_OBJECT

	public:
		P_SinglePictureViewer(QWidget* _parent);		//���캯��
		~P_SinglePictureViewer();						//��������
		
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
		I_SPVView* m_GraphView;			//��ͼ
	public:
										//�ؼ� - �ؽ��ؼ�
		void rebuildUI();
										//�ؼ� - ����UI
		void clearUI();
		
	//-----------------------------------
	//----����
	public slots:
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

	//-----------------------------------
	//----ͼƬ
	public slots:
										//ͼƬ - ���� ͼƬ��Դ
		void setSource(QPixmap pixmap);

};

#endif // P_SinglePictureViewer_H