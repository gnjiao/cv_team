#include "FindArcItem.h"
#include <qDebug>


FindArcItem::FindArcItem(Point2D center, Point2D firstPoint, Point2D secondPoint,bool direction)
{
	setFlags(ItemIsSelectable | ItemIsMovable);
	setAcceptHoverEvents(true);
	m_centerPoint = center;
	m_firstPoint = firstPoint;
	m_secondPoint = secondPoint;
	m_direction = direction;
	m_circle = Circle2D(center, m_firstPoint.DistanceTo(m_centerPoint));
	m_horizontalLine = Line2D(Point2D(0, 0), Point2D(1, 0));
	m_firstLine = Line2D(m_centerPoint, m_firstPoint);
	m_secondLine = Line2D(m_centerPoint, m_secondPoint);
	m_length = 30;
	m_width = 20;
	m_count = 10;
	m_penWidth = 0;
	m_color = Qt::green;
	generateRect();

}


FindArcItem::~FindArcItem()
{
}

void FindArcItem::setCount(int count)
{
	m_count = count;
	prepareGeometryChange();
	update();
}

void FindArcItem::setWidth(double width)
{
	m_width = width;
	prepareGeometryChange();
	update();
}

void FindArcItem::setLength(double length)
{
	m_length = length;
	prepareGeometryChange();
	update();
}

void FindArcItem::setCircle(Circle2D c)
{
	this->m_circle = c;
	generateRect();
	setPos(0, 0);
	prepareGeometryChange();
	update();
}

void FindArcItem::setCircle(Circle2D c, AffineTrans trans)
{
	setCircle(c.TranslateBy(trans.Translation.ToVector2D()).RotateBy(trans.Center, trans.Rotation));
}

void FindArcItem::setColor(QColor color)
{
	m_color = color;
	update();
}

void FindArcItem::setPenWidth(qreal width)
{
	m_penWidth = width;
}

int FindArcItem::getCount() const
{
	return m_count;
}

double FindArcItem::getWidth() const
{
	return m_width;
}

double FindArcItem::getLength() const
{
	return m_length;
}

Circle2D FindArcItem::getCircle()
{
	QPointF pt = mapToScene(QPointF(m_circle.Center().X(), m_circle.Center().Y()));
	return Circle2D(Point2D(pt.x(), pt.y()), m_circle.Radius());
}

QColor FindArcItem::getColor() const
{
	return m_color;
}

std::vector<geo::RotRect2D> FindArcItem::getRects() const
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

void FindArcItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	if (1 == isInResizeArea(event->pos()))
	{
		isResizing = 1;
	}
	else if (2 == isInResizeArea(event->pos()))
	{
		isResizing = 2;
	}
	else if (3 == isInResizeArea(event->pos()))
	{
		isResizing = 3;
	}
	else
	{
		isResizing = 0;
	}

	QGraphicsItem::mousePressEvent(event);
}

void FindArcItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	Point2D pos = Point2D(event->pos().x(), event->pos().y());
	double away = sqrt(pow((event->pos().x() - m_circle.Center().X()), 2) + pow((event->pos().y() - m_circle.Center().Y()), 2));
	if (isResizing == 1)
	{
		m_firstLine = Line2D(m_centerPoint,pos);	
		m_firstPoint = m_centerPoint + (m_firstLine.Direction()* m_circle.Radius());
		m_firstLine = Line2D(m_centerPoint, m_firstPoint);
		double startAngle = -m_horizontalLine.SignedAngleTo(m_firstLine).Degrees();
		double endAngle = 360 - m_horizontalLine.SignedAngleTo(m_secondLine).Degrees();
		double spanAngle = endAngle-startAngle;
		//qDebug() << "start angle:" << startAngle;
		//qDebug() << "end angle:" << endAngle;
		//qDebug() << "span angle:" << spanAngle;
		prepareGeometryChange();
		return;
	}
	else if (isResizing == 2)
	{
		m_secondLine = Line2D(m_centerPoint, pos);
		m_secondPoint = m_centerPoint + (m_secondLine.Direction()* m_circle.Radius());
		m_secondLine = Line2D(m_centerPoint, m_secondPoint);
		double startAngle = -m_horizontalLine.SignedAngleTo(m_firstLine).Degrees();
		double endAngle = 360 - m_horizontalLine.SignedAngleTo(m_secondLine).Degrees();
		double spanAngle = endAngle-startAngle;
		//qDebug() << "start angle:" << startAngle;
		//qDebug() << "end angle:" << endAngle;
		//qDebug() << "span angle:" << spanAngle;
		prepareGeometryChange();
		return;
	}
	else if (isResizing == 3)
	{
		m_circle = Circle2D(m_circle.Center(), away);
		m_firstPoint = m_centerPoint + (m_firstLine.Direction()* m_circle.Radius());
		m_secondPoint = m_centerPoint + (m_secondLine.Direction()* m_circle.Radius());
		prepareGeometryChange();
		return;
	}

	QGraphicsItem::mouseMoveEvent(event);
}

void FindArcItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	isResizing = 0;
	setCursor(Qt::ArrowCursor);
	QGraphicsItem::mouseReleaseEvent(event);
}

void FindArcItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
	if (1 == isInResizeArea(event->pos()))
	{
		setCursor(Qt::SizeAllCursor);
		return;
	}
	else if (2 == isInResizeArea(event->pos()))
	{
		setCursor(Qt::SizeAllCursor);
		return;
	}
	else if (3 == isInResizeArea(event->pos()))
	{
		Line2D posLine = Line2D(Point2D(event->pos().x(), event->pos().y()), m_centerPoint);
		QMatrix matrix;
		matrix.rotate(posLine.AngleTo(m_horizontalLine).Degrees() + 45);
		QPixmap pix = QPixmap(":/CV_TEAM/icons/resize_vertical.png").transformed(matrix, Qt::SmoothTransformation);
		setCursor(pix);
		return;
	}
	else if (0 == isInResizeArea(event->pos()))
	{
		setCursor(Qt::ClosedHandCursor);
	}
	QGraphicsItem::hoverMoveEvent(event);
}

void FindArcItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * item, QWidget * widget)
{
	double startAngle = -m_horizontalLine.SignedAngleTo(m_firstLine).Degrees();
	double endAngle = 360 - m_horizontalLine.SignedAngleTo(m_secondLine).Degrees();
	double spanAngle = endAngle-startAngle;
	if (spanAngle >= 360)
	{
		spanAngle = spanAngle - 360;
	}
	//设定扇形所在的矩形，扇形画在矩形内部
	QRectF rectangle(m_circle.Center().X()-m_circle.Radius(), m_circle.Center().Y() - m_circle.Radius(), 2* m_circle.Radius(), 2*m_circle.Radius());
	painter->drawPie(rectangle, startAngle*16, spanAngle*16);

	//QPointF center(m_circle.Center().X(), m_circle.Center().Y());
	//m_pen.setCapStyle(Qt::PenCapStyle::MPenCapStyle);
	//m_pen.setColor(m_color); 
	//m_pen.setWidth(m_penWidth); 
	//m_pen.setStyle(Qt::DotLine);
	//painter->setPen(m_pen);
	//painter->drawEllipse(center, m_circle.Radius(), m_circle.Radius());
	//generateRect();
	//m_pen.setStyle(Qt::SolidLine);
	//painter->setPen(m_pen);
	//for (int i = 0; i < m_count; i++)
	//{
	//	drawRect(painter, m_rotRects[i]);
	//	drawTriangle(painter, m_rotRects[i]);
	//}
}

QRectF FindArcItem::boundingRect() const
{
	QRectF m_rect(m_circle.Center().X() - (m_circle.Radius() + abs(m_length)),
		m_circle.Center().Y() - (m_circle.Radius() + abs(m_length)),
		2 * (m_circle.Radius() + abs(m_length)),
		2 * (m_circle.Radius() + abs(m_length)));
	return m_rect;
}

void FindArcItem::drawRect(QPainter * painter, geo::RotRect2D &m_ro_Rect)
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

void FindArcItem::generateRect()
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

void FindArcItem::drawTriangle(QPainter * painter, geo::RotRect2D &m_rotRect)
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

int FindArcItem::isInResizeArea(const QPointF & pos)
{
	//判断鼠标点位置
	double startPointAway = sqrt(pow((pos.x() - m_firstPoint.X()), 2) + pow((pos.y() - m_firstPoint.Y()), 2));
	double endPointAway = sqrt(pow((pos.x() - m_secondPoint.X()), 2) + pow((pos.y() - m_secondPoint.Y()), 2));
	double radiusAway= sqrt(pow((pos.x() - m_circle.Center().X()), 2) + pow((pos.y() - m_circle.Center().Y()), 2));
	if (startPointAway < 8)
		return 1;
	else if (endPointAway < 8)
		return 2;
	else if ((radiusAway > m_circle.Radius() - 5) && (radiusAway < m_circle.Radius() + 5))
		return 3;
	else
		return 0;
}
