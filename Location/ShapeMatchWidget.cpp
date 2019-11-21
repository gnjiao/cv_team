#include "ShapeMatchWidget.h"

ShapeMatchWidget::ShapeMatchWidget(QWidget *parent)
	: QDialog(parent)
{
	this->setWindowTitle("ShapeMatchWidget");
	this->resize(1000, 800);
	makeToolBar();
	makeWidget();
	makeLayout();
	makeBaseWidget();
	makeItemWidget();
	initWidget();
	Eraser = new EraserItem(50, 50, 50, 50);
	ROI = new ROIItem(150, 150, 150, 150);
	Search_tool = new SearchTool(200, 200, 200, 200);
	this->fr_view->getScene()->addItem(Eraser);
	this->fr_view->getScene()->addItem(ROI);
	this->fr_view->getScene()->addItem(Search_tool);
	Eraser->QGraphicsItem::setVisible(false);
	ROI->setVisible(false);
	CalOperator = new ShapeMatchOperator();
	CalOperator->initOutput();

}
ShapeMatchWidget::~ShapeMatchWidget()
{
	delete CalOperator;
}
void ShapeMatchWidget::setRefImage(QImage Image)
{
	this->RefImage = Image;
}
void ShapeMatchWidget::setOperator(ShapeMatchOperator * opra)
{
	this->SaveOperator = opra;
	syncOperator(SaveOperator, CalOperator);
	CalOperator->setInspectionKey(opra->getInspectionKey());
	CalOperator->loadTemplate();
	if (CalOperator->m_createTemplateParam.isCreated)
	{
		CalOperator->createTemplate();
	}
	//ROI
	//setRoiChecked(m_shell2->roiChecked);
	//this->setROI(m_shell2->ROI);

	//show create param
	//this->Search_tool->s(CalOperator->m_createTemplateParam.roi);
	this->MinThresh_sp->setValue(CalOperator->m_createTemplateParam.minThreshold);
	this->MaxThresh_sp->setValue(CalOperator->m_createTemplateParam.maxThreshold);
	m_featurePoints.clear();
	for each (auto var in CalOperator->m_createTemplateParam.featurePoints)
	{
		m_featurePoints.push_back(cv::Point(var.X(), var.Y()));
	}
	//show find param
	this->Greedness_sp->setValue(CalOperator->m_findTemplateParam.greediness);
	this->SearchCount_sp->setValue(CalOperator->m_findTemplateParam.maxMatchNum);
	this->MaxAngle_sp->setValue(CalOperator->m_findTemplateParam.maxAngle);
	this->MinAngle_sp->setValue(CalOperator->m_findTemplateParam.minAngle);
	this->MaxScale_sp->setValue(CalOperator->m_findTemplateParam.maxScale);
	this->MinScale_sp->setValue(CalOperator->m_findTemplateParam.minScale);
	this->MinScore_sp->setValue(CalOperator->m_findTemplateParam.minScore);
	if (CalOperator->input_mat)
	{
		m_imgCurrent = CalOperator->input_mat->GetValue();
		this->fr_view->setImage(m_imgCurrent);
	}
	updateOutput();
}
InputBox * ShapeMatchWidget::getImageComboBox()
{
	return this->image_cb;
}
void ShapeMatchWidget::on_action_openFile_triggered()
{
	if (action_edit->isChecked())
		return;
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
	fr_view->setImage(fileImg);
	m_imgCurrent = fileImg;
}
void ShapeMatchWidget::on_action_play_triggered()
{
	cv::Mat src;
	if (this->action_edit->isChecked())
	{
		src = CalOperator->input_refMat->GetValue();
	}
	else
	{
		src = m_imgCurrent.clone();
	}
	double minimumScore = this->MinScore_sp->value();
	double greediness = this->Greedness_sp->value();
	double startAngle = this->MinAngle_sp->value();
	double endAngle = this->MaxAngle_sp->value();
	double minScale = this->MinScale_sp->value();
	double maxScale = this->MaxScale_sp->value();
	int maxMatchNum = this->SearchCount_sp->value();
	if (CalOperator->m_createTemplateParam.isCreated == false)
	{
		botMsgBox->setText(tr("No template was created"));
		return;
	}
	if (0 != CalOperator->setFindTemplateParams(minimumScore, greediness, startAngle, endAngle, minScale, maxScale, maxMatchNum))
	{
		botMsgBox->setText(tr("Parameter Error"));
		return;
	}
	CalOperator->ROI_Checked = this->ROI_cb->isChecked();
	CalOperator->setROI(this->ROI->getRect());
	if (CalOperator->test(src) != NodeState::Success)
	{
		botMsgBox->setText(tr("Search Fail"));
		TableWidget->clearContents();
		this->fr_view->setImage(m_imgCurrent);
		return;
	}
	updateOutput();
}
void ShapeMatchWidget::on_action_edit_triggered(bool checked)
{
	this->Search_tool->setVisible(checked);
	if (checked)
	{
		action_edit->setIcon(QIcon(":/CV_TEAM/icons/edit_red.png"));
		if (CalOperator->input_refMat)
		{
			this->Search_tool->setVisible(true);
			if (CalOperator->input_refMat->GetValue().channels() == 1)
				cv::cvtColor(CalOperator->input_refMat->GetValue(), m_imgShow, CV_GRAY2BGR);
			else
				CalOperator->input_refMat->GetValue().copyTo(m_imgShow);
			DrawPointsOnPic(m_featurePoints, m_imgShow, m_color_creatTemplate);
			this->fr_view->setImage(m_imgShow);
		}
	}
	else
	{
		action_edit->setIcon(QIcon(":/CV_TEAM/icons/edit_black.png"));
		Search_tool->setVisible(false);
		Eraser_cb->setChecked(false);
		Eraser->QGraphicsItem::setVisible(false);
		this->fr_view->setImage(CalOperator->input_mat->GetValue());
	}
}
void ShapeMatchWidget::on_action_edit_finish_triggered()
{
	//geo::Rect2D SearchTool = this->Search_tool->getRect();
	int lowThresh = this->MinThresh_sp->value();
	int highThresh = this->MaxThresh_sp->value();
	cv::Mat refMat = m_imgCurrent;
	if (CalOperator->input_refMat->GetValue().channels() == 1)
		cv::cvtColor(CalOperator->input_refMat->GetValue(), refMat, CV_GRAY2BGR);
	else
		CalOperator->input_refMat->GetValue().copyTo(refMat);
	CalOperator->createTemplate(refMat, this->Search_tool->getRect(), m_featurePoints, lowThresh, highThresh);
	CalOperator->createTemplate();
	
	syncOperator(CalOperator, SaveOperator);
	SaveOperator->saveTemplate();
	SaveOperator->save();
	botMsgBox->setText("Template Saved!");
}
void ShapeMatchWidget::on_action_findFeaturePoint_triggered()
{
	geo::Rect2D SearchTool = Search_tool->getRect();
	if (CalOperator->input_refMat == nullptr)
		return;
	if (!CalOperator->input_refMat->GetValue().data)
		return;
	int lowThresh = this->MinThresh_sp->value();
	int highThresh = this->MaxThresh_sp->value();
	cv::Mat refMat = CalOperator->input_refMat->GetValue();
	if (CalOperator->input_refMat->GetValue().channels() > 1)
	{
		cv::cvtColor(CalOperator->input_refMat->GetValue(), refMat, CV_BGR2GRAY);
	}
	else
	{
		CalOperator->input_refMat->GetValue().copyTo(refMat);
	}
	if (0 != CalOperator->getFeaturePoints(refMat, SearchTool, lowThresh, highThresh, m_featurePoints))
	{
		botMsgBox->setText("Search Feature Points Fail!");
		return;
	}
	if (!m_featurePoints.size())
	{
		botMsgBox->setText("Feature Points Empty!");
		return;
	}
	if (CalOperator->input_refMat->GetValue().channels() < 3)
		cv::cvtColor(CalOperator->input_refMat->GetValue(), refMat, CV_GRAY2BGR);
	else
		CalOperator->input_refMat->GetValue().copyTo(refMat);
	DrawPointsOnPic(m_featurePoints, refMat, m_color_creatTemplate);
	this->botMsgBox->setText("Search Feature Points Success!");
	this->fr_view->setImage(refMat);
}
void ShapeMatchWidget::on_image_cb_Changed(int index)
{
}
void ShapeMatchWidget::on_MinScore_sl_Changed(int value)
{
	MinScore_sp->setValue(value / 100.0);
}
void ShapeMatchWidget::on_SearchCount_sl_Changed(int value)
{
	SearchCount_sp->setValue(value);
}
void ShapeMatchWidget::on_MinAngle_sl_Changed(int value)
{
	MinAngle_sp->setValue(value);
}
void ShapeMatchWidget::on_MaxAngle_sl_Changed(int value)
{
	MaxAngle_sp->setValue(value);
}
void ShapeMatchWidget::on_MinScale_sl_Changed(int value)
{
	MinScale_sp->setValue(value / 100.0);
}
void ShapeMatchWidget::on_MaxScale_sl_Changed(int value)
{
	MaxScale_sp->setValue(value / 100.0);
}
void ShapeMatchWidget::on_Greedness_sl_Changed(int value)
{
	Greedness_sp->setValue(value / 100.0);
}
void ShapeMatchWidget::on_MinThresh_sl_Changed(int value)
{
	MinThresh_sp->setValue(value);

}
void ShapeMatchWidget::on_MaxThresh_sl_Changed(int value)
{
	MaxThresh_sp->setValue(value);
}
void ShapeMatchWidget::on_MinScore_Changed(int value)
{
	MinScore_sl->setValue(value * 100);
	CalOperator->m_findTemplateParam.minScore = value;
}
void ShapeMatchWidget::on_SearchCount_Changed(int value)
{
	SearchCount_sl->setValue(value);
	
}
void ShapeMatchWidget::on_MinAngle_Changed(int value)
{
	MinAngle_sl->setValue(value);
	CalOperator->m_findTemplateParam.minAngle = value;
}
void ShapeMatchWidget::on_MaxAngle_Changed(int value)
{
	MaxAngle_sl->setValue(value);
	CalOperator->m_findTemplateParam.maxAngle = value;
}
void ShapeMatchWidget::on_MinScale_Changed(int value)
{
	MinScale_sl->setValue(value * 100);
	CalOperator->m_findTemplateParam.minScale = value;
}
void ShapeMatchWidget::on_MaxScale_Changed(int value)
{
	MaxScale_sl->setValue(value * 100);
	CalOperator->m_findTemplateParam.maxScale = value;
}
void ShapeMatchWidget::on_Greedness_Changed(int value)
{
	Greedness_sl->setValue(value * 100);
	CalOperator->m_findTemplateParam.greediness = value;
}
void ShapeMatchWidget::on_MinThresh_Changed(int value)
{
	MinThresh_sl->setValue(value);
	
}
void ShapeMatchWidget::on_MaxThresh_Changed(int value)
{
	MaxThresh_sl->setValue(value);
}
void ShapeMatchWidget::on_SearchTool_cb_Clicked(bool checked)
{
	if (checked)
	{
		this->Search_tool->setVisible(true);
	}
	else
	{
		this->Search_tool->setVisible(false);
	}
}
void ShapeMatchWidget::on_SearchTool_btn_Clicked(bool checked)
{
	m_pColorDialog = new QColorDialog();
	connect(m_pColorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(on_SearchTool_getcolor(QColor)));
	m_pColorDialog->exec();
}
void ShapeMatchWidget::on_ROI_cb_Clicked(bool checked)
{
	if (checked)
	{
		this->ROI->setVisible(true);
	}
	else
	{
		this->ROI->setVisible(false);
	}
}
void ShapeMatchWidget::on_ROI_btn_Clicked(bool checked)
{
	m_pColorDialog = new QColorDialog();
	connect(m_pColorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(on_ROI_getcolor(QColor)));
	m_pColorDialog->exec();
}
void ShapeMatchWidget::on_Eraser_cb_Clicked(bool checked)
{
	if (checked)
	{
		this->Eraser->QGraphicsItem::setVisible(true);
	}
	else
	{
		this->Eraser->QGraphicsItem::setVisible(false);
	}
}
void ShapeMatchWidget::on_Eraser_btn_Clicked(bool checked)
{
	m_pColorDialog = new QColorDialog();
	connect(m_pColorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(on_Eraser_getcolor(QColor)));
	m_pColorDialog->exec();
}
void ShapeMatchWidget::on_SearchTool_getcolor(QColor color)
{
	this->Search_tool->setColor(color);
}
void ShapeMatchWidget::on_ROI_getcolor(QColor color)
{
	this->ROI->setColor(color);
}
void ShapeMatchWidget::on_Eraser_getcolor(QColor color)
{
	this->Eraser->setColor(color);
}
void ShapeMatchWidget::on_OK_btn_Clicked(bool b)
{
	syncOperator(CalOperator, SaveOperator);
	SaveOperator->save();
	SaveOperator->saveTemplate();
	this->close();
}
void ShapeMatchWidget::on_Cancel_btn_Clicked(bool b)
{
	this->close();
}
void ShapeMatchWidget::doOpenFile(cv::Mat src)
{
}
void ShapeMatchWidget::makeToolBar()
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
	action_edit_finish = new QAction(tr("Edit Finish"), this);
	action_edit_finish->setIcon(QIcon(":/CV_TEAM/icons/Confirm.png"));
	action_edit_finish->setEnabled(true);
	action_findFeaturePoint = new QAction(tr("Find Feature Points"), this);
	action_findFeaturePoint->setIcon(QIcon(":/CV_TEAM/icons/feature.png"));
	action_findFeaturePoint->setEnabled(true);
	toolBar->addAction(action_openFile);
	toolBar->addAction(action_play);
	toolBar->addAction(action_edit);
	toolBar->addAction(action_edit_finish);
	//toolBar->addSeparator();
	toolBar->addAction(action_findFeaturePoint);
}
void ShapeMatchWidget::makeWidget()
{
	TabWidget = new QTabWidget(this);
	TabWidget->setTabPosition(QTabWidget::North);
	QFrame* page1 = new QFrame(TabWidget);
	BaseParamWidget = TabWidget->widget(TabWidget->addTab(page1, tr("BaseParam")));
	QFrame* page2 = new QFrame(TabWidget);
	SetItemWidget = TabWidget->widget(TabWidget->addTab(page2, tr("SetItem")));
	QFrame* page3 = new QFrame(TabWidget);
	OutputWidget = TabWidget->widget(TabWidget->addTab(page3, tr("Output")));
	TableWidget = new QTableWidget(10, 6);
	QStringList header;
	header << "Score" << "Delta X" << "Delta Y" << "Rotate Angle" << "Scale" << "Duration";
	TableWidget->setHorizontalHeaderLabels(header);
	for (size_t i = 0;i < 6;i++)
	{
		TableWidget->setColumnWidth(i, 112);
	}


	TableWidget->setItem(0, 0, new QTableWidgetItem("Jan"));
	TableWidget->setItem(1, 0, new QTableWidgetItem("Feb"));
	TableWidget->setItem(2, 0, new QTableWidgetItem("Mar"));
	TableWidget->item(0, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	TableWidget->item(0, 0)->setBackgroundColor(QColor(119, 136, 153, 50));
}
void ShapeMatchWidget::makeLayout()
{
	fr_view = new FrameView();
	QVBoxLayout *vlay = new QVBoxLayout();
	vlay->addWidget(toolBar);
	vlay->addWidget(TabWidget);
	QVBoxLayout *vlay1 = new QVBoxLayout();
	QSplitter *splitter = new QSplitter(Qt::Vertical);
	splitter->setHandleWidth(0);
	QList<int> list0;
	list0.append(600);
	list0.append(200);
	splitter->addWidget(fr_view);
	splitter->addWidget(TableWidget);
	splitter->setSizes(list0);
	QWidget *leftWidget = new QWidget();
	leftWidget->setLayout(vlay);
	QSplitter *splitter2 = new QSplitter(Qt::Horizontal);
	splitter2->setHandleWidth(0);
	splitter2->addWidget(leftWidget);
	splitter2->addWidget(splitter);
	QList<int> list;
	list.append(270);
	list.append(730);
	splitter2->setSizes(list);
	botMsgBox = new QLabel("message label");
	botMsgBox->setFrameStyle(QFrame::Panel);
	botMsgBox->setFixedHeight(30);
	botMsgBox->setFrameShadow(QFrame::Sunken);

	vlay1->addWidget(splitter2);
	QFont ft;
	ft.setPointSize(18);
	botMsgBox->setFont(ft);
	QPalette pa;
	pa.setColor(QPalette::WindowText, Qt::blue);
	botMsgBox->setPalette(pa);
	botMsgBox->setText("Welcome To Shape Match!");
	Cancel_btn = new QPushButton("Cancel");
	Cancel_btn->setMaximumWidth(90);
	OK_btn = new QPushButton("OK");
	OK_btn->setMaximumWidth(90);
	QHBoxLayout *hlay1 = new QHBoxLayout();

	hlay1->addWidget(botMsgBox);
	hlay1->addWidget(Cancel_btn);
	hlay1->addWidget(OK_btn);
	vlay1->addLayout(hlay1);
	this->setLayout(vlay1);
}
void ShapeMatchWidget::makeBaseWidget()
{
	QPalette pa;
	pa.setColor(QPalette::WindowText, Qt::black);
	QFont ft;
	QHBoxLayout *hlay = new QHBoxLayout();
	QVBoxLayout *vlay = new QVBoxLayout();
	QLabel *image_label = new QLabel("Input image:");
	image_label->setPalette(pa);
	image_cb = new InputBox();
	image_cb->addItem("null");
	image_cb->addItem("last");
	image_cb->addItem("next");
	hlay->addWidget(image_label);
	hlay->addWidget(image_cb);
	hlay->setAlignment(Qt::AlignVCenter);
	QGroupBox *input_gb = new QGroupBox("Operator Input");
	input_gb->setLayout(hlay);
	QVBoxLayout *vlay2 = new QVBoxLayout();
	vlay2->addWidget(input_gb);
	QGroupBox *CorePara_gb = new QGroupBox("Core Parameter");
	vlay2->addWidget(CorePara_gb);
	CorePara_gb->setLayout(vlay);
	QGroupBox *SearchCount_gb = new QGroupBox("Search Count");
	SearchCount_gb->setFont(ft);
	SearchCount_sp = new QSpinBox();
	SearchCount_sl = new QSlider();
	SearchCount_sl->setOrientation(Qt::Horizontal);
	QHBoxLayout *hlay7 = new QHBoxLayout();
	hlay7->addWidget(SearchCount_sl);
	hlay7->addWidget(SearchCount_sp);
	SearchCount_gb->setLayout(hlay7);
	vlay->addWidget(SearchCount_gb);
	QGroupBox *MinScore_gb = new QGroupBox("Minimun Score");
	MinScore_gb->setFont(ft);
	MinScore_sp = new QDoubleSpinBox();
	MinScore_sl = new QSlider();
	MinScore_sl->setOrientation(Qt::Horizontal);
	QHBoxLayout *hlay6 = new QHBoxLayout();
	hlay6->addWidget(MinScore_sl);
	hlay6->addWidget(MinScore_sp);
	MinScore_gb->setLayout(hlay6);
	vlay->addWidget(MinScore_gb);
	QGroupBox *MinAngle_gb = new QGroupBox("Minimun Angle");
	MinAngle_gb->setFont(ft);
	MinAngle_sp = new QSpinBox();
	MinAngle_sl = new QSlider();
	MinAngle_sl->setOrientation(Qt::Horizontal);
	QHBoxLayout *hlay8 = new QHBoxLayout();
	hlay8->addWidget(MinAngle_sl);
	hlay8->addWidget(MinAngle_sp);
	MinAngle_gb->setLayout(hlay8);
	vlay->addWidget(MinAngle_gb);
	QGroupBox *MaxAngle_gb = new QGroupBox("Maximun Angle");
	MaxAngle_gb->setFont(ft);
	MaxAngle_sp = new QSpinBox();
	MaxAngle_sl = new QSlider();
	MaxAngle_sl->setOrientation(Qt::Horizontal);
	QHBoxLayout *hlay9 = new QHBoxLayout();
	hlay9->addWidget(MaxAngle_sl);
	hlay9->addWidget(MaxAngle_sp);
	MaxAngle_gb->setLayout(hlay9);
	vlay->addWidget(MaxAngle_gb);
	QGroupBox *MinScale_gb = new QGroupBox("Minimun Scale");
	MinScale_gb->setFont(ft);
	MinScale_sp = new QDoubleSpinBox();
	MinScale_sl = new QSlider();
	MinScale_sl->setOrientation(Qt::Horizontal);
	QHBoxLayout *hlay10 = new QHBoxLayout();
	hlay10->addWidget(MinScale_sl);
	hlay10->addWidget(MinScale_sp);
	MinScale_gb->setLayout(hlay10);
	vlay->addWidget(MinScale_gb);
	QGroupBox *MaxScale_gb = new QGroupBox("Maximun Scale");
	MaxScale_gb->setFont(ft);
	MaxScale_sp = new QDoubleSpinBox();
	MaxScale_sl = new QSlider();
	MaxScale_sl->setOrientation(Qt::Horizontal);
	QHBoxLayout *hlay11 = new QHBoxLayout();
	hlay11->addWidget(MaxScale_sl);
	hlay11->addWidget(MaxScale_sp);
	MaxScale_gb->setLayout(hlay11);
	vlay->addWidget(MaxScale_gb);
	QGroupBox *Greedness_gb = new QGroupBox("Greedness");
	Greedness_gb->setFont(ft);
	Greedness_sp = new QDoubleSpinBox();
	Greedness_sl = new QSlider();
	Greedness_sl->setOrientation(Qt::Horizontal);
	QHBoxLayout *hlay12 = new QHBoxLayout();
	hlay12->addWidget(Greedness_sl);
	hlay12->addWidget(Greedness_sp);
	Greedness_gb->setLayout(hlay12);
	vlay->addWidget(Greedness_gb);
	ft.setPointSize(14);
	input_gb->setFont(ft);
	input_gb->setPalette(pa);
	input_gb->setMaximumHeight(100);
	CorePara_gb->setFont(ft);
	CorePara_gb->setPalette(pa);
	this->BaseParamWidget->setLayout(vlay2);
}
void ShapeMatchWidget::makeItemWidget()
{
	QPalette pa;
	pa.setColor(QPalette::WindowText, Qt::black);
	QFont ft;
	QVBoxLayout *vlay = new QVBoxLayout();
	QGroupBox *Core_para_gb = new QGroupBox("Core Parameter");
	MinThresh_sp = new QSpinBox();
	MinThresh_sl = new QSlider();
	MinThresh_sl->setOrientation(Qt::Horizontal);
	QHBoxLayout *hlay1 = new QHBoxLayout();
	hlay1->addWidget(MinThresh_sl);
	hlay1->addWidget(MinThresh_sp);
	QGroupBox *MinThresh_gb = new QGroupBox("Min Thresh");
	MinThresh_gb->setFont(ft);

	MinThresh_gb->setLayout(hlay1);
	MaxThresh_sp = new QSpinBox();
	MaxThresh_sl = new QSlider();
	MaxThresh_sl->setOrientation(Qt::Horizontal);
	QHBoxLayout *hlay2 = new QHBoxLayout();
	hlay2->addWidget(MaxThresh_sl);
	hlay2->addWidget(MaxThresh_sp);
	QGroupBox *MaxThresh_gb = new QGroupBox("Max Thresh");
	MaxThresh_gb->setFont(ft);
	MaxThresh_gb->setLayout(hlay2);
	QVBoxLayout *vlay1 = new QVBoxLayout();
	vlay1->addWidget(MinThresh_gb);
	vlay1->addWidget(MaxThresh_gb);
	Core_para_gb->setLayout(vlay1);
	Core_para_gb->setMaximumHeight(200);
	vlay->addWidget(Core_para_gb);
	QGroupBox *settingItem_gb = new QGroupBox("Setting Item");
	SearchTool_cb = new QCheckBox("Search Tool");
	SearchTool_btn = new QPushButton("...");
	SearchTool_btn->setMaximumWidth(50);
	QHBoxLayout *hlay3 = new QHBoxLayout();
	hlay3->addWidget(SearchTool_cb);
	hlay3->addWidget(SearchTool_btn);
	ROI_cb = new QCheckBox(" ROI ");
	ROI_btn = new QPushButton("...");
	ROI_btn->setMaximumWidth(50);
	QHBoxLayout *hlay4 = new QHBoxLayout();
	hlay4->addWidget(ROI_cb);
	hlay4->addWidget(ROI_btn);
	QGroupBox *Eraser_gb = new QGroupBox("Eraser");
	Eraser_cb = new QCheckBox(" Eraser");
	Eraser_btn = new QPushButton("...");
	Eraser_btn->setMaximumWidth(50);
	QHBoxLayout *hlay5 = new QHBoxLayout();
	hlay5->addWidget(Eraser_cb);
	hlay5->addWidget(Eraser_btn);
	Eraser_sp = new QSpinBox();
	Eraser_sl = new QSlider();
	Eraser_sl->setOrientation(Qt::Horizontal);
	QHBoxLayout *hlay6 = new QHBoxLayout();
	hlay6->addWidget(Eraser_sl);
	hlay6->addWidget(Eraser_sp);
	QVBoxLayout *vlay_eraser = new QVBoxLayout();
	vlay_eraser->addLayout(hlay5);
	vlay_eraser->addLayout(hlay6);
	Eraser_gb->setLayout(vlay_eraser);
	QVBoxLayout *vlay2 = new QVBoxLayout();
	vlay2->addLayout(hlay3);
	vlay2->addLayout(hlay4);
	//vlay2->addWidget(Eraser_gb);
	settingItem_gb->setLayout(vlay2);
	vlay->addWidget(settingItem_gb);
	settingItem_gb->setMaximumHeight(300);
	ft.setPointSize(14);
	Eraser_gb->setFont(ft);
	Core_para_gb->setFont(ft);
	settingItem_gb->setFont(ft);
	vlay->addWidget(Eraser_gb);
	ExtendedGroupBox *Blank_gb = new ExtendedGroupBox("Ref Image");
	Blank_gb->setMinimumHeight(200);
	vlay->addWidget(Blank_gb);
	QLabel *labelPic = new QLabel(Blank_gb);
	labelPic->setFixedWidth(250);
	labelPic->setFixedHeight(250);
	Blank_gb->addWidget(labelPic);

	RefImage.load("C:\\Users\\Administrator\\Desktop\\board-01.png");

	QPixmap pixmap = QPixmap::fromImage(RefImage);

	int with = labelPic->width();

	int height = labelPic->height();

	//QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // ±¥ÂúÌî³ä

	QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // °´±ÈÀýËõ·Å

	labelPic->setPixmap(fitpixmap);
	this->SetItemWidget->setLayout(vlay);
}
void ShapeMatchWidget::initWidget()
{
	image_cb->setDataType(ToolDataType::eMat);

	image_cb->loadItems();
	MinScore_sp->setMinimum(0.1);
	MinScore_sp->setMaximum(1);
	MinScore_sp->setSingleStep(0.1);
	MinScore_sl->setMinimum(1);
	MinScale_sl->setMaximum(100);
	MinScore_sp->setValue(0.9);
	MinScale_sl->setValue(90);
	SearchCount_sp->setMinimum(1);
	SearchCount_sp->setMaximum(100);
	SearchCount_sp->setSingleStep(1);
	SearchCount_sl->setMinimum(1);
	SearchCount_sl->setMaximum(100);
	SearchCount_sp->setValue(1);
	SearchCount_sl->setValue(1);
	MinAngle_sp->setMinimum(-180);
	MinAngle_sp->setMaximum(180);
	MinAngle_sp->setSingleStep(1);
	MinAngle_sl->setMinimum(-180);
	MinAngle_sl->setMaximum(180);
	MinAngle_sp->setValue(-10);
	MinAngle_sl->setValue(-10);
	MaxAngle_sp->setMinimum(-180);
	MaxAngle_sp->setMaximum(180);
	MaxAngle_sp->setSingleStep(1);
	MaxAngle_sl->setMinimum(-180);
	MaxAngle_sl->setMaximum(180);
	MaxAngle_sp->setValue(10);
	MaxAngle_sl->setValue(10);
	MinScale_sp->setMinimum(0.5);
	MinScale_sp->setMaximum(1);
	MinScale_sp->setSingleStep(0.1);
	MinScale_sl->setMinimum(50);
	MinScale_sl->setMaximum(100);
	MinScale_sp->setValue(0.9);
	MinScale_sl->setValue(90);
	MaxScale_sp->setMinimum(1);
	MaxScale_sp->setMaximum(2);
	MaxScale_sp->setSingleStep(0.1);
	MaxScale_sl->setMinimum(100);
	MaxScale_sl->setMaximum(200);
	MaxScale_sp->setValue(1.1);
	MaxScale_sl->setValue(110);
	Greedness_sp->setMinimum(0);
	Greedness_sp->setMaximum(0.99);
	Greedness_sp->setSingleStep(0.1);
	Greedness_sl->setMinimum(0);
	Greedness_sl->setMaximum(99);
	Greedness_sp->setValue(0.7);
	Greedness_sl->setValue(70);
	MinThresh_sp->setMinimum(1);
	MinThresh_sp->setMaximum(255);
	MinThresh_sp->setSingleStep(1);
	MinThresh_sl->setMinimum(1);
	MinThresh_sl->setMaximum(255);
	MinThresh_sp->setValue(10);
	MinThresh_sl->setValue(10);
	MaxThresh_sp->setMinimum(1);
	MaxThresh_sp->setMaximum(255);
	MaxThresh_sp->setSingleStep(1);
	MaxThresh_sl->setMinimum(1);
	MaxThresh_sl->setMaximum(255);
	MaxThresh_sp->setValue(30);
	MaxThresh_sl->setValue(30);
	SearchTool_cb->setChecked(true);
	connect(action_openFile, SIGNAL(triggered(bool)), this, SLOT(on_action_openFile_triggered()));
	connect(action_play, SIGNAL(triggered(bool)), this, SLOT(on_action_play_triggered()));
	connect(action_edit, SIGNAL(triggered(bool)), this, SLOT(on_action_edit_triggered(bool)));
	connect(action_edit_finish, SIGNAL(triggered(bool)), this, SLOT(on_action_edit_finish_triggered()));
	connect(action_findFeaturePoint, SIGNAL(triggered(bool)), this, SLOT(on_action_findFeaturePoint_triggered()));
	connect(image_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(on_image_cb_Changed(int)));
	connect(MinScore_sl, SIGNAL(valueChanged(int)), this, SLOT(on_MinScore_sl_Changed(int)));
	connect(SearchCount_sl, SIGNAL(valueChanged(int)), this, SLOT(on_SearchCount_sl_Changed(int)));
	connect(MinAngle_sl, SIGNAL(valueChanged(int)), this, SLOT(on_MinAngle_sl_Changed(int)));
	connect(MaxAngle_sl, SIGNAL(valueChanged(int)), this, SLOT(on_MaxAngle_sl_Changed(int)));
	connect(MinScale_sl, SIGNAL(valueChanged(int)), this, SLOT(on_MinScale_sl_Changed(int)));
	connect(MaxScale_sl, SIGNAL(valueChanged(int)), this, SLOT(on_MaxScale_sl_Changed(int)));
	connect(Greedness_sl, SIGNAL(valueChanged(int)), this, SLOT(on_Greedness_sl_Changed(int)));
	connect(MinThresh_sl, SIGNAL(valueChanged(int)), this, SLOT(on_MinThresh_sl_Changed(int)));
	connect(MaxThresh_sl, SIGNAL(valueChanged(int)), this, SLOT(on_MaxThresh_sl_Changed(int)));
	connect(MinScore_sp, SIGNAL(valueChanged(int)), this, SLOT(on_MinScore_Changed(int)));
	connect(SearchCount_sp, SIGNAL(valueChanged(int)), this, SLOT(on_SearchCount_Changed(int)));
	connect(MinAngle_sp, SIGNAL(valueChanged(int)), this, SLOT(on_MinAngle_Changed(int)));
	connect(MaxAngle_sp, SIGNAL(valueChanged(int)), this, SLOT(on_MaxAngle_Changed(int)));

	connect(MinScale_sp, SIGNAL(valueChanged(int)), this, SLOT(on_MinScale_Changed(int)));

	connect(MaxScale_sp, SIGNAL(valueChanged(int)), this, SLOT(on_MaxScale_Changed(int)));
	connect(Greedness_sp, SIGNAL(valueChanged(int)), this, SLOT(on_Greedness_Changed(int)));
	connect(MinThresh_sp, SIGNAL(valueChanged(int)), this, SLOT(on_MinThresh_Changed(int)));
	connect(MaxThresh_sp, SIGNAL(valueChanged(int)), this, SLOT(on_MaxThresh_Changed(int)));
	connect(SearchTool_cb, SIGNAL(clicked(bool)), this, SLOT(on_SearchTool_cb_Clicked(bool)));
	connect(SearchTool_btn, SIGNAL(clicked(bool)), this, SLOT(on_SearchTool_btn_Clicked(bool)));
	connect(ROI_cb, SIGNAL(clicked(bool)), this, SLOT(on_ROI_cb_Clicked(bool)));
	connect(ROI_btn, SIGNAL(clicked(bool)), this, SLOT(on_ROI_btn_Clicked(bool)));
	connect(Eraser_cb, SIGNAL(clicked(bool)), this, SLOT(on_Eraser_cb_Clicked(bool)));
	connect(Eraser_btn, SIGNAL(clicked(bool)), this, SLOT(on_Eraser_btn_Clicked(bool)));
	connect(OK_btn, SIGNAL(clicked(bool)), this, SLOT(on_OK_btn_Clicked(bool)));
	connect(Cancel_btn, SIGNAL(clicked(bool)), this, SLOT(on_Cancel_btn_Clicked(bool)));

}
void ShapeMatchWidget::updateOutput()
{
	for each(auto var in CalOperator->output_allResults)
	{
		var.featurePoints.clear();
	}
	if (CalOperator->input_refMat == nullptr)
	{
		botMsgBox->setText(tr("Refrence Image is NULL"));
		return;
	}
	if (CalOperator->input_mat == nullptr)
	{
		botMsgBox->setText("Input Image is NULL");
		return;
	}
	cv::Mat src;
	if (this->action_edit->isChecked())
	{
		src = CalOperator->input_refMat->GetValue();
	}
	else
	{
		src = m_imgCurrent;
	}
	if (src.channels() < 3)
		cv::cvtColor(src, m_imgShow, CV_GRAY2BGR);
	else
		src.copyTo(m_imgShow);
	for (size_t i = 0; i < CalOperator->output_allResults.size(); i++)
	{

		botMsgBox->setText(tr("Search Success!"));
		TemplateResult result = CalOperator->output_allResults.at(i);
		if (!result.featurePoints.size())
		{
			botMsgBox->setText(tr("Search Fail!"));
			return;
		}
		TableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(result.score)));
		TableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(result.translation.X()) + " Pixel"));
		TableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(result.translation.Y()) + " Pixel"));
		TableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(result.rotation.Degrees()) + " ¡ã"));
		TableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(result.scale)));
		TableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(result.timeSpan) + " ms"));
		for (size_t j = 0;j < 6;j++)
		{
			TableWidget->item(i, j)->setBackgroundColor(QColor(119, 136, 153, 50));
			TableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		}
		vector<cv::Point> featurePoints;
		featurePoints.clear();
		for each (auto var in result.featurePoints)
		{
			featurePoints.push_back(cv::Point(var.X(), var.Y()));
		}
		DrawPointsOnPic(featurePoints, m_imgShow, m_color_findTemplate);
	}
	this->fr_view->setImage(m_imgShow);
}
void ShapeMatchWidget::syncOperator(ShapeMatchOperator * AA, ShapeMatchOperator * BB)
{
	BB->m_state = AA->m_state;
	BB->m_errorCode = AA->m_errorCode;
	BB->setDuration(AA->getDuration());
	BB->input_mat = AA->input_mat;
	BB->input_refMat = AA->input_refMat;
	BB->setROI(AA->getROI());
	BB->m_createTemplateParam = AA->m_createTemplateParam;
	BB->m_findTemplateParam = AA->m_findTemplateParam;
	BB->output_best_affineTrans->SetValue(AA->output_best_affineTrans->GetValue());
	BB->output_best_center->SetValue(AA->output_best_center->GetValue());
	BB->output_best_translation->SetValue(AA->output_best_translation->GetValue());
	BB->output_best_rotation->SetValue(AA->output_best_rotation->GetValue());
	BB->output_best_score->SetValue(AA->output_best_score->GetValue());
}

void ShapeMatchWidget::DrawPointsOnPic(vector<cv::Point> pt, cv::Mat & img, cv::Scalar color)
{
	for (size_t i = 0; i < pt.size(); i++)
	{
		cv::drawMarker(img, pt[i], color, cv::MarkerTypes::MARKER_DIAMOND, 3);
	}
}