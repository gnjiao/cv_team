#pragma once
#include <QGraphicsItem>
#include "geo\Point2D.h"
#include "geo\Circle2D.h"
#include <QPainter>

class CircleSetItem :
	public QGraphicsItem
{
public:
	enum CircleType
	{
		Line,
		dotLine,
		dotdotLine,
	};
	CircleSetItem();
	CircleSetItem(geo::Circle2D circle);
	CircleSetItem(std::vector<geo::Circle2D> circles);
	~CircleSetItem();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
	QRectF boundingRect() const override;
	void setCircle(geo::Circle2D);
	void setCircles(std::vector<geo::Circle2D> circles);
	void setColor(QColor);
	void setLineWidth(int width);
	void setCircleType(CircleType);
private:
	std::vector<geo::Circle2D> m_circleSet;
	QColor m_color = QColor(Qt::blue);
	int m_LineWidth = 1;
	CircleType m_CircleType = CircleType::Line;
};
