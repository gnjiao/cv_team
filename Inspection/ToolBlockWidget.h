#pragma once
#include <boost\archive\text_iarchive.hpp>
#include <boost\archive\text_oarchive.hpp>
#include <boost\serialization\base_object.hpp>
#include <QMenu>
#include <QAction>
#include <QDialog>
#include <QContextMenuEvent>
#include "DragTreeWidget.h"
#include "DataModel\BaseWidget.h"
#include "DataModel\BaseNode.h"
#include "Location\FindLineWidget.h"
#include "OperatorManager.h"
#include "ToolBlockManager.h"

class ToolBlockWidget : public DragTreeWidget
{
	Q_OBJECT
public:
	ToolBlockWidget(QWidget *parent = Q_NULLPTR);
	~ToolBlockWidget();
	void setInspection(Inspection* inspection);
	void updateTree();
	vector<QDialog*> OperatorWidget;
protected:
	void doSelected(int index);
	void doDraged(int index);
	void doInserted(int index, QDropEvent* event, QTreeWidgetItem* item);
	void contextMenuEvent(QContextMenuEvent* event);
	void dropEvent(QDropEvent* event);
signals:
	void signalToolBlockChanged(ToolBlock* toolBlock);
private slots:
	void on_action_Delete_triggered();
	void on_action_Edit_triggered();
private:
	QDialog * creatOperatorWidget(BaseNode*);
	void swapItem(int index, QDropEvent * event, QTreeWidgetItem * item, bool addOrInsert);
	void setTreeItemIcon(QTreeWidgetItem* item, string text);
	void ReserveSpaceToAddItem();
	void clearOperatorWidgets();
	BaseNode* m_selectedNode = nullptr;
	QMenu* m_popMenu;
	QAction* Delete_ac;
	QAction* Edit_ac;
};