#pragma once

#include <QGraphicsItem>
#include <QRectF>
#include <QPointF>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QStyleOptionGraphicsItem>

#include "geo\Rect2D.h"

const qreal roi_cResizePos[] = { 9, 6, 3 };

class ROIItem : public QGraphicsItem
{
	
public:
	ROIItem(int x, int y, int width, int height);
	~ROIItem();

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

	QColor m_color = QColor(Qt::blue);



};
