#ifndef I_SPVView_H
#define I_SPVView_H

#include <QGraphicsView>
#include "i_SPVScene.h"

#include "Source/GraphModule/middleware/mouseResizeController/p_MouseResizeController.h"

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
		P_MouseResizeController* m_p_MouseResizeController;
	public:
										//���� - ��ȡ����
		I_SPVScene* getScene();
										//���� - ��ȡ������ſ�����
		P_MouseResizeController* getMouseResizeController();


	//-----------------------------------
	//----������
	private slots:
										//������ - ����������
		void scrollValueChanged();

	//-----------------------------------
	//----�����¼�
	private:
										//�����¼� - ��갴��
		void I_SPVView::mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//�����¼� - ���̧��
		void I_SPVView::mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//�����¼� - ����ƶ�
		void I_SPVView::mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
										//�����¼� - ������
		void I_SPVView::wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

};

#endif // I_SPVView_H
