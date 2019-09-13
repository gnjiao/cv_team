#include "DragTreeWidget.h"


DragTreeWidget::DragTreeWidget(QWidget *parent)
	: QTreeWidget(parent)
{
}
DragTreeWidget::~DragTreeWidget()
{
}
void DragTreeWidget::addRootItem(QString itemName)
{
	this->addTopLevelItem(new QTreeWidgetItem(this, QStringList(QString(itemName))));
}
void DragTreeWidget::mouseMoveEvent(QMouseEvent * event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		if ((event->pos() - m_dragPosition).manhattanLength() > QApplication::startDragDistance())
		{
			if (m_selectedItem)
			{
				QDrag* drag = new QDrag(this);
				QMimeData* mimeData = new QMimeData();
				mimeData->setText(m_selectedItem->text(0));
				mimeData->setData(typeid(*this).name(), NULL);
				drag->setMimeData(mimeData);
				doDraged(indexFromItem(m_selectedItem).row());
				drag->exec(Qt::MoveAction);
			}
		}
	}
	QTreeWidget::mouseMoveEvent(event);
}
void DragTreeWidget::mousePressEvent(QMouseEvent * event)
{
	m_dragPosition = event->pos();
	m_selectedItem = itemAt(event->pos());
	if (m_selectedItem != NULL)
	{
		doSelected(indexFromItem(m_selectedItem).row());
	}
	QTreeWidget::mousePressEvent(event);
}
void DragTreeWidget::dragEnterEvent(QDragEnterEvent * event)
{
	event->setDropAction(Qt::MoveAction);
	event->accept();
}
void DragTreeWidget::dragLeaveEvent(QDragLeaveEvent * event)
{
	/*QTreeWidget::dragLeaveEvent(event);*/
}
void DragTreeWidget::dragMoveEvent(QDragMoveEvent * event)
{
	event->setDropAction(Qt::MoveAction);
	event->accept();
}