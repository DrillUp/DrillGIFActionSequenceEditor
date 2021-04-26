#ifndef P_AnimPictureViewer_H
#define P_AnimPictureViewer_H

#include <QFileInfo>
#include <QtGui/QMouseEvent>
#include "private/i_APVView.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ�鿴��-��ͼ ��װ��.h
		�汾��		v1.03
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
	//----����֡
	public slots:
										//����֡ - ���� ͼƬ��Դ
										//		��˵��1�������ڵײ��ѹ̻�������ֻ��������Դ�ļ���QPixmap��֧�֡�
										//		��˵��2����������������л�֡����Դ�����ظ����������Դ�����л�֡���������ظ���
		void setSource(QList<QFileInfo> file_list);
										//����֡ - ��ȡ ͼƬ��Դ
		QList<QFileInfo> getSource();
										//����֡ - �����Դ
		void clearSource();
										//����֡ - �л�֡������������
										//		��˵�������������Խ�磬������ʾ�κ�ͼ��
		void setAnimFrame(int index);
										//����֡ - �л�֡��������Դ���ƣ�
										//		��˵������û����Դ���Զ���ӡ����ʱ��ȥ�ء�
		void setAnimFile(QFileInfo file);
		

	//-----------------------------------
	//----����
	public slots:
										//���� - ����������
		void setGridLine(int column, int row);
										//���� - ���������
		void clearGridLine();
		
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

#endif // P_AnimPictureViewer_H