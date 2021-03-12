#ifndef I_APVScene_H
#define I_APVScene_H

#include <QGraphicsScene>

/*
-----==========================================================-----
		�ࣺ		ͼƬ�鿴�� ����.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�����������е���Ӻ������ӡ�
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
		QColor m_backgroundColor;					//��ɫ
		QGraphicsPixmapItem* m_maskBackground;		//����
	private:
									//���� - ˢ�±���
		void refreshBackground();
									//���� - ��ȡ��������ͼ
		QPixmap getBitmapMaskBackground();
		
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
	//----����
	private:
		QList<QGraphicsPixmapItem*> m_animList;		//����֡�б�
		int m_curFrame;
	public:
									//���� - �л�֡������������
		void setAnimFrame(int index);
									//���� - �л�֡��������Դ���ƣ�
		void setAnimName(QFileInfo file);
									//���� - ˢ��֡��ʾ
		void refreshFrame();
									//���� - ��Դ�߶�
		int getMaxHeight();
									//���� - ��Դ���
		int getMaxWidth();

};

#endif // I_APVScene_H
