#ifndef LTBAR_H
#define LTBAR_H
#include <QWidget>
class QMenuBar;
class QuickAccessBar;
class LTBarPrivate;
class QHBoxLayout;
class LTBar : public QWidget
{
	Q_OBJECT
public:
	enum AdditionalControlPosition {
		TitlePosition, MenuPosition
	};
	enum LTStyle {
		WindowStyle, ClassicStyle, MergedStyle, DialogStyle
	};
	explicit LTBar(QWidget *parent);
	~LTBar();
	QMenuBar* menuBar() const;
	void showMenuBar(bool show = false);
	bool isMenuBarVisible() const;
	QuickAccessBar* quickAccessBar() const;
	QHBoxLayout *menuLayout();
	void showQuickAccess(bool show = true);
	bool isQuickAccessVisible() const;
	void setHoverColor(const QColor &color);
	void setPressColor(const QColor &color);
	void setTextColor(const QColor &color);
	void setBackgroundColor(const QColor &color);
	void addAdditionalControl(QAction *action, AdditionalControlPosition position);
	void addAdditionalControl(QWidget *widget, AdditionalControlPosition position);
	void setApplicationWidget(const QString &label, QWidget *widget);
	void setApplicationButtonBkColor(const QColor &color);
	void setWidgetResizable(bool resizable);
	void setTitleBarHeight(int height);
	void setLTStyle(LTStyle style);
	void updateWidgetFlags();
signals:
	void maximizationChanged(bool maximized);
protected:
	virtual bool eventFilter(QObject* object, QEvent* event);
private:
	LTBarPrivate *d;
};
#endif // LTBAR_H