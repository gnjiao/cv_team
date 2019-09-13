#include "LineSetItem.h"

LineSetItem::LineSetItem()
{
	setPos(0, 0);
}

LineSetItem::LineSetItem(geo::Line2D line)
{
	m_lineSet.push_back(line);
	setPos(0, 0);
}

LineSetItem::LineSetItem(std::vector<geo::Line2D> lineSet)
{
	m_lineSet = lineSet;
	setPos(0, 0);
}
LineSetItem::~LineSetItem()
{
}
void LineSetItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * item, QWidget * widget)
{
	QPen pen;
	pen.setWidth(0);
	pen.setColor(m_color);
	pen.setCapStyle(Qt::RoundCap);
	QPen oldpen = painter->pen();
	painter->setPen(pen);
	int counts = m_lineSet.size();
	switch (m_LineType)
	{
	case LineType::Line:
		pen.setWidth(m_LineWidth);
		painter->setPen(pen);
		for (int i = 0; i < counts; i++)
		{
			QLineF line = QLineF(QPointF(m_lineSet[i].StartPoint().X(),m_lineSet[i].StartPoint().Y()),
								 QPointF(m_lineSet[i].EndPoint().X(), m_lineSet[i].EndPoint().Y()));
			painter->drawLine(line);
		}
		pen.setWidth(0);
		painter->setPen(pen);
		break;
	case LineType::dotLine:
		for (int i = 0; i < counts; i++)
		{

		}
		break;
	case LineType::dotdotLine:
		for (int i = 0; i < counts; i++)
		{

		}
		break;
	}
	painter->setPen(oldpen);
}
QRectF LineSetItem::boundingRect() const
{
	std::vector<double> m_x, m_y;
	int counts = m_lineSet.size();
	for (int i = 0; i < counts; i++)
	{
		m_x.push_back(m_lineSet[i].StartPoint().X());
		m_x.push_back(m_lineSet[i].EndPoint().X());
		m_y.push_back(m_lineSet[i].StartPoint().Y());
		m_y.push_back(m_lineSet[i].EndPoint().Y());
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
void LineSetItem::setLine(geo::Line2D line)
{
	m_lineSet.clear();
	this->m_lineSet.push_back(line);
	this->setPos(0.5, 0.5);
	prepareGeometryChange();
	update();
}
void LineSetItem::setLines(std::vector<geo::Line2D> lines)
{
	m_lineSet = lines;
	this->setPos(0.5, 0.5);
	prepareGeometryChange();
	update();
}
void LineSetItem::setColor(QColor color)
{
	m_color = color;
	prepareGeometryChange();
	update();
}
void LineSetItem::setLineWidth(int width)
{
	m_LineWidth = abs(width);
	prepareGeometryChange();
	update();
}
void LineSetItem::setLineType(LineType mktp)
{
	m_LineType = mktp;
	prepareGeometryChange();
	update();
}
