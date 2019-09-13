#pragma once
#include <QGraphicsItem>
#include "geo\Point2D.h"
#include "geo\Line2D.h"
#include <QPainter>

class LineSetItem :
	public QGraphicsItem
{
public:
	enum LineType
	{
		Line,
		dotLine,
		dotdotLine,
	};
	LineSetItem();
	LineSetItem(geo::Line2D lineSet);
	LineSetItem(std::vector<geo::Line2D> lineSet);
	~LineSetItem();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
	QRectF boundingRect() const override;
	void setLine(geo::Line2D);
	void setLines(std::vector<geo::Line2D> lines);
	void setColor(QColor);
	void setLineWidth(int width);
	void setLineType(LineType);
private:
	std::vector<geo::Line2D> m_lineSet;
	QColor m_color = QColor(Qt::blue);
	int m_LineWidth = 1;
	LineType m_LineType = LineType::Line;
};
