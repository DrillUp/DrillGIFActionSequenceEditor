#ifndef I_APVScene_H
#define I_APVScene_H

#include <QGraphicsScene>
#include "Source/GraphModule/item/maskBackgroundItem/i_MaskBackgroundItem.h"
#include "Source/GraphModule/item/gridLineItem/p_GridLineItem.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ�鿴��-��ͼ ����.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�鿴�仯��ͼ�Ļ�����
					����ϸ��.cpp��
-----==========================================================-----
*/
class I_APVScene : public QGraphicsScene
{
    Q_OBJECT
	
	public:
		I_APVScene(QWidget *parent = 0);		//���캯��
		~I_APVScene();							//��������
		void init();							//��ʼ��
		
		
	//-----------------------------------
	//----����¼�
	private:
										//�����¼� - ��갴��
		void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
										//�����¼� - ���̧��
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
										//�����¼� - ����ƶ�
		void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
										//�����¼� - ���˫��
		void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;


	//-----------------------------------
	//----����
	private:
		int m_canvasWidth;			//�������
		int m_canvasHeight;			//�����߶�
		int m_canvasThickness;		//������Ѫ�ߺ��
		int m_pixelWidth;			//ͼ����
		int m_pixelHeight;			//ͼ��߶�
		
	//-----------------------------------
	//----��������
	private:
		QColor m_gridLineColor;						//�����ߵ�ɫ
		QColor m_backgroundColor;					//������ɫ
		P_GridLineItem* m_P_GridLineItem;			//������
		I_MaskBackgroundItem* m_maskBackground;		//����
	public:
									//���� - ����������
		void setGridLine(int column, int row);
									//���� - ���������
		void clearGridLine();
	private:
									//���� - ˢ�±���
		void refreshBackground();
		
	//-----------------------------------
	//----��Դ
	private:
		QList<QFileInfo> m_fileList;
		QList<QPixmap> m_bitmapList;
	public:
									//��Դ - ������Դ
		void setSource(QList<QFileInfo> file_list);
									//��Դ - ��ȡ��Դ
		QList<QFileInfo> getSource();
									//��Դ - �����Դ
		void clearSource();

	protected:
									//��Դ - �ؽ�����
		void rebuildScene();
									//��Դ - �����Դ��˽�У�
		void addSource(QFileInfo file);

	//-----------------------------------
	//----����֡
	private:
		QList<QGraphicsPixmapItem*> m_animList;		//����֡�б�
		int m_curFrame;
	public:
									//����֡ - �л�֡������������
		void setAnimFrame(int index);
									//����֡ - �л�֡��������Դ���ƣ�
		void setAnimName(QFileInfo file);
									//����֡ - ˢ��֡��ʾ
		void refreshFrame();
									//����֡ - ���߶�
		int getMaxHeight();
									//����֡ - �����
		int getMaxWidth();

};

#endif // I_APVScene_H
