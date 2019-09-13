#include "findLineItem.h"

#include <QRectF>

#include <QCursor>


findLineItem::findLineItem()

{

	setFlags(ItemIsSelectable);

	setAcceptHoverEvents(true);

	setZValue(100);

	m_rect = geo::RotRect2D(geo::Point2D(90, 20), 30, 150, Eigen::Vector2d(0, 1));

	m_line = Line2D((m_rect.Point0() + m_rect.Point1())*0.5, (m_rect.Point3() + m_rect.Point2())*0.5);

	m_clp_width = 20;

	m_count = 5;

	setPos(0, 0);

	generateRect();

}





findLineItem::~findLineItem()

{

}



void findLineItem::setAimArea(geo::RotRect2D &rect)//rect??scene???????

{
	m_rect = rect;

	m_line = Line2D((m_rect.Point0() + m_rect.Point1())*0.5, (m_rect.Point3() + m_rect.Point2())*0.5);

	generateRect();

	prepareGeometryChange();

	update();

}



void findLineItem::setAimArea(geo::Line2D &aline)
{

	geo::RotRect2D rect = geo::RotRect2D(

		geo::Point2D(aline.StartPoint()*0.5 + aline.EndPoint()*0.5),

		m_rect.Lenght(),

		aline.Length(),

		aline.DirectionBy()

		);

	setAimArea(rect);

}



void findLineItem::setAimArea(geo::RotRect2D &rect, AffineTrans &trans)

{

	setAimArea(rect.TranslateBy(trans.Translation.ToVector2D())

		.RotateBy(trans.Center, trans.Rotation));

}



void findLineItem::setWidth(double width)

{

	m_clp_width = width;

	generateRect();

	prepareGeometryChange();

	update();

}



void findLineItem::setCount(int count)

{

	this->m_count = count;

	generateRect();

	prepareGeometryChange();

	update();

}





void findLineItem::setStartPoint(Point2D start)
{
	this->m_line = Line2D(start, m_line.EndPoint());

	generateRect();

	prepareGeometryChange();

	update();
}

void findLineItem::setEndPoint(Point2D end)
{
	this->m_line = Line2D(m_line.StartPoint(), end);

	generateRect();

	prepareGeometryChange();

	update();
}

void findLineItem::setLine(Line2D line)
{
	this->m_line = line;

	generateRect();

	prepareGeometryChange();

	update();

}

std::vector<RotRect2D> findLineItem::get_Rects()

{

	std::vector<RotRect2D> toscene;

	QPointF pt;

	for (int i = 0; i < m_rotRects.size(); i++)

	{

		pt = QPointF(m_rotRects[i].Center().X(), m_rotRects[i].Center().Y());

		pt = this->mapToScene(pt);

		toscene.push_back(RotRect2D(Point2D(pt.x(), pt.y()), m_rect.Lenght(), m_clp_width, m_rotRects[i].Direction()));

	}

	return toscene;

}



Line2D findLineItem::getLine()

{

	geo::Line2D line = geo::Line2D(

		geo::Point2D(m_line.StartPoint().X() + pos().x(), m_line.StartPoint().Y() + pos().y()),

		geo::Point2D(m_line.EndPoint().X() + pos().x(), m_line.EndPoint().Y() + pos().y()));

	return line;

}



RotRect2D findLineItem::get_AimRect()

{

	QPointF pt = mapToScene(QPointF(m_rect.Center().X(), m_rect.Center().Y()));

	RotRect2D range = RotRect2D(

		Point2D(pt.x(), pt.y()),

		m_rect.Lenght(),

		m_rect.Width(),

		m_rect.Direction());

	return range;

}





//==============================================================================//



void findLineItem::mousePressEvent(QGraphicsSceneMouseEvent * event)

{

	QPointF startp(m_line.StartPoint().X(), m_line.StartPoint().Y());

	QPointF endp(m_line.EndPoint().X(), m_line.EndPoint().Y());

	if (isInArea(event->pos(), startp, m_sensitive))

	{

		isResizing = 1;

		QGraphicsItem::mousePressEvent(event);

		return;

	}



	if (isInArea(event->pos(), endp, m_sensitive))

	{

		isResizing = 2;

		QGraphicsItem::mousePressEvent(event);

		return;

	}



	if (cal_pointTOline(event->pos(), geo::Line2D(m_rect.Point1(), m_rect.Point2())) <= m_sensitive

		|| cal_pointTOline(event->pos(), geo::Line2D(m_rect.Point0(), m_rect.Point3())) <= m_sensitive)

	{

		isResizing = 3; 



		geo::Point2D mpos = geo::Point2D(event->pos().x(), event->pos().y());

		geo::Point2D foot = m_line.ClosestPointTo(mpos, false);

		double crossproduct = (m_line.EndPoint().X() - m_line.StartPoint().X())*(mpos.Y() - foot.Y())

			- (m_line.EndPoint().Y() - m_line.StartPoint().Y())*(mpos.X() - foot.X());

		m_clickmark = crossproduct;

		QGraphicsItem::mousePressEvent(event);

		return;

	}



	if (cal_pointTOline(event->pos(), m_line) <= m_rect.Lenght())

	{

		isResizing = 4;

		m_clickpos = geo::Point2D(event->pos().x(), event->pos().y()) - m_rect.Center();

		QGraphicsItem::mousePressEvent(event);

		return;

	}

	QGraphicsItem::mousePressEvent(event);

}



void findLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)

{

	if (isResizing)

	{

		Point2D m_mouse_pos = Point2D(event->pos().x(), event->pos().y());

		if (isResizing == 1)

		{

			setCursor(Qt::SizeAllCursor);

			geo::Line2D aline = geo::Line2D(m_mouse_pos, m_line.EndPoint());

			Eigen::Vector2d v(-aline.DirectionBy());
			
			m_rect = geo::RotRect2D((m_mouse_pos + m_line.EndPoint())*0.5, m_rect.Lenght(), aline.Length(), v);

			m_line = Line2D((m_rect.Point0() + m_rect.Point1())*0.5, (m_rect.Point3() + m_rect.Point2())*0.5);

			generateRect();

			prepareGeometryChange();

			update();

			QGraphicsItem::mouseMoveEvent(event);

			return;

		}

		if (isResizing == 2)
		{

			setCursor(Qt::SizeAllCursor);

			geo::Line2D aline = geo::Line2D(m_line.StartPoint(), m_mouse_pos);
			Eigen::Vector2d v(-aline.DirectionBy());
			m_rect = geo::RotRect2D(
				(m_mouse_pos + m_line.StartPoint())*0.5,
				m_rect.Lenght(),
				aline.Length(),
				v);

			m_line = Line2D((m_rect.Point0() + m_rect.Point1())*0.5, (m_rect.Point3() + m_rect.Point2())*0.5);

			generateRect();

			prepareGeometryChange();

			update();

			QGraphicsItem::mouseMoveEvent(event);

			return;

		}

		if (isResizing == 3)

		{

			double newLength = cal_pointTOline(event->pos(), m_line) * 2;

			//SignedAngleTo,ClosestPointTo

			geo::Point2D mpos = geo::Point2D(event->pos().x(), event->pos().y());

			geo::Point2D foot = m_line.ClosestPointTo(mpos, false);



			double crossproduct = (m_line.EndPoint().X() - m_line.StartPoint().X())*(mpos.Y() - foot.Y())

				- (m_line.EndPoint().Y() - m_line.StartPoint().Y())*(mpos.X() - foot.X());

			if ((crossproduct * m_clickmark) >= 0)

			{

				m_rect = geo::RotRect2D(m_rect.Center(), newLength, m_rect.Width(), m_rect.Direction());

			}

			else

			{
				Eigen::Vector2d v(-m_rect.Direction());

				m_rect = geo::RotRect2D(m_rect.Center(), newLength, m_rect.Width(),v);

			}

			m_line = Line2D((m_rect.Point0() + m_rect.Point1())*0.5, (m_rect.Point3() + m_rect.Point2())*0.5);

			generateRect();

			setCursor(Qt::UpArrowCursor);

			prepareGeometryChange();

			update();

			QGraphicsItem::mouseMoveEvent(event);

			return;

		}

		if (isResizing == 4)

		{

			m_rect = geo::RotRect2D((m_mouse_pos - m_clickpos),m_rect.Lenght(), m_rect.Width(), m_rect.Direction());

			m_line = Line2D((m_rect.Point0() + m_rect.Point1())*0.5, (m_rect.Point3() + m_rect.Point2())*0.5);

			setCursor(Qt::SizeAllCursor);

			generateRect();

			prepareGeometryChange();

			update();

			QGraphicsItem::mouseMoveEvent(event);

			return;

		}

	}

	QGraphicsItem::mouseMoveEvent(event);

}



void findLineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)

{

	isResizing = false;

	setCursor(Qt::ArrowCursor);

	m_clickmark = 0;

	QGraphicsItem::mouseReleaseEvent(event);

}



void findLineItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)

{

	QPointF start_(m_line.StartPoint().X(), m_line.StartPoint().Y());

	QPointF end_(m_line.EndPoint().X(), m_line.EndPoint().Y());

	if ((isInArea(event->pos(), start_, m_sensitive)) || (isInArea(event->pos(), end_, m_sensitive)))

	{

		setCursor(Qt::SizeAllCursor);

	}

	else

	{

		setCursor(Qt::ArrowCursor);

	}



	QGraphicsItem::hoverMoveEvent(event);

}





QRectF findLineItem::boundingRect() const

{

	std::vector<double> m_x, m_y;

	m_x.push_back(m_rotRects[0].Point0().X());

	m_x.push_back(m_rotRects[m_count - 1].Point0().X());

	m_x.push_back(m_rotRects[0].Point1().X());

	m_x.push_back(m_rotRects[m_count - 1].Point1().X());

	m_x.push_back(m_rotRects[0].Point2().X());

	m_x.push_back(m_rotRects[m_count - 1].Point2().X());

	m_x.push_back(m_rotRects[0].Point3().X());

	m_x.push_back(m_rotRects[m_count - 1].Point3().X());



	m_y.push_back(m_rotRects[0].Point0().Y());

	m_y.push_back(m_rotRects[m_count - 1].Point0().Y());

	m_y.push_back(m_rotRects[0].Point1().Y());

	m_y.push_back(m_rotRects[m_count - 1].Point1().Y());

	m_y.push_back(m_rotRects[0].Point2().Y());

	m_y.push_back(m_rotRects[m_count - 1].Point2().Y());

	m_y.push_back(m_rotRects[0].Point3().Y());

	m_y.push_back(m_rotRects[m_count - 1].Point3().Y());



	std::sort(m_x.begin(), m_x.end());

	std::sort(m_y.begin(), m_y.end());



	QRectF m_rect(m_x[0] - 5, m_y[0] - 5,

		*(m_x.end() - 1) - m_x[0] + 15,

		*(m_y.end() - 1) - m_y[0] + 10);

	return m_rect;

}

QPainterPath findLineItem::shape() const
{
	QPainterPath path;

	QVector<QPoint> points; 
	
	points.append(QPoint(this->m_rotRects.back().Point3().X(), this->m_rotRects.back().Point3().Y()));

	points.append(QPoint(this->m_rotRects.back().Point2().X(), this->m_rotRects.back().Point2().Y()));

	points.append(QPoint(this->m_rotRects[0].Point1().X(), this->m_rotRects[0].Point1().Y()));
	
	points.append(QPoint(this->m_rotRects[0].Point0().X(), this->m_rotRects[0].Point0().Y()));
	
	QPolygon poly(points);
	
	path.addPolygon(poly);

	return path;
}







void findLineItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * item, QWidget * widget)

{

	QPointF start_point = QPointF(m_line.StartPoint().X(), m_line.StartPoint().Y());

	QPointF end_point = QPointF(m_line.EndPoint().X(), m_line.EndPoint().Y());

	QPen pen, oldpen;

	oldpen = painter->pen();

	pen.setCapStyle(Qt::PenCapStyle::MPenCapStyle);

	pen.setWidth(0);

	pen.setStyle(Qt::DotLine);

	pen.setColor(m_color);

	painter->setPen(pen);

	painter->drawLine(start_point, end_point);



	pen.setWidth(0);

	pen.setStyle(Qt::SolidLine);

	pen.setColor(m_color);

	painter->setPen(pen);

	RotRect2D small_rect = RotRect2D(m_line.StartPoint(), m_sensitive, m_sensitive, m_line.Direction());

	RotRect2D small_rect1 = RotRect2D(m_line.EndPoint(), m_sensitive, m_sensitive, m_line.Direction());

	draw_rect(painter, small_rect);

	draw_rect(painter, small_rect1);



	

	pen.setStyle(Qt::SolidLine);

	pen.setColor(m_color);

	pen.setWidth(0);

	painter->setPen(pen);

	for (int i = 0; i < m_count; i++)

	{

		draw_rect(painter, m_rotRects[i]);

	}



	pen.setStyle(Qt::SolidLine);

	pen.setColor(m_color);

	painter->setPen(pen);

	for (int i = 0; i < m_count; i++)

	{

		draw_triangle(painter, m_rotRects[i]);

	}

	painter->setPen(oldpen);

}



void findLineItem::draw_rect(QPainter * painter, RotRect2D &m_ro_Rect)

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



void findLineItem::draw_triangle(QPainter * painter, RotRect2D &m_rotRect)

{

	Line2D l_line = m_line.LineTo(m_rotRect.Point1(), false);

	Point2D point1 = m_rotRect.Point1() - 3 * l_line.Direction();

	Point2D point2 = point1 - m_line.Direction() * 3;

	Point2D point3 = point1 + m_line.Direction() * 3;

	QPointF Qpoint1(m_rotRect.Point1().X(), m_rotRect.Point1().Y());

	QPointF Qpoint2(point2.X(), point2.Y());

	QPointF Qpoint3(point3.X(), point3.Y());



	painter->drawLine(Qpoint1, Qpoint2);

	painter->drawLine(Qpoint3, Qpoint1);

}



//==============================================================================//




bool findLineItem::isInArea(QPointF pos, QPointF position, float range)

{

	float dist = sqrtf(pow(pos.x() - position.x(), 2) + pow(pos.y() - position.y(), 2));

	if (dist <range)

	{

		return true;

	}

	else

	{

		return false;

	}

}





void findLineItem::generateRect()

{

	double steplength = (m_line.Length() - m_clp_width) / (m_count - 1);

	Vector2d step(m_line.EndPoint().X() - m_line.StartPoint().X(),

		m_line.EndPoint().Y() - m_line.StartPoint().Y());

	step = step.normalized();



	m_rotRects.clear();

	geo::RotRect2D unitReect;

	geo::Point2D unitmark = geo::Point2D((m_line.StartPoint() + step*(m_clp_width*0.5)));

	for (int i = 0; i < m_count; i++)

	{

		unitReect = geo::RotRect2D(

			unitmark,

			m_rect.Lenght(),

			m_clp_width,

			m_rect.Direction()

			);

		m_rotRects.push_back(unitReect);

		unitmark = unitmark + step*steplength;

	}

}



float findLineItem::cal_pointTOline(QPointF pos, geo::Line2D &line)

{

	Point2D point(pos.x(), pos.y());

	Line2D vertLine = line.LineTo(point, false);

	return vertLine.Length();

}

