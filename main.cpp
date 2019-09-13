#include "CV_TEAM.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QSplashScreen>
#include <windows.h>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QFile file(":/StyleSheet/stylesheets/flatwhite.css");
	bool s = file.open(QFile::ReadOnly);
	a.setStyleSheet(file.readAll());
	file.close();
	CV_TEAM w;
	QSplashScreen *splash = new QSplashScreen();
	QPixmap pix = QPixmap(":/CV_TEAM/icons/logo.jpg");
	splash->setPixmap(pix);
	splash->show();
	Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
	splash->showMessage("Setting up the main window...",topRight,Qt::red);
	QTextEdit *textEdit = new QTextEdit();
	splash->showMessage("Loading modules...", topRight, Qt::red);
	splash->finish(&w);
	delete splash;
	//w.show();

	return a.exec();
}






