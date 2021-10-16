#ifndef I_FCTLeaf_H
#define I_FCTLeaf_H

#include "Source/Utils/common/TTool.h"
#include "c_FPT_Config.h"

/*
-----==========================================================-----
		�ࣺ		Ҷ��.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		Ҷ�ӿؼ��ࡣ
-----==========================================================-----
*/
class I_FPT_Leaf : public QTreeWidgetItem{

	public:
		I_FPT_Leaf(C_FPT_Config* config, QTreeWidgetItem *parent = nullptr);
		~I_FPT_Leaf();

	//-----------------------------------
	//----����
		TTOOL_DATA_PROPERTY(int, Id);							//��ʶ
		TTOOL_DATA_PROPERTY(QString, Name);						//����
		TTOOL_DATA_PROPERTY(QString, Type);						//��������

		TTOOL_DATA_PROPERTY(QString, Leaf_name_Symbol);			//���Ʒ�֧ - ����ĸ��ʶ
	
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

#endif // I_FCTLeaf_H
