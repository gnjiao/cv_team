#pragma once

#include <QGraphicsItem>
#include <QRectF>
#include <QPointF>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

#include "geo\Rect2D.h"

const qreal eraser_cResizePos[] = { 6, 3 };
class EraserItem : public QWidget, public QGraphicsItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)

public:
	EraserItem(int x,int y,int width,int height);
	~EraserItem();

	void setColor(QColor);

	geo::Rect2D getRect();

protected:
	QRectF boundingRect() const;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

signals:
	void EraserChanged(int width,int height);


private:

	bool IsInResizeArea(const QPointF& pos);


	QRectF	m_size;

	bool m_bIsResizing;

	QColor m_color = QColor(Qt::red);


};
