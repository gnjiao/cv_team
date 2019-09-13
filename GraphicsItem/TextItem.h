#pragma once

#include <QGraphicsItem>
#include <QRectF>
#include <QPointF>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QStyleOptionGraphicsItem>

#include "geo\Rect2D.h"
#include <string>
const qreal text_cResizePos[] = { 9, 6, 3 };

class TextItem : public QGraphicsItem
{

public:
	TextItem(int x, int y, int size = 12, std::string text = "null string");
	~TextItem();

	void setColor(QColor);

	geo::Rect2D getRect();

	void setText(std::string);

	void setSize(int size);


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

	int font_size;

	std::string m_text;

};
