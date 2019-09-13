#include "FrontSightItem.h"
FrontSightItem::FrontSightItem()
{
	setFlag(ItemIsSelectable, false);
	setFlag(ItemIsFocusable, false);
	m_pen.setWidth(0);
	m_pen.setColor(m_color);
	m_pen.setStyle(Qt::DashLine);
	updateLine();
	updateCenterSight();
}
FrontSightItem::~FrontSightItem()
{
}
void FrontSightItem::setColor(QColor color)
{
	m_color = color;
	m_pen.setColor(m_color);
	update();
}
void FrontSightItem::setMarkType(int ty)
{
	m_centerType = ty;
	if (ty == 2)
		updateCenterSight();
	update();
}
void FrontSightItem::setMarkCenter(double length)
{
	m_markDNmm = length;
	updateCenterSight();
	update();
}
void FrontSightItem::setImageSize(QRectF size)
{
	d_rectPic = size;
	updateLine();
	updateCenterSight();
	prepareGeometryChange();
	update();
}
void FrontSightItem::setRatioXPixPerMM(double ratioX)
{
	m_rPixPerMM = ratioX;
	updateCenterSight();
	update();
}
void FrontSightItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * item, QWidget * widget)
{
	QPen oldpen = painter->pen();
	painter->setPen(m_pen);
	painter->drawLine(d_lineH);
	painter->drawLine(d_lineV);
	painter->drawLine(d_lineA);
	painter->drawLine(d_lineB);
	switch (m_centerType)
	{
	case 0:
		painter->drawEllipse(d_rectC);
		break;
	case 1:
		painter->drawRect(d_rectC);
		break;
	case 2:
		m_pen.setStyle(Qt::SolidLine);
		painter->setPen(m_pen);
		for (int j = 0; j < d_lineS.size(); j++)
		{
			painter->drawLine(d_lineS[j]);
		}
		m_pen.setStyle(Qt::DashLine);
		painter->setPen(m_pen);
		break;
	default:
		break;
	}
	painter->drawRect(d_rectPic);
	painter->setPen(oldpen);
}
QRectF FrontSightItem::boundingRect() const
{
	qreal bx, by, bw, bh;
	bx = qMin(d_rectC.left(), d_rectPic.left());
	by = qMin(d_rectC.top(), d_rectPic.top());
	bw = qMax(d_rectC.width(), d_rectPic.width());
	bh = qMax(d_rectC.height(), d_rectPic.height());
	QRectF bRect = QRectF(bx - 10, by - 10, bw + 20, bh + 20);
	return bRect;
}
void FrontSightItem::updateLine()
{
	double midX = (d_rectPic.left() + d_rectPic.right())*0.5;
	double midY = (d_rectPic.top() + d_rectPic.bottom())*0.5;
	d_lineH = QLineF(d_rectPic.left(), midY, d_rectPic.right(), midY);
	d_lineV = QLineF(midX, d_rectPic.top(), midX, d_rectPic.bottom());
	if (d_rectPic.width()>d_rectPic.height())
	{
		double delta = d_rectPic.height() * 0.5;
		d_lineA = QLineF(midX - delta, d_rectPic.top(), midX - delta, d_rectPic.bottom());
		d_lineB = QLineF(midX + delta, d_rectPic.top(), midX + delta, d_rectPic.bottom());
	}
	else
	{
		double delta = d_rectPic.width() * 0.5;
		d_lineA = QLineF(d_rectPic.left(), midY - delta, d_rectPic.right(), midY - delta);
		d_lineB = QLineF(d_rectPic.left(), midY + delta, d_rectPic.right(), midY + delta);
	}
}
void FrontSightItem::updateCenterSight()
{
	double DN_pix = abs(m_rPixPerMM * m_markDNmm);
	double corX = (d_rectPic.left() + d_rectPic.right())*0.5 - DN_pix * 0.5;
	double corY = (d_rectPic.top() + d_rectPic.bottom())*0.5 - DN_pix * 0.5;
	d_rectC = QRectF(corX, corY, DN_pix, DN_pix);
	d_lineS.clear();
	if (m_centerType == 2)
	{
		double dlt4 = DN_pix * 0.25;
		double dlt10 = DN_pix * 0.1;
		double dlt20 = DN_pix * 0.05;
		double dlt16 = dlt4 * 0.2500;
		double dlt32 = dlt4 * 0.1250;
		double midX = corX + DN_pix * 0.5;
		double midY = corY + DN_pix * 0.5;
		d_lineS.push_back(QLineF(midX + dlt10, corY, midX - dlt10, corY));
		d_lineS.push_back(QLineF(midX + dlt10, corY + DN_pix, midX - dlt10, corY + DN_pix));
		d_lineS.push_back(QLineF(corX, midY + dlt10, corX, midY - dlt10));
		d_lineS.push_back(QLineF(corX + DN_pix, midY + dlt10, corX + DN_pix, midY - dlt10));
		d_lineS.push_back(QLineF(midX + dlt16, corY + dlt4, midX - dlt16, corY + dlt4));
		d_lineS.push_back(QLineF(midX + dlt16, corY + DN_pix - dlt4, midX - dlt16, corY + DN_pix - dlt4));
		d_lineS.push_back(QLineF(corX + dlt4, midY + dlt16, corX + dlt4, midY - dlt16));
		d_lineS.push_back(QLineF(corX + DN_pix - dlt4, midY + dlt16, corX + DN_pix - dlt4, midY - dlt16));
		for (int i = 1; i < 20; i++)
		{
			d_lineS.push_back(QLineF(midX, corY + i * dlt20, midX + dlt32, corY + i * dlt20));
			d_lineS.push_back(QLineF(corX + i * dlt20, midY, corX + i * dlt20, midY - dlt32));
		}
		d_lineS.push_back(QLineF(midX, corY, midX, corY + DN_pix));
		d_lineS.push_back(QLineF(corX, midY, corX + DN_pix, midY));
		d_lineS.push_back(QLineF(corX, corY, corX + dlt10, corY));
		d_lineS.push_back(QLineF(corX, corY, corX, corY + dlt10));
		d_lineS.push_back(QLineF(corX + DN_pix, corY, corX + DN_pix - dlt10, corY));
		d_lineS.push_back(QLineF(corX + DN_pix, corY, corX + DN_pix, corY + dlt10));
		d_lineS.push_back(QLineF(corX, corY + DN_pix, corX + dlt10, corY + DN_pix));
		d_lineS.push_back(QLineF(corX, corY + DN_pix, corX, corY + DN_pix - dlt10));
		d_lineS.push_back(QLineF(corX + DN_pix, corY + DN_pix, corX + DN_pix - dlt10, corY + DN_pix));
		d_lineS.push_back(QLineF(corX + DN_pix, corY + DN_pix, corX + DN_pix, corY + DN_pix - dlt10));
	}
}
//