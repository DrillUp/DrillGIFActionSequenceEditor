#ifndef W_ObjectSelector_H
#define W_ObjectSelector_H

#include "ui_W_ObjectSelector.h"
#include "private/c_ObjectSelectorConfig.h"

/*
-----==========================================================-----
		�ࣺ		����ѡ�� ����.h
		�汾��		v1.01
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�� QJsonObject�б� ��ѡ��һ������Ĵ��ڡ���Ҫ��id��name��id����Ϊ���֣�obj����Ϊ�գ�
					Ҳ���Դ� id_list��name_list ��ѡ��һ��id�������ַ������볤����ȣ�
					����ϸ��cpp��
-----==========================================================-----
*/
class P_ObjectSortController;
class C_ObjectSortData;
class W_ObjectSelector : public QDialog
{
	Q_OBJECT

	public:
		W_ObjectSelector(QWidget *parent = 0);
		~W_ObjectSelector();

	//-----------------------------------
	//----�ؼ�
	protected:
		int cur_selectIndex_page;
		int cur_selectIndex_context;
	public slots:
										//�ؼ� - ˢ�±��
		void refreshTable();
										//�ؼ� - ˢ�±��
		void refreshTable_page();
		void refreshTable_context();
										//�ؼ� - ҳ�仯
		void currentPageChanged(QTableWidgetItem* cur, QTableWidgetItem* pre);
		
	//-----------------------------------
	//----ѡ��������
	protected:
		C_ObjectSelectorConfig m_config;	//ѡ��������
	public:
										//ѡ�������� - ���ܿ���
		void setConfigBtnVisible(bool visible);
										//ѡ�������� - ���ò���
		void setConfigParam(QJsonObject config);
										//ѡ�������� - ȡ������
		QJsonObject getConfigParam();
	public slots:
										//ѡ�������� - �༭����
		void openConfigParamWindow();


	//-----------------------------------
	//----��Դ����
	protected:
		P_ObjectSortController* m_source_ObjectSortController;		//�������������
		QList<QJsonObject>* m_source_list;							//ԭ����
	public:
										//��Դ���� - ����
		void loadSource(QList<QJsonObject>* obj_list);
		void loadSource(QList<QJsonObject>* obj_list, QString id_symbol, QString name_symbol, QString type_symbol);
										//��Դ���� - ��ȡѡ�е�����
		int getSelectedSourceId();
		QString getSelectedSourceName();
		QString getSelectedSourceType();
	private:
										//��Դ���� - ��ȡѡ�еĶ���˽�У�
		C_ObjectSortData* getSelectedSource();
	
	//-----------------------------------
	//----����
	public:
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();
	public slots:
										//���� - �ύ���ݣ�У�飩
		void acceptData();	

	private:
		Ui::W_ObjectSelector ui;
	//-----------------------------------
};

#endif // W_ObjectSelector_H
