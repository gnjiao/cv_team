#include "FindCircleItem.h"



FindCircleItem::FindCircleItem(geo::Circle2D c)
{
	setFlags(ItemIsSelectable | ItemIsMovable);
	setAcceptHoverEvents(true);
	m_circle = c;
	m_length = 30;
	m_width = 20;
	m_count = 10;
	m_penWidth = 0;
	m_color = Qt::green;
	generateRect();

}


FindCircleItem::~FindCircleItem()
{
}

void FindCircleItem::setCount(int count)
{
	m_count = count;
	prepareGeometryChange();
	update();
}

void FindCircleItem::setWidth(double width)
{
	m_width = width;
	prepareGeometryChange();
	update();
}

void FindCircleItem::setLength(double length)
{
	m_length = length;
	prepareGeometryChange();
	update();
}

void FindCircleItem::setCircle(Circle2D c)
{
	this->m_circle = c;
	generateRect();
	setPos(0, 0);
	prepareGeometryChange();
	update();
}

void FindCircleItem::setCircle(Circle2D c, AffineTrans trans)
{
	setCircle(c.TranslateBy(trans.Translation.ToVector2D()).RotateBy(trans.Center, trans.Rotation));
}

void FindCircleItem::setColor(QColor color)
{
	m_color = color;
	update();
}

void FindCircleItem::setPenWidth(qreal width)
{
	m_penWidth = width;
}

int FindCircleItem::getCount() const
{
	return m_count;
}

double FindCircleItem::getWidth() const
{
	return m_width;
}

double FindCircleItem::getLength() const
{
	return m_length;
}

Circle2D FindCircleItem::getCircle()
{
	QPointF pt = mapToScene(QPointF(m_circle.Center().X(), m_circle.Center().Y()));
	return Circle2D(Point2D(pt.x(), pt.y()), m_circle.Radius());
}

QColor FindCircleItem::getColor() const
{
	return m_color;
}

std::vector<geo::RotRect2D> FindCircleItem::getRects() const
{
	std::vector<geo::RotRect2D> rects;
	for each (auto var in m_rotRects)
	{
		QPointF center = QPointF(var.Center().X(), var.Center().Y());
		QPointF scene_center = this->mapToScene(center);
		rects.push_back(RotRect2D(Point2D(scene_center.x(), scene_center.y()), m_length, m_width, var.Direction()));
	}
	return rects;
}

void FindCircleItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	if (1 == isInResizeArea(event->pos()))
	{
		isResizing = 1;
	}
	else if (2 == isInResizeArea(event->pos()))	
	{
		isResizing = 2;
	}
	QGraphicsItem::mousePressEvent(event);
}

void FindCircleItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	double away = sqrt(pow((event->pos().x() - m_circle.Center().X()), 2) + pow((event->pos().y() - m_circle.Center().Y()), 2));
	if (isResizing == 1)
	{
		m_circle = Circle2D(m_circle.Center(), away);
		prepareGeometryChange();
		//qDebug() <<"away"<< away;
		return;
	}
	if (isResizing == 2)
	{
		double new_length = (away - m_circle.Radius())*2.0;
		m_length = new_length;
		prepareGeometryChange();
		return;
	}
	QGraphicsItem::mouseMoveEvent(event);
}

void FindCircleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	isResizing = false;
	setCursor(Qt::ArrowCursor);
	QGraphicsItem::mouseReleaseEvent(event);
}

void FindCircleItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
	QGraphicsItem::hoverMoveEvent(event);
}

void FindCircleItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * item, QWidget * widget)
{
	
	QPointF center(m_circle.Center().X(), m_circle.Center().Y());
	m_pen.setCapStyle(Qt::PenCapStyle::MPenCapStyle);
	m_pen.setColor(m_color); 
	m_pen.setWidth(m_penWidth); 
	m_pen.setStyle(Qt::DotLine);
	painter->setPen(m_pen);
	painter->drawEllipse(center, m_circle.Radius(), m_circle.Radius());
	generateRect();
	m_pen.setStyle(Qt::SolidLine);
	painter->setPen(m_pen);
	for (int i = 0; i < m_count; i++)
	{
		drawRect(painter, m_rotRects[i]);
		drawTriangle(painter, m_rotRects[i]);
	}
}

QRectF FindCircleItem::boundingRect() const
{
	QRectF m_rect(m_circle.Center().X() - (m_circle.Radius() + abs(m_length)),
		m_circle.Center().Y() - (m_circle.Radius() + abs(m_length)),
		2 * (m_circle.Radius() + abs(m_length)),
		2 * (m_circle.Radius() + abs(m_length)));
	return m_rect;
}

void FindCircleItem::drawRect(QPainter * painter, geo::RotRect2D &m_ro_Rect)
{
	QPointF point[] = { QPointF(m_ro_Rect.Point0().X(), m_ro_Rect.Point0().Y()),
		QPointF(m_ro_Rect.Point1().X(), m_ro_Rect.Point1().Y()),
		QPointF(m_ro_Rect.Point1().X(), m_ro_Rect.Point1().Y()),
		QPointF(m_ro_Rect.Point2().X(), m_ro_Rect.Point2().Y()),
		QPointF(m_ro_Rect.Point2().X(), m_ro_Rect.Point2().Y()),
		QPointF(m_ro_Rect.Point3().X(), m_ro_Rect.Point3().Y()),
		QPointF(m_ro_Rect.Point3().X(), m_ro_Rect.Point3().Y()),
		QPointF(m_ro_Rect.Point0().X(), m_ro_Rect.Point0().Y())
	};
	painter->drawLines(point, 4);
}

void FindCircleItem::generateRect()
{
	m_rotRects.clear();
	m_rotRects.push_back(RotRect2D(Point2D(m_circle.Center().X() + m_circle.Radius(), m_circle.Center().Y()), m_length, m_width, Vector2d(1, 0)));
	Point2D pointfirst = Point2D(m_circle.Center().X() + m_circle.Radius(), m_circle.Center().Y());
	Point2D center = m_circle.Center();
	Angle angle = Angle(360.0f / m_count, RadOrDeg::Deg);
	Point2D temp;
	for (int k = 1; k < m_count; k++)
	{
		temp = pointfirst.RotateBy(center, angle*k);
		m_rotRects.push_back(RotRect2D(temp, m_length, m_width, Vector2d(cos(k*angle.Radians()), sin(k*angle.Radians()))));
	}
}

void FindCircleItem::drawTriangle(QPainter * painter, geo::RotRect2D &m_rotRect)
{
	Line2D line = Line2D(m_rotRect.Point0(), m_rotRect.Point1());
	Line2D l_line = line.LineTo(m_rotRect.Point2(), false);
	Point2D point1 = m_rotRect.Point1() - 3 * line.Direction();
	Point2D point2 = point1 - l_line.Direction() * 3;
	Point2D point3 = point1 + l_line.Direction() * 3;
	QPointF Qpoint1(m_rotRect.Point1().X(), m_rotRect.Point1().Y());
	QPointF Qpoint2(point2.X(), point2.Y());
	QPointF Qpoint3(point3.X(), point3.Y());
	painter->drawLine(Qpoint1, Qpoint2);
	painter->drawLine(Qpoint3, Qpoint1);
}

int FindCircleItem::isInResizeArea(const QPointF & pos)
{
	double away = sqrt(pow((pos.x() - m_circle.Center().X()), 2) + pow((pos.y() - m_circle.Center().Y()), 2));
	if ((away > m_circle.Radius() - 5) && (away < m_circle.Radius() + 5))
	{
		return 1;
	}
	else if ((away > m_circle.Radius() + m_length / 2.0 - 5) && (away < m_circle.Radius() + m_length / 2.0 + 5))
	{
		return 2;
	}
	else
		return 0;
}
