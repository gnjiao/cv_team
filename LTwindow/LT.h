#ifndef LT_H
#define LT_H
#include <QObject>
class LT
{
public:
	/// color
	static QColor argbToColor(const QString &argb);
	static QString colorToArgb(const QColor &color); /// ffaabbcc
													 // file
	static void showInExplorer(QString fileName);
	// settings
	static void setSysSettings(const QString &organization, const QString &application);
};
#endif // LT_H