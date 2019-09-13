#include <QToolBar>
#ifndef QUICKACCESSBAR_H
#define QUICKACCESSBAR_H
#include <QMenu>
class QuickAccessBarPrivate;
class QuickAccessBar : public QToolBar
{
	Q_OBJECT
public:
	explicit QuickAccessBar(QWidget *parent = nullptr);
public:
	QAction* actionCustomizeButton() const;
	void setActionVisible(QAction* action, bool visible);
	bool isActionVisible(QAction* action) const;
	int visibleCount() const;
	void setHoverColor(const QColor &color);
	void setPressColor(const QColor &color);
	void setTextColor(const QColor &color);
protected:
	virtual void actionEvent(QActionEvent*);
private:
	QuickAccessBarPrivate *d;
};
#endif // QUICKACCESSBAR_H