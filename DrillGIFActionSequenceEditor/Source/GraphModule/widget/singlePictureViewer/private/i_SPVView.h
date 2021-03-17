#ifndef I_SPVView_H
#define I_SPVView_H

#include <QGraphicsView>
#include "i_SPVScene.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ�鿴��-��ͼ ��ͼ.h
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���ӿؼ��볡�����м��࣬���ƹ����������Ź��ܡ�
					����ϸ��.cpp��
-----==========================================================-----
*/
class I_SPVView : public QGraphicsView
{
    Q_OBJECT
	
	public:
		I_SPVView(QWidget *parent = 0);			//���캯��
		~I_SPVView();							//��������
		void init();							//��ʼ��
		
		
	//-----------------------------------
	//----����
	private:
		I_SPVScene* m_scene;
	public:
									//���� - ��ȡ����
		I_SPVScene* getScene();


	//-----------------------------------
	//----����
	private:
		double m_scale;					//���ű���
		double m_lastScale;				//
		QString m_scaleWheelModifier;	//���������η�
	public:
										//���� - ��С
		void zoomIn();
										//���� - �Ŵ�
		void zoomOut();
										//���� - ��С����
		void zoomReset();
										//���� - ��ȡ����ֵ
		double getScale();
										//���� - ���ù����������η�
										//		��˵��������"ctrl"��"alt"��"ֻ����"�����ַ�����ʾ �ر� �������š�
		void setScaleWheelModifier(QString charModifier);
	signals:
										//���� - ����ֵ�ı�
		void scaleChanged(double scale);
	private:
										//���� - ��С����
		void zoomResetPrivate();

	//-----------------------------------
	//----�����ק
	private:
		bool m_mousePressed;
		QPoint m_mousePos;
		QPoint m_mousePosLast;


	//-----------------------------------
	//----������
	private slots:
										//������ - ����������
		void scrollValueChanged();

	//-----------------------------------
	//----�����¼�
	private:
										//�����¼� - ��갴�£���δʹ�ã�
		void I_SPVView::mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//�����¼� - ���̧����δʹ�ã�
		void I_SPVView::mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//�����¼� - ����ƶ�����δʹ�ã�
		void I_SPVView::mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//�����¼� - ������
		void I_SPVView::wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

};

#endif // I_SPVView_H
