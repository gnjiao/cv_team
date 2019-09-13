#ifndef SCREENHELPER_H
#define SCREENHELPER_H
#include <QRect>
#include <QList>
/* on screen display */
class ScreenHelper
{
public:
	ScreenHelper();
	int currentScreen(const int x);
	QRect screenRect(const int current);
	static QRect normalRect();
private:
	QList<QRect> m_screenRects;
};
#endif // SCREENHELPER_H