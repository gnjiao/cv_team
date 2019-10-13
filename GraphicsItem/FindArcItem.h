#pragma once

#include "geo\RotRect2D.h"
#include "geo\Rect2D.h"
#include "geo\Point2D.h"
#include "geo\Line2D.h"
#include "geo\AffineTrans.h"
#include "geo\Circle2D.h"

#include <QGraphicsItem>
#include <QPainter>
#include <QColor>
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>

using namespace geo;
using namespace Eigen;

class FindArcItem : public QGraphicsItem
{
public:
	FindArcItem(Point2D center,Point2D first,Point2D second,bool dir);
	~FindArcItem();
	void setCount(int count);
	void setWidth(double width);
	void setLength(double length);
	void setCircle(Circle2D c);
	void setCircle(Circle2D c, AffineTrans trans);
	void setColor(QColor color);
	void setPenWidth(qreal width);

	int getCount() const;	
	double getWidth()const;
	double getLength() const;
	Circle2D getCircle();
	QColor getColor()const;
	//geo::RotRect2D get_Rects(int count);
	std::vector<geo::RotRect2D> getRects()const;


protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
	QRectF boundingRect() const override;

private:

	void drawRect(QPainter * painter, geo::RotRect2D &m_ro_Rect);
	void generateRect();
	void drawTriangle(QPainter * painter, geo::RotRect2D &m_rotRect);
	int isInResizeArea(const QPointF &pos);

	Point2D m_firstPoint;
	Point2D m_secondPoint;
	Point2D m_centerPoint;
	Circle2D m_circle;
	Line2D m_firstLine;
	Line2D m_secondLine;
	Line2D m_horizontalLine;
	bool m_direction;
	std::vector<geo::RotRect2D> m_rotRects;

	QColor m_color;
	QPen m_pen;
	qreal m_penWidth;
	double m_length;
	double m_width;
	int isResizing;
	int m_count;

};
