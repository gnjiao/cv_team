#include "CustomTabWidget.h"

CustomTabWidget::CustomTabWidget(QWidget *parent): QTabWidget(parent)
{
	this->setTabPosition(QTabWidget::North);
	this->tabBar()->setStyle(new CustomTabStyle);
	this->setStyleSheet("QTabWidget::pane{ border-left: 0px solid #eeeeee;}");
	Qt::WindowFlags flags = this->windowFlags();
	this->setWindowFlags(flags | Qt::FramelessWindowHint);
	this->resize(1000, 800);
	this->show();
}

CustomTabWidget::~CustomTabWidget()
{
}