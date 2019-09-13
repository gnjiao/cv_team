#include "PointSetItem.h"

PointSetItem::PointSetItem()
{
	setPos(0, 0);
}
PointSetItem::PointSetItem(std::vector<geo::Point2D> pointSet)
{
	m_pointSet = pointSet;
	setPos(0, 0);
}
PointSetItem::~PointSetItem()
{
}
void PointSetItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * item, QWidget * widget)
{
	QPen ppap;
	ppap.setWidth(0);
	ppap.setColor(m_color);
	ppap.setCapStyle(Qt::RoundCap);
	QPen oldpen = painter->pen();
	painter->setPen(ppap);
	int counts = m_pointSet.size();
	QPointF pointA;
	QLineF lineA, lineB;
	QRectF rectA;
	switch (m_marktype)
	{
	case point:
		ppap.setWidth(m_diameter);
		painter->setPen(ppap);
		for (int i = 0; i < counts; i++)
		{
			pointA = QPointF(m_pointSet[i].X(), m_pointSet[i].Y());
			painter->drawPoint(pointA);
		}
		ppap.setWidth(0);
		painter->setPen(ppap);
		break;
	case cross:
		for (int i = 0; i < counts; i++)
		{
			lineA = QLineF(m_pointSet[i].X() - m_diameter*0.5, m_pointSet[i].Y() - m_diameter*0.5,
				m_pointSet[i].X() + m_diameter*0.5, m_pointSet[i].Y() + m_diameter*0.5);
			lineB = QLineF(m_pointSet[i].X() - m_diameter*0.5, m_pointSet[i].Y() + m_diameter*0.5,
				m_pointSet[i].X() + m_diameter*0.5, m_pointSet[i].Y() - m_diameter*0.5);
			painter->drawLine(lineA);
			painter->drawLine(lineB);
		}
		break;
	case plus:
		for (int i = 0; i < counts; i++)
		{
			lineA = QLineF(m_pointSet[i].X() - m_diameter*0.5, m_pointSet[i].Y(),
				m_pointSet[i].X() + m_diameter*0.5, m_pointSet[i].Y());
			lineB = QLineF(m_pointSet[i].X(), m_pointSet[i].Y() - m_diameter*0.5,
				m_pointSet[i].X(), m_pointSet[i].Y() + m_diameter*0.5);
			painter->drawLine(lineA);
			painter->drawLine(lineB);
		}
		break;
	case circle:
		for (int i = 0; i < counts; i++)
		{
			rectA = QRectF(m_pointSet[i].X() - m_diameter*0.5, m_pointSet[i].Y() - m_diameter*0.5, m_diameter, m_diameter);
			painter->drawEllipse(rectA);
		}
		break;
	case rect:
		for (int i = 0; i < counts; i++)
		{
			rectA = QRectF(m_pointSet[i].X() - m_diameter*0.5, m_pointSet[i].Y() - m_diameter*0.5, m_diameter, m_diameter);
			painter->drawRect(rectA);
		}
		break;
	}
	painter->setPen(oldpen);
}
QRectF PointSetItem::boundingRect() const
{
	std::vector<double> m_x, m_y;
	int counts = m_pointSet.size();
	for (int i = 0; i < counts; i++)
	{
		m_x.push_back(m_pointSet[i].X());
		m_y.push_back(m_pointSet[i].Y());
	}
	std::sort(m_x.begin(), m_x.end());
	std::sort(m_y.begin(), m_y.end());
	qreal r = m_diameter * 0.5;
	QRectF bdr;
	if (m_x.size()>0 && m_y.size()>0)
	{
		bdr = QRectF(m_x[0] - r - 2, m_y[0] - r - 2,
			*(m_x.end() - 1) - m_x[0] + m_diameter + 4,
			*(m_y.end() - 1) - m_y[0] + m_diameter + 4);
	}
	return bdr;
}
void PointSetItem::setPoints(std::vector<geo::Point2D> pts)
{
	m_pointSet = pts;
	this->setPos(0.5, 0.5);
	prepareGeometryChange();
	update();
}
void PointSetItem::setPoint(geo::Point2D point)
{
	m_pointSet.clear();
	this->m_pointSet.push_back(point);
	this->setPos(0.5, 0.5);
	prepareGeometryChange();
	update();
}
void PointSetItem::setColor(QColor colo)
{
	m_color = colo;
	prepareGeometryChange();
	update();
}
void PointSetItem::setDiameter(int dia)
{
	m_diameter = abs(dia);
	prepareGeometryChange();
	update();
}
void PointSetItem::setMarkType(MarkType mktp)
{
	m_marktype = mktp;
	prepareGeometryChange();
	update();
}