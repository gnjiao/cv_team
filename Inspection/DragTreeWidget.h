#pragma once
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDrag>
#include <QMimeData>
#include <QEvent>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QApplication>
#include <QPoint>
#include <QStringList>
#include <QList>
#include <QByteArray>

class DragTreeWidget : public QTreeWidget
{
	Q_OBJECT
public:
	DragTreeWidget(QWidget *parent);
	~DragTreeWidget();
	void addRootItem(QString itemName);
protected:
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void dragEnterEvent(QDragEnterEvent* event);
	void dragLeaveEvent(QDragLeaveEvent* event);
	void dragMoveEvent(QDragMoveEvent* event);

	virtual void doSelected(int index) {}
	virtual void doDraged(int index) {}
	virtual void doAdded(QDropEvent * event, QTreeWidgetItem* item) {}
	virtual void doInserted(int index, QDropEvent * event, QTreeWidgetItem* item) {}
	QTreeWidgetItem* m_selectedItem = NULL;
private:
	QPoint m_dragPosition;
};