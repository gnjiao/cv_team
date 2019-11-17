#include "InspectionDialog.h"
InspectionDialog::InspectionDialog(QWidget *parent) : QDialog(parent)
{
	this->resize(1200, 800);
	this->setWindowTitle("InspectionDialog");
	this->setObjectName("InspectionDialog");
	this->setWindowFlag(Qt::FramelessWindowHint);
	makeToolBar();
	makeMainLayout();
	
}
InspectionDialog::~InspectionDialog()
{
}
void InspectionDialog::setInspection(Inspection *ins)
{
	this->toolBlockWidget->setInspection(ins);
	m_inspection = ins;
}
void InspectionDialog::updateTreeWidget()
{
	if (m_inspection->m_toolBlock == nullptr)
	{
		m_inspection->m_toolBlock = new ToolBlock();
	}
	this->toolBlockWidget->setInspection(m_inspection);
	toolBlockWidget->updateTree();
}
QList<QGraphicsItem*> InspectionDialog::getShowItem()
{
	return this->m_showItems;
}
FrameView * InspectionDialog::getFrameView()
{
	return this->fr_view;
}
void InspectionDialog::setTitle(QString title)
{
	this->m_title->setTitleContent(title, 16);
}
void InspectionDialog::on_action_PlayImage_triggered()
{
	QFileDialog *openFileDlg = new QFileDialog(this);
	openFileDlg->setFileMode(QFileDialog::AnyFile);
	openFileDlg->setNameFilter(tr("Images(*.png *.bmp *.jpg)"));
	openFileDlg->setViewMode(QFileDialog::Detail);
	QStringList fileNames;
	if (openFileDlg->exec())
		fileNames = openFileDlg->selectedFiles();
	if (fileNames.isEmpty()) return;
	std::string path = fileNames.at(0).toLocal8Bit().toStdString();
	cv::Mat src = cv::imread(path.c_str());
	fr_view->setImage(src);
	m_inspection->GetAcquisition()->SetCurrentMat(src);
	m_inspection->execute();
}
void InspectionDialog::on_action_PlayCamera_triggered()
{
}
void InspectionDialog::on_action_Repeat_triggered()
{
	if (m_inspection == nullptr)
		return;
	m_inspection->execute();
	updateResult();
}
void InspectionDialog::on_action_Display_ac_triggered()
{
	m_displayWidget = new DisplayWidget();
	m_displayWidget->setKey(m_inspection->GetKey());
	connect(m_displayWidget, SIGNAL(DisplaySettingOK()), this, SLOT(on_DisplaySettingOK()));
	m_displayWidget->show();
}
void InspectionDialog::on_action_Output_triggered()
{
}
void InspectionDialog::on_action_Exit_triggered()
{
	this->close();
}
void InspectionDialog::on_ToolBlockItemDoubleClicked(QTreeWidgetItem * item)
{
	ToolBlockMGR->initOperator(toolBlockWidget->indexOfTopLevelItem(item), true, &(toolBlockWidget->OperatorWidget));
}
void InspectionDialog::onButtonMinClicked()
{
	if (Qt::Tool == (windowFlags() & Qt::Tool))
	{
		hide();
	}
	else
	{
		showMinimized();
	}
}
void InspectionDialog::onButtonMaxClicked()
{
	if (max)
	{
		this->setGeometry(location);
	}
	else
	{
		location = this->geometry();
		this->setGeometry(qApp->desktop()->availableGeometry());
	}
	max = !max;
}
void InspectionDialog::onButtonCloseClicked()
{
	this->close();
}
void InspectionDialog::on_SelectFile_Clicked(QString path_)
{
	string path = path_.toLocal8Bit().toStdString();
	cv::Mat src = cv::imread(path.c_str());
	fr_view->setImage(src);
	fr_view->adaptToWindow(true);
	m_inspection->GetAcquisition()->SetCurrentMat(src);
	m_inspection->execute();
	toolBlockWidget->updateTree();
	updateResult();
}
void InspectionDialog::on_Next_btn_Clicked(QString path_)
{
	m_path = path_.toLocal8Bit().toStdString();
	cv::Mat src = cv::imread(m_path.c_str());
	fr_view->setImage(src);
	fr_view->adaptToWindow(true);
	m_inspection->GetAcquisition()->SetCurrentMat(src);
	m_inspection->execute();
	toolBlockWidget->updateTree();
	updateResult();
}
void InspectionDialog::on_Last_btn_Clicked(QString path_)
{
	m_path = path_.toLocal8Bit().toStdString();
	cv::Mat src = cv::imread(m_path.c_str());
	fr_view->setImage(src);
	fr_view->adaptToWindow(true);
	m_inspection->GetAcquisition()->SetCurrentMat(src);
	m_inspection->execute();
	toolBlockWidget->updateTree();
	updateResult();
}
void InspectionDialog::on_Start_btn_Clicked()
{
	runFlag = true;
	ready = true;
	std::thread *autoThread = new std::thread(bind(&InspectionDialog::AutoRun, this));
}
void InspectionDialog::on_Stop_btn_Clicked()
{
	runFlag = false;
	ready = false;
}
void InspectionDialog::on_DisplaySettingOK()
{
	showAllOutputItems();
}
void InspectionDialog::on_excuteFinished_Triggered()
{
	toolBlockWidget->updateTree();
	updateResult();
	m_fileWidget->on_Next_btn_Clicked();
	ready = true;
}
void InspectionDialog::makeToolBar()
{
	toolBar = new QToolBar();
	toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	toolBar->setContentsMargins(0, 0, 0, 0);
	PlayImage_ac = new QAction("Play Image");
	PlayImage_ac->setIcon(QIcon(":/CV_TEAM/icons/Openfile.png"));
	PlayCamera_ac = new QAction("Play Camera");
	PlayCamera_ac->setIcon(QIcon(":/CV_TEAM/icons/Recorder.png"));
	Repeat_ac = new QAction("Repeat");
	Repeat_ac->setIcon(QIcon(":/CV_TEAM/icons/execute.png"));
	Display_ac = new QAction("Display");
	Display_ac->setIcon(QIcon(":/CV_TEAM/icons/display.png"));
	Output_ac = new QAction("Output");
	Output_ac->setIcon(QIcon(":/CV_TEAM/icons/output.png"));
	Exit_ac = new QAction(" Exit ");
	Exit_ac->setIcon(QIcon(":/CV_TEAM/icons/exit.png"));
	this->toolBar->addAction(PlayImage_ac);
	this->toolBar->addAction(PlayCamera_ac);
	this->toolBar->addAction(Repeat_ac);
	this->toolBar->addAction(Display_ac);
	this->toolBar->addAction(Output_ac);
	this->toolBar->addSeparator();
	this->toolBar->addAction(Exit_ac);
	toolBar->setStyleSheet("QToolBar{color:black}"
		"QToolBar{background-color:#E4E4E4}"
		"QToolBar{border:0px}"
		"QToolBar{border-radius:5px}"
		"QToolBar{padding:2px 4px}"
		);
	connect(PlayImage_ac, SIGNAL(triggered()), this, SLOT(on_action_PlayImage_triggered()));
	connect(PlayCamera_ac, SIGNAL(triggered()), this, SLOT(on_action_PlayCamera_triggered()));
	connect(Repeat_ac, SIGNAL(triggered()), this, SLOT(on_action_Repeat_triggered()));
	connect(Display_ac, SIGNAL(triggered()), this, SLOT(on_action_Display_ac_triggered()));
	connect(Output_ac, SIGNAL(triggered()), this, SLOT(on_action_Output_triggered()));
	connect(Exit_ac, SIGNAL(triggered()), this, SLOT(on_action_Exit_triggered()));
}
void InspectionDialog::makeMainLayout()
{
	m_title = new TitleBar(this);
	m_title->setButtonType(ButtonType::MIN_BUTTON);
	m_title->raise();
	m_title->setBackgroundColor(QColor(115, 115, 115, 70));
	m_title->setTitleContent("InspectionDialog", 16);
	m_title->setTitleIcon(":/CV_TEAM/icons/inspection.png");
	location = this->geometry();
	max = false;
	//QLabel *title_lb = new QLabel(" ");
	//title_lb->setFixedHeight(30);
	toolBoxWidget = new ToolBoxWidget();
	toolBlockWidget = new ToolBlockWidget();
	fr_view = new FrameView();
	QVBoxLayout *vlay = new QVBoxLayout();
	QHBoxLayout *hlay = new QHBoxLayout();
	m_fileWidget = new FileWidget();
	QSplitter *splitter1 = new QSplitter(Qt::Vertical);
	splitter1->setHandleWidth(0);
	splitter1->addWidget(fr_view);
	splitter1->addWidget(m_fileWidget);
	QList<int> list1;
	list1.append(700);
	list1.append(150);
	splitter1->setSizes(list1);
	QWidget *widget1 = new QWidget();
	QHBoxLayout *hlay1 = new QHBoxLayout();
	hlay1->addWidget(splitter1);
	widget1->setLayout(hlay1);
	QSplitter *splitter2 = new QSplitter(Qt::Horizontal);
	splitter2->setHandleWidth(0);
	splitter2->addWidget(widget1);
	splitter2->addWidget(toolBoxWidget);
	splitter2->addWidget(toolBlockWidget);
	QList<int> list;
	list.append(630);
	list.append(220);
	list.append(250);
	splitter2->setSizes(list);
	vlay->addWidget(m_title);
	vlay->addWidget(toolBar);
	QLabel *space_lb = new QLabel();
	space_lb->setMaximumHeight(2);
	//space->setFrameStyle(QFrame::Panel);
	space_lb->setFrameShadow(QFrame::Sunken);
	QPalette palette;
	palette.setColor(QPalette::Background, QColor(119, 136, 153));
	space_lb->setAutoFillBackground(true);
	space_lb->setPalette(palette);
	vlay->addWidget(space_lb);
	vlay->addWidget(splitter2);
	vlay->setSpacing(0);
	vlay->setContentsMargins(0, 0, 0, 5);
	this->setLayout(vlay);
	connect(toolBlockWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(on_ToolBlockItemDoubleClicked(QTreeWidgetItem *)));
	connect(m_title, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(m_title, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_title, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_title, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));

	connect(m_fileWidget, SIGNAL(SelectFile(QString)), this, SLOT(on_SelectFile_Clicked(QString)));
	connect(m_fileWidget, SIGNAL(Next_btn_Clicked(QString)), this, SLOT(on_Next_btn_Clicked(QString)));
	connect(m_fileWidget, SIGNAL(Last_btn_Clicked(QString)), this, SLOT(on_Last_btn_Clicked(QString)));
	connect(m_fileWidget, SIGNAL(Start_btn_Clicked()), this, SLOT(on_Start_btn_Clicked()));
	connect(m_fileWidget, SIGNAL(Stop_btn_Clicked()), this, SLOT(on_Stop_btn_Clicked()));
	connect(this, SIGNAL(excuteFinished()), this, SLOT(on_excuteFinished_Triggered()));
}
void InspectionDialog::showAllOutputItems()
{
	if (m_inspection == nullptr)
	{
		return;
	}
	cv::Mat image = m_inspection->GetAcquisition()->GetCurrentMat().clone();
	cv::Mat output;
	output = image;
	if ((image.channels() == 1) && (!image.empty()))
	{
		std::vector<cv::Mat> chanels;
		output = cv::Mat::zeros(image.rows, image.cols, CV_8UC3);
		for (size_t i = 0; i < 3; i++)
		{
			chanels.push_back(image);
		}
		cv::merge(chanels, output);
	}
	for each (auto var in m_showItems)
	{
		delete var;
		var = nullptr;
	}
	m_showItems.clear();
	for each (auto child in m_inspection->m_toolBlock->Children())
	{
		BaseOperator* shell = (BaseOperator*)child;
		if (toolModel::Success == shell->GetState())
		{
			for each (auto var in shell->output()->vectorCircle)
			{
				if (!var->GetDisplayChecked())
				{
					CircleSetItem* circleItem = new CircleSetItem();
					circleItem->setFlag(QGraphicsItem::ItemIsMovable, false);
					circleItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
					circleItem->setCircle(var->GetValue());
					QColor color = QColor(var->GetColorR(), var->GetColorG(), var->GetColorB());
					circleItem->setColor(color);
					m_showItems.append(circleItem);
					//double radius = var->GetValue().Radius();
					//std::stringstream s1;
					//s1 << radius;
					//std::stringstream s2;
					//s2 << circle_index;
					////cv::circle(src, cv::Point2d(center_x, center_y), radius, Scalar(0, 255, 0));
					//cv::putText(output, "R" + s2.str() + "=" + s1.str(), Point2d(15, 25 + 25 * index), cv::FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 255, 0), 1);
					//index++;
					//circle_index++;
				}
			}
			for each (auto var in shell->output()->vectorLine)//??
			{
				if (!var->GetDisplayChecked())
				{
					LineSetItem* lineItem = new LineSetItem();
					lineItem->setFlag(QGraphicsItem::ItemIsMovable, false);
					lineItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
					lineItem->setLine(var->GetValue());
					QColor color = QColor(var->GetColorR(), var->GetColorG(), var->GetColorB());
					lineItem->setColor(color);
					m_showItems.append(lineItem);
					//double length = var->GetValue().Length();
					//std::stringstream s1;
					//s1 << length;
					//std::stringstream s2;
					//s2 << line_index;
					////cv::line(src, cv::Point2d(start_point_x, start_point_y), cv::Point2d(end_point_x, end_point_y), Scalar(0, 255, 0));
					//cv::putText(output, "L" + s2.str() + "=" + s1.str(), Point2d(15, 25 + 25 * index), cv::FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 255, 0), 1);
					//index++;
					//line_index++;
				}
			}
			for each (auto var in shell->output()->vectorPoint)
			{
				if (!var->GetDisplayChecked())
				{
					PointSetItem* pointItem = new PointSetItem();
					pointItem->setFlag(QGraphicsItem::ItemIsMovable, false);
					pointItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
					pointItem->setPoint(var->GetValue());
					QColor color = QColor(var->GetColorR(), var->GetColorG(), var->GetColorB());
					pointItem->setColor(color);
					m_showItems.append(pointItem);
					//double point_x = var->GetValue().X();
					//double point_y = var->GetValue().Y();
					//std::stringstream s1;
					//s1 << point_x;
					//std::stringstream s2;
					//s2 << point_y;
					//cv::putText(output, s1.str() + " " + s2.str(), Point2d(15, 25 + 25 * index), cv::FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 255, 0), 1);
					//index++;
				}
			}
			for each (auto var in shell->output()->vectorRegion)
			{
				if (!var->GetDisplayChecked())
					shell->drawFeatures(output);
			}
			for each (auto var in shell->output()->vectorFeaturePoints)
			{
				if (!var->GetDisplayChecked())
					shell->drawFeatures(output);
				else
				{
					return;
				}
			}
			for each (auto var in shell->output()->vectorString)
			{
				if (!var->GetDisplayChecked())
				{
					TextItem* textItem = new TextItem(0, 0, 18, var->GetValue());
					textItem->setFlag(QGraphicsItem::ItemIsMovable, false);
					textItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
					QColor color = QColor(var->GetColorR(), var->GetColorG(), var->GetColorB());
					textItem->setColor(color);
					m_showItems.append(textItem);
				}
			}
		}
	}
	for each (auto var in m_showItems)
	{
		this->fr_view->getScene()->addItem(var);
	}
	if (output.empty())
		return;
	this->fr_view->setImage(output);
}
void InspectionDialog::updateResult()
{
	if (m_inspection == nullptr)
	{
		return;
	}
	for (int i = 0; i < m_inspection->m_toolBlock->Children().size(); i++)
	{
		QModelIndex index = this->toolBlockWidget->model()->index(i, 0);
		this->toolBlockWidget->setCurrentIndex(index);
		BaseOperator* shell = (BaseOperator*)m_inspection->m_toolBlock->GetChild(i);
		NodeState state = shell->GetState();
		QTreeWidgetItem* item = this->toolBlockWidget->currentItem();
		switch (state)
		{
		case toolModel::Idel:
			item->setIcon(0, QIcon(":/CV_TEAM/icons/state_yellow.png"));
			break;
		case toolModel::Success:
			item->setIcon(0, QIcon(":/CV_TEAM/icons/state_green.png"));
			break;
		case toolModel::Failure:
			item->setIcon(0, QIcon(":/CV_TEAM/icons/state_red.png"));
			break;
		case toolModel::Running:
			break;
		}
		this->toolBlockWidget->currentItem()->setText(1, QString::number(shell->getDuration()) + " ms");
	}
	showAllOutputItems();
}

void InspectionDialog::AutoRun()
{
	while (runFlag)
	{
		if (ready)
		{
			std::lock_guard<std::mutex> mtx_locker(mtx);
			m_inspection->execute();
			emit excuteFinished();
			Sleep(500);
			ready = false;
		}
		Sleep(100);
	}


}
