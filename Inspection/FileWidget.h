#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QThread>
#include <QTime>
#include <QApplication>
#include <QFileDialog>
#include <QDir>
#include <set>
class FileWidget : public QWidget
{
	Q_OBJECT
public:
	FileWidget(QWidget *parent = Q_NULLPTR);
	~FileWidget();
	QString m_filePath;
	QString m_currentPath;
	QStringList m_allImageNameList;
	QString m_directory;
	int m_fileIndex = 0;
public slots:
	void on_Next_btn_Clicked();
	void on_Last_btn_Clicked();
	void on_Start_btn_Clicked();
	void on_Open_btn_Clicked();
private:
	void Sleep(int);
	QPushButton *Next_btn;
	QPushButton *Last_btn;
	QPushButton *Start_btn;
	QPushButton *Open_btn;
	QLabel *m_filePath_lb;

	bool isStart = true;
signals:
	void SelectFile(QString);
	void Next_btn_Clicked(QString);
	void Last_btn_Clicked(QString);
	void Start_btn_Clicked();
	void Stop_btn_Clicked();
};