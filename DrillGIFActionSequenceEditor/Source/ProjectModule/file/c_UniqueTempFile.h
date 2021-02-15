#pragma once
#include <QList>
#include <QMap>
#include <QUuid>

/*
-----==========================================================-----
		�ࣺ		Ψһ�ļ� ʵ����.h
		�汾��		v1.17
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		�����ļ�ΪΨһ�ļ���ʵ���ࡣ
					
		˵����		1.��temp�ļ�����ȫ��ר�á�
					2.ֻ��һ���ļ�ָ�룬���Ҫ�洢����Ҫ���ʵ���࣬ͬʱ��Ҫ��ԭ�ļ�ת�ơ�
					3.ֻ��temp�ļ���һ��Ŀ¼��ʹ�ã����ܶ༶�ļ��С�

		�ṹ��		��OriginalFile ԭ�ļ�����ID����
								�̶� + Ψһ + �����޸� ��ԭ�ļ�
					��CommonFile ��ǰ�ļ�����name����
								������ʱ�����ǡ��޸ģ����߻ָ�Ϊԭ�ļ���
-----==========================================================-----
*/
class C_UniqueTempFile{

	public:
		C_UniqueTempFile();
		~C_UniqueTempFile();
		
	//-----------------------------------
	//----��ʼ��
	private:
		QString id;						//���
		QString name;					//����
		QString suffix;					//�ļ���׺
		//QFile							//��ǰ�ļ���ʵʱ���ԣ�
	public:
										//��ʼ�� - ���Ƶ�ǰ�ļ��ã��Զ�����temp�ļ����У�
										//		�������� "F:/../filename.facet"
										//		��˵��1�������ظ�ִ�иú�����
										//		��˵��2��ע�⣬�Ḳ��ԭ��temp�ļ����е�"filename.facet"�ļ����������Ҫ���ݡ�
		void initFormRealFile(QString fileurl);
										//��ʼ�� - �������ļ���
										//		������1�� "xxx_xxx_xxx"
										//		������2�� "filename"
										//		������3�� "facet"
										//		��˵��1�������ظ�ִ�иú������ظ�ִ��ʱ��id��ͬ����ԭ�ļ��ᱻɾ���ؽ���
										//		��˵��2���������Ҫר�Ž���һ���ļ����ƣ���Ҫ��ȫ�����ļ�ʱ��ʹ�ø÷�����
										//				 ��ʼ����û�е�ǰ�ļ�����Ҫ�ֶ����ɡ�
		void initFormData(QString id, QString filename, QString suffix);
		
	//-----------------------------------
	//----�ļ�
	public:
										//�ļ� - �ļ��Ƿ����
										//		�����ء� ture/false
		bool exists();
										//�ļ� - �Ƿ�Ϊ�գ�id�����ơ���׺����ǰ�ļ� ȱһ�򷵻�true��
										//		�����ء� ture/false
		bool isNull();
										//�ļ� - �����Ϣ������ǰ�ļ���ԭ�ļ�
										//		��˵���� ֻ��id���������ơ���׺���ļ���ԭ�ļ�ȫ��
		void clearAll();
		void deleteAll();

	//-----------------------------------
	//----����
	public:
										//���� - ��ȡΨһID
										//		�����ء� "unique_xxxxxxxxx"������·����
		QString getID();
										//���� - ��ȡ��׺
										//		�����ء� "facet"
		QString getSuffix();
										//���� - ��ȡ�ַ���
										//		�����ء� "filename"
		QString getName();
										//���� - �޸��ַ���
										//		�������� "filename"
		void setName(QString name);
										//���� - �޸��ַ���
										//		������1�� "filename"
										//		������2�� "facet"
		void setNameAndSuffix(QString name, QString suffix);
	private:
										//���� - �����Ƿ�ȱ��
		bool isAnyDataEmpty();
		
	//-----------------------------------
	//----ԭ�ļ���˽�У�
	public:
										//ԭ�ļ� - ��ȡԭ�ļ�
										//			��˵����ֻ���������޸ġ����Ҫ�޸ģ���recoverCommonFile�ٲ���CommonFile��
		QFileInfo getOriginalFile();
	private:
										//ԭ�ļ� - ��·����F:/../temp/workspace/unique/��
		QString getOriginalFileParentPath();
										//ԭ�ļ� - ��ȡԭ�ļ���������׺��
		QString getOriginalFileName();
										//ԭ�ļ� - ��鲢����ԭ�ļ�
		void checkAndBuildOriginalFile();
										//ԭ�ļ� - ɾ��ԭ�ļ�
		void deleteOriginalFile();
		
	//-----------------------------------
	//----��ǰ�ļ�
	public:
										//��ǰ�ļ� - ��ȡ��ǰ�ļ�
		QFileInfo getCommonFile();
										//��ǰ�ļ� - ��ȡ�ļ���������׺��
										//			�����ء� "filename.facet"������·����
										//			�����ء� ""���ļ������������
		QString getCommonFileName();
										//��ǰ�ļ� - ��ȡ�ļ�����·��
										//			�����ء� "F:/../temp/workspace/filename.facet"
										//			�����ء� ""���ļ������������
		QString getCommonFileFullPath();
										//��ǰ�ļ� - �ָ�Ϊԭ�ļ�
		void recoverCommonFile();
										//��ǰ�ļ� - ɾ��
										//			��˵���� id�����ơ���׺������ֻ�ǵ�ǰ�ļ���ɾ��
		void deleteCommonFile();
	private:
										//��ǰ�ļ� - ��鲢������ǰ�ļ�
		void checkAndBuildCommonFile();

	//-----------------------------------
	//----������
	public:
										//ʵ���� -> QJsonObject
		QJsonObject getJsonObject();
										//QJsonObject -> ʵ����
		void setJsonObject(QJsonObject obj);
										//���������
		const bool operator== (const C_UniqueTempFile& a)const;
};

