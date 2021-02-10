#include "stdafx.h"
#include "s_ChineseManager.h"

/*
-----==========================================================-----
		类：		中文管理器.cpp
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		提供中文转换、排序等功能。

		目标：		-> 判断中文字符
					-> 中文转拼音
					-> 获取拼音首字母
					-> 数字转中文
					
		使用方法：
				>打开：
			
-----==========================================================-----
*/

S_ChineseManager::S_ChineseManager(){
	this->m_textCodeGBK = QTextCodec::codecForName("GBK");		//初始化qt提供的gbk的解码器
}
S_ChineseManager::~S_ChineseManager() {
}

/* --------------------------------------------------------------
----------ChineseManager 单例
*/
S_ChineseManager* S_ChineseManager::cur_manager = NULL;
S_ChineseManager* S_ChineseManager::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_ChineseManager();
	}
	return cur_manager;
}


/*-------------------------------------------------
		基本 - 判断中文字符
*/
bool S_ChineseManager::hasAnyChineseCharacter(QString test_str) {
	return test_str.contains(QRegExp("[\\x4e00-\\x9fa5]+"));
}


/*-------------------------------------------------
		中文拼音 - 获取首字母
*/
QString S_ChineseManager::getChineseFirstSpell(QString chinese_string){
	if (chinese_string == ""){ return ""; }

	QByteArray buf = this->m_textCodeGBK->fromUnicode(chinese_string);
	int size = buf.size();
	quint16 *array = new quint16[size + 1];
	QString alphbats;

	for (int i = 0, j = 0; i < buf.size(); i++, j++){
		if ((quint8)buf[i] < 0x80){					//小于0x80的都是符号，字母，数字等
			alphbats.append(QChar(buf[i]));	
			continue; 
		}
		array[j] = (((quint8)buf[i]) << 8) + (quint8)buf[i + 1];	//计算gbk编码
		i++;
		alphbats.append(convert(array[j]));							//相当于查表，用gbk编码得到首拼音字母
	}
	delete[] array;
	return alphbats;
}

/*-------------------------------------------------
		中文拼音 - 单个字符判断
*/
bool S_ChineseManager::codeIn(wchar_t start, wchar_t end, wchar_t code){
	if (code >= start && code <= end){ return true;}
	return false;
}
char S_ChineseManager::convert(int n){
	if (codeIn(0xB0A1, 0xB0C4, n)) return 'a';
	if (codeIn(0XB0C5, 0XB2C0, n)) return 'b';
	if (codeIn(0xB2C1, 0xB4ED, n)) return 'c';
	if (codeIn(0xB4EE, 0xB6E9, n)) return 'd';
	if (codeIn(0xB6EA, 0xB7A1, n)) return 'e';
	if (codeIn(0xB7A2, 0xB8c0, n)) return 'f';
	if (codeIn(0xB8C1, 0xB9FD, n)) return 'g';
	if (codeIn(0xB9FE, 0xBBF6, n)) return 'h';
	if (codeIn(0xBBF7, 0xBFA5, n)) return 'j';
	if (codeIn(0xBFA6, 0xC0AB, n)) return 'k';
	if (codeIn(0xC0AC, 0xC2E7, n)) return 'l';
	if (codeIn(0xC2E8, 0xC4C2, n)) return 'm';
	if (codeIn(0xC4C3, 0xC5B5, n)) return 'n';
	if (codeIn(0xC5B6, 0xC5BD, n)) return 'o';
	if (codeIn(0xC5BE, 0xC6D9, n)) return 'p';
	if (codeIn(0xC6DA, 0xC8BA, n)) return 'q';
	if (codeIn(0xC8BB, 0xC8F5, n)) return 'r';
	if (codeIn(0xC8F6, 0xCBF0, n)) return 's';
	if (codeIn(0xCBFA, 0xCDD9, n)) return 't';
	if (codeIn(0xCDDA, 0xCEF3, n)) return 'w';
	if (codeIn(0xCEF4, 0xD188, n)) return 'x';
	if (codeIn(0xD1B9, 0xD4D0, n)) return 'y';
	if (codeIn(0xD4D1, 0xD7F9, n)) return 'z';
	return '@';		//@表示生僻字
}

/*-------------------------------------------------
		中文拼音 - 获取中文的拼音
*/
QString S_ChineseManager::getChinesePinYin(QString chinese){

	static int pyvalue[] = { -20319, -20317, -20304, -20295, -20292, -20283, -20265, -20257, -20242, -20230, -20051, -20036, -20032, -20026,
		-20002, -19990, -19986, -19982, -19976, -19805, -19784, -19775, -19774, -19763, -19756, -19751, -19746, -19741, -19739, -19728,
		-19725, -19715, -19540, -19531, -19525, -19515, -19500, -19484, -19479, -19467, -19289, -19288, -19281, -19275, -19270, -19263,
		-19261, -19249, -19243, -19242, -19238, -19235, -19227, -19224, -19218, -19212, -19038, -19023, -19018, -19006, -19003, -18996,
		-18977, -18961, -18952, -18783, -18774, -18773, -18763, -18756, -18741, -18735, -18731, -18722, -18710, -18697, -18696, -18526,
		-18518, -18501, -18490, -18478, -18463, -18448, -18447, -18446, -18239, -18237, -18231, -18220, -18211, -18201, -18184, -18183,
		-18181, -18012, -17997, -17988, -17970, -17964, -17961, -17950, -17947, -17931, -17928, -17922, -17759, -17752, -17733, -17730,
		-17721, -17703, -17701, -17697, -17692, -17683, -17676, -17496, -17487, -17482, -17468, -17454, -17433, -17427, -17417, -17202,
		-17185, -16983, -16970, -16942, -16915, -16733, -16708, -16706, -16689, -16664, -16657, -16647, -16474, -16470, -16465, -16459,
		-16452, -16448, -16433, -16429, -16427, -16423, -16419, -16412, -16407, -16403, -16401, -16393, -16220, -16216, -16212, -16205,
		-16202, -16187, -16180, -16171, -16169, -16158, -16155, -15959, -15958, -15944, -15933, -15920, -15915, -15903, -15889, -15878,
		-15707, -15701, -15681, -15667, -15661, -15659, -15652, -15640, -15631, -15625, -15454, -15448, -15436, -15435, -15419, -15416,
		-15408, -15394, -15385, -15377, -15375, -15369, -15363, -15362, -15183, -15180, -15165, -15158, -15153, -15150, -15149, -15144,
		-15143, -15141, -15140, -15139, -15128, -15121, -15119, -15117, -15110, -15109, -14941, -14937, -14933, -14930, -14929, -14928,
		-14926, -14922, -14921, -14914, -14908, -14902, -14894, -14889, -14882, -14873, -14871, -14857, -14678, -14674, -14670, -14668,
		-14663, -14654, -14645, -14630, -14594, -14429, -14407, -14399, -14384, -14379, -14368, -14355, -14353, -14345, -14170, -14159,
		-14151, -14149, -14145, -14140, -14137, -14135, -14125, -14123, -14122, -14112, -14109, -14099, -14097, -14094, -14092, -14090,
		-14087, -14083, -13917, -13914, -13910, -13907, -13906, -13905, -13896, -13894, -13878, -13870, -13859, -13847, -13831, -13658,
		-13611, -13601, -13406, -13404, -13400, -13398, -13395, -13391, -13387, -13383, -13367, -13359, -13356, -13343, -13340, -13329,
		-13326, -13318, -13147, -13138, -13120, -13107, -13096, -13095, -13091, -13076, -13068, -13063, -13060, -12888, -12875, -12871,
		-12860, -12858, -12852, -12849, -12838, -12831, -12829, -12812, -12802, -12607, -12597, -12594, -12585, -12556, -12359, -12346,
		-12320, -12300, -12120, -12099, -12089, -12074, -12067, -12058, -12039, -11867, -11861, -11847, -11831, -11798, -11781, -11604,
		-11589, -11536, -11358, -11340, -11339, -11324, -11303, -11097, -11077, -11067, -11055, -11052, -11045, -11041, -11038, -11024,
		-11020, -11019, -11018, -11014, -10838, -10832, -10815, -10800, -10790, -10780, -10764, -10587, -10544, -10533, -10519, -10331,
		-10329, -10328, -10322, -10315, -10309, -10307, -10296, -10281, -10274, -10270, -10262, -10260, -10256, -10254, -10246 };
	static char pystr[398][7] = { "a", "ai", "an", "ang", "ao", "ba", "bai", "ban", "bang", "bao", "bei", "ben", "beng", "bi", "bian", "biao",
		"bie", "bin", "bing", "bo", "bu", "ca", "cai", "can", "cang", "cao", "ce", "ceng", "cha", "chai", "chan", "chang", "chao", "che", "chen",
		"cheng", "chi", "chong", "chou", "chu", "chuai", "chuan", "chuang", "chui", "chun", "chuo", "ci", "cong", "cou", "cu", "cuan", "cui",
		"cun", "cuo", "da", "dai", "dan", "dang", "dao", "de", "deng", "di", "dian", "diao", "die", "ding", "diu", "dong", "dou", "du", "duan",
		"dui", "dun", "duo", "e", "en", "er", "fa", "fan", "fang", "fei", "fen", "feng", "fo", "fou", "fu", "ga", "gai", "gan", "gang", "gao",
		"ge", "gei", "gen", "geng", "gong", "gou", "gu", "gua", "guai", "guan", "guang", "gui", "gun", "guo", "ha", "hai", "han", "hang",
		"hao", "he", "hei", "hen", "heng", "hong", "hou", "hu", "hua", "huai", "huan", "huang", "hui", "hun", "huo", "ji", "jia", "jian",
		"jiang", "jiao", "jie", "jin", "jing", "jiong", "jiu", "ju", "juan", "jue", "jun", "ka", "kai", "kan", "kang", "kao", "ke", "ken",
		"keng", "kong", "kou", "ku", "kua", "kuai", "kuan", "kuang", "kui", "kun", "kuo", "la", "lai", "lan", "lang", "lao", "le", "lei",
		"leng", "li", "lia", "lian", "liang", "liao", "lie", "lin", "ling", "liu", "long", "lou", "lu", "lv", "luan", "lue", "lun", "luo",
		"ma", "mai", "man", "mang", "mao", "me", "mei", "men", "meng", "mi", "mian", "miao", "mie", "min", "ming", "miu", "mo", "mou", "mu",
		"na", "nai", "nan", "nang", "nao", "ne", "nei", "nen", "neng", "ni", "nian", "niang", "niao", "nie", "nin", "ning", "niu", "nong",
		"nu", "nv", "nuan", "nue", "nuo", "o", "ou", "pa", "pai", "pan", "pang", "pao", "pei", "pen", "peng", "pi", "pian", "piao", "pie",
		"pin", "ping", "po", "pu", "qi", "qia", "qian", "qiang", "qiao", "qie", "qin", "qing", "qiong", "qiu", "qu", "quan", "que", "qun",
		"ran", "rang", "rao", "re", "ren", "reng", "ri", "rong", "rou", "ru", "ruan", "rui", "run", "ruo", "sa", "sai", "san", "sang",
		"sao", "se", "sen", "seng", "sha", "shai", "shan", "shang", "shao", "she", "shen", "sheng", "shi", "shou", "shu", "shua",
		"shuai", "shuan", "shuang", "shui", "shun", "shuo", "si", "song", "sou", "su", "suan", "sui", "sun", "suo", "ta", "tai",
		"tan", "tang", "tao", "te", "teng", "ti", "tian", "tiao", "tie", "ting", "tong", "tou", "tu", "tuan", "tui", "tun", "tuo",
		"wa", "wai", "wan", "wang", "wei", "wen", "weng", "wo", "wu", "xi", "xia", "xian", "xiang", "xiao", "xie", "xin", "xing",
		"xiong", "xiu", "xu", "xuan", "xue", "xun", "ya", "yan", "yang", "yao", "ye", "yi", "yin", "ying", "yo", "yong", "you",
		"yu", "yuan", "yue", "yun", "za", "zai", "zan", "zang", "zao", "ze", "zei", "zen", "zeng", "zha", "zhai", "zhan", "zhang",
		"zhao", "zhe", "zhen", "zheng", "zhi", "zhong", "zhou", "zhu", "zhua", "zhuai", "zhuan", "zhuang", "zhui", "zhun", "zhuo",
		"zi", "zong", "zou", "zu", "zuan", "zui", "zun", "zuo", "@" };		//@表示生僻字

	int chrasc = 0;

	char* pcReturnString = NULL;

	QByteArray ba = chinese.toLocal8Bit();
	const char * acBa = ba.constData();
	int length = strlen(acBa);
	char* nowchar = new char[length + 1];
	memset(nowchar, 0, length + 1);
	memcpy(nowchar, acBa, length);

	char *returnstr = new char[6 * length + 1];
	memset(returnstr, 0, 6 * length + 1);

	int offset = 0;
	for (int j = 0; j < length;) // 循环处理字节数组
	{
		if (nowchar[j] >= 0 && nowchar[j] < 128) // 非汉字处理
		{
			returnstr[offset] = nowchar[j];
			offset++;
			j++;
			continue;
		}

		// 汉字处理
		chrasc = nowchar[j] * 256 + nowchar[j + 1] + 256;

		if (chrasc > 0 && chrasc < 160)
		{
			returnstr[offset] = nowchar[j];
			offset++;
			j++;
		}
		else
		{
			for (int i = (sizeof(pyvalue) / sizeof(pyvalue[0]) - 1); i >= 0; i--)
			{
				if (pyvalue[i] <= chrasc)
				{
					strcpy(returnstr + offset, pystr[i]);
					offset += strlen(pystr[i]);
					if (j + 2 < length)
					{
						strcpy(returnstr + offset, " ");
						offset++;
					}

					break;
				}
			}
			j += 2;
		}
	}
	QString returnString = returnstr;
	delete[]returnstr;
	delete[]nowchar;
	return returnString;

}



/*-------------------------------------------------
		数字转中文 - 转小写
*/
QString S_ChineseManager::toChineseNumLower(int num) {
	return toChineseNum(num, QStringList() << "" << "十" << "百" << "千" << "万" << "十" << "百" << "千" << "亿" << "十" << "百" << "千",
		QStringList() << "零" << "一" << "二" << "三" << "四" << "五" << "六" << "七" << "八" << "九");
}
QString S_ChineseManager::toChineseNumLower(long num) {
	return toChineseNum(num, QStringList() << "" << "十" << "百" << "千" << "万" << "十" << "百" << "千" << "亿" << "十" << "百" << "千",
		QStringList() << "零" << "一" << "二" << "三" << "四" << "五" << "六" << "七" << "八" << "九");
}
/*-------------------------------------------------
		数字转中文 - 转大写
*/
QString S_ChineseManager::toChineseNumUpper(int num) {
	return toChineseNum(num, QStringList() << "" << "拾" << "佰" << "仟" << "万" << "拾" << "佰" << "仟" << "亿" << "拾" << "佰" << "仟",
		QStringList() << "零" << "壹" << "贰" << "叁" << "肆" << "伍" << "陆" << "柒" << "捌" << "玖");
}
QString S_ChineseManager::toChineseNumUpper(long num) {
	return toChineseNum(num, QStringList() << "" << "拾" << "佰" << "仟" << "万" << "拾" << "佰" << "仟" << "亿" << "拾" << "佰" << "仟",
		QStringList() << "零" << "壹" << "贰" << "叁" << "肆" << "伍" << "陆" << "柒" << "捌" << "玖");
}
/*-------------------------------------------------
		数字转中文 - 私有方法
*/
QString S_ChineseManager::toChineseNum(double value, QStringList numerical_unit, QStringList numerical_value){

	// > 判断正负号
	QString numberSign;		//存储符号
	if (value < 0){
		numberSign = "（负数）";
	}

	// > 将数据的绝对值 转换成字符串，如-58 转成 “58.00”
	QString number = QString::number(qAbs(value), 'f', 2);//qAbs绝对值 ，保留两位小数
	QString Left_of_Point;						//小数点左侧，整数部分
	QString Right_of_Point;						//小数点右侧，小数部分(保留两位)  xxxx.yy
	int length = number.length() - 3;			//整数部分的长度，（精确度为2，去除小数和小数点）
	if (length > 12){
		return "（输入的数值已超过中文表示范围）";
	}

	// > 数数整数低位多少个连零
	int counter_0 = 0;
	for (int i = length - 1; i >= 0; i--){
		if ((number[i].toLatin1() - '0') == 0){ //从个位开始，向高位走
			counter_0++;
		}
		else{
			break;
		}
	}
	if (length == 1 && counter_0 == 1){ //0.x
		counter_0 = 0; //不进行过滤
	}

	for (int i = 0, flag = 1; i < length - counter_0; i++){
		// 5     8       1    2   3
		//伍 拾 捌    壹佰 贰拾  叁
		if ((number[i].toLatin1() - '0') == 0){
			if ((flag != 0 && (length - 1 - i) % 4 != 0) || length == 1){	//flag！=0  表示前一个数值 不为0
				Left_of_Point += numerical_value.at(0);						//后面不用添加 单位
			}
			if ((length - 1 - i) % 4 == 0){									//如果0处于分段处，后面需添加单位
				Left_of_Point += numerical_unit[length - 1 - i];			//添加数值单位
			}
			flag = 0; //标记

		}
		else{
			flag = 1;
			Left_of_Point += numerical_value[number[i].toLatin1() - '0'];	//'5'-'0'==5
			Left_of_Point += numerical_unit[length - 1 - i];				//添加数值单位
		}
	}

	int totalLength = number.length();
	if (number[totalLength - 2] == '0' && number[totalLength - 1] == '0'){
		QString Bigcn = numberSign + Left_of_Point;
		return Bigcn;
	}
	else if (number[totalLength - 2] != '0'&&number[totalLength - 1] == '0'){
		QString Bigcn = numberSign + Left_of_Point + "点" + numerical_value[number[totalLength - 2].toLatin1() - '0'];
		return Bigcn;
	}
	else{
		QString Bigcn = numberSign + Left_of_Point + "点" + numerical_value[number[totalLength - 2].toLatin1() - '0'] + numerical_value[number[totalLength - 1].toLatin1() - '0'];
		return Bigcn;
	}

	return "（异常情况）";
}

