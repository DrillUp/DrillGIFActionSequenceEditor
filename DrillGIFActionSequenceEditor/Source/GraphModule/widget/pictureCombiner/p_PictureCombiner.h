#ifndef P_PictureCombiner_H
#define P_PictureCombiner_H

#include <QtWidgets>
#include "ui_P_PictureCombiner.h"
#include "Source/GraphModule/widget/singlePictureViewer/p_SinglePictureViewer.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ�ϳ���.h
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	ͼ��ģ��
		���ܣ�		���Խ�����ͼƬ�Ծ������ʽ�ϲ���һ��
					����ϸ��cpp��
-----==========================================================-----
*/
class P_PictureCombiner : public QWidget
{
	Q_OBJECT

	public:
		P_PictureCombiner(QWidget *parent = 0);
		~P_PictureCombiner();
		
	//-----------------------------------
	//----��ͼ
	protected:
		QString m_iconSrcPath;
		P_SinglePictureViewer* m_p_SinglePictureViewer;
	public slots:
										//��ͼ - ���ű����л�
		void zoomValueChanged(double value);
										//��ͼ - ����ѡ�仯
		void gridLineToggled(bool checked);

	//-----------------------------------
	//----����
	public slots:
										//���� - �����仯
		void rowChanged(int value);
		void columnChanged(int value);
										//���� - ��ѡ�仯������Ӧ��
		void rowToggled(bool checked);
		void columnToggled(bool checked);
										//���� - ������仯
		void comboxChanged(QString text);
	protected:
										//�ؼ� - ��������
		void autoFillNum();
	
	//-----------------------------------
	//----֡����
	public slots:
										//֡���� - �����仯
		void frameWidthChanged(int value);
		void frameHeightChanged(int value);
										//֡���� - ��ѡ�仯��Ĭ��ֵ��
		void frameWidthDefaultToggled(bool checked);
		void frameHeightDefaultToggled(bool checked);

		
	//-----------------------------------
	//----����
	public:
										//���� - �ػ�ͼƬ
		void redrawBitmap();
										//���� - ��ȡ�����
		int getMaxWidth();
										//���� - ��ȡ���߶�
		int getMaxHeight();
										//���� - �жϸ߿�һ��
		bool isSizeFit();
	protected:
										//���� - ��ָ���ķ�Χ�ڻ���ͼ���Զ����У��Զ�����
		void drawBitmap_CenterAndCut(QPainter* painter, QPixmap bitmap, QRect frame);
										//���� - ��ȡ���Ƶ�ͼƬ
		QPixmap getDrawedBitmap(int p_width,int p_height);
		
	//-----------------------------------
	//----��
	protected:
		bool m_slotBlock;
		QPixmap local_draw;
		QList<QPixmap> local_bitmapTank;
	public:
										//�� - ��������
		void setData(QList<QFileInfo> file_tank);
		void setData(QList<QPixmap> bitmap_tank);
										//�� - ȡ������
		QPixmap getData();
										//�� - �������� -> ui����
		void putDataToUi();							
										//�� - ui���� -> ��������
		void putUiToData();

	private:
		Ui::P_PictureCombiner ui;

};

#endif // W_ImportMeshDialog_H
