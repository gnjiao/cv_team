#include "LTbutton.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QAction>
#include "LT.h"
LTButton::LTButton(QWidget *parent)
	: QToolButton(parent)
{
	setToolButtonStyle(Qt::ToolButtonIconOnly);
	// setIconSize(QSize(18,18));
	setAutoRaise(true);
	m_bMouseHover = false;
	m_bMousePress = false;
	m_hasBorder = true;
	m_bRound = false;
	m_hoverColor = QColor(255, 255, 255, 50);
	m_pressColor = QColor(0, 0, 0, 50);
	m_textColor = QColor(255, 255, 255);
	m_normalColor = QColor();
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	setColor(Qt::white);
}
LTButton::~LTButton()
{
}
void LTButton::select(bool selected)
{
	m_bMousePress = selected;
	if (!selected) {
		m_bMouseHover = false;
	}
	update();
}
void LTButton::setTextColor(const QColor &color)
{
	m_textColor = color;
}
void LTButton::setColor(const QColor &color)
{
	if (m_hasBorder) {
		setStyleSheet(QString("QToolButton::menu-indicator { image: None; }"
			"QToolButton{background-color: transparent;color: #%1;}")
			.arg(LT::colorToArgb(color)));
	}
	else {
		setStyleSheet(QString("QToolButton::menu-indicator { image: None; }"
			"QToolButton{background-color: transparent;color: #%1;border:none;}")
			.arg(LT::colorToArgb(color)));
	}
}
void LTButton::setNormalColor(const QColor &color)
{
	m_normalColor = color;
}
void LTButton::setHasMenu(bool has)
{
	m_hasMenu = has;
}
void LTButton::setHasBorder(bool has)
{
	m_hasBorder = has;
	setColor(m_textColor);
	update();
}
void LTButton::setRound(bool round)
{
	m_bRound = round;
}
void LTButton::enterEvent(QEvent *event)
{
	Q_UNUSED(event);
	if (!isEnabled()) {
		return;
	}
	m_bMouseHover = true;
}
void LTButton::leaveEvent(QEvent *event)
{
	Q_UNUSED(event);
	m_bMouseHover = false;
}
void LTButton::mousePressEvent(QMouseEvent *event)
{
	if (!isEnabled()) {
		return;
	}
	if (event->button() == Qt::LeftButton) {
		m_bMousePress = true;
		if (m_hasMenu || actionHasMenu()) {
			emit menuTriggered(event);
		}
		update();
	}
}
void LTButton::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		m_bMousePress = false;
		update();
		// don't emit when move out
		if (this->rect().contains(event->pos())) {
			emit clicked();
		}
	}
}
void LTButton::paintEvent(QPaintEvent *event)
{
	if (m_bMousePress) {
		painterInfo(m_pressColor);
	}
	else if (m_bMouseHover) {
		painterInfo(m_hoverColor);
	}
	else {
		if (m_normalColor.isValid()) {
			painterInfo(m_normalColor);
		}
	}
	QToolButton::paintEvent(event);
}
void LTButton::painterInfo(QColor &color)
{
	QPainter painter(this);
	QPen pen(Qt::NoBrush, 1);
	painter.setPen(pen);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setBrush(color);
	if (m_bRound) {
		int r = qMin(width(), height()) / 2;
		painter.drawEllipse(QPointF(width() / 2, height() / 2), r, r);
	}
	else {
		painter.drawRect(rect());
	}
}
bool LTButton::actionHasMenu()
{
	QAction *action = this->defaultAction();
	if (action && action->menu()) {
		return true;
	}
	return false;
}