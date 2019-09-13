#include "EraserItem.h"

EraserItem::EraserItem(int x, int y, int width, int height) : m_size(0,0, width, height),m_bIsResizing(false)
{
	setAcceptHoverEvents(true); 
	setPos(x,y);

	setFlag(QGraphicsItem::ItemIsMovable, true);  
	setFlag(QGraphicsItem::ItemIsSelectable, true);  
	setFlag(QGraphicsItem::ItemIsFocusable, true);  
	
}

EraserItem::~EraserItem()
{
}

void EraserItem::setColor(QColor color)
{
	this->m_color = color;
	prepareGeometryChange();

}

geo::Rect2D EraserItem::getRect()
{
	QPointF PointTL = mapToScene(m_size.topLeft());

	QPointF PointBR = mapToScene(m_size.bottomRight());

	QRectF RoiInScene = QRectF(PointTL, PointBR);

	return geo::Rect2D(geo::Point2D(RoiInScene.left(), RoiInScene.top()), geo::Point2D(RoiInScene.right(), RoiInScene.bottom()));
}


QRectF EraserItem::boundingRect() const
{
	return  QRectF(0, 0, m_size.width()+3, m_size.height()+3);
}

void EraserItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
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
		
		for (int i = 0; i < 2; ++i)
		
			painter->drawLine(w - eraser_cResizePos[i], h, w, h - eraser_cResizePos[i]);

		painter->setRenderHint(QPainter::Antialiasing, false); 
	}
	else
	{
		painter->setPen(m_color);
		
		painter->drawRect(QRectF(0, 0, m_size.width(), m_size.height()));

	}




}

void EraserItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
	if (m_bIsResizing || (IsInResizeArea(event->pos()) && isSelected()))
		
		QGraphicsItem::setCursor(Qt::SizeFDiagCursor);
	
	else
		
		QGraphicsItem::setCursor(QPixmap(":/CV_TEAM/icons/eraser_green.png"));

	QGraphicsItem::hoverMoveEvent(event);
}

void EraserItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	if (m_bIsResizing)
	{
		QGraphicsItem::setCursor(Qt::SizeFDiagCursor);
		
		qreal w = event->pos().x();
		
		qreal h = event->pos().y();
		
		if (w > 5)
		
			m_size.setWidth(w);

		if (h > 5)
			
			m_size.setHeight(h);

		emit EraserChanged(m_size.width(),m_size.height());
		
		prepareGeometryChange();  
	}
	
	else	
	{
	
		QGraphicsItem::mouseMoveEvent(event);
	}

}



void EraserItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	
	
	static qreal z = 0.0;
	
	setZValue(z += 1.0);

	if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
	{	
		if (IsInResizeArea(event->pos()))
		{
			m_bIsResizing = true;
			QGraphicsItem::setCursor(Qt::SizeFDiagCursor);	
		}
			
		QGraphicsItem::setCursor(QPixmap(":/CV_TEAM/icons/eraser_red.png"));

		QGraphicsItem::mousePressEvent(event);
	}
		
	else
	{

		QGraphicsItem::setCursor(QPixmap(":/CV_TEAM/icons/eraser_green.png"));

		QGraphicsItem::mousePressEvent(event);
	}	

}

void EraserItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	if (m_bIsResizing)
		
		m_bIsResizing = false;
	
	else
	
		QGraphicsItem::mouseReleaseEvent(event);

}

bool EraserItem::IsInResizeArea(const QPointF & pos)
{

	return (pos.x() - m_size.width() + eraser_cResizePos[0]) > (m_size.height() - pos.y());

}

