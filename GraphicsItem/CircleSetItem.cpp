#include "CircleSetItem.h"

CircleSetItem::CircleSetItem()
{
	setPos(0, 0);
}

CircleSetItem::CircleSetItem(geo::Circle2D line)
{
	m_circleSet.push_back(line);
	setPos(0, 0);
}

CircleSetItem::CircleSetItem(std::vector<geo::Circle2D> circleSet)
{
	m_circleSet = circleSet;
	setPos(0, 0);
}
CircleSetItem::~CircleSetItem()
{
}
void CircleSetItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * item, QWidget * widget)
{
	QPen pen;
	pen.setWidth(0);
	pen.setColor(m_color);
	pen.setCapStyle(Qt::RoundCap);
	QPen oldpen = painter->pen();
	painter->setPen(pen);
	int counts = m_circleSet.size();
	switch (m_CircleType)
	{
	case CircleType::Line:
		pen.setWidth(m_LineWidth);
		painter->setPen(pen);
		for (int i = 0; i < counts; i++)
		{
			QPointF center(m_circleSet[i].Center().X(), m_circleSet[i].Center().Y());
			painter->drawEllipse(center, m_circleSet[i].Radius(), m_circleSet[i].Radius());
			painter->drawPoint(center);
		}
		pen.setWidth(0);
		painter->setPen(pen);
		break;
	case CircleType::dotLine:
		for (int i = 0; i < counts; i++)
		{

		}
		break;
	case CircleType::dotdotLine:
		for (int i = 0; i < counts; i++)
		{

		}
		break;
	}
	painter->setPen(oldpen);
}
QRectF CircleSetItem::boundingRect() const
{
	std::vector<double> m_x, m_y;
	int counts = m_circleSet.size();
	for (int i = 0; i < counts; i++)
	{
		m_x.push_back(m_circleSet[i].Center().X() - m_circleSet[i].Radius());
		m_x.push_back(m_circleSet[i].Center().X() + m_circleSet[i].Radius());
		m_y.push_back(m_circleSet[i].Center().Y() - m_circleSet[i].Radius());
		m_y.push_back(m_circleSet[i].Center().Y() + m_circleSet[i].Radius());
	}
	std::sort(m_x.begin(), m_x.end());
	std::sort(m_y.begin(), m_y.end());
	qreal r = m_LineWidth * 0.5;
	QRectF rect;
	if (m_x.size()>0 && m_y.size()>0)
	{
		rect = QRectF(m_x[0] - r - 2, m_y[0] - r - 2,
			*(m_x.end() - 1) - m_x[0] + m_LineWidth + 4,
			*(m_y.end() - 1) - m_y[0] + m_LineWidth + 4);
	}
	return rect;
}
void CircleSetItem::setCircle(geo::Circle2D circle)
{
	m_circleSet.clear();
	this->m_circleSet.push_back(circle);
	this->setPos(0.5, 0.5);
	prepareGeometryChange();
	update();
}
void CircleSetItem::setCircles(std::vector<geo::Circle2D> circles)
{
	m_circleSet = circles;
	this->setPos(0.5, 0.5);
	prepareGeometryChange();
	update();
}
void CircleSetItem::setColor(QColor color)
{
	m_color = color;
	prepareGeometryChange();
	update();
}
void CircleSetItem::setLineWidth(int width)
{
	m_LineWidth = abs(width);
	prepareGeometryChange();
	update();
}
void CircleSetItem::setCircleType(CircleType mktp)
{
	m_CircleType = mktp;
	prepareGeometryChange();
	update();
}
