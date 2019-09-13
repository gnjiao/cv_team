#pragma once

#include <QGraphicsItem>
#include <QRectF>
#include <QPointF>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QStyleOptionGraphicsItem>

#include "geo\Rect2D.h"


const qreal tool_cResizePos[] = { 9, 6, 3 };

class SearchTool : public QGraphicsItem
{
	
public:
	SearchTool(int x, int y, int width, int height);
	~SearchTool();

	void setColor(QColor);

	geo::Rect2D getRect();

protected:
	QRectF boundingRect() const;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private:

	bool IsInResizeArea(const QPointF& pos);


	QRectF	m_size;

	bool m_bIsResizing;

	QColor m_color = QColor(Qt::green);


};
