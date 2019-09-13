#pragma once
#include <QMainWindow>

class LTwindowPrivate;
class LTBar;
class LTwindow : public QMainWindow
{
	Q_OBJECT
public:
	LTwindow(QWidget *parent = nullptr);
	LTBar *LtBar()const;
	void setFixedSize(const QSize &s);
	void setFixedSize(int w, int h);
	void setFixedWidth(int w);
	void setFixedHeight(int h);
	void setWindowFlags(Qt::WindowFlags type);
	void setTitleBackgrondColor(QColor color);
signals:
	void resizable(bool resizable);
protected:
	virtual void paintEvent(QPaintEvent *event);
private:
	LTwindowPrivate *d;
};