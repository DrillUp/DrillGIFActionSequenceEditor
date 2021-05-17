#include "stdafx.h"
#include "s_RmmvCaller_ActionSeq.h"

#include "Source/ProjectModule/s_ProjectManager.h"
#include "Source/ProjectModule/file/s_TempFileManager.h"
#include "Source/PluginModule/storageData/s_PluginDataContainer.h"
#include "Source/PluginModule/lengthEditor/s_LEAnnotationReader.h"
#include "Source/PluginModule/lengthEditor/s_LEConfigWriter.h"
#include "Source/ActionSeqModule/actionSeqData/s_ActionSeqDataContainer.h"
#include "../base/s_RmmvDataContainer.h"

/*
-----==========================================================-----
		�ࣺ		rmmv�����ߣ��������� ר�ã�.cpp
		����ģ�飺	����ģ��
		���ܣ�		��rmmv������ģ�飬ֻ���� �������� ����ļ���

		�ӹ��ܣ�	->�ļ�
						-> ��ȡ����ļ�
						-> ��ȡϵͳ�ļ�
					
		ʹ�÷�����
				>�򿪣�
					
			
-----==========================================================-----
*/

S_RmmvCaller_ActionSeq::S_RmmvCaller_ActionSeq(){
	this->init();
}
S_RmmvCaller_ActionSeq::~S_RmmvCaller_ActionSeq() {
}

/* --------------------------------------------------------------
----------RmmvFileManager ����
*/
S_RmmvCaller_ActionSeq* S_RmmvCaller_ActionSeq::cur_manager = NULL;
S_RmmvCaller_ActionSeq* S_RmmvCaller_ActionSeq::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_RmmvCaller_ActionSeq();
	}
	return cur_manager;
}
/* --------------------------------------------------------------
----------��ʼ��
*/
void S_RmmvCaller_ActionSeq::init(){
}


/*-------------------------------------------------
		���� - ��ȡȫ����ع����ļ�
*/
bool S_RmmvCaller_ActionSeq::loadAllRmmvFile(C_RmmvProjectData rmmvProjectData) {
	if (rmmvProjectData.isNull()){ return false; }

	// > ��ȡ����ļ�
	QFileInfo plugin_info = S_RmmvDataContainer::getInstance()->getRmmvFile_PluginsData();
	QFile plugin_file(plugin_info.absoluteFilePath());
	if (!plugin_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "����", "δ�ҵ�plugins.js�ļ���", QMessageBox::Yes);
		return false;
	}
	QString plugin_context = plugin_file.readAll();
	S_PluginDataContainer::getInstance()->loadPluginData(plugin_context);		//����ȡ�ļ���
	plugin_file.close();

	// > ��ȡϵͳ�ļ�
	QFileInfo system_info = S_RmmvDataContainer::getInstance()->getRmmvFile_System();
	QFile system_file(system_info.absoluteFilePath());
	if (!system_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "����", "δ�ҵ�system.js�ļ���", QMessageBox::Yes);
		return false;
	}
	QString system_context = system_file.readAll();// �����޲�����
	system_file.close();

	return true;
}

/*-------------------------------------------------
		���� - ������Դ�ļ�
*/
void S_RmmvCaller_ActionSeq::coverSourceFileToTemp(C_RmmvProjectData rmmvProjectData){
	if (rmmvProjectData.isNull()){ return ; }


	// > ��Դ�ļ���img/Special__actionSeq
	QString temp_path = S_TempFileManager::getInstance()->getTempFileUrl() + "/Special__actionSeq";
	S_TempFileManager::getInstance()->remove_Dir(QDir(temp_path));
	S_TempFileManager::getInstance()->copy_Dir(QDir(rmmvProjectData.path + "/img/Special__actionSeq"), QDir(temp_path));

}



/*-------------------------------------------------
		���� - ��rmmv���̳У�
*/
C_RmmvProjectData S_RmmvCaller_ActionSeq::callRmmvSelect(){
	C_RmmvProjectData result = P_RmmvCaller::callRmmvSelect();
	S_RmmvDataContainer::getInstance()->modify(result);		//�򿪺�������¼λ��
	return result;
}
/*-------------------------------------------------
		���� - ������������ǵ�rmmv��
*/
void S_RmmvCaller_ActionSeq::saveDataToRmmv(C_RmmvProjectData rmmvProjectData){
	//���麯��ֱ�Ӹ�д��


	// > �������ǿ��������
	QFileInfo file_COAS = S_RmmvDataContainer::getInstance()->getRmmvFile_Plugin("Drill_CoreOfActionSequence");
	if (file_COAS.exists()){
		QFile file_COAS_file(file_COAS.absoluteFilePath());
		if (!file_COAS_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			//����ȡʧ��ʱ����������
		}
		QString COAS_context = file_COAS_file.readAll();
		file_COAS_file.close();

		// > ǿ�Ƹ���
		if (COAS_context.contains("��������-%d")){
			QFileInfo COAS_updateFile = QFileInfo(QApplication::applicationDirPath() + "/tools/Drill_CoreOfActionSequence.js");
			S_TempFileManager::getInstance()->copy_File(COAS_updateFile, file_COAS_file);
		}
	}

	// ---------------------------------------------
	// > ��ȡ����ļ�����
	QFileInfo plugin_info = S_RmmvDataContainer::getInstance()->getRmmvFile_PluginsData();
	QFile plugin_file(plugin_info.absoluteFilePath());
	if (!plugin_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "����", "�޷���plugins.js�ļ���", QMessageBox::Yes);
		return ;
	}
	QString plugin_context = plugin_file.readAll();
	S_PluginDataContainer::getInstance()->loadPluginDataNoSignal(plugin_context);		//����ȡ�ļ���
	plugin_file.close();


	// > �����ļ�
	if (rmmvProjectData.optional_backup == true){
		S_TempFileManager::getInstance()->copy_File(plugin_info, plugin_info.absolutePath() + "/plugins_bak.js");
	}


	// ---------------------------------------------
	// > ��ȡע�� + ʵ�ʳ���
	QString script_path = QApplication::applicationDirPath() + "/tools/Drill_CoreOfActionSequence.js";
	QFileInfo pluginScript_info = S_RmmvDataContainer::getInstance()->getRmmvFile_Plugin("Drill_CoreOfActionSequence");
	if (pluginScript_info.exists() == false){
		S_TempFileManager::getInstance()->copy_File(QFileInfo(script_path), pluginScript_info);
	}
	C_LEAnnotation* le_annotation = S_LEAnnotationReader::getInstance()->readPlugin(pluginScript_info.absoluteFilePath());
	
	// > �޸�����
	C_PluginData* data = S_PluginDataContainer::getInstance()->getPluginData("Drill_CoreOfActionSequence");
	if (data == nullptr){
		
		data = new C_PluginData();
		data->name = "Drill_CoreOfActionSequence";
		data->status = true;
		S_PluginDataContainer::getInstance()->op_add(data);
	}
	data->description = le_annotation->pluginDesc + "���༭���༭��";
	data->parameters = S_ActionSeqDataContainer::getInstance()->getActionSeqData();	//���� ���������������� �У���ȡ���������ݣ�
	S_PluginDataContainer::getInstance()->op_modify(data);


	// ---------------------------------------------
	// > д���ļ�
	if (!plugin_file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "����", "�޷���plugins.js�ļ���", QMessageBox::Yes);
		return;
	}
	QString new_context = S_PluginDataContainer::getInstance()->writePluginData(); 
	QTextStream write_stream(&plugin_file);
	QTextCodec *codec2 = QTextCodec::codecForName("utf-8");
	write_stream.flush();
	write_stream.setCodec(codec2);
	write_stream.seek(0);				//����0��ʼд��
	write_stream << new_context;
	plugin_file.close();


	// ---------------------------------------------
	// > �ļ����
	QString src_path = rmmvProjectData.path + "/img/Special__actionSeq";
	S_TempFileManager::getInstance()->remove_Dir(src_path);

	// > �ļ�ת��
	QDir(src_path).mkdir(src_path);
	QList<QFileInfo> info_list = S_ActionSeqDataContainer::getInstance()->getAllRelatedFile();
	for (int i = 0; i < info_list.count(); i++){
		QFileInfo info = info_list.at(i);
		S_TempFileManager::getInstance()->copy_File(info.absoluteFilePath(), src_path + "/" + info.fileName());
	}

	// ---------------------------------------------
	// > ������ֵ����
	QFile script_file(pluginScript_info.absoluteFilePath());
	if (!script_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		//����ȡʧ��ʱ����������
	}else{
		QString script_context = script_file.readAll();
		script_file.close();
		C_LEConfigData le_data = C_LEConfigData();
		C_ActionSeqLength as_len = S_ActionSeqDataContainer::getInstance()->getActionSeqLength();

		// > ��������ר����� �������к��� �����Բ�����֪��
		le_data.initParam("Drill_CoreOfActionSequence", "��������-%d", as_len.realLen_actionSeq);
		script_context = S_LEConfigWriter::getInstance()->doOverwritePlugin(script_context, le_annotation->getParamByKey("��������-%d"), le_data);

		le_data.initParam("Drill_CoreOfActionSequence", "״̬Ԫ-%d", as_len.realLen_state);
		script_context = S_LEConfigWriter::getInstance()->doOverwritePlugin(script_context, le_annotation->getParamByKey("״̬Ԫ-%d"), le_data);

		le_data.initParam("Drill_CoreOfActionSequence", "����Ԫ-%d", as_len.realLen_action);
		script_context = S_LEConfigWriter::getInstance()->doOverwritePlugin(script_context, le_annotation->getParamByKey("����Ԫ-%d"), le_data);

		// > д��ű�
		if (script_context != "" &&
			script_file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text)) {

			QTextStream write_stream(&script_file);
			QTextCodec *codec2 = QTextCodec::codecForName("utf-8");
			write_stream.flush();
			write_stream.setCodec(codec2);
			write_stream.seek(0);
			write_stream << script_context;
			script_file.close();
		}
	}


	// ---------------------------------------------
	// > �ļ���ǣ����˵����
	QString explain_context = "";
	explain_context = explain_context + "\n  ���ļ������Բ�� Drill_CoreOfActionSequence ϵͳ - GIF�������к��ġ�";
	explain_context = explain_context + "\n  ������ԷŶ���ͼƬ��ɵ�GIFŶ��";
	explain_context = explain_context + "\n";
	explain_context = explain_context + "\n  ----------------------------------------------------------------------";
	explain_context = explain_context + "\n  ���ļ��� " + S_ProjectManager::getInstance()->data_ProjectData.getSoftname() + " �Զ����ɣ�";
	explain_context = explain_context + "\n";
	explain_context = explain_context + "\n  ��Ŀ����" + S_ProjectManager::getInstance()->data_ProjectData.getName();
	explain_context = explain_context + "\n  ����ʱ�䣺" + QDateTime::currentDateTime().toString("yyyy��MM��dd�� hhʱmm��ss��");
	explain_context = explain_context + "\n  ת���ļ���" + QString::number(info_list.count()) + "��";
	explain_context = explain_context + "\n  ----------------------------------------------------------------------";
	explain_context = explain_context + "\n";
		
	QString explain_file_name = src_path + "/_���˵�� " + QDate::currentDate().toString("yyyy-MM-dd") + ".txt";
	QFile explain_file(explain_file_name);
	if (!explain_file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text)) {
		return;
	}
	QTextStream write_stream2(&explain_file);
	QTextCodec *codec3 = QTextCodec::codecForName("utf-8");
	write_stream2.flush();
	write_stream2.setCodec(codec3);
	write_stream2.seek(0);				//����0��ʼд��
	write_stream2 << explain_context;
	plugin_file.close();

	QMessageBox::information(nullptr, "��ʾ", "����ɹ���", QMessageBox::Yes);
}