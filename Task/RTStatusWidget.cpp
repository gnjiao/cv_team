#include "RTStatusWidget.h"


StatusWidget::StatusWidget(int key,QWidget *parent): QWidget(parent)
{
	this->setWindowFlag(Qt::FramelessWindowHint);
	this->setMaximumHeight(200);
	this->setMaximumWidth(200);
	TaskKey = key;
	QFont ft;
	ft.setPointSize(14);

	QLabel *pic_lb = new QLabel();
	pic_lb->setPixmap(QPixmap(":/CV_TEAM/icons/task.png"));
	QLabel *OKorNG = new QLabel("Task " + QString::number(TaskKey));
	OKorNG->setFont(ft);
	OKorNG_lb=new QLabel("  Idle");
	QPalette pa;
	pa.setColor(QPalette::WindowText, Qt::red);
	OKorNG_lb->setPalette(pa);
	ft.setPointSize(20);
	ft.setFamily("Î¢ÈíÑÅºÚ");
	ft.setBold(true);
	OKorNG_lb->setFont(ft);

	QLabel *ExecCount = new QLabel("Execute Count:");
	ExecCount_lb=new QLabel("0");

	QLabel *OKCount = new QLabel("OK Count:");
	OKCount_lb=new QLabel("0");

	QLabel *OKRatio = new QLabel("OK Ratio:");
	OKRatio_lb=new QLabel("0");

	QLabel *NGCount = new QLabel("NG Count:");
	NGCount_lb=new QLabel("0");

	QLabel *NGRatio = new QLabel("NG Ratio:");
	NGRatio_lb=new QLabel("0");

	QLabel *NGInspection = new QLabel("NG Inspection:");
	NGInspection_lb = new QLabel("None");
	
	QVBoxLayout *vlay = new QVBoxLayout();

	QHBoxLayout *hlay1 = new QHBoxLayout();
	hlay1->addWidget(pic_lb);
	hlay1->addWidget(OKorNG);
	hlay1->addWidget(OKorNG_lb);
	hlay1->addStretch();
	QHBoxLayout *hlay2 = new QHBoxLayout();
	hlay2->addWidget(ExecCount);
	hlay2->addWidget(ExecCount_lb);

	QHBoxLayout *hlay3 = new QHBoxLayout();
	hlay3->addWidget(OKCount);
	hlay3->addWidget(OKCount_lb);

	QHBoxLayout *hlay4 = new QHBoxLayout();
	hlay4->addWidget(OKRatio);
	hlay4->addWidget(OKRatio_lb);

	QHBoxLayout *hlay5 = new QHBoxLayout();
	hlay5->addWidget(NGCount);
	hlay5->addWidget(NGCount_lb);

	QHBoxLayout *hlay6 = new QHBoxLayout();
	hlay6->addWidget(NGRatio);
	hlay6->addWidget(NGRatio_lb);

	QHBoxLayout *hlay7 = new QHBoxLayout();
	hlay7->addWidget(NGInspection);
	hlay7->addWidget(NGInspection_lb);

	vlay->addLayout(hlay1);
	vlay->addLayout(hlay2);
	vlay->addLayout(hlay3);
	vlay->addLayout(hlay4);
	vlay->addLayout(hlay5);
	vlay->addLayout(hlay6);
	vlay->addLayout(hlay7);

	QLabel *space = new QLabel();
	space->setMaximumHeight(3);
	//space->setFrameStyle(QFrame::Panel);
	space->setFrameShadow(QFrame::Sunken);
	QPalette palette;
	palette.setColor(QPalette::Background, QColor(119, 136, 153));
	space->setAutoFillBackground(true);
	space->setPalette(palette);
	vlay->addWidget(space);

	this->setLayout(vlay);
}

StatusWidget::~StatusWidget()
{
}

void StatusWidget::setOKorNG(bool b)
{
	if (b)
	{
		OKorNG_lb->setText("  OK");
		QPalette pa;
		pa.setColor(QPalette::WindowText, Qt::green);
		OKorNG_lb->setPalette(pa);
	}
	else
	{
		OKorNG_lb->setText("  NG");
		QPalette pa;
		pa.setColor(QPalette::WindowText, Qt::red);
		OKorNG_lb->setPalette(pa);
	}
}

void StatusWidget::setExecCount(int count)
{
	ExecCount_lb->setText(QString::number(count));
}

void StatusWidget::setOKCount(int count)
{
	OKCount_lb->setText(QString::number(count));
}

void StatusWidget::setNGCount(int count)
{
	NGCount_lb->setText(QString::number(count));
}

void StatusWidget::setNGInspection(int count)
{
	NGInspection_lb->setText(QString::number(count));
}


RTStatusWidget::RTStatusWidget(QWidget *parent)
	: QDockWidget(parent)
{
	this->setWindowTitle("Status View");
	this->setMinimumWidth(200);
	this->setMaximumWidth(300);
	for (size_t i = 0;i < 3;i++)
	{
		m_statusWidgets.push_back(new StatusWidget(i));
	}
	QVBoxLayout *vlay = new QVBoxLayout();
	m_statusWidgets[0]->show();
	vlay->addWidget(m_statusWidgets[0]);
	vlay->addWidget(m_statusWidgets[1]);
	vlay->addWidget(m_statusWidgets[2]);
	QWidget *wid = new QWidget(this);
	vlay->addStretch();
	wid->setLayout(vlay);
	
	this->setWidget(wid);	
}

RTStatusWidget::~RTStatusWidget()
{
}
