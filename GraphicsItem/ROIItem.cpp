#include "ROIItem.h"

ROIItem::ROIItem(int x, int y, int width, int height) : m_size(0,0,width, height), m_bIsResizing(false)
{
	setAcceptHoverEvents(true);
	this->setPos(x, y);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);

}

ROIItem::~ROIItem()
{
}

void ROIItem::setColor(QColor color)
{
	this->m_color = color;
	prepareGeometryChange();
	update();
}

geo::Rect2D ROIItem::getRect()
{
	QPointF PointTL = mapToScene(m_size.topLeft());

	QPointF PointBR = mapToScene(m_size.bottomRight());

	QRectF RoiInScene = QRectF(PointTL, PointBR);

	return geo::Rect2D(geo::Point2D(RoiInScene.left(), RoiInScene.top()), geo::Point2D(RoiInScene.right(), RoiInScene.bottom()));
}

QRectF ROIItem::boundingRect() const
{
	return  QRectF(0, 0, m_size.width() + 3, m_size.height() + 3);
}

void ROIItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{

	if (option->state & QStyle::State_Selected)
	{
		// ÖØµã

		if (option->state & QStyle::State_HasFocus)
		{
			painter->setRenderHint(QPainter::Antialiasing, true);

			painter->setPen(Qt::yellow);
		}

		else
		{
			painter->setPen(m_color);
		}

		// draw frame
		painter->drawRect(QRectF(0, 0, m_size.width(), m_size.height()));

		// resize line
		qreal w = m_size.width();
		qreal h = m_size.height();
		painter->setPen(Qt::red);
		for (int i = 0; i < 3; ++i)
			painter->drawLine(w - roi_cResizePos[i], h, w, h - roi_cResizePos[i]);

		painter->setRenderHint(QPainter::Antialiasing, false);
	}
	else
	{
		painter->setPen(m_color);
		painter->drawRect(QRectF(0, 0, m_size.width(), m_size.height()));

	}




}

void ROIItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
	if (m_bIsResizing || (IsInResizeArea(event->pos()) && isSelected()))
		setCursor(Qt::SizeFDiagCursor);
	else
		setCursor(QPixmap(":/CV_TEAM/icons/roi.png"));

	QGraphicsItem::hoverMoveEvent(event);
}

void ROIItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	if (m_bIsResizing)
	{
		setCursor(Qt::SizeFDiagCursor);
		qreal w = event->pos().x();
		qreal h = event->pos().y();
		if (w > 0)
			m_size.setWidth(w);
		if (h > 0)
			m_size.setHeight(h);

		prepareGeometryChange();
	}
	else
	{
		QGraphicsItem::mouseMoveEvent(event);
	}
}

void ROIItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	
	static qreal z = 0.0;
	setZValue(z += 1.0);

	if (event->button() == Qt::LeftButton && IsInResizeArea(event->pos()))
	{
		m_bIsResizing = true;
		setCursor(Qt::SizeFDiagCursor);
	}

	else
		QGraphicsItem::mousePressEvent(event);

}

void ROIItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	if (event->button() == Qt::LeftButton && m_bIsResizing)
		m_bIsResizing = false;
	else
		QGraphicsItem::mouseReleaseEvent(event);

}

bool ROIItem::IsInResizeArea(const QPointF & pos)
{
	return (pos.x() - m_size.width() + roi_cResizePos[0]) > (m_size.height() - pos.y());
}
