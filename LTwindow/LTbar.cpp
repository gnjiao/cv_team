#include "LTbar.h"
#include "LTbutton.h"
#include "quickaccessbar.h"
#include "cursor.h"
#include "screenhelper.h"
#include "LT.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMouseEvent>
#include <QEvent>
#include <QMainWindow>
#include <QApplication>
#define TITLE_BAR_HEIGHT 25
#define MENU_BAR_HEIGHT 25
static QColor s_backgroundColor = QColor(0, 100, 200, 100);
//static QColor s_backgroundColor = QColor(255, 0, 0, 50);
class LTBarPrivate : public QWidget
{
	Q_OBJECT
public:
	explicit LTBarPrivate(QWidget *parent = nullptr);
	~LTBarPrivate();
	void showQuickAccessBar(bool show);
	void showMenuBar(bool show);
	void setWidgetResizable(bool resizable);
	void setWidgetMovable(bool movable);
	void setWidgetMaximizable(bool maximizable);
	void addAdditionalControl(QAction *action, LTBar::AdditionalControlPosition position);
	void addAdditionalControl(QWidget *widget, LTBar::AdditionalControlPosition position);
	void init();
	void setLTStyle(LTBar::LTStyle style);
	void createTitleWidget();
	void createMenuWidget();
	void createWindowButtons();
	void updateMenuColor();
	void updateWindowButtons();
public:
	void registerWidget(QWidget *widget);
private:
	void restoreWidget(QWidget *pWidget);
	void maximizeWidget(QWidget *pWidget);
public:
	// window
	bool windowTitleChange(QObject *obj);
	bool windowIconChange(QObject *obj);
	void windowSizeChange(QObject *obj);
	void windowStateChange(QObject *obj);
	// mouse event
	void handleWidgetMouseEvent(QObject *obj, QEvent *event);
	// 
	void updateCursorShape(const QPoint &gMousePos);
	//
	void resizeWidget(const QPoint &gMousePos);
	//
	void moveWidget(const QPoint &gMousePos);
	//
	void handleMousePressEvent(QMouseEvent *event);
	//
	void handleMouseReleaseEvent(QMouseEvent *event);
	//
	void handleMouseMoveEvent(QMouseEvent *event);
	//
	void handleLeaveEvent(QEvent *event);
	//
	void handleHoverMoveEvent(QHoverEvent *event);
	QPoint calcDragPoint(QWidget *pWindow, QMouseEvent *event) const;
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
signals:
	void windowResizable(bool resizable);
	void windowMovable(bool movable);
public:
	LTBar *q;
	QHBoxLayout *m_logoLayout;
	// title widget
	QWidget *m_titleWidget;
	LTButton *m_logoButton;
	QuickAccessBar *m_quickAccessBar;
	QSpacerItem *m_leftSpacerItem;
	QSpacerItem *m_rightSpacerItem;
	QLabel *m_titleLabel;
	QHBoxLayout *m_titleAdditionalControlArea;
	LTButton *m_maximizeButton;
	LTButton *m_minimizeButton;
	LTButton *m_closeButton;
	QWidget *m_systemGroup;
	bool m_bQuickAccessVisible;
	// menu widget
	QWidget *m_menuWidget;
	LTButton *m_applicationButton;
	QHBoxLayout *m_appButtonLayout;
	QMenuBar *m_menuBar;
	QHBoxLayout *m_menuAdditionalControlArea;
	QHBoxLayout *m_menuBarArea;
	QSpacerItem *m_middleSpacerItem;
	bool m_bMenuBarVisible;
	// screen
	int m_currentScreen;
	// main window
	QWidget *m_mainWidget;
	QPoint m_dragPos;
	Cursor m_pressCursor;
	Cursor m_moveCursor;
	bool m_bEdgePressed;
	bool m_bCursorShapeChanged;
	Qt::WindowFlags m_windowFlags;
	bool m_bWidgetResizable;
	bool m_bWidgetMovable;
	///
	QRect m_normalRect;
	QPoint m_movePoint;
	QPoint m_dragPoint;
	bool m_bLeftButtonPressed;
	bool m_isMaximized;
	bool m_isMinimized;
	bool m_bWidgetMaximizable;
	LTButton *m_menuButton;
	QMenu *m_menu;
	QAction *m_maximizeAction;
	QAction *m_minimizeAction;
	QAction *m_restoreAction;
	QAction *m_closeAction;
	QList<LTButton *> m_additionalButtons;
	QWidget *m_applicationWidget;
	QColor m_menuHoverColor;
	QColor m_menuTextColor;
	QColor m_menuPressColor;
	// added on 2018-3-11 19:31:09
	int m_titleBarHeight;
	int m_menuBarHeight;
	// added on 2018-4-30 13:39:32
	LTBar::LTStyle m_style;
	private slots:
	void slotWindowButtonClicked();
	void aboutToShowSystemMenu();
	void aboutToHideSystemMenu();
	void systemMenuTriggered(QMouseEvent *e);
	void aboutToShowMenu();
	void aboutToHideMenu();
	void menuTriggered(QMouseEvent *e);
	void applicationMenuTriggered(QMouseEvent *e);
};
LTBarPrivate::LTBarPrivate(QWidget *parent)
	: QWidget(parent)
{
	m_isMaximized = false;
	m_isMinimized = false;
	m_bLeftButtonPressed = false;
	m_currentScreen = 0;
	m_bQuickAccessVisible = false;
	m_bMenuBarVisible = false;
	m_bEdgePressed = false;
	m_bCursorShapeChanged = false;
	m_bWidgetMovable = true;
	m_bWidgetResizable = true;
	m_bWidgetMaximizable = true;
	q = nullptr;
	m_logoLayout = nullptr;
	m_titleWidget = nullptr;
	m_logoButton = nullptr;
	m_quickAccessBar = nullptr;
	m_leftSpacerItem = nullptr;
	m_titleLabel = nullptr;
	m_titleAdditionalControlArea = nullptr;
	m_maximizeButton = nullptr;
	m_minimizeButton = nullptr;
	m_closeButton = nullptr;
	m_systemGroup = nullptr;
	m_menuWidget = nullptr;
	m_applicationButton = nullptr;
	m_appButtonLayout = nullptr;
	m_menuBar = nullptr;
	m_menuAdditionalControlArea = nullptr;
	m_menuBarArea = nullptr;
	m_middleSpacerItem = nullptr;
	m_mainWidget = nullptr;
	m_menuButton = nullptr;
	m_additionalButtons.clear();
	m_applicationWidget = nullptr;
	m_menuTextColor = QColor(250, 250, 250);
	m_menuHoverColor = QColor(255, 255, 255, 50);
	m_menuPressColor = QColor(255, 255, 255, 50);
	m_titleBarHeight = TITLE_BAR_HEIGHT;
	m_menuBarHeight = MENU_BAR_HEIGHT;
	m_style = LTBar::WindowStyle;
	m_normalRect = ScreenHelper::normalRect();
}
LTBarPrivate::~LTBarPrivate()
{
}
void LTBarPrivate::showQuickAccessBar(bool show)
{
	if (m_bQuickAccessVisible == show) {
		return;
	}
	m_bQuickAccessVisible = show;
	if (show) {
		m_quickAccessBar->show();
		m_leftSpacerItem->changeSize(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	}
	else {
		m_quickAccessBar->hide();
		m_leftSpacerItem->changeSize(0, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);
	}
}
void LTBarPrivate::showMenuBar(bool show)
{
	if (m_bMenuBarVisible == show) {
		return;
	}
	m_bMenuBarVisible = show;
	m_menuWidget->setVisible(show);
	if (show) {
		setFixedHeight(m_titleBarHeight + m_menuBarHeight);
	}
	else {
		setFixedHeight(m_titleBarHeight);
	}
}
void LTBarPrivate::setWidgetResizable(bool resizable)
{
	m_bWidgetResizable = resizable;
}
void LTBarPrivate::setWidgetMovable(bool movable)
{
	m_bWidgetMovable = movable;
}
void LTBarPrivate::setWidgetMaximizable(bool maximizable)
{
	m_bWidgetMaximizable = maximizable;
}
void LTBarPrivate::addAdditionalControl(QAction *action, LTBar::AdditionalControlPosition position)
{
	LTButton *button = new LTButton();
	button->setText(action->text());
	button->setToolTip(action->toolTip());
	button->setIcon(action->icon());
	button->setDefaultAction(action);
	connect(button, SIGNAL(menuTriggered(QMouseEvent *)), this, SLOT(menuTriggered(QMouseEvent *)));
	connect(button, SIGNAL(clicked(bool)), action, SIGNAL(triggered(bool)));
	if (position == LTBar::TitlePosition) {
		m_titleAdditionalControlArea->addWidget(button);
	}
	else if (position == LTBar::MenuPosition) {
		m_menuAdditionalControlArea->addWidget(button);
	}
	m_additionalButtons.append(button);
}
void LTBarPrivate::addAdditionalControl(QWidget *widget, LTBar::AdditionalControlPosition position)
{
	if (position == LTBar::TitlePosition) {
		m_titleAdditionalControlArea->addWidget(widget);
	}
	else if (position == LTBar::MenuPosition) {
		m_menuAdditionalControlArea->addWidget(widget);
	}
}
void LTBarPrivate::init()
{
	m_logoLayout = new QHBoxLayout();
	m_logoLayout->setContentsMargins(0, 0, 0, 0);
	m_logoLayout->setSpacing(0);
	createTitleWidget();
	createMenuWidget();
	QVBoxLayout *rightLayout = new QVBoxLayout();
	rightLayout->setMargin(0);
	rightLayout->setSpacing(0);
	rightLayout->addWidget(m_titleWidget);
	rightLayout->addWidget(m_menuWidget);
	QHBoxLayout *layout = new QHBoxLayout();
	layout->setContentsMargins(1, 1, 1, 0);
	layout->setSpacing(0);
	layout->addLayout(m_logoLayout);
	layout->addLayout(rightLayout);
	setLayout(layout);
	updateWindowButtons();
}
void LTBarPrivate::setLTStyle(LTBar::LTStyle style)
{
	if (m_style == style) {
		return;
	}
	m_style = style;
	// clear layout
	m_logoLayout->removeItem(m_appButtonLayout);
	QHBoxLayout *titleLayout = qobject_cast<QHBoxLayout *>(m_titleWidget->layout());
	titleLayout->removeWidget(m_logoButton);
	titleLayout->removeWidget(m_quickAccessBar);
	titleLayout->removeItem(m_appButtonLayout);
	titleLayout->removeItem(m_menuBarArea);
	titleLayout->removeItem(m_leftSpacerItem);
	titleLayout->removeWidget(m_titleLabel);
	titleLayout->removeItem(m_rightSpacerItem);
	titleLayout->removeItem(m_titleAdditionalControlArea);
	titleLayout->removeItem(m_menuAdditionalControlArea);
	titleLayout->removeWidget(m_systemGroup);
	QHBoxLayout *menuLayout = qobject_cast<QHBoxLayout *>(m_menuWidget->layout());
	menuLayout->removeItem(m_appButtonLayout);
	menuLayout->removeItem(m_menuBarArea);
	menuLayout->removeItem(m_middleSpacerItem);
	menuLayout->removeItem(m_menuAdditionalControlArea);
	m_applicationButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
	m_applicationButton->setIconSize(QSize(18, 18));
	m_applicationButton->setRound(false);
	m_appButtonLayout->setContentsMargins(0, 0, 0, 0);
	m_logoButton->setVisible(false);
	showMenuBar(true);
	// set style
	if (style == LTBar::WindowStyle) {
		m_logoButton->setVisible(true);
		titleLayout->addWidget(m_logoButton/*,0,Qt::AlignVCenter*/);
		titleLayout->addWidget(m_quickAccessBar);
		titleLayout->addItem(m_leftSpacerItem);
		titleLayout->addWidget(m_titleLabel);
		titleLayout->addItem(m_rightSpacerItem);
		titleLayout->addItem(m_titleAdditionalControlArea);
		titleLayout->addWidget(m_systemGroup);
		m_appButtonLayout->setContentsMargins(0, 2, 0, 2);
		menuLayout->addLayout(m_appButtonLayout);
		menuLayout->addLayout(m_menuBarArea);
		menuLayout->addItem(m_middleSpacerItem);
		menuLayout->addLayout(m_menuAdditionalControlArea);
	}
	else if (style == LTBar::DialogStyle) {
		m_logoButton->setVisible(true);
		titleLayout->addWidget(m_logoButton/*,0,Qt::AlignVCenter*/);
		titleLayout->addWidget(m_titleLabel);
		titleLayout->addItem(m_rightSpacerItem);
		titleLayout->addLayout(m_titleAdditionalControlArea);
		titleLayout->addWidget(m_systemGroup);
		showMenuBar(false);
	}
	else if (style == LTBar::ClassicStyle) {
		m_applicationButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
		m_applicationButton->setIconSize(QSize(48, 48));
		m_applicationButton->setRound(true);
		m_logoLayout->addLayout(m_appButtonLayout);
		titleLayout->addWidget(m_quickAccessBar);
		titleLayout->addItem(m_leftSpacerItem);
		titleLayout->addWidget(m_titleLabel);
		titleLayout->addItem(m_rightSpacerItem);
		titleLayout->addLayout(m_titleAdditionalControlArea);
		titleLayout->addWidget(m_systemGroup);
		menuLayout->addLayout(m_menuBarArea);
		menuLayout->addItem(m_middleSpacerItem);
		menuLayout->addLayout(m_menuAdditionalControlArea);
	}
	else if (style == LTBar::MergedStyle) {
		m_applicationButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
		titleLayout->addLayout(m_appButtonLayout);
		titleLayout->addWidget(m_quickAccessBar);
		titleLayout->addLayout(m_menuBarArea);
		titleLayout->addItem(m_leftSpacerItem);
		titleLayout->addWidget(m_titleLabel);
		titleLayout->addItem(m_rightSpacerItem);
		titleLayout->addLayout(m_menuAdditionalControlArea);
		titleLayout->addLayout(m_titleAdditionalControlArea);
		titleLayout->addWidget(m_systemGroup);
		showMenuBar(false);
	}
}
void LTBarPrivate::createTitleWidget()
{
	m_titleWidget = new QWidget();
	m_titleWidget->setFixedHeight(m_titleBarHeight);
	createWindowButtons();
	m_leftSpacerItem = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);
	m_rightSpacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	m_quickAccessBar = new QuickAccessBar();
	m_quickAccessBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	m_quickAccessBar->hide();
	m_titleAdditionalControlArea = new QHBoxLayout();
	m_titleAdditionalControlArea->setMargin(0);
	m_titleAdditionalControlArea->setSpacing(0);
	m_systemGroup = new QWidget();
	QHBoxLayout *systemButtonLayout = new QHBoxLayout();
	systemButtonLayout->setMargin(0);
	systemButtonLayout->setSpacing(0);
	systemButtonLayout->addWidget(m_minimizeButton/*, 0, Qt::AlignTop*/);
	systemButtonLayout->addWidget(m_maximizeButton/*, 0, Qt::AlignTop*/);
	systemButtonLayout->addWidget(m_closeButton/*, 0, Qt::AlignTop*/);
	m_systemGroup->setLayout(systemButtonLayout);
	QHBoxLayout *layout = new QHBoxLayout();
	layout->setContentsMargins(1, 0, 0, 0);
	layout->setSpacing(0);
	layout->addWidget(m_logoButton/*,0,Qt::AlignVCenter*/);
	layout->addWidget(m_quickAccessBar);
	layout->addItem(m_leftSpacerItem);
	layout->addWidget(m_titleLabel);
	layout->addItem(m_rightSpacerItem);
	layout->addLayout(m_titleAdditionalControlArea);
	layout->addWidget(m_systemGroup);
	m_titleWidget->setLayout(layout);
}
void LTBarPrivate::createMenuWidget()
{
	m_menuWidget = new QWidget();
	m_menuWidget->setFixedHeight(m_menuBarHeight);
	m_menuWidget->hide();
	m_applicationButton = new LTButton();
	m_applicationButton->setHasMenu(true);
	//m_applicationButton->setText(tr("Application"));
	m_applicationButton->setText(tr(""));
	m_applicationButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
	m_applicationButton->setNormalColor(QColor(240, 130, 0));
	connect(m_applicationButton, SIGNAL(menuTriggered(QMouseEvent *)), this, SLOT(applicationMenuTriggered(QMouseEvent *)));
	m_applicationWidget = new QWidget(this);
	QVBoxLayout *applicationLayout = new QVBoxLayout();
	applicationLayout->setMargin(0);
	applicationLayout->setSpacing(0);
	m_applicationWidget->setLayout(applicationLayout);
	m_applicationWidget->setWindowFlags(Qt::Popup);
	m_menuBarArea = new QHBoxLayout();
	m_menuBarArea->setMargin(0);
	m_menuBarArea->setSpacing(0);
	m_middleSpacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	m_menuAdditionalControlArea = new QHBoxLayout();
	m_menuAdditionalControlArea->setMargin(0);
	m_menuAdditionalControlArea->setSpacing(0);
	m_appButtonLayout = new QHBoxLayout();
	m_appButtonLayout->setContentsMargins(0, 2, 0, 2);
	m_appButtonLayout->setSpacing(0);
	m_appButtonLayout->addWidget(m_applicationButton);
	QHBoxLayout *layout = new QHBoxLayout();
	layout->setContentsMargins(1, 0, 0, 0);
	layout->setSpacing(0);
	layout->addLayout(m_appButtonLayout);
	layout->addLayout(m_menuBarArea);
	layout->addItem(m_middleSpacerItem);
	layout->addLayout(m_menuAdditionalControlArea);
	m_menuWidget->setLayout(layout);
}
void LTBarPrivate::createWindowButtons()
{
	m_logoButton = new LTButton();
	m_logoButton->setHasMenu(true);
	m_logoButton->setIcon(QIcon(":/main/logo"));
	connect(m_logoButton, SIGNAL(menuTriggered(QMouseEvent *)), this, SLOT(systemMenuTriggered(QMouseEvent *)));
	m_titleLabel = new QLabel();
	QFont font("Microsoft Yahei", 10);
	m_titleLabel->setFont(font);
	m_titleLabel->setAlignment(Qt::AlignCenter);
	m_titleLabel->setStyleSheet("color:white;");
	m_minimizeButton = new LTButton();
	m_minimizeButton->setIcon(QIcon(":/main/min"));
	m_minimizeButton->setToolTip(tr("minimize"));
	// m_minimizeButton->setIconSize(QSize(22,22));
	connect(m_minimizeButton, SIGNAL(clicked(bool)), this, SLOT(slotWindowButtonClicked()));
	m_maximizeButton = new LTButton();
	m_maximizeButton->setIcon(QIcon(":/main/max"));
	m_maximizeButton->setToolTip(tr("maximize"));
	// m_maximizeButton->setIconSize(QSize(22,22));
	connect(m_maximizeButton, SIGNAL(clicked(bool)), this, SLOT(slotWindowButtonClicked()));
	m_closeButton = new LTButton();
	m_closeButton->setIcon(QIcon(":/main/close"));
	m_closeButton->setToolTip(tr("close"));
	m_closeButton->setIconSize(QSize(22, 22));
	connect(m_closeButton, SIGNAL(clicked(bool)), this, SLOT(slotWindowButtonClicked()));
	m_closeButton->setHoverColor(QColor(207, 0, 0, 230));
	m_closeButton->setPressColor(QColor(207, 0, 0, 150));
	m_menu = new QMenu(this);
	connect(m_menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShowSystemMenu()));
	connect(m_menu, SIGNAL(aboutToHide()), this, SLOT(aboutToHideSystemMenu()));
	m_maximizeAction = new QAction(QIcon(":/main/max2"), tr("maximize"), this);
	m_minimizeAction = new QAction(QIcon(":/main/min2"), tr("minimize"), this);
	m_restoreAction = new QAction(QIcon(":/main/restore2"), tr("restore"), this);
	m_closeAction = new QAction(QIcon(":/main/close2"), tr("close"), this);
	m_closeAction->setShortcut(QString("Alt+F4"));
	connect(m_maximizeAction, SIGNAL(triggered(bool)), m_maximizeButton, SIGNAL(clicked(bool)));
	connect(m_minimizeAction, SIGNAL(triggered(bool)), m_minimizeButton, SIGNAL(clicked(bool)));
	connect(m_restoreAction, SIGNAL(triggered(bool)), m_maximizeButton, SIGNAL(clicked(bool)));
	connect(m_closeAction, SIGNAL(triggered(bool)), m_closeButton, SIGNAL(clicked(bool)));
	m_menu->addAction(m_restoreAction);
	m_menu->addAction(m_minimizeAction);
	m_menu->addAction(m_maximizeAction);
	m_menu->addSeparator();
	m_menu->addAction(m_closeAction);
}
void LTBarPrivate::updateMenuColor()
{
	if (m_menuBar == nullptr) {
		return;
	}
	m_menuBar->setStyleSheet(QString(
		"QMenuBar{background-color: transparent;margin:1px;color: #%1;}"
		"QMenuBar::item{background-color: transparent;}"
		"QMenuBar::item:disabled{color: gray;}"
		"QMenuBar::item:pressed{background:#%2;}"
		"QMenuBar::item:hover{background:#%3;}"
		)
		.arg(LT::colorToArgb(m_menuTextColor))
		.arg(LT::colorToArgb(m_menuPressColor))
		.arg(LT::colorToArgb(m_menuHoverColor)));
}
void LTBarPrivate::updateWindowButtons()
{
	m_maximizeButton->setVisible(m_windowFlags & Qt::WindowMaximizeButtonHint);
	m_minimizeButton->setVisible(m_windowFlags & Qt::WindowMinimizeButtonHint);
	m_maximizeAction->setEnabled(m_windowFlags & Qt::WindowMaximizeButtonHint);
	m_minimizeAction->setEnabled(m_windowFlags & Qt::WindowMinimizeButtonHint);
	m_restoreAction->setEnabled(false);
	setWidgetMaximizable(m_windowFlags & Qt::WindowMaximizeButtonHint);
	// m_logoButton->setHasMenu((m_windowFlags & Qt::Dialog) != Qt::Dialog);
}
void LTBarPrivate::registerWidget(QWidget *widget)
{
	m_mainWidget = widget;
	m_windowFlags = widget->windowFlags();
	m_mainWidget->setMouseTracking(true);
	m_mainWidget->setAttribute(Qt::WA_Hover, true);
}
void LTBarPrivate::restoreWidget(QWidget *pWidget)
{
	m_isMaximized = false;
	pWidget->setGeometry(m_normalRect);
	emit windowResizable(true);
}
void LTBarPrivate::maximizeWidget(QWidget *pWidget)
{
	// ???????§³?????????,?????????????????
	m_isMaximized = true;
	// ?????????????????
	int x = pWidget->frameGeometry().x() + pWidget->frameGeometry().width() / 2;
	ScreenHelper sreen;
	m_currentScreen = sreen.currentScreen(x);
	QRect rect = sreen.screenRect(m_currentScreen);
	pWidget->setGeometry(rect);
	emit windowResizable(false);
}
bool LTBarPrivate::windowTitleChange(QObject *obj)
{
	QWidget *pWidget = qobject_cast<QWidget *>(obj);
	if (pWidget) {
		QFont font = QApplication::font();
		font.setPointSize(11);
		m_titleLabel->setText(pWidget->windowTitle());
		m_titleLabel->setFont(font);
		return true;
	}
	return false;
}
bool LTBarPrivate::windowIconChange(QObject *obj)
{
	QWidget *pWidget = qobject_cast<QWidget *>(obj);
	if (pWidget) {
		QIcon icon = pWidget->windowIcon();
		if (!icon.isNull()) {
			m_logoButton->setIcon(icon);
			m_applicationButton->setIcon(icon);
		}
		return true;
	}
	return false;
}
void LTBarPrivate::windowSizeChange(QObject *obj)
{
	QWidget *pWindow = qobject_cast<QWidget *>(obj);
	if (pWindow) {
		bool bMaximize = pWindow->isMaximized();
		if (bMaximize) {
			// ?????????????????windows??????
			pWindow->setWindowState(pWindow->windowState() & ~Qt::WindowMaximized);
			this->maximizeWidget(pWindow);
		}
		windowStateChange(obj);
		// m_maximizeButton->setStyle(QApplication::style());
	}
}
void LTBarPrivate::windowStateChange(QObject *obj)
{
	Q_UNUSED(obj);
	if (m_isMaximized) {
		m_maximizeButton->setToolTip(tr("restore"));
		m_maximizeButton->setProperty("maximizeProperty", "restore");
		m_maximizeButton->setIcon(QIcon(":/main/restore"));
	}
	else {
		m_maximizeButton->setProperty("maximizeProperty", "maximize");
		m_maximizeButton->setToolTip(tr("maximize"));
		m_maximizeButton->setIcon(QIcon(":/main/max"));
	}
	if (m_windowFlags & Qt::WindowMaximizeButtonHint) {
		m_maximizeAction->setEnabled(!m_isMaximized);
		m_restoreAction->setEnabled(m_isMaximized);
	}
	else {
		m_maximizeAction->setEnabled(false);
		m_restoreAction->setEnabled(false);
	}
}
void LTBarPrivate::handleWidgetMouseEvent(QObject *obj, QEvent *event)
{
	Q_UNUSED(obj);
	switch (event->type()) {
	case QEvent::MouseButtonPress:
		handleMousePressEvent(static_cast<QMouseEvent *>(event));
		break;
	case QEvent::MouseButtonRelease:
		handleMouseReleaseEvent(static_cast<QMouseEvent *>(event));
		break;
	case QEvent::MouseMove:
		handleMouseMoveEvent(static_cast<QMouseEvent *>(event));
		break;
	case QEvent::Leave:
		handleLeaveEvent(static_cast<QMouseEvent *>(event));
		break;
	case QEvent::HoverMove:
		handleHoverMoveEvent(static_cast<QHoverEvent *>(event));
		break;
	default:
		break;
	}
}
void LTBarPrivate::updateCursorShape(const QPoint &gMousePos)
{
	if (m_mainWidget->isFullScreen() || m_mainWidget->isMaximized() || m_isMaximized) {
		if (m_bCursorShapeChanged) {
			m_mainWidget->unsetCursor();
		}
		return;
	}
	m_moveCursor.recalculate(gMousePos, m_mainWidget->frameGeometry());
	if (m_moveCursor.m_bOnTopLeftEdge || m_moveCursor.m_bOnBottomRightEdge) {
		m_mainWidget->setCursor(Qt::SizeFDiagCursor);
		m_bCursorShapeChanged = true;
	}
	else if (m_moveCursor.m_bOnTopRightEdge || m_moveCursor.m_bOnBottomLeftEdge) {
		m_mainWidget->setCursor(Qt::SizeBDiagCursor);
		m_bCursorShapeChanged = true;
	}
	else if (m_moveCursor.m_bOnLeftEdge || m_moveCursor.m_bOnRightEdge) {
		m_mainWidget->setCursor(Qt::SizeHorCursor);
		m_bCursorShapeChanged = true;
	}
	else if (m_moveCursor.m_bOnTopEdge || m_moveCursor.m_bOnBottomEdge) {
		m_mainWidget->setCursor(Qt::SizeVerCursor);
		m_bCursorShapeChanged = true;
	}
	else {
		if (m_bCursorShapeChanged) {
			m_mainWidget->unsetCursor();
			m_bCursorShapeChanged = false;
		}
	}
}
void LTBarPrivate::resizeWidget(const QPoint &gMousePos)
{
	QRect origRect;
	origRect = m_mainWidget->frameGeometry();
	int left = origRect.left();
	int top = origRect.top();
	int right = origRect.right();
	int bottom = origRect.bottom();
	origRect.getCoords(&left, &top, &right, &bottom);
	int minWidth = m_mainWidget->minimumWidth();
	int minHeight = m_mainWidget->minimumHeight();
	if (m_pressCursor.m_bOnTopLeftEdge) {
		left = gMousePos.x();
		top = gMousePos.y();
	}
	else if (m_pressCursor.m_bOnBottomLeftEdge) {
		left = gMousePos.x();
		bottom = gMousePos.y();
	}
	else if (m_pressCursor.m_bOnTopRightEdge) {
		right = gMousePos.x();
		top = gMousePos.y();
	}
	else if (m_pressCursor.m_bOnBottomRightEdge) {
		right = gMousePos.x();
		bottom = gMousePos.y();
	}
	else if (m_pressCursor.m_bOnLeftEdge) {
		left = gMousePos.x();
	}
	else if (m_pressCursor.m_bOnRightEdge) {
		right = gMousePos.x();
	}
	else if (m_pressCursor.m_bOnTopEdge) {
		top = gMousePos.y();
	}
	else if (m_pressCursor.m_bOnBottomEdge) {
		bottom = gMousePos.y();
	}
	QRect newRect(QPoint(left, top), QPoint(right, bottom));
	if (newRect.isValid()) {
		if (minWidth > newRect.width()) {
			if (left != origRect.left()) {
				newRect.setLeft(origRect.left());
			}
			else {
				newRect.setRight(origRect.right());
			}
		}
		if (minHeight > newRect.height()) {
			if (top != origRect.top()) {
				newRect.setTop(origRect.top());
			}
			else {
				newRect.setBottom(origRect.bottom());
			}
		}
		m_mainWidget->setGeometry(newRect);
	}
}
void LTBarPrivate::moveWidget(const QPoint &gMousePos)
{
	m_mainWidget->move(gMousePos - m_dragPos);
}
void LTBarPrivate::handleMousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && !m_isMaximized) { // add !m_isMaximized 2017-7-27 22:52:39
		m_bEdgePressed = true;
		QRect frameRect = m_mainWidget->frameGeometry();
		m_pressCursor.recalculate(event->globalPos(), frameRect);
		m_dragPos = event->globalPos() - frameRect.topLeft();
	}
}
void LTBarPrivate::handleMouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		m_bEdgePressed = false;
		m_pressCursor.reset();
		if (m_bWidgetResizable) {
			updateCursorShape(event->globalPos());
		}
	}
}
void LTBarPrivate::handleMouseMoveEvent(QMouseEvent *event)
{
	if (m_bEdgePressed) {
		if (m_bWidgetResizable && m_pressCursor.m_bOnEdges) {
			resizeWidget(event->globalPos());
		}
		// else if (d->m_bWidgetMovable && m_bLeftButtonTitlePressed)
		// {
		// moveWidget(event->globalPos());
		// }
	}
	else if (m_bWidgetResizable) {
		updateCursorShape(event->globalPos());
	}
}
void LTBarPrivate::handleLeaveEvent(QEvent *event)
{
	Q_UNUSED(event);
	if (!m_bEdgePressed) {
		m_mainWidget->unsetCursor();
	}
}
void LTBarPrivate::handleHoverMoveEvent(QHoverEvent *event)
{
	if (!m_bEdgePressed && m_bWidgetResizable) {
		updateCursorShape(m_mainWidget->mapToGlobal(event->pos()));
	}
}
QPoint LTBarPrivate::calcDragPoint(QWidget *pWindow, QMouseEvent *event) const
{
	// ???????????????????????????????
	int mouseX = event->globalX();
	ScreenHelper screen;
	QRect rect = screen.screenRect(m_currentScreen);
	int maxWidth = rect.x() + rect.width();
	int screenX = rect.x();
	int orgWidth = m_normalRect.width();
	if (orgWidth == 0) { // ?????????????,orgWidth????0
		orgWidth = pWindow->minimumWidth();
	}
	QPoint point;
	point.setY(0);
	if (mouseX - screenX < orgWidth / 2) {
		point.setX(screenX);
	}
	else if (maxWidth - mouseX < orgWidth / 2) {
		point.setX(maxWidth - orgWidth);
	}
	else {
		point.setX(mouseX - orgWidth / 2);
	}
	return point;
}
void LTBarPrivate::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		m_bLeftButtonPressed = true;
		QWidget *pWindow = m_mainWidget;
		if (pWindow->isTopLevel()) {
			if (m_isMaximized) {
				m_dragPoint = calcDragPoint(pWindow, event);
				m_movePoint = event->globalPos() - m_dragPoint;
			}
			else {
				m_movePoint = event->globalPos() - pWindow->pos();
			}
		}
	}
	event->ignore();
}
void LTBarPrivate::mouseReleaseEvent(QMouseEvent *event)
{
	m_bLeftButtonPressed = false;
	// maximize on the top of the screen
	if (!m_isMaximized) {
		if (event->globalY() == 0) {
			m_mainWidget->move(m_mainWidget->frameGeometry().x(), 10);
			if (m_bWidgetMaximizable) {
				emit m_maximizeButton->click();
			}
		}
		else {
			int y = m_mainWidget->frameGeometry().y();
			if (y < 0) {
				m_mainWidget->move(m_mainWidget->frameGeometry().x(), 10);
			}
		}
	}
	event->ignore();
}
void LTBarPrivate::mouseMoveEvent(QMouseEvent *event)
{
	if (m_bLeftButtonPressed) {
		QWidget *pWindow = m_mainWidget;
		if (pWindow->isTopLevel()) {
			if (m_bWidgetMaximizable && m_isMaximized) {
				// restoreWidget(pWindow);
				// modified on 2017-7-27 22:54:15 , drag on miximization
				if (event->globalY() > 2 * m_moveCursor.m_nBorderWidth) {
					m_dragPoint = calcDragPoint(pWindow, event);
					m_movePoint = event->globalPos() - m_dragPoint;
					restoreWidget(pWindow);
				}
			}
			else {
				if (m_bWidgetResizable && m_pressCursor.m_bOnEdges) {
					event->ignore();
				}
				else {
					pWindow->move(event->globalPos() - m_movePoint);
				}
			}
		}
	}
}
void LTBarPrivate::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		if (m_bWidgetMaximizable) {
			emit m_maximizeButton->click();
		}
	}
}
void LTBarPrivate::slotWindowButtonClicked()
{
	LTButton *pButton = qobject_cast<LTButton *>(sender());
	QWidget *pWindow = m_mainWidget;
	if (pWindow) {
		if (pButton == m_minimizeButton) {
			pWindow->showMinimized();
			m_isMinimized = true;
		}
		else if (pButton == m_maximizeButton) {
			if (m_isMaximized) {
				restoreWidget(pWindow);
			}
			else {
				m_normalRect = pWindow->frameGeometry();
				maximizeWidget(pWindow);
			}
			m_isMinimized = false;
		}
		else if (pButton == m_closeButton) {
			pWindow->close();
		}
	}
}
void LTBarPrivate::aboutToShowSystemMenu()
{
	m_logoButton->select(true);
}
void LTBarPrivate::aboutToHideSystemMenu()
{
	m_logoButton->select(false);
}
void LTBarPrivate::systemMenuTriggered(QMouseEvent *e)
{
	LTButton *button = qobject_cast<LTButton *>(sender());
	if (button == nullptr) {
		return;
	}
	int x = e->x();
	int y = e->y();
	QPoint pos = e->globalPos();
	pos.setX(pos.x() - x);
	pos.setY(pos.y() - y + button->height());
	m_menu->popup(pos);
}
void LTBarPrivate::aboutToShowMenu()
{
	if (m_menuButton) {
		m_menuButton->select(true);
	}
}
void LTBarPrivate::aboutToHideMenu()
{
	if (m_menuButton) {
		m_menuButton->select(false);
	}
}
void LTBarPrivate::menuTriggered(QMouseEvent *e)
{
	LTButton *button = qobject_cast<LTButton *>(sender());
	if (button == nullptr) {
		return;
	}
	QAction *action = button->defaultAction();
	QMenu *menu = action->menu();
	if (menu == nullptr) {
		return;
	}
	connect(menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShowMenu()));
	connect(menu, SIGNAL(aboutToHide()), this, SLOT(aboutToHideMenu()));
	m_menuButton = button;
	int x = e->x();
	int y = e->y();
	QPoint pos = e->globalPos();
	pos.setX(pos.x() - x);
	pos.setY(pos.y() - y + button->height());
	menu->popup(pos);
}
void LTBarPrivate::applicationMenuTriggered(QMouseEvent *e)
{
	if (m_applicationWidget == nullptr) {
		return;
	}
	if (m_applicationWidget->layout()->itemAt(0) == nullptr) {
		return;
	}
	LTButton *button = qobject_cast<LTButton *>(sender());
	if (button == nullptr) {
		return;
	}
	int x = e->x();
	int y = e->y();
	QPoint pos = e->globalPos();
	pos.setX(pos.x() - x);
	pos.setY(pos.y() - y + button->height());
	m_applicationWidget->move(pos);
	m_applicationWidget->setWindowModality(Qt::WindowModal);
	m_applicationWidget->show();
	m_applicationButton->select(false);
}
LTBar::LTBar(QWidget *parent)
	: QWidget(parent), d(new LTBarPrivate(this))
{
	Q_ASSERT(parent);
	d->q = this;
	d->registerWidget(parent);
	d->init();
	parent->installEventFilter(this);
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setMargin(0);
	layout->setSpacing(0);
	layout->addWidget(d);
	setLayout(layout);
	if (s_backgroundColor.isValid()) {
		setBackgroundColor(s_backgroundColor);
	}
	connect(d, SIGNAL(windowResizable(bool)), this, SIGNAL(maximizationChanged(bool)));
}
LTBar::~LTBar()
{
}
QMenuBar *LTBar::menuBar() const
{
	if (d->m_menuBar == nullptr) {
		d->m_menuBar = new QMenuBar();
		d->m_menuBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
		d->updateMenuColor();
		d->m_menuBarArea->addWidget(d->m_menuBar);
	}
	return d->m_menuBar;
}
void LTBar::showMenuBar(bool show)
{
	d->showMenuBar(show);
}
bool LTBar::isMenuBarVisible() const
{
	return d->m_bMenuBarVisible;
}
QuickAccessBar *LTBar::quickAccessBar() const
{
	return d->m_quickAccessBar;
}
QHBoxLayout * LTBar::menuLayout()
{
	return d->m_appButtonLayout;
}
void LTBar::showQuickAccess(bool show)
{
	d->showQuickAccessBar(show);
}
bool LTBar::isQuickAccessVisible() const
{
	return d->m_bQuickAccessVisible;
}
void LTBar::setHoverColor(const QColor &color)
{
	foreach(LTButton * button, d->m_additionalButtons) {
		button->setHoverColor(color);
	}
	d->m_menuHoverColor = color;
	d->updateMenuColor();
	d->m_quickAccessBar->setHoverColor(color);
}
void LTBar::setPressColor(const QColor &color)
{
	foreach(LTButton * button, d->m_additionalButtons) {
		button->setPressColor(color);
	}
	d->m_menuPressColor = color;
	d->updateMenuColor();
	d->m_quickAccessBar->setPressColor(color);
}
void LTBar::setTextColor(const QColor &color)
{
	foreach(LTButton * button, d->m_additionalButtons) {
		button->setTextColor(color);
	}
	d->m_titleLabel->setStyleSheet(QString("color:#%1;").arg(LT::colorToArgb(color)));
	d->m_menuTextColor = color;
	d->updateMenuColor();
	d->m_quickAccessBar->setTextColor(color);
}
void LTBar::setBackgroundColor(const QColor &color)
{
	QPalette palette;
	palette.setColor(QPalette::Background, color);
	setPalette(palette);
	setAutoFillBackground(true);
	s_backgroundColor = color;
}
void LTBar::addAdditionalControl(QAction *action, LTBar::AdditionalControlPosition position)
{
	d->addAdditionalControl(action, position);
}
void LTBar::addAdditionalControl(QWidget *widget, LTBar::AdditionalControlPosition position)
{
	d->addAdditionalControl(widget, position);
}
void LTBar::setApplicationWidget(const QString &label, QWidget *widget)
{
	d->m_applicationButton->setText(label);
	d->m_applicationWidget->layout()->addWidget(widget);
}
void LTBar::setApplicationButtonBkColor(const QColor &color)
{
	d->m_applicationButton->setNormalColor(color);
}
void LTBar::setWidgetResizable(bool resizable)
{
	d->setWidgetResizable(resizable);
}
void LTBar::setTitleBarHeight(int height)
{
	if (height > TITLE_BAR_HEIGHT && height <= TITLE_BAR_HEIGHT + 15) {
		d->m_titleBarHeight = height;
		d->m_titleWidget->setFixedHeight(d->m_titleBarHeight);
		if (isMenuBarVisible()) {
			d->setFixedHeight(d->m_titleBarHeight + d->m_menuBarHeight);
		}
	}
}
void LTBar::setLTStyle(LTBar::LTStyle style)
{
	d->setLTStyle(style);
}
void LTBar::updateWidgetFlags()
{
	d->m_windowFlags = d->m_mainWidget->windowFlags();
	d->updateWindowButtons();
}
bool LTBar::eventFilter(QObject *object, QEvent *event)
{
	switch (event->type()) {
	case QEvent::WindowTitleChange: {
		if (d->windowTitleChange(object)) {
			return true;
		}
		break;
	}
	case QEvent::WindowIconChange: {
		if (d->windowIconChange(object)) {
			return true;
		}
		break;
	}
	case QEvent::WindowStateChange: {
		d->windowStateChange(object);
		return true;
	}
	case QEvent::Resize: {
		d->windowSizeChange(object);
		return true;
	}
	case QEvent::MouseMove:
	case QEvent::HoverMove:
	case QEvent::MouseButtonPress:
	case QEvent::MouseButtonRelease:
	case QEvent::MouseButtonDblClick: {
		d->handleWidgetMouseEvent(object, event);
		return true;
	}
	default:
		break;
	}
	return QObject::eventFilter(object, event);
}
#include "LTbar.moc"