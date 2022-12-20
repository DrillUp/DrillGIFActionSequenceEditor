#include "stdafx.h"
#include "W_StateNodeRelationTable.h"

#include "Source/Utils/widgetFormSenior/NodeData/S_NodeFactoryContainer.h"
#include "Source/ActionSeqModule/reprintedCore/Drill_COAS_Init.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		状态节点关系表.cpp
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		根据输入的状态元和状态节点数据，生成一张关系表。
					【通过 状态节点_树图 的方式，即时构建，即时生成大表】

		使用方法：
				> 打开窗口
					W_StateNodeRelationTable d;
					d.setData_StateData(this->getConfigParam());
					d.setData_StateNodeData(this->getConfigParam());
					d.exec();
-----==========================================================-----
*/

W_StateNodeRelationTable::W_StateNodeRelationTable(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----最大化/最小化
	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowCloseButtonHint;
	flags |= Qt::WindowMaximizeButtonHint;
	flags |= Qt::WindowMinimizeButtonHint;
	this->setWindowFlags(flags);

	//-----------------------------------
	//----初始化参数
	this->m_availableStateDataList.clear();
	this->m_availableStateNodeDataList.clear();
	this->m_C_StateTreeNodeFactory = C_StateTreeNodeFactoryPtr::create();
	S_NodeFactoryContainer::getInstance()->addFactory(this->m_C_StateTreeNodeFactory);
	this->m_errorBlock = false;

	//-----------------------------------
	//----事件绑定
	
	//-----------------------------------
	//----初始化ui
	TTool::_chinese_(ui.buttonBox);
}

W_StateNodeRelationTable::~W_StateNodeRelationTable(){
	S_NodeFactoryContainer::getInstance()->removeFactory(this->m_C_StateTreeNodeFactory->getId());
	this->m_C_StateTreeNodeFactory.clear();
}


/*-------------------------------------------------
		控件 - 刷新表格
*/
void W_StateNodeRelationTable::refreshTable(){
	if (this->m_errorBlock == true){ return; }

	// > 构建未成功情况
	if (this->m_C_StateTreeNodeFactory->hasTreeRoot() == false){
		//...
		return;
	}

	// > 列
	int tree_deep = this->m_C_StateTreeNodeFactory->getTreeDeep();
	this->setTableColumnCount(tree_deep);

	// > 行
	QList<C_TreeNodePtr> leaf_list = this->m_C_StateTreeNodeFactory->getTreeLeafAll();
	ui.tableWidget->setRowCount(leaf_list.count());

	// > 动画序列
	QTableWidgetItem* first_item = new QTableWidgetItem("动画序列");
	ui.tableWidget->setItem(0, 0, first_item);
	ui.tableWidget->setSpan(0, 0, ui.tableWidget->rowCount(), 1);

	// > 第N层遍历（从层1开始，层0为树根，不含叶子）
	for (int i = 1; i < tree_deep; i++){
		QList<C_TreeNodePtr> leaf_list = this->m_C_StateTreeNodeFactory->getTreeNode_ByDeep(i);
		int cur_row = 0;
		for (int j = 0; j < leaf_list.count(); j++){
			C_StateTreeNodePtr stateNode = leaf_list.at(j).dynamicCast<C_StateTreeNode>();

			// > 叶子节点 暂时不画
			if (stateNode->isTreeLeaf()){
				continue;
			}

			// > 如果 行列 被其他格子占用，则往下继续找行，然后填入
			for (int k = 0; k < ui.tableWidget->rowCount(); k++){
				if (ui.tableWidget->rowSpan(cur_row, i) == 1 &&
					ui.tableWidget->columnSpan(cur_row, i) == 1){
					break;
				}else{
					cur_row += 1;
				}
			}

			// > 越界后不再绘制
			if (cur_row >= ui.tableWidget->rowCount()){ break; }


			// > 绘制控件块
			QWidget* w = this->createItemByNodeData(stateNode);
			ui.tableWidget->setCellWidget(cur_row, i, w);
			//qDebug() << "绘制顺序：" << stateNode->getName();

			// > 跨行分配
			int rowSpan = stateNode->getChildLeaf_Recursion().count();
			if (rowSpan == 0){ rowSpan = 1; }	//（叶子节点占1行）

			// > 跨列分配（如果往后没有树枝了，直接跨列）
			int colSpan = 1;
			if (stateNode->getChildBranch_Current().count() == 0){
				colSpan += tree_deep-2 - stateNode->getTree_CurLayer();
			}

			ui.tableWidget->setSpan(cur_row, i, rowSpan, colSpan);
			cur_row += 1;
		}
	}

	// > 叶子单独绘制
	QList<C_TreeNodePtr> leaf_all = this->m_C_StateTreeNodeFactory->getTreeLeafAll();
	for (int j = 0; j < leaf_all.count(); j++){
		C_StateTreeNodePtr stateNode = leaf_all.at(j).dynamicCast<C_StateTreeNode>();
		QWidget* w = this->createItemByNodeData(stateNode);
		ui.tableWidget->setCellWidget(j, tree_deep-1, w);
	}

	// > 根据内容自适应
	for (int i = 1; i < ui.tableWidget->columnCount(); i++){
		ui.tableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
	}
}
/*-------------------------------------------------
		控件 - 设置表格列
*/
void W_StateNodeRelationTable::setTableColumnCount(int count){
	if (count < 3){ return; }
	
	// > 列数
	ui.tableWidget->setColumnCount(count);

	// > 列名
	QStringList colName_list;
	colName_list.append("动画\n序列");
	for (int i = 0; i < count - 2; i++){
		colName_list.append("状态节点"+QString::number(i+1)+"层");
	}
	colName_list.append("状态元");
	ui.tableWidget->setHorizontalHeaderLabels(colName_list);

	// > 列宽情况
	ui.tableWidget->horizontalHeader()->resizeSection(0, 40);
}
/*-------------------------------------------------
		控件 - 设置状态节点
*/
QWidget* W_StateNodeRelationTable::createItemByNodeData(C_StateTreeNodePtr node){
	QWidget* w = new QWidget(ui.tableWidget);
	QHBoxLayout* layout = new QHBoxLayout(w);
	layout->setContentsMargins(10,2,10,2);
	layout->setSpacing(10);
	w->setLayout(layout);

	// > 单独的标签
	QLabel* label_name = new QLabel();
	QString name = node->getName();
	label_name->setText(name);
	layout->addWidget(label_name);
	layout->setStretch(0, 1);

	// > 标签后缀
	if (node->isObjData_StateNode()){
		QLabel* label_suffix = new QLabel();
		label_suffix->setText(node->getNodeDescription());
		label_suffix->setStyleSheet("color:#DD2222;");

		layout->addWidget(label_suffix);
		layout->setStretch(1, 0);
	}
	return w;
}

/*-------------------------------------------------
		数据 - 构建状态节点图
*/
void W_StateNodeRelationTable::rebuildNodeFactory(){
	if (this->m_errorBlock == true){ return; }

	// > 树根
	C_StateTreeNodePtr root = this->m_C_StateTreeNodeFactory->createStateTreeNode("动画序列");

	// > 第一层节点
	for (int i = 0; i < this->m_availableStateNodeDataList.count(); i++){
		QJsonObject data = this->m_availableStateNodeDataList.at(i);
		QString name = data["节点名称"].toString();
		C_StateTreeNodePtr next_node = this->m_C_StateTreeNodeFactory->createStateTreeNode(name);
		next_node->setObjData_StateNode(this->getStateNodeByName(name));
		next_node->connectNode(root);
		qDebug() << "树图-创建状态节点：" << name;

		// > 递归添加节点
		this->createNode_Recursion(next_node, 1);
	}

	// > 树根初始化
	if (this->m_C_StateTreeNodeFactory->hasIsolated() == true){
		QMessageBox::about(nullptr, "错误", "状态节点构建时出现孤岛型嵌套，请重新检查状态节点的嵌套情况。");
		this->m_errorBlock = true;
		return;
	}
	if (this->m_C_StateTreeNodeFactory->isTree() == false){
		QMessageBox::about(nullptr, "错误", "状态节点不构成树结构，存在死循环环路，请重新检查状态节点的嵌套情况。");
		this->m_errorBlock = true;
		return;
	}
	this->m_C_StateTreeNodeFactory->setTreeRoot(root);
}
/*-------------------------------------------------
		数据 - 递归添加节点
*/
void W_StateNodeRelationTable::createNode_Recursion(C_StateTreeNodePtr node, int layer_deep){
	// 由于状态元、状态节点存在重复的情况，因此需要递归添加重复的数据
	if (this->m_errorBlock == true){ return; }
	if (layer_deep > 20){
		QMessageBox::about(nullptr, "错误", "状态节点的嵌套中存在死循环，请重新检查状态节点的嵌套情况。");
		this->m_errorBlock = true;
		return;
	}
	
	// > 状态元数据情况（叶子节点，不操作）
	if (node->isObjData_State()){
		return;
	}
	
	// > 状态节点数据情况（继续向下递归）
	if (node->isObjData_StateNode()){
		
		// > 子节点为状态元
		if (node->isPlayType_State()){
			QStringList name_list = node->getChildList_State();
			for (int i = 0; i < name_list.count(); i++){
				QString name = name_list.at(i);
				qDebug() << "树图-创建状态元：" << name;
				C_StateTreeNodePtr next_node = this->m_C_StateTreeNodeFactory->createStateTreeNode(name);
				next_node->setObjData_State(this->getStateByName(name));
				next_node->connectNode(node);
				this->createNode_Recursion(next_node, layer_deep + 1);
			}
		}

		// > 子节点为状态节点
		if (node->isPlayType_StateNode()){
			QStringList name_list = node->getChildList_StateNode();
			for (int i = 0; i < name_list.count(); i++){
				QString name = name_list.at(i);
				qDebug() << "树图-创建状态节点：" << name;
				C_StateTreeNodePtr next_node = this->m_C_StateTreeNodeFactory->createStateTreeNode(name);
				next_node->setObjData_StateNode(this->getStateNodeByName(name));
				next_node->connectNode(node);
				this->createNode_Recursion(next_node, layer_deep+1);
			}
		}

		return;
	}
}
/*-------------------------------------------------
		数据 - 获取状态元
*/
QJsonObject W_StateNodeRelationTable::getStateByName(QString name){
	for (int i = 0; i < this->m_availableStateDataList.count(); i++){
		QJsonObject data = this->m_availableStateDataList.at(i);
		QString data_name = data["状态元名称"].toString();
		if (data_name == name){
			return data;
		}
	}
	return QJsonObject();
}
QList<QJsonObject> W_StateNodeRelationTable::getStateByNameList(QStringList name_list){
	QList<QJsonObject> result_list;
	for (int i = 0; i < this->m_availableStateDataList.count(); i++){
		QJsonObject data = this->m_availableStateDataList.at(i);
		QString name = data["状态元名称"].toString();
		if (name_list.contains(name)){
			result_list.append(data);
		}
	}
	return result_list;
}
/*-------------------------------------------------
		数据 - 获取状态节点
*/
QJsonObject W_StateNodeRelationTable::getStateNodeByName(QString name){
	for (int i = 0; i < this->m_availableStateNodeDataList.count(); i++){
		QJsonObject data = this->m_availableStateNodeDataList.at(i);
		QString data_name = data["节点名称"].toString();
		if (data_name == name){
			return data;
		}
	}
	return QJsonObject();
}
QList<QJsonObject> W_StateNodeRelationTable::getStateNodeByNameList(QStringList name_list){
	QList<QJsonObject> result_list;
	for (int i = 0; i < this->m_availableStateNodeDataList.count(); i++){
		QJsonObject data = this->m_availableStateNodeDataList.at(i);
		QString name = data["节点名称"].toString();
		if (name_list.contains(name)){
			result_list.append(data);
		}
	}
	return result_list;
}


/*-------------------------------------------------
		窗口 - 设置数据（修改）
*/
void W_StateNodeRelationTable::setData_StateData(QList<QJsonObject> data_list) {
	
	// > 排除空对象
	this->m_availableStateDataList.clear();
	for (int i = 0; i < data_list.count(); i++){
		QJsonObject data = data_list.at(i);
		if (data["状态元名称"].toString() == ""){ continue; }
		this->m_availableStateDataList.append(data);
	}
	this->putDataToUi();
}
void W_StateNodeRelationTable::setData_StateNodeData(QList<QJsonObject> data_list) {

	// > 排除空对象
	this->m_availableStateNodeDataList.clear();
	for (int i = 0; i < data_list.count(); i++){
		QJsonObject data = data_list.at(i);
		bool isEmpty = Drill_COAS_Init::getInstance()->drill_COAS_checkStateNodeIsEmpty_Chinese(&data);	//空数据检查
		if (isEmpty){ continue; }
		this->m_availableStateNodeDataList.append(data);
	}
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_StateNodeRelationTable::putDataToUi() {
	if (this->m_availableStateDataList.count() == 0){ return; }
	if (this->m_availableStateNodeDataList.count() == 0){ return; }

	// > 构建 状态节点图
	this->rebuildNodeFactory();

	// > 刷新表格
	this->refreshTable();
};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_StateNodeRelationTable::putUiToData() {
	//（无操作）
};