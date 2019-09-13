#pragma once
#include <QGraphicsItem>
#include "geo\Point2D.h"
#include <QPainter>

class PointSetItem :
	public QGraphicsItem
{
public:
	enum MarkType
	{
		point,
		cross,
		plus,
		circle,
		rect
	};
	PointSetItem();
	PointSetItem(std::vector<geo::Point2D> pointSet);
	~PointSetItem();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
	QRectF boundingRect() const override;
	void setPoints(std::vector<geo::Point2D> pts);
	void setPoint(geo::Point2D);
	void setColor(QColor);
	void setDiameter(int dia);
	void setMarkType(MarkType mktp);
private:
	std::vector<geo::Point2D> m_pointSet;
	QColor m_color = QColor(Qt::blue);
	int m_diameter = 5;
	MarkType m_marktype = circle;
};