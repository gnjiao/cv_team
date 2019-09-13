#pragma once
#include <QTreeWidgetItem>
#include "DragTreeWidget.h"

class ToolBoxWidget : public DragTreeWidget
{
	Q_OBJECT
public:

	ToolBoxWidget(QWidget *parent = Q_NULLPTR);

	~ToolBoxWidget();

private:

	QTreeWidgetItem *m_root_pretreatment;

	QTreeWidgetItem *m_root_location;

	QTreeWidgetItem *m_root_measurement;

	QTreeWidgetItem *m_root_recognition;

};

