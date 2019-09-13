#include "TextItem.h"

TextItem::TextItem(int x, int y, int size,std::string text) : 
	m_size(0, 0, text.size() * 7 * size / 10, size + 4), m_bIsResizing(false),m_text(text)
{
	setAcceptHoverEvents(true);
	this->setPos(x, y);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	font_size = size;
	m_size.setWidth(m_text.size() * 7 * font_size / 10);
	m_size.setHeight(font_size + 6);
}

TextItem::~TextItem()
{
}

void TextItem::setColor(QColor color)
{
	this->m_color = color;
	prepareGeometryChange();
	update();
}

geo::Rect2D TextItem::getRect()
{
	QPointF PointTL = mapToScene(m_size.topLeft());

	QPointF PointBR = mapToScene(m_size.bottomRight());

	QRectF RoiInScene = QRectF(PointTL, PointBR);

	return geo::Rect2D(geo::Point2D(RoiInScene.left(), RoiInScene.top()), geo::Point2D(RoiInScene.right(), RoiInScene.bottom()));
}

void TextItem::setText(std::string text)
{
	m_text = text;
	prepareGeometryChange();
	update();
}

void TextItem::setSize(int size)
{
	font_size = size;
	m_size.setWidth(m_text.size() * 7 * font_size / 10);
	m_size.setHeight(font_size + 6);
	prepareGeometryChange();
	update();
}

QRectF TextItem::boundingRect() const
{
	return  QRectF(0, 0, m_size.width()+5, font_size + 10);
}

void TextItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	QFont font;
	font.setPointSizeF(font_size);
	if (option->state & QStyle::State_Selected)
	{
		// 重点

		if (option->state & QStyle::State_HasFocus)
		{
			painter->setRenderHint(QPainter::Antialiasing, true);

			painter->setPen(m_color);

		}

		else
		{
			painter->setPen(m_color);
		}

		// draw frame
		painter->setPen(Qt::DashDotLine); //没起作用
		painter->setPen(m_color);
		painter->drawRect(QRectF(0, 0, m_size.width(), m_size.height()));
		painter->setFont(font);
		painter->setPen(Qt::SolidLine);
		painter->setPen(m_color);
		painter->drawText(0, font_size+3, QString::fromStdString(m_text));
		// resize line
		qreal w = m_size.width();
		qreal h = m_size.height();
		painter->setPen(m_color);

		painter->setRenderHint(QPainter::Antialiasing, false);
	}
	else
	{
		painter->setPen(m_color);
		painter->setFont(font);
		//painter->drawRect(QRectF(0, 0, m_text.size() * 7 * font_size / 10, font_size + 2));
		painter->drawText(0, font_size, QString::fromStdString(m_text));
	}

}

void TextItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
	if (m_bIsResizing || (IsInResizeArea(event->pos()) && isSelected()))
		setCursor(Qt::SizeVerCursor);
	else
		setCursor(QPixmap(":/CV_TEAM/icons/text.png"));

	QGraphicsItem::hoverMoveEvent(event);
}

void TextItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	if (m_bIsResizing)
	{
		setCursor(Qt::SizeVerCursor);
		qreal w = event->pos().x();
		qreal h = event->pos().y();
		if (w > 5)
			m_size.setWidth(m_text.size() * 7 * font_size / 10);
		if (h > 5 && h < 50)
		{
			this->font_size = h;	
			m_size.setHeight(font_size + 6);
		}
		
		prepareGeometryChange();
	}
	else
	{
		QGraphicsItem::mouseMoveEvent(event);
	}
}

void TextItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{

	static qreal z = 0.0;
	setZValue(z += 1.0);

	if (event->button() == Qt::LeftButton && IsInResizeArea(event->pos()))
	{
		m_bIsResizing = true;
		setCursor(Qt::SizeVerCursor);
	}

	else
		QGraphicsItem::mousePressEvent(event);

}

void TextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	if (event->button() == Qt::LeftButton && m_bIsResizing)
		m_bIsResizing = false;
	else
		QGraphicsItem::mouseReleaseEvent(event);

}

bool TextItem::IsInResizeArea(const QPointF & pos)
{
	return (pos.y() > m_size.height() - 5) && (pos.y() < m_size.height() + 5);
}
