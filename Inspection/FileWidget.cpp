#include "FileWidget.h"
FileWidget::FileWidget(QWidget *parent)
	: QWidget(parent)
{
	QGroupBox *main_gb = new QGroupBox();
	Next_btn = new QPushButton(this);
	Last_btn = new QPushButton(this);
	Start_btn = new QPushButton(this);
	Next_btn->setToolTip("Next");
	Last_btn->setToolTip("Last");
	Start_btn->setToolTip("Start");
	Next_btn->setStyleSheet("border:none;background:none;");
	Last_btn->setStyleSheet("border:none;background:none;");
	Start_btn->setStyleSheet("border:none;background:none;");
	Next_btn->setIcon(QIcon(":/CV_TEAM/icons/next.png"));
	Last_btn->setIcon(QIcon(":/CV_TEAM/icons/last.png"));
	Start_btn->setIcon(QIcon(":/CV_TEAM/icons/fileStart.png"));
	Next_btn->setIconSize(QSize(40, 40));
	Last_btn->setIconSize(QSize(40, 40));
	Start_btn->setIconSize(QSize(40, 40));
	QHBoxLayout *hlay = new QHBoxLayout();
	QLabel *space = new QLabel(" ");
	QLabel *space1 = new QLabel(" ");
	hlay->addStretch();
	hlay->addWidget(Last_btn);
	hlay->addWidget(space);
	hlay->addWidget(Start_btn);
	hlay->addWidget(space1);
	hlay->addWidget(Next_btn);
	hlay->addStretch();
	QFont ft;
	ft.setPointSize(14);
	QLabel *filePath_lb = new QLabel("File Path:");
	filePath_lb->setFont(ft);
	m_filePath_lb = new QLabel();
	m_filePath_lb->setFrameStyle(QFrame::Panel);
	m_filePath_lb->setMaximumHeight(30);
	m_filePath_lb->setMinimumWidth(450);
	m_filePath_lb->setFrameShadow(QFrame::Sunken);
	m_filePath_lb->setAlignment(Qt::AlignRight);
	ft.setPointSize(12);
	m_filePath_lb->setFont(ft);
	Open_btn = new QPushButton("...");
	Open_btn->setFixedSize(60, 30);
	QHBoxLayout *hlay1 = new QHBoxLayout();
	hlay1->addStretch();
	hlay1->addWidget(filePath_lb);
	hlay1->addWidget(m_filePath_lb);
	hlay1->addWidget(Open_btn);
	hlay1->addStretch();
	QVBoxLayout *vlay = new QVBoxLayout(this);
	vlay->addLayout(hlay);
	vlay->addLayout(hlay1);
	vlay->addStretch();
	main_gb->setLayout(vlay);
	QVBoxLayout *mainlay = new QVBoxLayout();
	mainlay->setContentsMargins(0, 0, 0, 0);
	mainlay->addWidget(main_gb);
	this->setLayout(mainlay);
	connect(Next_btn, SIGNAL(clicked()), this, SLOT(on_Next_btn_Clicked()));
	connect(Last_btn, SIGNAL(clicked()), this, SLOT(on_Last_btn_Clicked()));
	connect(Start_btn, SIGNAL(clicked()), this, SLOT(on_Start_btn_Clicked()));
	connect(Open_btn, SIGNAL(clicked()), this, SLOT(on_Open_btn_Clicked()));
}
FileWidget::~FileWidget()
{
}
void FileWidget::on_Next_btn_Clicked()
{
	this->Next_btn->setIcon(QIcon(":/CV_TEAM/icons/next_gray.png"));
	Sleep(200);
	this->Next_btn->setIcon(QIcon(":/CV_TEAM/icons/next.png"));
	if (m_allImageNameList.size() == 0)
	{
		return;
	}
	m_fileIndex++;
	if (m_fileIndex >= m_allImageNameList.size())
	{
		m_fileIndex = 0;
	}
	m_currentPath = m_directory + "\\" + m_allImageNameList[m_fileIndex];
	this->m_filePath_lb->setText(m_currentPath);
	emit Next_btn_Clicked(m_currentPath);
}
void FileWidget::on_Last_btn_Clicked()
{
	this->Last_btn->setIcon(QIcon(":/CV_TEAM/icons/last_gray.png"));
	Sleep(200);
	this->Last_btn->setIcon(QIcon(":/CV_TEAM/icons/last.png")); m_fileIndex--;
	if (m_allImageNameList.size() == 0)
	{
		return;
	}
	if (m_fileIndex < 0)
	{
		m_fileIndex = 0;
	}
	m_currentPath = m_directory + "\\" + m_allImageNameList[m_fileIndex];
	this->m_filePath_lb->setText(m_currentPath);
	emit Last_btn_Clicked(m_currentPath);
}
void FileWidget::on_Start_btn_Clicked()
{
	if (isStart)
	{
		Start_btn->setIcon(QIcon(":/CV_TEAM/icons/fileStop.png"));
		Start_btn->setToolTip("Stop");
		isStart = false;
	}
	else
	{
		Start_btn->setIcon(QIcon(":/CV_TEAM/icons/fileStart.png"));
		Start_btn->setToolTip("Start");
		isStart = true;
	}
}
void FileWidget::on_Open_btn_Clicked()
{
	m_fileIndex = 0;
	m_directory = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("OpenPath"), QDir::currentPath()));
	if (!m_directory.isEmpty())
	{
		this->m_filePath_lb->setText(m_directory);
	}
	QDir dir(m_directory);
	dir.setFilter(QDir::Files | QDir::NoSymLinks);
	QStringList filters;
	filters << "*.png" << "*.jpg" << "*.bmp" << "*.jpeg";
	dir.setNameFilters(filters);
	m_allImageNameList = dir.entryList();
	if (m_allImageNameList.count() <= 0)
	{
		return;
	}
	if (m_allImageNameList.size() > 0)
	{
		this->m_currentPath = m_allImageNameList[m_fileIndex];
		m_currentPath = m_directory + "\\" + m_allImageNameList[m_fileIndex];
		this->m_filePath_lb->setText(m_currentPath);
		emit SelectFile(m_currentPath);
	}
}
void FileWidget::Sleep(int msec)
{
	QTime reachTime = QTime::currentTime().addMSecs(msec);
	while (QTime::currentTime() < reachTime)
	{
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
	}
}