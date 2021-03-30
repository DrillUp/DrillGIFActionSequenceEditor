#pragma once

#include <QWidget>
#include "c_FastClass.h"

/*
-----==========================================================-----
		�ࣺ		���ٱ�.h
		�汾��		v1.14
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���ٻ��Ƴ���Ҫ��д�ı��С�
					�����ܹ����ٻ�ȡ����ֵ��
					����ϸ��cpp��
-----==========================================================-----
*/
class C_FastForm;
class P_FastForm : public QObject
{
	Q_OBJECT

	public:
		P_FastForm(QWidget* _parent);		//���캯��
		~P_FastForm();						//��������
		
	//-----------------------------------
	//----���ؼ�
	protected:
		QWidget* m_parent;				//����
		QGridLayout* m_layout;			//������
	private:
										//���ؼ���ʼ��
		void initWidgetAndLayout(QWidget* _parent);


	//-----------------------------------
	//----׼��
	protected:
		QList<C_FastClassParam> m_param_list;	//׼������
	public:
										//��׼�� - ��������
		void prepareQSpacerItem();
										//��׼�� - ������ǩ
		void prepareQLabel(QString name);
										//��׼�� - ��ǩ + �����
		void prepareQLineEdit(QString name, QString default_value);
										//��׼�� - ��ǩ + ������
		void prepareQComboBox(QString name, QStringList choose_list, QString default_value);

										//׼�� - ������
		void prepareFastClass(C_FastClass c_f);
										//׼�� - ����ȫ������
		void clearPrepare();
										//׼�� - �Ƿ�׼������
		bool hasPreparedSequence();
		
	//-----------------------------------
	//----�ؼ�
	protected:
		QList<C_FastForm*> m_form_list;
	public:
										//�ؼ� - �ؽ��ؼ�
		void rebuildUI();
										//�ؼ� - ����UI
		virtual void clearUI();
										//�ؼ� - ������׺���
		void lockSuffixWidth(int width);

	//-----------------------------------
	//----��ȡ
	public:
										//��ȡ - �༭��û���򷵻�nullptr��
		QLineEdit* getQLineEditByName(QString name);
		QList<QLineEdit*> getAllQLineEdit();
										//��ȡ - ������û���򷵻�nullptr��
		QComboBox* getQComboBoxByName(QString name);
		QList<QComboBox*> getAllQComboBox();
										//��ȡ - ��ѡ��û���򷵻�nullptr��
		QCheckBox* getQCheckBoxByName(QString name);
		QList<QCheckBox*> getAllQCheckBox();
										//��ȡ - ���ֿؼ���û���򷵻�nullptr��
		QSpinBox* getQSpinBoxByName(QString name);
		QList<QSpinBox*> getAllQSpinBox();
										//��ȡ - ���ֿؼ�double��û���򷵻�nullptr��
		QDoubleSpinBox* getQDoubleSpinBoxByName(QString name);
		QList<QDoubleSpinBox*> getAllQDoubleSpinBox();

										//��ȡ - Ⱥ��-���пؼ���û���򷵻�nullptr��
		QWidget* getOneLineByName(QString class_name);
		QList<QWidget*> getOneLineByDepth(int depth);
		QList<QWidget*> getAllOneLine();
										//��ȡ - Ⱥ��-Ⱥ��ؼ���û���򷵻�nullptr��
		QGroupBox* getQGroupBoxByName(QString class_name);
		QList<QGroupBox*> getQGroupBoxByDepth(int depth);
		QList<QGroupBox*> getAllQGroupBox();
										//��ȡ - Ⱥ��-����ؼ���û���򷵻�nullptr��
		QWidget* getQWidgetGroupByName(QString class_name);
		QList<QWidget*> getQWidgetGroupByDepth(int depth);
		QList<QWidget*> getAllQWidgetGroup();
		
	//-----------------------------------
	//----����
	public:
										//���� - ��ȡ�����б�
		QStringList getAllNameChinese();
		QStringList getAllNameEnglish();
		QStringList getAllNameShowing();
										//���� - ��ȡ����ֵ
		QString getValueByName(QString name);
										//���� - ����ĳ���ؼ���ֵ
		void setValueByName(QString name,QString value);

										//���� - ��ȡ�������� { ����:ֵ } ��
		virtual QJsonObject getJsonObject();
										//���� - ���ò������� { ����:ֵ } ��
		virtual void setJsonObject(QJsonObject p);
										//���� - ���ò����������������Ĭ��ֵ��
		virtual void setJsonObjectAutoDefault(QJsonObject p);
										//���� - ���������Ƿ�ǿ�
		virtual bool isAllNotNullConform();
		
	//-----------------------------------
	//----������˽�У�
	protected:
		QList<QWidget*> m_frames;		//�������
		int m_depth;					//��ȣ�����uiʱʹ�ã�
	protected:
										//���� - ��������
		virtual C_FastForm* _createForm();
										//���� - ��ʼ��
		virtual void _initForm(C_FastForm* c_f);
										//���� - �����ؼ�
		virtual void _addSingle(C_FastClassParam c_fp, QGridLayout* layout, int curRow, int curCol);
										//���� - ���пؼ�
		virtual void _addOneLine(C_FastClass c_fc, QGridLayout* layout, int curRow, int curCol, bool editable = true);
										//���� - Ⱥ��ؼ�
		virtual void _addQGroupBox(C_FastClass c_fc, QGridLayout* layout, int curRow, int curCol, bool editable = true);
										//���� - ����ؼ�
		virtual void _addQWidgetGroup(C_FastClass c_fc, QGridLayout* layout, int curRow, int curCol, bool editable = true);
	protected:
										//���� - ���飨û���򷵻�nullptr��
		C_FastForm* getFastFormByName(QString name);
		QList<C_FastForm*> getAllFastFormByType(QString type);
										//���� - ��ȡ���
		int getDepth(QString statusTipData);

};
/*
-----==========================================================-----
		�ࣺ		����.h
		����ģ�飺	����ģ��
		���ܣ�		���������Ͻṹ��
-----==========================================================-----
*/
class C_FastForm{
	public:
		C_FastForm();		
		~C_FastForm();		
	public:
		C_FastClassParam params;					//����

		QLabel* label;								//��ǩ����ʾ���ƣ�
		QLineEdit* lineEdit;						//�ı��༭��
		QComboBox* comboBox;						//������
		QStyledItemDelegate* comboBoxDelegate;		//������ѡ������
		QCheckBox* checkBox;						//��ѡ��
		QSpinBox* spinBox;							//���ֿؼ�
		QDoubleSpinBox* doubleSpinBox;				//���ֿؼ���double��
		QPlainTextEdit* plainTextEdit;				//�ı���

		QLabel* label_suffix;						//��׺
		QSpacerItem* spacerItem;					//����

	public:
		virtual void xxx();		//ǿ����vsʶ������ת������
};