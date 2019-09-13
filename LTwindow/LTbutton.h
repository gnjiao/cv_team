#ifndef LTBUTTON_H
#define LTBUTTON_H
#include <QToolButton>

class LTButtonPrivate;
class LTButton : public QToolButton
{
	Q_OBJECT
public:
	explicit LTButton(QWidget *parent = nullptr);
	~LTButton();
	void select(bool selected);
	void setHoverColor(const QColor &color) { m_hoverColor = color; }
	QColor hoverColor() const { return m_hoverColor; }
	void setPressColor(const QColor &color) { m_pressColor = color; }
	QColor pressColor() const { return m_pressColor; }
	void setTextColor(const QColor &color);
	void setColor(const QColor &color);
	void setNormalColor(const QColor &color);
	void setHasMenu(bool has);
	void setHasBorder(bool has);
	void setRound(bool round);
private:
	void painterInfo(QColor &color);
	bool actionHasMenu();
signals:
	void menuTriggered(QMouseEvent *e);
protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
public:
	bool m_hasMenu = false;
private:
	bool m_bMouseHover;
	bool m_bMousePress;
	bool m_hasBorder;
	bool m_bRound;
	QColor m_hoverColor;
	QColor m_pressColor;
	QColor m_textColor;
	QColor m_normalColor;
};
#endif // LTBUTTON_H