#pragma once
#include <QGraphicsItem>
#include <QPen>
#include <QPainter>
#include <QColor>
class FrontSightItem :public QGraphicsItem
{
public:
	FrontSightItem();
	~FrontSightItem();
	void setColor(QColor clr);
	void setMarkType(int ty);
	void setMarkCenter(double length);
	void setImageSize(QRectF size);
	void setRatioXPixPerMM(double ratioX);
	double getRatioXPixPerMM() { return m_rPixPerMM; }
protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
	QRectF boundingRect() const override;
private:
	void updateLine();
	void updateCenterSight();
	double m_rPixPerMM = 10;
	double m_markDNmm = 10;
	int m_centerType = 0;
	QPen m_pen;
	QColor m_color = QColor(0, 255, 255);
	QLineF d_lineH = QLineF(0, 0, 0, 0);
	QLineF d_lineV = QLineF(0, 0, 0, 0);
	QLineF d_lineA = QLineF(0, 0, 0, 0);
	QLineF d_lineB = QLineF(0, 0, 0, 0);
	QRectF d_rectC = QRectF(0, 0, 0, 0);
	QRectF d_rectPic = QRectF(0, 0, 0, 0);
	std::vector<QLineF> d_lineS;
};