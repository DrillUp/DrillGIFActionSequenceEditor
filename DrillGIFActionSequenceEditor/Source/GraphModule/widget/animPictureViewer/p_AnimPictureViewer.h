#ifndef P_AnimPictureViewer_H
#define P_AnimPictureViewer_H

#include <QFileInfo>
#include <QtGui/QMouseEvent>
#include "private/i_APVView.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ�鿴�� ��װ��.h
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		ͨ��new���Զ���һ��QWidget�����ٳ� ͼƬ�鿴�� ��QGraphView��
					����ϸ��.cpp��
-----==========================================================-----
*/
class P_AnimPictureViewer : public QObject
{
	Q_OBJECT

	public:
		P_AnimPictureViewer(QWidget* _parent);		//���캯��
		~P_AnimPictureViewer();						//��������
		
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
		I_APVView* m_GraphView;			//��ͼ
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
	//----����֡
	public slots:
										//����֡ - ���� ͼƬ��Դ
		void setSource(QList<QFileInfo> file_list);
		void setSource(QList<QPixmap> bitmap_list);
										//����֡ - �л�֡
		void setAnimFrame(int index);

};

#endif // P_AnimPictureViewer_H