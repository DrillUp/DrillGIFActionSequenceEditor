#include "stdafx.h"
#include "TTool.h"

/*
-----==========================================================-----
		类：		Drill_up工具包.cpp
		版本：		v1.20
		作者：		drill_up
		编码：		UTF-8
		所属模块：	工具模块
		功能：		一些基本的工具函数，以及代码。（用于防坑）
		
		目标：		ui：
						-> 快速赋值 b给a			【索引：函数 _QString_/_double_/_int_ 】
						-> buttonBox中文化			【索引：函数 _chinese_】
					数字：
						-> 榨取int					【索引：函数 _to_int_ 】
						-> 榨取double				【索引：函数 _to_double_ 】
						-> 字符零填充				【索引：函数 _zeroFill_ 】
					单位与SI：
						-> SI写法转换				【索引：关键字"SI"】
						-> 单位字符串显示			【索引：关键字"to_Unit"】
					正则：
						-> 正则获取					【索引：关键字"QRegExp"】
						-> 判断中文
						-> 字符串后缀数字+1
					文件名操作：
						-> 获取后缀
						-> 换后缀
						-> 替换非法字符
						-> 获取文件夹下的所有文件名
						-> 获取文件夹下的所有文件夹名
					http：
						-> Url参数转码
					html：
						-> 文本转html文本
						-> 颜色转html颜色
					快速操作：
						-> double转字符串
						-> int转字符串
						-> 获取唯一id
						-> getter和setter宏定义（头文件中）
						-> 列表转换
					QJson：
						-> 将QJsonObject放到另一个里面
						-> 列表字符串QJson转换
					QStringList：
						-> 清理空行
						-> 去除注释行（根据前缀）
						-> 全部行保留小数位
						-> 全部行添加前缀/后缀
					QWidget：
						-> 快速创建样式QLabel

		说明：		1._QString_函数用于消除很多修改情况，比如 lineedit与doublespinbox 相互改的情况。
					2._QString_函数用于消除很多麻烦，QTableWidgetItem等各种编辑框赋值情况。
					3._double_之所以要用指针，是因为： TTool::_double_(&local_CurveRadius, ui.lineEdit);
					  当你进行_double_赋值时，指针不允许通过 x() 函数的直接返回值，这就提醒你要用local_xxx建立一个中转的值。
					  因为操作的是ui，所以有必要data和ui之间进行中转。

		使用方法：	全部为静态方法，开箱即用。

-----==========================================================-----
*/
TTool::TTool(){
}
TTool::~TTool(){
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
	result.replace("&", "&amp;");	//（顺序不能乱，防止重复替换）
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

	file_name = file_name.replace("{", replace_str);	//其实这两个字符可以设置，只是防止程序错误
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
		if (dir_str == "." || dir_str == "..") { continue; }	//"."当前目录，".."上一级目录
		result.append(dir_str);
	}
	return result;
}

/*========================================================================*/

void TTool::_chinese_(QDialogButtonBox *buttonBox){
	if (buttonBox->button(QDialogButtonBox::Ok) != nullptr){
		buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
	}
	if (buttonBox->button(QDialogButtonBox::Save) != nullptr){
		buttonBox->button(QDialogButtonBox::Save)->setText("保存");
	}
	if (buttonBox->button(QDialogButtonBox::SaveAll) != nullptr){
		buttonBox->button(QDialogButtonBox::SaveAll)->setText("全部保存");
	}
	if (buttonBox->button(QDialogButtonBox::Open) != nullptr){
		buttonBox->button(QDialogButtonBox::Open)->setText("打开");
	}
	if (buttonBox->button(QDialogButtonBox::Yes) != nullptr){
		buttonBox->button(QDialogButtonBox::Yes)->setText("是");
	}
	if (buttonBox->button(QDialogButtonBox::YesToAll) != nullptr){
		buttonBox->button(QDialogButtonBox::YesToAll)->setText("全部选是");
	}
	if (buttonBox->button(QDialogButtonBox::No) != nullptr){
		buttonBox->button(QDialogButtonBox::No)->setText("否");
	}
	if (buttonBox->button(QDialogButtonBox::NoToAll) != nullptr){
		buttonBox->button(QDialogButtonBox::NoToAll)->setText("全部选否");
	}
	if (buttonBox->button(QDialogButtonBox::Abort) != nullptr){
		buttonBox->button(QDialogButtonBox::Abort)->setText("中止");
	}
	if (buttonBox->button(QDialogButtonBox::Retry) != nullptr){
		buttonBox->button(QDialogButtonBox::Retry)->setText("重试");
	}
	if (buttonBox->button(QDialogButtonBox::Ignore) != nullptr){
		buttonBox->button(QDialogButtonBox::Ignore)->setText("忽略");
	}
	if (buttonBox->button(QDialogButtonBox::Close) != nullptr){
		buttonBox->button(QDialogButtonBox::Close)->setText("关闭");
	}
	if (buttonBox->button(QDialogButtonBox::Cancel) != nullptr){
		buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
	}
	if (buttonBox->button(QDialogButtonBox::Discard) != nullptr){
		buttonBox->button(QDialogButtonBox::Discard)->setText("放弃");
	}
	if (buttonBox->button(QDialogButtonBox::Help) != nullptr){
		buttonBox->button(QDialogButtonBox::Help)->setText("帮助");
	}
	if (buttonBox->button(QDialogButtonBox::Apply) != nullptr){
		buttonBox->button(QDialogButtonBox::Apply)->setText("应用");
	}
	if (buttonBox->button(QDialogButtonBox::Reset) != nullptr){
		buttonBox->button(QDialogButtonBox::Reset)->setText("重置");
	}
	if (buttonBox->button(QDialogButtonBox::RestoreDefaults) != nullptr){
		buttonBox->button(QDialogButtonBox::RestoreDefaults)->setText("恢复默认");
	}
}
void TTool::_chinese_(QDialogButtonBox *buttonBox, QDialogButtonBox::StandardButton type, QString name){
	if (buttonBox->button(type) != nullptr){
		buttonBox->button(type)->setText(name);
	}
}

/*========================================================================*/

/*-----------------获取ui值-------------------*/
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

/*-----------------获取ui返回值-------------------*/

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

/*-----------------赋值给ui------------------*/
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

QString TTool::_QString_escapeToOneLine_(QString data){
	data = data.replace("\n", "\\n");
	data = data.replace("\r", "\\r");
	data = data.replace("\t", "\\t");
	data = data.replace("\"", "\\\"");
	data = "\"" + data + "\"";
	return data;
}
QString TTool::_QString_escapeToRows_(QString data){
	if (data.length() < 2 ){ return data; }
	if (data[0] == '\"'){ data = data.mid(1,data.count()-2); }
	data = data.replace("\\n", "\n");
	data = data.replace("\\r", "\r");
	data = data.replace("\\t", "\t");
	data = data.replace("\\\"", "\"");
	return data;
}

/*-----------------获取ui值（double）-------------------*/

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
void TTool::_double_(QComboBox *a, double b){
	if (a != nullptr){ a->setCurrentText(QString::number(b, 'g', 16)); }
}
void TTool::_double_(QTableWidgetItem *a, double b){
	if (a != nullptr){ a->setText(QString::number(b, 'g', 16)); }
}
void TTool::_double_(QLineEdit *a, double b){
	if (a != nullptr) {
		a->setText(QString::number(b, 'g', 16));}
}
void TTool::_double_(QDoubleSpinBox *a, double b){
	if (a != nullptr){ a->setValue(b); }
}

/*-----------------获取ui值（int）-------------------*/

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
	while ((pos = re.indexIn(a, pos)) != -1)		//匹配不到时，函数返回-1 跳出循环
	{
		pos += re.matchedLength();					//匹配后，取出匹配长度
		results.append(re.cap(0).toInt());			//取出匹配的值，存入results
	}
	return results;
}
int TTool::_to_int_bySI_(QString a){
	QRegExp re(TTool::_get_re_int_SI_());		//int最大值是 -2147483648 ~ +2147483647 (4字节) ， T和p单位太大，而且mun会把int变成0。
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
	while ((pos = re.indexIn(a, pos)) != -1)	//匹配不到时，函数返回-1 跳出循环
	{
		pos += re.matchedLength();				//匹配后，取出匹配长度
		int res = re.cap(1).toInt();
		QString unit = re.cap(2);
		if (unit == "k"){ res *= 1000;}
		if (unit == "M"){ res *= 1000000;}
		if (unit == "G"){ res *= 1000000000;}
		results.append(res);					//取出匹配的值，存入results
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
	while ((pos = re.indexIn(a, pos)) != -1)		//匹配不到时，函数返回-1 跳出循环
	{
		pos += re.matchedLength();					//匹配后，取出匹配长度
		results.append(re.cap(0).toInt());			//取出匹配的值，存入results
	}
	return results;
}
double TTool::_to_double_e_(QString a){
	QRegExp re(TTool::_get_re_double_e_());	//e后面必须为整数
	re.setMinimal(false);
	re.indexIn(a, 0);
	return re.cap(0).toDouble();
}
QList<double> TTool::_to_doubles_e_(QString a){
	QRegExp re(TTool::_get_re_double_e_());	//e后面必须为整数
	re.setMinimal(false);
	QList<double> results;
	int pos = 0;
	while ((pos = re.indexIn(a, pos)) != -1)		//匹配不到时，函数返回-1 跳出循环
	{
		pos += re.matchedLength();					//匹配后，取出匹配长度
		results.append(re.cap(0).toDouble());		//取出匹配的值，存入results
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
	while ((pos = re.indexIn(a, pos)) != -1)		//匹配不到时，函数返回-1 跳出循环
	{
		pos += re.matchedLength();					//匹配后，取出匹配长度
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
		results.append(res);						//取出匹配的值，存入results
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
		return QString::number(b, 'g', 12) + "T";		//16位有极小误差
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
QString TTool::_get_re_int_SI_() {			//int最大值是 -2147483648 ~ +2147483647 (4字节) ， T和p单位太大，而且mun会把int变成0。
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
QRegExpValidator* TTool::_getValidator_re_int_(QWidget* parent) {
	return new QRegExpValidator(QRegExp(_get_re_int_()), parent);
}
QRegExpValidator* TTool::_getValidator_re_int_SI_(QWidget* parent) {
	return new QRegExpValidator(QRegExp(_get_re_int_SI_()), parent);
}
QRegExpValidator* TTool::_getValidator_re_double_(QWidget* parent){
	return new QRegExpValidator(QRegExp(_get_re_double_()), parent);
}
QRegExpValidator* TTool::_getValidator_re_double_e_(QWidget* parent){
	return new QRegExpValidator(QRegExp(_get_re_double_e_()), parent);
}
QRegExpValidator* TTool::_getValidator_re_double_SI_(QWidget* parent){
	return new QRegExpValidator(QRegExp(_get_re_double_SI_()), parent);
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
QString TTool::_QString_suffix_addOne_(QString modify_str) {
	if (modify_str == ""){ return ""; }

	QRegExp re("(\\d+)$");	//（末尾，不含负数）
	re.setMinimal(false);
	int index = re.indexIn(modify_str);
	if (index == -1){
		return modify_str + "2";
	}

	int value = re.cap(0).toInt() + 1;
	modify_str = modify_str.mid(0, index);
	modify_str = modify_str + QString::number(value);
	return modify_str;
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


/*========================================================================*/
QList<bool>  TTool::_QList_QStringToBool_(QList<QString> list){
	QList<bool> result_list = QList<bool>();
	for (int i = 0; i < list.count(); i++){
		if (list.at(i).toLower() == "true"){
			result_list.append(true);
		}else{
			result_list.append(false);
		}
	}
	return result_list;
}
QList<int>  TTool::_QList_QStringToInt_(QList<QString> list){
	QList<int> result_list = QList<int>();
	for (int i = 0; i < list.count(); i++){
		result_list.append(list.at(i).toInt());
	}
	return result_list;
}
QList<double>  TTool::_QList_QStringToDouble_(QList<QString> list){
	QList<double> result_list = QList<double>();
	for (int i = 0; i < list.count(); i++){
		result_list.append(list.at(i).toDouble());
	}
	return result_list;
}
QList<QJsonObject>  TTool::_QList_QStringToQJsonObject_(QList<QString> list){
	QList<QJsonObject> result_list = QList<QJsonObject>();
	for (int i = 0; i < list.count(); i++){
		QJsonDocument jsonDocument = QJsonDocument::fromJson(list.at(i).toUtf8());
		QJsonObject obj = jsonDocument.object();
		result_list.append(obj);
	}
	return result_list;
}
QList<QString>  TTool::_QList_BoolToQString_(QList<bool> list){
	QList<QString> result_list = QList<QString>();
	for (int i = 0; i < list.count(); i++){
		if (list.at(i) == true){
			result_list.append("true");
		}else{
			result_list.append("false");
		}
	}
	return result_list;
}
QList<QString>  TTool::_QList_IntToQString_(QList<int> list){
	QList<QString> result_list = QList<QString>();
	for (int i = 0; i < list.count(); i++){
		result_list.append(QString::number( list.at(i) ));
	}
	return result_list;
}
QList<QString>  TTool::_QList_DoubleToQString_(QList<double> list){
	QList<QString> result_list = QList<QString>();
	for (int i = 0; i < list.count(); i++){
		result_list.append(QString::number( list.at(i) ));
	}
	return result_list;
}
QList<QString>  TTool::_QList_QJsonObjectToQString_(QList<QJsonObject> list){
	QList<QString> result_list = QList<QString>();
	for (int i = 0; i < list.count(); i++){
		result_list.append(QJsonDocument(list.at(i)).toJson(QJsonDocument::Compact));
	}
	return result_list;
}

QList<double>  TTool::_QList_IntToDouble_(QList<int> list){
	QList<double> result_list = QList<double>();
	for (int i = 0; i < list.count(); i++){
		result_list.append(list.at(i));
	}
	return result_list;
}
QList<int>  TTool::_QList_DoubleToInt_floor_(QList<double> list){
	QList<int> result_list = QList<int>();
	for (int i = 0; i < list.count(); i++){
		result_list.append(qFloor(list.at(i)));
	}
	return result_list;
}
QList<int>  TTool::_QList_DoubleToInt_ceil_(QList<double> list){
	QList<int> result_list = QList<int>();
	for (int i = 0; i < list.count(); i++){
		result_list.append(qCeil(list.at(i)));
	}
	return result_list;
}
QList<int>  TTool::_QList_DoubleToInt_round_(QList<double> list){
	QList<int> result_list = QList<int>();
	for (int i = 0; i < list.count(); i++){
		result_list.append(qRound(list.at(i)));
	}
	return result_list;
}



/*========================================================================*/
void TTool::_QJsonObject_put_(QJsonObject *a, QJsonObject b) {
	for (int i = 0; i < b.keys().count(); i++){
		QString key = b.keys().at(i);
		a->insert(key, b.value(key));
	}
}


QJsonArray TTool::_QJsonArray_BoolToA_(QList<bool> list){
	QJsonArray result = QJsonArray();
	for (int i = 0; i < list.count(); i++){
		result.append(list.at(i));
	}
	return result;
}
QJsonArray TTool::_QJsonArray_IntToA_(QList<int> list){
	QJsonArray result = QJsonArray();
	for (int i = 0; i < list.count(); i++){
		result.append(list.at(i));
	}
	return result;
}
QJsonArray TTool::_QJsonArray_DoubleToA_(QList<double> list){
	QJsonArray result = QJsonArray();
	for (int i = 0; i < list.count(); i++){
		result.append(list.at(i));
	}
	return result;
}
QJsonArray TTool::_QJsonArray_QStringToA_(QList<QString> list){
	QJsonArray result = QJsonArray();
	for (int i = 0; i < list.count(); i++){
		result.append(list.at(i));
	}
	return result;
}
QJsonArray TTool::_QJsonArray_QJsonObjectToA_(QList<QJsonObject> list){
	QJsonArray result = QJsonArray();
	for (int i = 0; i < list.count(); i++){
		result.append(list.at(i));
	}
	return result;
}


QList<bool> TTool::_QJsonArray_AToBool_(QJsonArray arr){
	QList<bool> result_list = QList<bool>();
	for (int i = 0; i < arr.count(); i++){
		result_list.append(arr.at(i).toBool());
	}
	return result_list;
}
QList<int> TTool::_QJsonArray_AToInt_(QJsonArray arr){
	QList<int> result_list = QList<int>();
	for (int i = 0; i < arr.count(); i++){
		result_list.append(arr.at(i).toInt());
	}
	return result_list;
}
QList<double> TTool::_QJsonArray_AToDouble_(QJsonArray arr){
	QList<double> result_list = QList<double>();
	for (int i = 0; i < arr.count(); i++){
		result_list.append(arr.at(i).toDouble());
	}
	return result_list;
}
QList<QString> TTool::_QJsonArray_AToQString_(QJsonArray arr){
	QList<QString> result_list = QList<QString>();
	for (int i = 0; i < arr.count(); i++){
		result_list.append(arr.at(i).toString());
	}
	return result_list;
}
QList<QJsonObject> TTool::_QJsonArray_AToQJsonObject_(QJsonArray arr){
	QList<QJsonObject> result_list = QList<QJsonObject>();
	for (int i = 0; i < arr.count(); i++){
		result_list.append(arr.at(i).toObject());
	}
	return result_list;
}


/*========================================================================*/
QList<bool>  TTool::_JSON_parse_To_QListBool_(QString context){
	QList<bool> result_list = QList<bool>();
	QJsonDocument jsonDocument = QJsonDocument::fromJson(context.toUtf8());
	QJsonArray arr = jsonDocument.array();
	for (int i = 0; i < arr.count(); i++){
		result_list.append(arr.at(i).toBool());
	}
	return result_list;
}
QList<int>  TTool::_JSON_parse_To_QListInt_(QString context){
	QList<int> result_list = QList<int>();
	QJsonDocument jsonDocument = QJsonDocument::fromJson(context.toUtf8());
	QJsonArray arr = jsonDocument.array();
	for (int i = 0; i < arr.count(); i++){
		result_list.append(arr.at(i).toInt());
	}
	return result_list;
}
QList<double>  TTool::_JSON_parse_To_QListDouble_(QString context){
	QList<double> result_list = QList<double>();
	QJsonDocument jsonDocument = QJsonDocument::fromJson(context.toUtf8());
	QJsonArray arr = jsonDocument.array();
	for (int i = 0; i < arr.count(); i++){
		result_list.append(arr.at(i).toDouble());
	}
	return result_list;
}
QList<QString>  TTool::_JSON_parse_To_QListQString_(QString context){
	QList<QString> result_list = QList<QString>();
	QJsonDocument jsonDocument = QJsonDocument::fromJson(context.toUtf8());
	QJsonArray arr = jsonDocument.array();
	for (int i = 0; i < arr.count(); i++){
		result_list.append(arr.at(i).toString());
	}
	return result_list;
}
QJsonObject  TTool::_JSON_parse_To_Obj_(QString context){
	QJsonDocument jsonDocument = QJsonDocument::fromJson(context.toUtf8());
	return jsonDocument.object();
}
QList<QJsonObject>  TTool::_JSON_parse_To_QListObj_(QString context){
	QList<QJsonObject> result_list = QList<QJsonObject>();
	QJsonDocument jsonDocument = QJsonDocument::fromJson(context.toUtf8());
	QJsonArray arr = jsonDocument.array();
	for (int i = 0; i < arr.count(); i++){
		result_list.append(arr.at(i).toObject());
	}
	return result_list;
}


QString  TTool::_JSON_stringify_(QList<bool> data){
	QJsonArray arr = QJsonArray();
	for (int i = 0; i < data.count(); i++){
		arr.append(data.at(i));
	}
	return QJsonDocument(arr).toJson(QJsonDocument::Compact);
}
QString  TTool::_JSON_stringify_(QList<int> data){
	QJsonArray arr = QJsonArray();
	for (int i = 0; i < data.count(); i++){
		arr.append(data.at(i));
	}
	return QJsonDocument(arr).toJson(QJsonDocument::Compact);
}
QString  TTool::_JSON_stringify_(QList<double> data){
	QJsonArray arr = QJsonArray();
	for (int i = 0; i < data.count(); i++){
		arr.append(data.at(i));
	}
	return QJsonDocument(arr).toJson(QJsonDocument::Compact);
}
QString  TTool::_JSON_stringify_(QList<QString> data){
	QJsonArray arr = QJsonArray();
	for (int i = 0; i < data.count(); i++){
		arr.append(data.at(i));
	}
	return QJsonDocument(arr).toJson(QJsonDocument::Compact);
}
QString  TTool::_JSON_stringify_(QJsonObject data){
	return QJsonDocument(data).toJson(QJsonDocument::Compact);
}
QString  TTool::_JSON_stringify_(QList<QJsonObject> data){
	QJsonArray arr = QJsonArray();
	for (int i = 0; i < data.count(); i++){
		arr.append(data.at(i));
	}
	return QJsonDocument(arr).toJson(QJsonDocument::Compact);
}

void  TTool::_QStringList_clearEmptyRows_(QStringList* data_list){
	for (int i = data_list->count()-1; i >=0; i--) {
		QString str = data_list->at(i).trimmed();
		if (str.isEmpty()) {
			data_list->removeAt(i);
		}
	}
}
void  TTool::_QStringList_clearComment_(QStringList* data_list, QString prefix){
	for (int i = data_list->count() - 1; i >= 0; i--) {
		QString str = data_list->at(i).trimmed();
		if (str.mid(0, prefix.length()) == prefix) {
			data_list->removeAt(i);
		}
	}
}
QStringList  TTool::_QStringList_decimalChange_(QStringList data_list, int decimal){
	QStringList result_list = QStringList();
	for (int i = 0; i < data_list.count(); i++) {
		QString str = data_list.at(i);
		str = QString::number(str.toDouble(), 'f', decimal);
		result_list.push_back(str);
	}
	return result_list;
}
QStringList TTool::_QStringList_addPrefix_(QStringList data_list, QString prefix){
	QStringList result_list = QStringList();
	for (int i = 0; i < data_list.count(); i++) {
		QString str = prefix;
		str.append(data_list.at(i));
		result_list.push_back(str);
	}
	return result_list;
}
QStringList TTool::_QStringList_addSuffix_(QStringList data_list, QString suffix){
	QStringList result_list = QStringList();
	for (int i = 0; i < data_list.count(); i++) {
		QString str = data_list.at(i);
		str.append(suffix);
		result_list.push_back(str);
	}
	return result_list;
}

QLabel* TTool::_CreateQWidget_headerLabel_(QString text, int fontsize, int padding, QWidget* parent){
	QLabel* q_label = new QLabel(parent);
	q_label->setText(text);
	q_label->setStyleSheet("padding:" + QString::number(padding) + "px;font-size:" + QString::number(fontsize) + "px;font-weight:700;");
	return q_label;
}
QWidget* TTool::_CreateQWidget_containsTarget_(QWidget* target, int padding){
	QWidget* q_w = new QWidget();
	QVBoxLayout* q_layout = new QVBoxLayout(q_w);
	q_w->setLayout(q_layout);
	q_layout->setMargin(padding);
	q_layout->addWidget(target);
	return q_w;
}
QStyledItemDelegate* TTool::_ChangeCombox_itemHeight_(QComboBox* target, int item_height){
	QStyledItemDelegate* delegate = new QStyledItemDelegate(target);
	target->setItemDelegate(delegate);
	target->setStyleSheet("QComboBox QAbstractItemView::item {min-height: " + QString::number(item_height) + "px;}");
	return delegate;
}