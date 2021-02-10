#ifndef W_FCTConfig_H
#define W_FCTConfig_H

#include "ui_W_FCTConfig.h"
#include "c_FCTConfig.h"

/*
-----==========================================================-----
		类：		树设置 窗口.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		编辑 树设置 窗口的一些配置。
					（详细见cpp）
-----==========================================================-----
*/
class W_FCTConfig : public QDialog
{
	Q_OBJECT

	public:
		W_FCTConfig(QWidget *parent = 0 );
		~W_FCTConfig();
		
	//-----------------------------------
	//----控件
	public:
										//回车过滤
		void keyPressEvent(QKeyEvent *event);
		
	public slots:
										//零填充勾选变化
		void zeroFillChanged(bool enable);				
										//自定义分支模式可用
		void setModeType(QString cur_type, QStringList type_list);

	//-----------------------------------
	//----窗口
	protected:
		C_FCTConfig local_data;
	public:
										//窗口 - 设置数据（修改）
		void setDataInModifyMode(C_FCTConfig data);
										//窗口 - 取出数据
		C_FCTConfig getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();

	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_FCTConfig ui;
	//-----------------------------------
};

#endif // W_FCTConfig_H
