#ifndef I_FCTLeaf_H
#define I_FCTLeaf_H

#include "Source/Utils/common/TTool.h"
#include "c_FCTConfig.h"

/*
-----==========================================================-----
		�ࣺ		Ҷ��.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		Ҷ�ӿؼ��ࡣ
-----==========================================================-----
*/
class I_FCTLeaf : public QTreeWidgetItem{

	public:
		I_FCTLeaf(C_FCTConfig* config, QTreeWidgetItem *parent = nullptr);
		~I_FCTLeaf();

	//-----------------------------------
	//----����
		TTOOL_DATA_PROPERTY(int, Id);							//��ʶ
		TTOOL_DATA_PROPERTY(QString, Name);						//����
		TTOOL_DATA_PROPERTY(QString, Type);						//��������

		TTOOL_DATA_PROPERTY(QString, Leaf_name_Symbol);			//���Ʒ�֧ - ����ĸ��ʶ
	
	protected:
		C_FCTConfig* m_config;

	//-----------------------------------
	//----�ؼ�
	public:
										//�ؼ� - ��ȡ��ʾ�ı�
		QString getShowingText();
										//�ؼ� - ������������ˢ��
		void refreshItemSelf();	

};

#endif // I_FCTLeaf_H
