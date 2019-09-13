#include "DataModel\BaseWidget.h"


BaseWidget::BaseWidget(QWidget *parent) : QDialog(parent)

{

	this->setWindowTitle("Base Widget");

	makeToolBar();

	makeWidget();

	fr_view = new FrameView();

	QHBoxLayout *hlay = new QHBoxLayout();

	QVBoxLayout *vlay = new QVBoxLayout();

	vlay->addWidget(toolBar);

	vlay->addWidget(fr_view);

	QSplitter *splitter =new QSplitter(Qt::Horizontal);

	splitter->setHandleWidth(0);

	splitter->addWidget(TabWidget);

	QWidget *right_widget = new QWidget();

	right_widget->setLayout(vlay);

	splitter->addWidget(right_widget);

	QList<int> list;

	list.append(300);

	list.append(700);

	splitter->setSizes(list);

	QVBoxLayout *vlay2 = new QVBoxLayout();

	vlay2->addWidget(splitter);

	botMsgBox = new QLabel("message label");

	botMsgBox->setFrameStyle(QFrame::Panel);

	botMsgBox->setMaximumHeight(30);

	botMsgBox->setFrameShadow(QFrame::Sunken);

	QFont ft;

	ft.setPointSize(18);

	botMsgBox->setFont(ft);

	QPalette pa;

	pa.setColor(QPalette::WindowText, Qt::blue);

	botMsgBox->setPalette(pa);

	botMsgBox->setText("this is test for label font");

	Cancel_btn = new QPushButton("Cancel");
	
	Cancel_btn->setMaximumWidth(90);

	OK_btn = new QPushButton("OK");

	OK_btn->setMaximumWidth(90);

	QHBoxLayout *hlay2 = new QHBoxLayout();

	hlay2->addWidget(botMsgBox);

	hlay2->addWidget(Cancel_btn);

	hlay2->addWidget(OK_btn);

	vlay2->addLayout(hlay2);

	this->setLayout(vlay2);

	connect(OK_btn, SIGNAL(clicked()), this, SLOT(on_OK_btn_clicked()));

	connect(Cancel_btn, SIGNAL(clicked()), this, SLOT(on_Cancel_btn_clicked()));

}



BaseWidget::~BaseWidget()

{

}



void BaseWidget::makeToolBar()

{

	toolBar = new QToolBar;

	toolBar->setStyleSheet("QToolBar{color:black}"

		"QToolBar{background-color:#E4E4E4}"

		"QToolBar{border:0px}"

		"QToolBar{border-radius:5px}"

		"QToolBar{padding:2px 4px}"

		);



	action_openFile = new QAction(tr("Open"), this);

	action_openFile->setIcon(QIcon(":/CV_TEAM/icons/openfile48.png"));

	action_openFile->setShortcut(QKeySequence::Open);

	action_openFile->setEnabled(true);



	action_play = new QAction(tr("Play"), this);

	action_play->setIcon(QIcon(":/CV_TEAM/icons/play48.png"));



	action_edit = new QAction(tr("Edit"), this);

	action_edit->setIcon(QIcon(":/CV_TEAM/icons/edit_red.png"));

	action_edit->setAutoRepeat(false);

	action_edit->setCheckable(true);

	action_edit->setChecked(true);

	action_edit->setEnabled(true);



	action_editCancel = new QAction(tr("EditCancel"), this);

	action_editCancel->setIcon(QIcon("Icons\\Close Window_50px.png"));

	action_editCancel->setEnabled(true);





	connect(action_openFile, SIGNAL(triggered()), this, SLOT(on_action_openFile_triggered()));

	connect(action_play, SIGNAL(triggered()), this, SLOT(on_action_play_triggered()));

	connect(action_edit, SIGNAL(toggled(bool)), this, SLOT(on_action_edit_toggled(bool)));

	connect(action_editCancel, SIGNAL(triggered(bool)), this, SLOT(on_action_editCancel_triggered()));


	toolBar->addAction(action_openFile);

	//toolBar->addSeparator();

	toolBar->addAction(action_play);

	//toolBar->addSeparator();

	toolBar->addAction(action_edit);

}



void BaseWidget::makeWidget()

{

	TabWidget = new QTabWidget(this);

	TabWidget->setTabPosition(QTabWidget::North);

	QFrame* page1 = new QFrame(TabWidget);

	BaseParamWidget = TabWidget->widget(TabWidget->addTab(page1, tr("BaseParam")));


	QFrame* page2 = new QFrame(TabWidget);

	SetItemWidget = TabWidget->widget(TabWidget->addTab(page2, tr("SetItem")));


	QFrame* page3 = new QFrame(TabWidget);

	OutputWidget = TabWidget->widget(TabWidget->addTab(page3, tr("Output")));

}



void BaseWidget::on_action_openFile_triggered()

{

	openFileDlg = new QFileDialog(this);

	openFileDlg->setFileMode(QFileDialog::AnyFile);

	openFileDlg->setNameFilter(tr("Images(*.png *.bmp *.jpg)"));

	openFileDlg->setViewMode(QFileDialog::Detail);


	QStringList fileNames;

	if (openFileDlg->exec())

		fileNames = openFileDlg->selectedFiles();

	if (fileNames.isEmpty()) return;

	std::string path = fileNames.at(0).toLocal8Bit().toStdString();

	cv::Mat fileImg = cv::imread(path.c_str());

	doOpenFile(fileImg);

	fr_view->setImage(fileImg);

}



void BaseWidget::on_action_play_triggered()

{
	doPlay();

}



void BaseWidget::on_action_edit_toggled(bool b)
{
	if (b)
	{
		action_edit->setIcon(QIcon(":/CV_TEAM/icons/edit_red.png"));
	}
	else
	{
		action_edit->setIcon(QIcon(":/CV_TEAM/icons/edit_black.png"));
	}
	doEdit(b);

}



void BaseWidget::on_action_editCancel_triggered()

{
	doCancel();
}



void BaseWidget::on_OK_btn_clicked()

{
	doOk();
}



void BaseWidget::on_Cancel_btn_clicked()

{
	doCancel();
}