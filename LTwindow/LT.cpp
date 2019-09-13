#include "LT.h"
#include "LT_p.h"
#include <QColor>
#include <QTextCodec>
#if defined(Q_OS_WIN32) && !defined(Q_CC_MSVC)
#include <Windows.h>
#include <shellapi.h>
#else
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#endif
QColor LT::argbToColor(const QString &argb)
{
	QColor color;
	bool ok = true;
	QRgb rgba = argb.toUInt(&ok, 16);
	if (ok) {
		color = QColor::fromRgba(rgba);
	}
	else {
		color = QColor(0, 0, 0);
	}
	return color;
}
QString LT::colorToArgb(const QColor &color)
{
	QString argb = QString("%1%2%3%4")
		.arg(color.alpha(), 2, 16, QLatin1Char('0'))
		.arg(color.red(), 2, 16, QLatin1Char('0'))
		.arg(color.green(), 2, 16, QLatin1Char('0'))
		.arg(color.blue(), 2, 16, QLatin1Char('0'));
	return argb;
}
void LT::showInExplorer(QString fileName)
{
	Q_UNUSED(fileName);
#if defined(Q_OS_WIN32) && !defined(Q_CC_MSVC)
	QString argsFile = QString("/select, %1").arg(fileName.replace("/", "\\"));
	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	ShellExecuteA(nullptr, "open", "explorer.exe", codec->fromUnicode(argsFile).constData(), nullptr, SW_SHOWDEFAULT);
#else
	QDesktopServices::openUrl(QUrl(QFileInfo(fileName).absolutePath(), QUrl::TolerantMode));
#endif
}
void LT::setSysSettings(const QString &organization, const QString &application)
{
	LTPrivate::g_settingsOrganization = organization;
	LTPrivate::g_settingsApplication = application;
}