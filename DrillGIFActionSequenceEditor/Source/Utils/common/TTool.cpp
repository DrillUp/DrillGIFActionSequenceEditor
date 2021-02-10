#include "stdafx.h"
#include "TTool.h"

/*
-----==========================================================-----
		�ࣺ		Drill_up���߰�.cpp
		�汾��		v1.15
		���ߣ�		drill_up
		���룺		UTF-8
		����ģ�飺	����ģ��
		���ܣ�		һЩ�����Ĺ��ߺ������Լ����롣�����ڷ��ӣ�
		
		Ŀ�꣺		ui��
						-> ���ٸ�ֵ b��a			������������ _QString_/_double_/_int_ ��
						-> buttonBox���Ļ�			������������ _chinese_��
					���֣�
						-> եȡint					������������ _to_int_ ��
						-> եȡdouble				������������ _to_double_ ��
						-> �ַ������				������������ _zeroFill_ ��
					��λ��SI��
						-> SIд��ת��				���������ؼ���"SI"��
						-> ��λ�ַ�����ʾ			���������ؼ���"to_Unit"��
					����
						-> �����ȡ					���������ؼ���"QRegExp"��
						-> �ж�����
					�ļ���������
						-> ��ȡ��׺
						-> ����׺
						-> �滻�Ƿ��ַ�
						-> ��ȡ�ļ����µ������ļ���
						-> ��ȡ�ļ����µ������ļ�����
					http��
						-> Url����ת��
					html��
						-> �ı�תhtml�ı�
						-> ��ɫתhtml��ɫ
					���ٲ�����
						-> doubleת�ַ���
						-> intת�ַ���
						-> ��ȡΨһid
						-> ��QJsonObject�ŵ���һ������
						-> getter��setter�궨�壨ͷ�ļ��У�

		˵����		1._QString_�������������ܶ��޸���������� lineedit��doublespinbox �໥�ĵ������
					2._QString_�������������ܶ��鷳��QTableWidgetItem�ȸ��ֱ༭��ֵ�����
					3._double_֮����Ҫ��ָ�룬����Ϊ�� TTool::_double_(&local_CurveRadius, ui.lineEdit);
					  �������_double_��ֵʱ��ָ�벻����ͨ�� x() ������ֱ�ӷ���ֵ�����������Ҫ��local_xxx����һ����ת��ֵ��
					  ��Ϊ��������ui�������б�Ҫdata��ui֮�������ת��

		ʹ�÷�����	ȫ��Ϊ��̬���������伴�á�

-----==========================================================-----
*/
TTool::TTool()
{
}


TTool::~TTool()
{
}

/*========================================================================*/

QByteArray TTool::encodeUrlData(QJsonObject json_obj){
	return QUrl( encodeUrlDataString(json_obj)).toEncoded();
}
QString TTool::encodeUrlDataString(QJsonObject json_obj){
	QString encode_data;
	for (size_t i = 0; i < json_obj.keys().size(); i++)
	{
		if (i != 0) encode_data += "&";

		QString cur_key = json_obj.keys().at(i);
		if (json_obj.value(cur_key).isString()){
			encode_data += QString("%1=").arg(cur_key) + (QUrl::toPercentEncoding(json_obj.value(cur_key).toString()));
		}
		else if (json_obj.value(cur_key).isObject()){
			QJsonObject obj = json_obj.value(cur_key).toObject();
			encode_data += QString("%1=").arg(cur_key) + (QUrl::toPercentEncoding(QString(QJsonDocument(obj).toJson()))) ;
		}
		else if (json_obj.value(cur_key).isDouble()){
			encode_data += QString("%1=%2").arg(cur_key).arg(json_obj.value(cur_key).toDouble());
		}
		else{
			encode_data += QString("%1=%2").arg(cur_key).arg(json_obj.value(cur_key).toInt());
		}
	}
	return encode_data;
}

/*========================================================================*/

QString TTool::_to_HtmlText_(QString text){
	QString result = QString(text);
	result.replace("&", "&amp;");	//��˳�����ң���ֹ�ظ��滻��
	result.replace(">", "&gt;");
	result.replace("<", "&lt;");
	result.replace("\"", "&quot;");
	result.replace(" ", "&nbsp;");
	result.replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;");
	result.replace("\n", "<br>");
	result.replace("\r", "<br>");
	return result;
}
QString TTool::_to_HtmlColor_(QColor color){
	return QString("#%1%2%3")
				.arg(QString::number(color.red(), 16))
				.arg(QString::number(color.green(), 16))
				.arg(QString::number(color.blue(), 16));
}
QString TTool::_to_HtmlColor_rgb_(QColor color){
	return QString("rgba(%1, %2, %3)")
				.arg(color.red())
				.arg(color.green())
				.arg(color.blue());
}
QString TTool::_to_HtmlColor_rgba_(QColor color){
	return QString("rgba(%1, %2, %3, %4)")
				.arg(color.red())
				.arg(color.green())
				.arg(color.blue())
				.arg(color.alpha());
}


/*========================================================================*/

QString TTool::getFileSuffix(QString file_name){
	return QFileInfo(file_name).suffix();
}
QString TTool::getFileNameWithNewSuffix(QString file_name, QString new_suffix){
	QString old_suffix = QFileInfo(file_name).suffix();
	return file_name.mid(0, file_name.length() - old_suffix.length()) + new_suffix;
}
QString TTool::getFixedFileName(QString file_name, QString replace_str){
	file_name = file_name.replace("\\\\", "/");
	file_name = file_name.replace("\\", "/");
	file_name = file_name.replace("\"", replace_str);
	file_name = file_name.replace("<", replace_str);
	file_name = file_name.replace(">", replace_str);
	file_name = file_name.replace("|", replace_str);
	file_name = file_name.replace(":", replace_str);
	file_name = file_name.replace("*", replace_str);
	file_name = file_name.replace("?", replace_str);

	file_name = file_name.replace("{", replace_str);	//��ʵ�������ַ��������ã�ֻ�Ƿ�ֹ�������
	file_name = file_name.replace("}", replace_str);
	return file_name;
}
QStringList TTool::getChildFileName(QString dir_name) {
	QDir dir_(dir_name);
	QFileInfoList f_list = dir_.entryInfoList(QStringList(), QDir::Files, QDir::Name);
	QStringList result = QStringList();
	for (int i = 0; i < f_list.length(); i++) {
		result.append(f_list.at(i).fileName());
	}
	return result;
}
QStringList TTool::getChildDirName(QString dir_name) {
	QDir dir_(dir_name);
	QFileInfoList f_list = dir_.entryInfoList(QStringList(), QDir::Dirs, QDir::Name);
	QStringList result = QStringList();
	for (int i = 0; i < f_list.length(); i++) {
		QString dir_str = f_list.at(i).fileName();
		if (dir_str == "." || dir_str == "..") { continue; }	//"."��ǰĿ¼��".."��һ��Ŀ¼
		result.append(dir_str);
	}
	return result;
}

/*========================================================================*/

void TTool::_chinese_(QDialogButtonBox *buttonBox){
	if (buttonBox->button(QDialogButtonBox::Ok) != nullptr){
		buttonBox->button(QDialogButtonBox::Ok)->setText("ȷ��");
	}
	if (buttonBox->button(QDialogButtonBox::Save) != nullptr){
		buttonBox->button(QDialogButtonBox::Save)->setText("����");
	}
	if (buttonBox->button(QDialogButtonBox::SaveAll) != nullptr){
		buttonBox->button(QDialogButtonBox::SaveAll)->setText("ȫ������");
	}
	if (buttonBox->button(QDialogButtonBox::Open) != nullptr){
		buttonBox->button(QDialogButtonBox::Open)->setText("��");
	}
	if (buttonBox->button(QDialogButtonBox::Yes) != nullptr){
		buttonBox->button(QDialogButtonBox::Yes)->setText("��");
	}
	if (buttonBox->button(QDialogButtonBox::YesToAll) != nullptr){
		buttonBox->button(QDialogButtonBox::YesToAll)->setText("ȫ��ѡ��");
	}
	if (buttonBox->button(QDialogButtonBox::No) != nullptr){
		buttonBox->button(QDialogButtonBox::No)->setText("��");
	}
	if (buttonBox->button(QDialogButtonBox::NoToAll) != nullptr){
		buttonBox->button(QDialogButtonBox::NoToAll)->setText("ȫ��ѡ��");
	}
	if (buttonBox->button(QDialogButtonBox::Abort) != nullptr){
		buttonBox->button(QDialogButtonBox::Abort)->setText("��ֹ");
	}
	if (buttonBox->button(QDialogButtonBox::Retry) != nullptr){
		buttonBox->button(QDialogButtonBox::Retry)->setText("����");
	}
	if (buttonBox->button(QDialogButtonBox::Ignore) != nullptr){
		buttonBox->button(QDialogButtonBox::Ignore)->setText("����");
	}
	if (buttonBox->button(QDialogButtonBox::Close) != nullptr){
		buttonBox->button(QDialogButtonBox::Close)->setText("�ر�");
	}
	if (buttonBox->button(QDialogButtonBox::Cancel) != nullptr){
		buttonBox->button(QDialogButtonBox::Cancel)->setText("ȡ��");
	}
	if (buttonBox->button(QDialogButtonBox::Discard) != nullptr){
		buttonBox->button(QDialogButtonBox::Discard)->setText("����");
	}
	if (buttonBox->button(QDialogButtonBox::Help) != nullptr){
		buttonBox->button(QDialogButtonBox::Help)->setText("����");
	}
	if (buttonBox->button(QDialogButtonBox::Apply) != nullptr){
		buttonBox->button(QDialogButtonBox::Apply)->setText("Ӧ��");
	}
	if (buttonBox->button(QDialogButtonBox::Reset) != nullptr){
		buttonBox->button(QDialogButtonBox::Reset)->setText("����");
	}
	if (buttonBox->button(QDialogButtonBox::RestoreDefaults) != nullptr){
		buttonBox->button(QDialogButtonBox::RestoreDefaults)->setText("�ָ�Ĭ��");
	}
}
void TTool::_chinese_(QDialogButtonBox *buttonBox, QDialogButtonBox::StandardButton type, QString name){
	if (buttonBox->button(type) != nullptr){
		buttonBox->button(type)->setText(name);
	}
}

/*========================================================================*/

/*-----------------��ȡuiֵ-------------------*/
void TTool::_QString_(QString *a, QComboBox *b, QString default_ ){
	if (b != nullptr){ 
		if (b->currentText().isEmpty()){ *a = default_; }
		else{ *a = b->currentText(); }
	}
}
void TTool::_QString_(QString *a, QTableWidgetItem *b, QString default_ ){
	if (b != nullptr){
		if (b->text().isEmpty()){ *a = default_; }
		else{ *a = b->text(); }
	}
}
void TTool::_QString_(QString *a, QTextEdit *b, QString default_){
	if (b != nullptr){
		if (b->toPlainText().isEmpty()){ *a = default_; }
		else{ *a = b->toPlainText(); }
	}
}
void TTool::_QString_(QString *a, QPlainTextEdit *b, QString default_){
	if (b != nullptr){
		if (b->toPlainText().isEmpty()){ *a = default_; }
		else{ *a = b->toPlainText(); }
	}
}
void TTool::_QString_(QString *a, QLineEdit *b, QString default_){
	if (b != nullptr){
		if (b->text().isEmpty()){ *a = default_; }
		else{ *a = b->text(); }
	}
}
void TTool::_QString_(QString *a, QDoubleSpinBox *b ){
	if (b != nullptr){
		*a = QString::number(b->value(), 'g', 16);
	}
}
void TTool::_QString_(QString *a, QSpinBox *b ){
	if (b != nullptr){
		*a = QString::number(b->value(), 'g', 16);
	}
}

/*-----------------��ȡui����ֵ-------------------*/

static QString _QString_(QComboBox *b, QString default_ = ""){
	if (b != nullptr){
		if (b->currentText().isEmpty()){ return default_; }
		else{ return b->currentText(); }
	}
	return default_;
}
static QString _QString_(QTableWidgetItem *b, QString default_ = ""){
	if (b != nullptr){
		if (b->text().isEmpty()){ return default_; }
		else{ return b->text(); }
	}
	return default_;
}
static QString _QString_(QTextEdit *b, QString default_ = ""){
	if (b != nullptr){
		if (b->toPlainText().isEmpty()){ return default_; }
		else{ return b->toPlainText(); }
	}
	return default_;
}
static QString _QString_(QPlainTextEdit *b, QString default_ = ""){
	if (b != nullptr){
		if (b->toPlainText().isEmpty()){ return default_; }
		else{ return b->toPlainText(); }
	}
	return default_;
}
static QString _QString_(QLineEdit *b, QString default_ = ""){
	if (b != nullptr){
		if (b->text().isEmpty()){ return default_; }
		else{ return b->text(); }
	}
	return default_;
}
static QString _QString_(QDoubleSpinBox *b){
	if (b != nullptr){
		return QString::number(b->value(), 'g', 16);
	}
	return "0.0";
}
static QString _QString_(QSpinBox *b){
	if (b != nullptr){
		return QString::number(b->value(), 'g', 16);
	}
	return "0";
}

/*-----------------��ֵ��ui------------------*/
void TTool::_QString_(QComboBox *a, QString *b, QString default_ ){
	if (a != nullptr){
		if (b->isEmpty()){ a->setCurrentText(default_); }
		else{ a->setCurrentText(*b); }
	}
}
void TTool::_QString_(QTableWidgetItem *a, QString *b, QString default_){
	if (a != nullptr){
		if (b->isEmpty()){ a->setText(default_); }
		else{ a->setText(*b); }
	}
}
void TTool::_QString_(QTextEdit *a, QString *b, QString default_){
	if (a != nullptr){
		if (b->isEmpty()){ a->setText(default_); }
		else{ a->setText(*b); }
	}
}
void TTool::_QString_(QPlainTextEdit *a, QString *b, QString default_){
	if (a != nullptr){
		if (b->isEmpty()){ a->setPlainText(default_); }
		else{ a->setPlainText(*b); }
	}
}
void TTool::_QString_(QLineEdit *a, QString *b, QString default_){
	if (a != nullptr){
		if (b->isEmpty()){ a->setText(default_); }
		else{ a->setText(*b); }
	}
}
void TTool::_QString_(QDoubleSpinBox *a, QString *b, QString default_){
	if (a != nullptr){
		a->setValue(b->toDouble());
	}
}
void TTool::_QString_(QSpinBox *a, QString *b, QString default_){
	if (a != nullptr){
		a->setValue(b->toInt());
	}
}

/*-----------------��ȡuiֵ��double��-------------------*/

void TTool::_double_(double *a, QComboBox *b){
	if (b != nullptr){ *a = b->currentText().toDouble(); }
}
void TTool::_double_(double *a, QTableWidgetItem *b){
	if (b != nullptr){ *a = b->text().toDouble(); }
}
void TTool::_double_(double *a, QLineEdit *b){
	if (b != nullptr){ *a = b->text().toDouble(); }
}
void TTool::_double_(double *a, QDoubleSpinBox *b){
	if (b != nullptr){ *a = b->value(); }
}
void TTool::_double_(QComboBox *a, double *b){
	if (a != nullptr){ a->setCurrentText(QString::number(*b, 'g', 16)); }
}
void TTool::_double_(QTableWidgetItem *a, double *b){
	if (a != nullptr){ a->setText(QString::number(*b, 'g', 16)); }
}
void TTool::_double_(QLineEdit *a, double *b){
	if (a != nullptr) {
		a->setText(QString::number(*b, 'g',16)); }
}
void TTool::_double_(QDoubleSpinBox *a, double *b){
	if (a != nullptr){ a->setValue(*b); }
}

/*-----------------��ȡuiֵ��int��-------------------*/

void TTool::_int_(int *a, QComboBox *b){
	if (b != nullptr){ *a = b->currentIndex(); }
}
void TTool::_int_(int *a, QLineEdit *b){
	if (b != nullptr){ *a = b->text().toInt(); }
}
void TTool::_int_(int *a, QTableWidgetItem *b){
	if (b != nullptr){ *a = b->text().toInt(); }
}
void TTool::_int_(int *a, QSpinBox *b){
	if (b != nullptr){ *a = b->value(); }
}
void TTool::_int_(QComboBox *a, int *b){
	if (a != nullptr){ a->setCurrentIndex( *b); }
}
void TTool::_int_(QTableWidgetItem *a, int *b){
	if (a != nullptr){ a->setText(QString::number(*b, 'g', 16)); }
}
void TTool::_int_(QLineEdit *a, int *b){
	if (a != nullptr){ a->setText(QString::number(*b, 'g', 16)); }
}
void TTool::_int_(QSpinBox *a, int *b){
	if (a != nullptr){ a->setValue(*b); }
}

/*========================================================================*/

int TTool::_to_int_(QString a){
	QRegExp re(TTool::_get_re_int_());
	re.setMinimal(false);
	re.indexIn(a, 0);
	return re.cap(0).toInt();
}
QList<int> TTool::_to_ints_(QString a){
	QRegExp re(TTool::_get_re_int_());
	re.setMinimal(false);
	QList<int> results;
	int pos = 0;
	while ((pos = re.indexIn(a, pos)) != -1)		//ƥ�䲻��ʱ����������-1 ����ѭ��
	{
		pos += re.matchedLength();					//ƥ���ȡ��ƥ�䳤��
		results.append(re.cap(0).toInt());			//ȡ��ƥ���ֵ������results
	}
	return results;
}
int TTool::_to_int_bySI_(QString a){
	QRegExp re(TTool::_get_re_int_SI_());		//int���ֵ�� -2147483648 ~ +2147483647 (4�ֽ�) �� T��p��λ̫�󣬶���mun���int���0��
	re.setMinimal(false);				
	re.indexIn(a, 0);
	int res = re.cap(1).toInt();
	QString unit = re.cap(2);
	if (unit == "k"){ res *= 1000; }
	if (unit == "M"){ res *= 1000000; }
	if (unit == "G"){ res *= 1000000000; }
	return res;
}
QList<int> TTool::_to_ints_bySI_(QString a){
	QRegExp re(TTool::_get_re_int_SI_());	
	re.setMinimal(false);				
	QList<int> results;
	int pos = 0;
	while ((pos = re.indexIn(a, pos)) != -1)	//ƥ�䲻��ʱ����������-1 ����ѭ��
	{
		pos += re.matchedLength();				//ƥ���ȡ��ƥ�䳤��
		int res = re.cap(1).toInt();
		QString unit = re.cap(2);
		if (unit == "k"){ res *= 1000;}
		if (unit == "M"){ res *= 1000000;}
		if (unit == "G"){ res *= 1000000000;}
		results.append(res);					//ȡ��ƥ���ֵ������results
	}
	return results;
}
QString TTool::_zeroFill_(int num, int digit) {
	return QString("%1").arg(num, digit, 10, QLatin1Char('0'));
}
QString TTool::_zeroFill_(long num, int digit) {
	return QString("%1").arg(num, digit, 10, QLatin1Char('0'));
}
QString TTool::_zeroFill_(int num, int digit, QLatin1Char ch) {
	return QString("%1").arg(num, digit, 10, ch);
}
QString TTool::_zeroFill_(long num, int digit, QLatin1Char ch) {
	return QString("%1").arg(num, digit, 10, ch);
}


/*========================================================================*/

double TTool::_to_double_(QString a){
	QRegExp re(TTool::_get_re_double_());	
	re.setMinimal(false);
	re.indexIn(a, 0);
	return re.cap(0).toDouble();
}
QList<double> TTool::_to_doubles_(QString a){
	QRegExp re(TTool::_get_re_double_());
	re.setMinimal(false);
	QList<double> results;
	int pos = 0;
	while ((pos = re.indexIn(a, pos)) != -1)		//ƥ�䲻��ʱ����������-1 ����ѭ��
	{
		pos += re.matchedLength();					//ƥ���ȡ��ƥ�䳤��
		results.append(re.cap(0).toInt());			//ȡ��ƥ���ֵ������results
	}
	return results;
}
double TTool::_to_double_e_(QString a){
	QRegExp re(TTool::_get_re_double_e_());	//e�������Ϊ����
	re.setMinimal(false);
	re.indexIn(a, 0);
	return re.cap(0).toDouble();
}
QList<double> TTool::_to_doubles_e_(QString a){
	QRegExp re(TTool::_get_re_double_e_());	//e�������Ϊ����
	re.setMinimal(false);
	QList<double> results;
	int pos = 0;
	while ((pos = re.indexIn(a, pos)) != -1)		//ƥ�䲻��ʱ����������-1 ����ѭ��
	{
		pos += re.matchedLength();					//ƥ���ȡ��ƥ�䳤��
		results.append(re.cap(0).toDouble());		//ȡ��ƥ���ֵ������results
	}
	return results;
}
double TTool::_to_double_bySI_(QString a){
	QRegExp re(TTool::_get_re_double_SI_());
	re.setMinimal(false);
	re.indexIn(a, 0);
	double res = (re.cap(1) + re.cap(2)).toDouble();
	QString unit = re.cap(3);				// m:1e-3, u:1e-6, n:1e-9, p:1e-12
	if (unit == "k"){ res *= 1000; }		// k:1e3,  M:1e6,  G:1e9,  T:1e12
	if (unit == "M"){ res *= 1000000; }
	if (unit == "G"){ res *= 1000000000; }
	if (unit == "T"){ res *= 1000000000000; }
	if (unit == "m"){ res *= 0.001; }
	if (unit == "u"){ res *= 0.000001; }
	if (unit == "n"){ res *= 0.000000001; }
	if (unit == "p"){ res *= 0.000000000001; }
	return res;
}
QList<double> TTool::_to_doubles_bySI_(QString a){
	QRegExp re(TTool::_get_re_double_SI_());
	re.setMinimal(false);
	QList<double> results;
	int pos = 0;
	while ((pos = re.indexIn(a, pos)) != -1)		//ƥ�䲻��ʱ����������-1 ����ѭ��
	{
		pos += re.matchedLength();					//ƥ���ȡ��ƥ�䳤��
		double res = (re.cap(1) + re.cap(2)).toDouble();
		QString unit = re.cap(3);					// m:1e-3, u:1e-6, n:1e-9, p:1e-12
		if (unit == "k"){ res *= 1000; }			// k:1e3,  M:1e6,  G:1e9,  T:1e12
		if (unit == "M"){ res *= 1000000; }
		if (unit == "G"){ res *= 1000000000; }
		if (unit == "T"){ res *= 1000000000000; }
		if (unit == "m"){ res *= 0.001; }
		if (unit == "u"){ res *= 0.000001; }
		if (unit == "n"){ res *= 0.000000001; }
		if (unit == "p"){ res *= 0.000000000001; }
		results.append(res);						//ȡ��ƥ���ֵ������results
	}
	return results;
}

/*========================================================================*/

QString TTool::_to_QString_bySI_(int a){
	if (a >= 1000000000){
		double b = a * 0.000000001;
		return QString::number(b, 'g', 12) + "G";
	}
	if (a >= 1000000){
		double b = a * 0.000001;
		return QString::number(b, 'g', 12) + "M";
	}
	if (a >= 1000){
		double b = a * 0.001;
		return QString::number(b, 'g', 12) + "k";
	}
	return QString::number(a, 'g', 12);
}
QString TTool::_to_QString_bySI_(double a){
	// m:1e-3, u:1e-6, n:1e-9, p:1e-12
	// k:1e3,  M:1e6,  G:1e9,  T:1e12

	if (a >= 1000000000000){
		double b = a * 0.000000000001;
		return QString::number(b, 'g', 12) + "T";		//16λ�м�С���
	}
	if (a >= 1000000000){
		double b = a * 0.000000001;
		return QString::number(b, 'g', 12) + "G";
	}
	if (a >= 1000000){
		double b = a * 0.000001;
		return QString::number(b, 'g', 12) + "M";
	}
	if (a >= 1000){
		double b = a * 0.001;
		return QString::number(b, 'g', 12) + "k";
	}
	if (a >= 1){
		return QString::number(a, 'g', 12);
	}
	if (a >= 0.001){
		double b = a * 1000;
		return QString::number(b, 'g', 12) + "m";
	}
	if (a >= 0.000001){
		double b = a * 1000000;
		return QString::number(b, 'g', 12) + "u";
	}
	if (a >= 0.000000001){
		double b = a * 1000000000;
		return QString::number(b, 'g', 12) + "n";
	}
	if (a >= 0.000000000001){
		double b = a * 1000000000000;
		return QString::number(b, 'g', 12) + "p";
	}
	return QString::number(a, 'g', 12);
}
QString TTool::_to_QString_bySI_(QString a){
	double d = TTool::_to_double_bySI_(a);
	return TTool::_to_QString_bySI_(d);
}

QString TTool::_to_Unit_(double d, QString unit){
	return QString::number(d, 'g', 16) + unit;
}
QString TTool::_to_Unit_(QString si_qstring, QString unit){
	return si_qstring + unit;
}
QString TTool::_to_Unit_includeEmptyCheck(double d, QString unit, QString default_){
	if (d == 0){
		return default_;
	}
	return QString::number(d, 'g', 16) + unit;
}
QString TTool::_to_Unit_includeEmptyCheck(QString si_qstring, QString unit, QString default_){
	if (si_qstring == "0" || si_qstring == ""){
		return default_;
	}
	return si_qstring + unit;
}

/*========================================================================*/

QString TTool::_get_re_int_() {
	return "(-?\\d+)";
}
QString TTool::_get_re_int_SI_() {			//int���ֵ�� -2147483648 ~ +2147483647 (4�ֽ�) �� T��p��λ̫�󣬶���mun���int���0��
	return "(-?\\d+)([kMG]?)";
}
QString TTool::_get_re_double_(){
	return "(-?\\d+\\.\\d*|-?\\d*\\.\\d+|-?\\d+)";
}
QString TTool::_get_re_double_e_(){
	return "(-?\\d+\\.\\d*|-?\\d*\\.\\d+|-?\\d+)([eE][-+]?\\d+)?";
}
QString TTool::_get_re_double_SI_(){
	return "(-?\\d+\\.\\d*|-?\\d*\\.\\d+|-?\\d+)([eE][-+]?\\d+)?([munpkMGT]?)";
}
QRegExpValidator* TTool::_getValidator_re_int_() {
	return new QRegExpValidator(QRegExp(_get_re_int_()));
}
QRegExpValidator* TTool::_getValidator_re_int_SI_() {
	return new QRegExpValidator(QRegExp(_get_re_int_SI_()));
}
QRegExpValidator* TTool::_getValidator_re_double_(){
	return new QRegExpValidator(QRegExp(_get_re_double_()));
}
QRegExpValidator* TTool::_getValidator_re_double_e_(){
	return new QRegExpValidator(QRegExp(_get_re_double_e_()));
}
QRegExpValidator* TTool::_getValidator_re_double_SI_(){
	return new QRegExpValidator(QRegExp(_get_re_double_SI_()));
}
bool TTool::_match_re_int_(QString test_str) {
	QRegExp re(_get_re_int_());
	re.setMinimal(false);
	QList<double> results;
	int pos = re.indexIn(test_str, 0);
	if (pos == -1) { return false; }
	return re.matchedLength() == test_str.length();
}
bool TTool::_match_re_int_SI_(QString test_str) {
	QRegExp re(_get_re_int_SI_());
	re.setMinimal(false);
	QList<double> results;
	int pos = re.indexIn(test_str, 0);
	if (pos == -1) { return false; }
	return re.matchedLength() == test_str.length();
}
bool TTool::_match_re_double_(QString test_str) {
	QRegExp re(_get_re_double_());
	re.setMinimal(false);
	QList<double> results;
	int pos = re.indexIn(test_str, 0);
	if (pos == -1) { return false; }
	return re.matchedLength() == test_str.length();
}
bool TTool::_match_re_double_e_(QString test_str) {
	QRegExp re(_get_re_double_e_());
	re.setMinimal(false);
	QList<double> results;
	int pos = re.indexIn(test_str, 0);
	if (pos == -1) { return false; }
	return re.matchedLength() == test_str.length();
}
bool TTool::_match_re_double_SI_(QString test_str) {
	QRegExp re(_get_re_double_SI_());
	re.setMinimal(false);
	QList<double> results;
	int pos = re.indexIn(test_str, 0);
	if (pos == -1) { return false; }
	return re.matchedLength() == test_str.length();
}
bool TTool::_has_any_chineseCharacter_(QString test_str) {
	return test_str.contains(QRegExp("[\\x4e00-\\x9fa5]+"));
}



/*========================================================================*/
QString TTool::_auto_(double d) {
	return QString::number(d,'g',16);
}
QString TTool::_auto_(int i) {
	return QString::number(i);
}
QString TTool::_auto_(uint i) {
	return QString::number(i);
}
QString TTool::_unique_id_() {
	QString result = QUuid::createUuid().toString();
	result.replace("{", "");
	result.replace("}", "");
	return result;
}
void TTool::_QJsonObject_put_(QJsonObject *a, QJsonObject b) {
	for (int i = 0; i < b.keys().count(); i++){
		QString key = b.keys().at(i);
		a->insert(key, b.value(key));
	}
}
