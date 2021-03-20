#ifndef I_APVView_H
#define I_APVView_H

#include <QGraphicsView>
#include "i_APVScene.h"

#include "Source/GraphModule/middleware/mouseResizeController/p_MouseResizeController.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ�鿴��-��ͼ ��ͼ.h
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
		P_MouseResizeController* m_p_MouseResizeController;
	public:
										//���� - ��ȡ����
		I_APVScene* getScene();
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
