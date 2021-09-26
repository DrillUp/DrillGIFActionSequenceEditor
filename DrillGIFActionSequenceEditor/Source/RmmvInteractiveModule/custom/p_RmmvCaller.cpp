#include "stdafx.h"
#include "p_RmmvCaller.h"

#include "Source/MainModule/drillgifactionsequenceeditor.h"

#include "Source/Utils/manager/callManager/s_CallManager.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		rmmv�����ߣ����ࣩ.cpp
		�汾��		v1.01
		����ģ�飺	����ģ��
		���ܣ�		��rmmv��������ĵ�����ֻ����һЩ���������Ĳ�����

		˵����		Ϊ�˱�֤�ӿ�Ψһ�ԣ�
					��rmmv���/�ļ���������ز������ڸý���ģ���С�
					����ģ�鶼����ֱ����rmmv���������뾭���� ���� ���н�����

		Ŀ�꣺		-> ��rmmv
					-> ����rmmv
					-> ����rmmv
					
		���⣺		> �������rmmv�༭�����ٱ�������������Ч�ķ�����
					> ��Ȼ����������rmmv���棬���Ǳ�������޸ģ���ʱ��rmmv�еı༭�������ǲ�ͬ���ġ�

		ʹ�÷�����
				>�̳и���
					
-----==========================================================-----
*/
P_RmmvCaller::P_RmmvCaller(){
}
P_RmmvCaller::~P_RmmvCaller() {
}

/*-------------------------------------------------
		rmmv - ��rmmv
*/
C_RmmvProjectData P_RmmvCaller::callRmmvSelect(){
	QFileDialog fd;
	fd.setWindowTitle("��rmmv����");
	fd.setAcceptMode(QFileDialog::AcceptOpen);		//�Ի������ͣ���/���棩����������ļ�������ʾ��
	fd.setDirectory(".");							//Ĭ��Ŀ¼
	fd.setNameFilters(QStringList()					//�ļ���ʽ
		<< "rmmv�����ļ�(*.rpgproject)"
		);
	fd.setViewMode(QFileDialog::Detail);

	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()) {
			//��û��ѡ���ļ�����������
			return C_RmmvProjectData();
		}
		QString file_path = fd.selectedFiles().at(0);

		C_RmmvProjectData data = C_RmmvProjectData();
		data.initFromFile(file_path);
		return data;
	}
	else {
		//������رջ���ȡ�������������
		return C_RmmvProjectData();
	}
}

/*-------------------------------------------------
		��rmmv�༭��
*/
void P_RmmvCaller::callRmmvSoftware(C_RmmvProjectData rmmvProjectData){
	QString rmmv_window_name = rmmvProjectData.getName() + " - RPG Maker MV";

	// > �Ѵ���ʱ�������
	int num = S_CallManager::getInstance()->getWindowNum(rmmv_window_name);
	if (num >= 1){
		S_CallManager::getInstance()->activeWindow(rmmv_window_name);
		return;
	}
	
	// > ��·��
	QDesktopServices::openUrl(QUrl("file:/" + rmmvProjectData.getRootPath() + "/Game.rpgproject"));
}

/*-------------------------------------------------
		rmmv - ����rmmv����rmmv�ȱ��棬Ȼ���Լ��棩
*/
void P_RmmvCaller::callRmmvSave(C_RmmvProjectData rmmvProjectData){
	QString rmmv_window_name = rmmvProjectData.getName() + " - RPG Maker MV";

	// > rmmv�������
	int num = S_CallManager::getInstance()->getWindowNum(rmmv_window_name);
	if (num > 1){
		QMessageBox::warning(DrillGIFActionSequenceEditor::getInstance(), "����", "��ͬʱ����" + TTool::_auto_(num) + "��һģһ����rmmv���̴��ڣ���ȷ��ֻ��һ��rmmv���̴��ڡ�", QMessageBox::Yes);
		return;
	}else if (num == 0){
		this->saveDataToRmmv(rmmvProjectData);
		return;
	}

	// > rmmv����
	HWND hwnd = S_CallManager::getInstance()->getWindow(rmmv_window_name);
	int success = S_CallManager::getInstance()->callSave(hwnd);

	// > ���̸��Ǳ���
	this->saveDataToRmmv(rmmvProjectData);

	// > ���ؽ���
	S_CallManager::getInstance()->activeLocalWindow( DrillGIFActionSequenceEditor::getInstance() );
	if (!success){
		QMessageBox::warning(DrillGIFActionSequenceEditor::getInstance(), "����", "����ʧ�ܣ����ȹر�����rmmv�����ڱ༭�Ĵ��ڡ�", QMessageBox::Yes);
		return;
	}
}
/*-------------------------------------------------
		rmmv - ����rmmv��Ҫ���û��ر�rmmv�����ܱ��棩
*/
void P_RmmvCaller::callRmmvSave_RequestingClose(C_RmmvProjectData rmmvProjectData){
	QString rmmv_window_name = rmmvProjectData.getName() + " - RPG Maker MV";

	// > rmmv�������
	int num = S_CallManager::getInstance()->getWindowNum(rmmv_window_name);
	if (num >= 1){
		QMessageBox::information(nullptr, "��ʾ", "����ʧ�ܡ�\n���ڸ��µ����ݲ���ͬ�����Ѵ򿪵�rmmv�༭���У�\n���Ա����ȹرչ���\"" + rmmvProjectData.getName() + "\"���ٽ��б��档", QMessageBox::Yes);
		return;
	}

	this->saveDataToRmmv(rmmvProjectData);
}

/*-------------------------------------------------
		rmmv - ����rmmv
*/
void P_RmmvCaller::callRmmvRun(C_RmmvProjectData rmmvProjectData){
	QString rmmv_window_name = rmmvProjectData.getName() + " - RPG Maker MV";

	//>�����������
	int num = S_CallManager::getInstance()->getWindowNum(rmmv_window_name);
	if (num > 1){
		QMessageBox::warning(DrillGIFActionSequenceEditor::getInstance(), "����", "��ͬʱ����" + TTool::_auto_(num) + "��һģһ����rmmv���̴��ڣ���ȷ��ֻ��һ��rmmv���̴��ڡ�", QMessageBox::Yes);
		return;
	}else if (num == 0){

		// ...����Ҫ�ҵ�һ�����ʵķ�������û��rmmv����ʱ������rmmv��

		QMessageBox::warning(DrillGIFActionSequenceEditor::getInstance(), "����", "��û�д�rmmv�����", QMessageBox::Yes);
		return;
	}

	//>��������
	HWND hwnd = S_CallManager::getInstance()->getWindow(rmmv_window_name);
	int success = S_CallManager::getInstance()->callRun(hwnd);

	//>���ؽ���
	S_CallManager::getInstance()->activeLocalWindow(DrillGIFActionSequenceEditor::getInstance());
	if (!success){
		QMessageBox::warning(DrillGIFActionSequenceEditor::getInstance(), "����", "����ʧ�ܣ����ȹر�����rmmv�����ڱ༭�Ĵ��ڡ�", QMessageBox::Yes);
		return;
	}
}
