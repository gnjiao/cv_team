#include "LTwindow.h"
#include "LTbar.h"
#include "screenhelper.h"
#include "LT_p.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QStatusBar>
#include <QSettings>
#include <QPainter>
class LTwindowPrivate
{
public:
	LTwindowPrivate();
	QStatusBar *statusBar;
	LTBar *LTbar;
};
LTwindowPrivate::LTwindowPrivate()
{
	LTbar = nullptr;
	statusBar = nullptr;
}
LTwindow::LTwindow(QWidget *parent)
	: QMainWindow(parent), d(new LTwindowPrivate())
{
	QMainWindow::setWindowFlags(Qt::FramelessWindowHint
		| Qt::WindowSystemMenuHint
		| Qt::WindowMinimizeButtonHint
		| Qt::WindowMaximizeButtonHint
		| Qt::Window
		);
	d->LTbar = new LTBar(this);
	d->statusBar = new QStatusBar(this);
	this->setStatusBar(d->statusBar);
	this->statusBar()->setSizeGripEnabled(true);
	connect(d->LTbar, SIGNAL(maximizationChanged(bool)), this, SIGNAL(resizable(bool)));
	setMenuWidget(d->LTbar);
	setMouseTracking(true);
	QRect geom = ScreenHelper::normalRect();
	resize(geom.width(), geom.height());
	raise();
	activateWindow();
}
LTBar * LTwindow::LtBar() const
{
	return d->LTbar;
}
void LTwindow::setFixedSize(const QSize &s)
{
	setFixedSize(s.width(), s.height());
}
void LTwindow::setFixedSize(int w, int h)
{
	d->LTbar->setWidgetResizable(false);
	QWidget::setFixedSize(w, h);
}
void LTwindow::setFixedWidth(int w)
{
	setFixedSize(w, this->height());
}
void LTwindow::setFixedHeight(int h)
{
	setFixedSize(this->width(), h);
}
void LTwindow::setWindowFlags(Qt::WindowFlags type)
{
	QMainWindow::setWindowFlags(type);
	d->LTbar->updateWidgetFlags();
}
void LTwindow::setTitleBackgrondColor(QColor color)
{
	d->LTbar->setBackgroundColor(color);
}
void LTwindow::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QSettings settings(LTPrivate::g_settingsOrganization,
		LTPrivate::g_settingsApplication);
	QString skinName = settings.value("skin").toString();
	if (skinName.isEmpty()) {
		skinName = QString(":/skin/default");
	}
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::gray);
	painter.drawPixmap(QRect(0, 0, this->width(), this->height()),
		QPixmap(skinName));
}