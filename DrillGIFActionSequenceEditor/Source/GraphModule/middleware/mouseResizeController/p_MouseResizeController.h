#ifndef P_MouseResizeController_H
#define P_MouseResizeController_H

#include <QtGui/QMouseEvent>
#include <QGraphicsView>


/*
-----==========================================================-----
		�ࣺ		������ſ�����.h
		�汾��		v1.01
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ÿ���������װ�� QGraphicsView ��ʹ���ܹ�֧�� ����������+�����ק ���ܡ�
					����ϸ��.cpp��
-----==========================================================-----
*/
class P_MouseResizeController : public QObject
{
	Q_OBJECT

	public:
		P_MouseResizeController();				//���캯��
		~P_MouseResizeController();				//��������

	//-----------------------------------
	//----��ʼ��
	private:
		QGraphicsView* m_view;
	public:
										//��ʼ�� - ������ͼ
		void initGraphicsView(QGraphicsView* view);
		
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
										//		������������"ctrl"��"alt"��"ֻ����"�����ַ�����ʾ �ر� �������š�
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
		QPoint m_mousePos;				//����ƶ���
		QPoint m_mousePosLast;			//�����λ�õ�

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
										//�����¼� - �����֣��ӿڣ�
										//			�����ء�������ֵ�������¼��Ƿ�������
		bool event_wheelEvent(QWheelEvent *event);

};

#endif // P_MouseResizeController_H