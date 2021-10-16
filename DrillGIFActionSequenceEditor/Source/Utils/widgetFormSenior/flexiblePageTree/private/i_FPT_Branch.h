#ifndef I_FCTBranch_H
#define I_FCTBranch_H

#include "Source/Utils/common/TTool.h"
#include "c_FPT_Config.h"

/*
-----==========================================================-----
		�ࣺ		��֦.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��֦�ؼ��ࡣ
-----==========================================================-----
*/
class I_FPT_Branch : public QTreeWidgetItem{

	public:
		I_FPT_Branch(C_FPT_Config* config, QTreeWidgetItem *parent = nullptr);
		~I_FPT_Branch();

	//-----------------------------------
	//----����
		TTOOL_DATA_PROPERTY(int, Id);							//���

		TTOOL_DATA_PROPERTY(int, Branch_id_Index);				//ID��֧ - ��ǰҳ������λ��
		TTOOL_DATA_PROPERTY(int, Branch_id_Bottom);				//ID��֧ - ��
		TTOOL_DATA_PROPERTY(int, Branch_id_Top);				//ID��֧ - ��

		TTOOL_DATA_PROPERTY(int, Branch_name_Index);			//���Ʒ�֧ - ��ǰҳ������λ��
		TTOOL_DATA_PROPERTY(QString, Branch_name_Showing);		//���Ʒ�֧ - ���Ʋ����ַ��ı�־
		TTOOL_DATA_PROPERTY(QString, Branch_name_Symbol);		//���Ʒ�֧ - ���Ʋ����ַ��ı�־

		TTOOL_DATA_PROPERTY(QString, Branch_type_Name);			//�Զ����֧ - ����
		TTOOL_DATA_PROPERTY(QString, Branch_type_Description);	//�Զ����֧ - ����
		
	protected:
		C_FPT_Config* m_config;		//������

	//-----------------------------------
	//----�ؼ�
	public:
										//�ؼ� - ��ȡ��ʾ�ı�
		QString getShowingText();
										//�ؼ� - ������������ˢ��
		void refreshItemSelf();	

};

#endif // I_FCTBranch_H
