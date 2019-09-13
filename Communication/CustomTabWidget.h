#pragma once
#include <QDialog>
#include <QProxyStyle>
#include <QStyleOptionTab>
#include <QPainter>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
class CustomTabStyle: public QProxyStyle
{
public:
	QSize sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const
	{
		QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
		if (type == QStyle::CT_TabBarTab) {
			s.transpose();
			s.rwidth() = 100; // 设置每个tabBar中item的大小
			s.rheight() = 68;
		}
		return s;
	}

	void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
	{
		if (element == CE_TabBarTabLabel)
		{
			if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option))
			{
				QRect allRect = tab->rect;
				if (tab->state & QStyle::State_Selected)
				{
					painter->save();
					painter->setPen(0x7F7F7F);//底色设置
					painter->setBrush(QBrush(0x7F7F7F));//底色设置
					painter->drawRect(allRect.adjusted(0, 0, 0, 0));//边框的距离
					painter->restore();
				}
				QTextOption option;
				option.setAlignment(Qt::AlignCenter);
				if (tab->state & QStyle::State_Selected)
				{
					painter->setPen(0xf8fcff);
				}
				else
				{
					painter->setPen(0x5d5d5d);
				}

				painter->drawText(allRect, tab->text, option);
				return;
			}
		}

		if (element == CE_TabBarTab)
		{
			QProxyStyle::drawControl(element, option, painter, widget);
		}
	
	}
};

class CustomTabWidget : public QTabWidget
{
public:
	CustomTabWidget(QWidget *parent = Q_NULLPTR);
	~CustomTabWidget();
private:

};

