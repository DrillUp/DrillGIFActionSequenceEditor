#include "stdafx.h"
#include "S_ProjectWindowManager.h"

#include "data/c_ProjectData.h"

#include "DrillGIFActionSequenceEditor.h"

/*
-----==========================================================-----
		�ࣺ		��Ŀ���� - ���ڹ���.cpp
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		�Դ򿪡��༭���ڽ��в�����������Ӧ��ؿ��ơ�
					�����ڡ�

		ʹ�÷�����
				> ֱ�ӵ��ã�
					S_ProjectWindowManager::getInstance()->newProject();
-----==========================================================-----
*/
S_ProjectWindowManager::S_ProjectWindowManager(){
}
S_ProjectWindowManager::~S_ProjectWindowManager(){
}

/* --------------------------------------------------------------
----------ProjectWindowManager ����
*/
S_ProjectWindowManager* S_ProjectWindowManager::project_manager = NULL;
S_ProjectWindowManager* S_ProjectWindowManager::getInstance() {
	if (project_manager == NULL) {
		project_manager = new S_ProjectWindowManager();
	}
	return project_manager;
}

/*-------------------------------------------------
		���� - �½����Ի���
*/
void S_ProjectWindowManager::newProject() {

	bool is_success = S_ProjectManager::getInstance()->newProject();

	// > �������ݱ仯ʱ
	if (is_success) {
		//...
	}
}
/*-------------------------------------------------
		���� - �򿪣��Ի���
*/
void S_ProjectWindowManager::openProject() {

	bool is_success = S_ProjectManager::getInstance()->openProject();

	// > �������ݱ仯ʱ
	if (is_success) {
		//...
	}
}
/*-------------------------------------------------
		���� - ���棨�Ի���
*/
void S_ProjectWindowManager::saveProject() {

	bool is_success = S_ProjectManager::getInstance()->saveProject();
}
/*-------------------------------------------------
		���� - ���Ϊ���Ի���
*/
void S_ProjectWindowManager::saveAs() {

	bool is_success = S_ProjectManager::getInstance()->saveAs();
}

/*-------------------------------------------------
		���� - �򿪹��̱ʼ�
*/
void S_ProjectWindowManager::openModifyWindowNote(){
	W_ProjectNote d(PROJECT_INSTANCE);
	d.setData(S_ProjectManager::getInstance()->data_ProjectData.note);
	if (d.exec() == QDialog::Accepted) {
		S_ProjectManager::getInstance()->data_ProjectData.note = d.getData();
	}
}