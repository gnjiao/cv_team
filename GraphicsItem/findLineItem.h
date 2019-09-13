#pragma once

#include "geo\rotrect2d.h"

#include "geo\RotRect2D.h"

#include "geo\Rect2D.h"

#include "geo\Point2D.h"

#include "geo\Line2D.h"

#include "geo\AffineTrans.h"

#include "sstream"

#include <Eigen/Dense>

#include <QGraphicsItem>

#include <QPainter>

#include <QGraphicsSceneMouseEvent>

#include <QCheckBox>

#include <QPainterPath>

#include <QPolygonF>

using namespace geo;

class findLineItem : public QGraphicsItem

{

public:

	findLineItem();

	~findLineItem();



	void setAimArea(geo::RotRect2D &rect); 

	void setAimArea(geo::Line2D &aline); 

	void setAimArea(geo::RotRect2D &rect, geo::AffineTrans &trans); 

	void setWidth(double width); 

	void setCount(int count); 

	void setColor(QColor color) { m_color = color; update(); } 

	void setSensitive(double sens) { m_sensitive = sens; } 

	void setStartPoint(Point2D start);

	void setEndPoint(Point2D end);

	void setLine(Line2D line);

	std::vector<RotRect2D> get_Rects(); 

	Line2D getLine(); 

	RotRect2D get_AimRect(); 

	

protected:

	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

	void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

	QRectF boundingRect() const override;

	QPainterPath shape()const override;

private:
	Line2D m_line;

	RotRect2D m_rect; 

	int isResizing; 

	double m_sensitive = 5.0; 

	double m_clickmark = 0; 

	geo::Point2D m_clickpos;

	std::vector<RotRect2D> m_rotRects; 

	double m_clp_width = 0; 

	int m_count = 0; 

	QColor m_color = QColor(Qt::green); 
									 
	void generateRect();

	float cal_pointTOline(QPointF pos, geo::Line2D &line);

	bool isInArea(QPointF pos, QPointF position, float range);

	void draw_rect(QPainter * painter, RotRect2D &m_ro_Rect);

	void draw_triangle(QPainter * painter, RotRect2D &m_rotRect);

};





