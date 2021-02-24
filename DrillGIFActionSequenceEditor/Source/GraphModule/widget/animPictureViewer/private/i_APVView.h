#ifndef I_APVView_H
#define I_APVView_H

#include <QGraphicsView>
#include "i_APVScene.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ�鿴�� ��ͼ.h
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���ӿؼ��볡�����м��࣬���ƹ����������Ź��ܡ�
					����ϸ��.cpp��
-----==========================================================-----
*/
class I_APVView : public QGraphicsView
{
    Q_OBJECT
	
	public:
		I_APVView(QWidget *parent = 0);			//���캯��
		~I_APVView();							//��������
		void init();							//��ʼ��
		
		
	//-----------------------------------
	//----����
	private:
		I_APVScene* m_scene;
	public:
									//���� - ��ȡ����
		I_APVScene* getScene();


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
		void I_APVView::mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//�����¼� - ���̧����δʹ�ã�
		void I_APVView::mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//�����¼� - ����ƶ�����δʹ�ã�
		void I_APVView::mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//�����¼� - ������
		void I_APVView::wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

};

#endif // I_APVView_H
