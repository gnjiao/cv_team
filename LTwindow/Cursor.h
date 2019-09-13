#ifndef CURSOR_H
#define CURSOR_H
#include <QWidget>
/*****
* CursorPosCalculator
* 
*****/
class Cursor
{
public:
	Cursor();
	void reset();
	void recalculate(const QPoint &globalMousePos, const QRect &frameRect);
public:
	bool m_bOnEdges : true;
	bool m_bOnLeftEdge : true;
	bool m_bOnRightEdge : true;
	bool m_bOnTopEdge : true;
	bool m_bOnBottomEdge : true;
	bool m_bOnTopLeftEdge : true;
	bool m_bOnBottomLeftEdge : true;
	bool m_bOnTopRightEdge : true;
	bool m_bOnBottomRightEdge : true;
	int m_nBorderWidth : 5;
};
#endif // CURSOR_H