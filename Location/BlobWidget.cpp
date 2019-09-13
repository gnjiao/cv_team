#include "BlobWidget.h"
#include <functional>
#include <limits>


bool compareFeatures(SIMPLEBLOB_SPACE::CResult &r1, SIMPLEBLOB_SPACE::CResult &r2, int index)
{
	switch (index)
	{
	case 0:
		return r1.location.x > r2.location.x;
	case 1:
		return r1.location.y > r2.location.y;
	case 2:
		return r1.area > r2.area;
	case 3:
		return r1.Inertia > r2.Inertia;
	case 4:
		return r1.Circularity > r2.Circularity;
	case 5:
		return r1.Convexity > r2.Convexity;
	case 6:
		return r1.Rectangularity > r2.Rectangularity;
	case 7:
		return r1.wide > r2.wide;
		break;
	case 8:
		return r1.heiht > r2.heiht;
		break;
	case 9:
		return r1.Compactness > r2.Compactness;
		break;
	case 10:
		return r1.Conlength > r2.Conlength;
		break;
	default:
		return r1.area > r2.area;
	}
}
BlobWidget::BlobWidget(QWidget *parent)
	: QDialog(parent)
{
	this->setWindowTitle("Blob Widegt");
	this->resize(1000, 800);

	makeToolBar();
	makeWidget();
	makeTableWidget();
	makeMainLayout();

	makeBaseParamWidget();
	initWidget();

	CalOperator = new BlobOperator();
}

BlobWidget::~BlobWidget()
{
}

void BlobWidget::on_action_openFile_triggered()
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

	fr_view->setImage(fileImg);

	m_currentImage = fileImg.clone();
}

void BlobWidget::on_action_play_triggered()
{
	
	updateParameter(true);
	cv::threshold(m_currentImage, m_thresholdedImage,Thresh_sp->value(), 255, cv::THRESH_BINARY);

	CalOperator->test(m_thresholdedImage);

	updateOutput();
}

void BlobWidget::on_action_edit_toggled(bool)
{
}

void BlobWidget::on_action_editCancel_triggered()
{
}

void BlobWidget::on_OK_btn_clicked()
{
}

void BlobWidget::on_Cancel_btn_clicked()
{
	this->close();
}

void BlobWidget::on_image_cb_Changed(int)
{
}

void BlobWidget::on_Thresh_sl_Changed(int value)
{
	this->Thresh_sp->setValue(value);
	cv::threshold(m_currentImage, m_thresholdedImage, value, 255, cv::THRESH_BINARY);
	this->fr_view->setImage(m_thresholdedImage);
}

void BlobWidget::on_Thresh_Changed(int value)
{
	this->Thresh_sl->setValue(value);


}

void BlobWidget::on_Layers_cb_Changed(int)
{

}

void BlobWidget::on_Feature_cb_Changed(int index)
{
	auto p = CalOperator->getParameters();
	switch (index)
	{
	case 0:
		Min_sp->setRange(0, INT_MAX);
		Min_sp->setSingleStep(50);
		Max_sp->setRange(0, INT_MAX);
		Max_sp->setSingleStep(50);
		Min_sp->setValue(p.Area.minValue);
		Max_sp->setValue(p.Area.maxValue);
		break;
	case 1:
		Min_sp->setRange(0, 1);
		Min_sp->setSingleStep(0.01);
		Max_sp->setRange(0, 1);
		Max_sp->setSingleStep(0.01);
		Min_sp->setValue(p.Circularity.minValue);
		Max_sp->setValue(p.Circularity.maxValue);
		break;
	case 2:
		Min_sp->setRange(0, 1);
		Min_sp->setSingleStep(0.01);
		Max_sp->setRange(0, 1);
		Max_sp->setSingleStep(0.01);
		Min_sp->setValue(p.Inertia.minValue);
		Max_sp->setValue(p.Inertia.maxValue);
		break;
	case 3:
		Min_sp->setRange(0, 1);
		Min_sp->setSingleStep(0.01);
		Max_sp->setRange(0, 1);
		Max_sp->setSingleStep(0.01);
		Min_sp->setValue(p.Convexity.minValue);
		Max_sp->setValue(p.Convexity.maxValue);
		break;
	case 4:
		Min_sp->setRange(0, 1);
		Min_sp->setSingleStep(0.01);
		Max_sp->setRange(0, 1);
		Max_sp->setSingleStep(0.01);
		Min_sp->setValue(p.Rectangularity.minValue);
		Max_sp->setValue(p.Rectangularity.maxValue);
		break;
	case 5:
		Min_sp->setRange(0, INT_MAX);
		Min_sp->setSingleStep(1);
		Max_sp->setRange(0, INT_MAX);
		Max_sp->setSingleStep(1);
		Min_sp->setValue(p.BolbWide.minValue);
		Max_sp->setValue(p.BolbWide.maxValue);
		break;
	case 6:
		Min_sp->setRange(0, INT_MAX);
		Min_sp->setSingleStep(1);
		Max_sp->setRange(0, INT_MAX);
		Max_sp->setSingleStep(1);
		Min_sp->setValue(p.BlobHeight.minValue);
		Max_sp->setValue(p.BlobHeight.maxValue);
		break;
	case 7:
		Min_sp->setRange(1, INT_MAX);
		Min_sp->setSingleStep(0.1);
		Max_sp->setRange(1, INT_MAX);
		Max_sp->setSingleStep(0.1);
		Min_sp->setValue(p.Compactness.minValue);
		Max_sp->setValue(p.Compactness.maxValue);
		break;
	case 8:
		Min_sp->setRange(0, INT_MAX);
		Min_sp->setSingleStep(1);
		Max_sp->setRange(0, INT_MAX);
		Max_sp->setSingleStep(1);
		Min_sp->setValue(p.Conlength.minValue);
		Max_sp->setValue(p.Conlength.maxValue);
		break;
	default:
		break;
	}
}

void BlobWidget::on_Mode_cb_Changed(int)
{

}

void BlobWidget::on_Max_sp_Changed(int)
{
}

void BlobWidget::on_Min_sp_Changed(int)
{

}

void BlobWidget::on_Add_btn_clicked()
{
	updateFeatureValue(true);
	updateTableWidgetFeatures();
}

void BlobWidget::on_Delete_btn_clicked()
{
	updateFeatureValue(false);

	updateTableWidgetFeatures();
}

void BlobWidget::on_AddAll_btn_clicked()
{
	setAllUes(true);
}

void BlobWidget::on_DeleteAll_btn_clicked()
{
	setAllUes(false);
}

void BlobWidget::on_resultTableWidget_cellClicked(int row, int col)
{
	if (m_currentImage.channels() == 1)
	{
		m_featureImage = cv::Mat::zeros(m_currentImage.rows, m_currentImage.cols, CV_8UC3);
		if (0 <= row&&CalOperator->pblob->m_result.size() > row)
		{
		}
		else
		{
			return;
		}
		CalOperator->pblob->Draw_results(m_featureImage, row);
		typedef cv::Vec3b Pixel;
		cv::Mat tempImage = m_currentImage;
		try
		{
			m_featureImage.forEach<Pixel>([&tempImage](Pixel &p, const int * position) -> void {
				if (p[0] == 0 && p[1] == 0 && p[2] == 0)
				{
					p[0] = p[1] = p[2] = tempImage.at<uchar>(position[0], position[1]);
				}
			});
		}
		catch (cv::Exception& e)
		{
		}
	}
	else if (m_currentImage.channels() == 3)
	{
		m_featureImage = m_currentImage.clone();
		CalOperator->pblob->Draw_results(m_featureImage, row);
	}
	this->fr_view->setImage(m_featureImage);
}

void BlobWidget::on_resultTableHeaderDoubleClicked(int index)
{
	sortIndex = index;
	updateResults(CalOperator->pblob->m_result);
}

void BlobWidget::makeToolBar()
{
	toolBar = new QToolBar;

	toolBar->setStyleSheet("QToolBar{color:black}"

		"QToolBar{background-color:lightgray}"

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

	action_edit->setIcon(QIcon(":/CV_TEAM/icons/edit48.png"));

	action_edit->setAutoRepeat(false);

	action_edit->setCheckable(true);

	action_edit->setChecked(false);

	action_edit->setEnabled(true);



	action_editCancel = new QAction(tr("EditCancel"), this);

	action_editCancel->setIcon(QIcon("Icons\\Close Window_50px.png"));

	action_editCancel->setEnabled(true);





	connect(action_openFile, SIGNAL(triggered(bool)), this, SLOT(on_action_openFile_triggered()));

	connect(action_play, SIGNAL(triggered(bool)), this, SLOT(on_action_play_triggered()));

	connect(action_edit, SIGNAL(toggled(bool)), this, SLOT(on_action_edit_toggled(bool)));

	connect(action_editCancel, SIGNAL(triggered(bool)), this, SLOT(on_action_editCancel_triggered()));


	toolBar->addAction(action_openFile);

	toolBar->addSeparator();

	toolBar->addAction(action_play);

	toolBar->addSeparator();

	toolBar->addAction(action_edit);
}

void BlobWidget::makeWidget()
{
	TabWidget = new QTabWidget(this);

	TabWidget->setTabPosition(QTabWidget::North);

	QFrame* page1 = new QFrame(TabWidget);

	BaseParamWidget = TabWidget->widget(TabWidget->addTab(page1, tr("BaseParam")));

	QFrame* page2 = new QFrame(TabWidget);

	SetItemWidget = TabWidget->widget(TabWidget->addTab(page2, tr("SetItem")));


	QFrame* page3 = new QFrame(TabWidget);

	OutputWidget = TabWidget->widget(TabWidget->addTab(page3, tr("Output")));

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
}

void BlobWidget::makeTableWidget()
{
	resultTableWidget = new QTableWidget(5, 3, this);

	resultTableWidget->setHorizontalHeaderLabels(QStringList() << "X" << "Y" << "Area");

	headerView = resultTableWidget->horizontalHeader();

	resultTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	resultTableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);

	resultTableWidget->setColumnWidth(0, 70);

	resultTableWidget->setColumnWidth(1, 70);

	resultTableWidget->setColumnWidth(2, 70);


	featureTableWidget = new QTableWidget(1, 4);

	QStringList m_list;

	m_list << tr("Feature") << tr("Min") << tr("Max") << tr("Mode");

	featureTableWidget->setHorizontalHeaderLabels(m_list);

	featureTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	featureTableWidget->setSelectionMode(QAbstractItemView::NoSelection);

	featureTableWidget->setColumnWidth(0, 75);

	featureTableWidget->setColumnWidth(1, 55);

	featureTableWidget->setColumnWidth(2, 55);

	featureTableWidget->setColumnWidth(3, 60);



}

void BlobWidget::makeMainLayout()
{
	fr_view = new FrameView();

	QSplitter *spliter = new QSplitter(Qt::Horizontal);
	spliter->addWidget(TabWidget);

	QWidget *rightWidget = new QWidget();

	QVBoxLayout *vlay = new QVBoxLayout();
	vlay->addWidget(toolBar);
	QSplitter *spliter1 = new QSplitter(Qt::Vertical);
	spliter1->addWidget(fr_view);
	spliter1->addWidget(resultTableWidget);
	QList<int> list1;
	list1.append(600);
	list1.append(200);
	spliter1->setSizes(list1);
	vlay->addWidget(spliter1);
	rightWidget->setLayout(vlay);

	QList<int> list;
	list.append(300);
	list.append(700);
	spliter->addWidget(rightWidget);
	spliter->setSizes(list);

	QVBoxLayout *vlay1 = new QVBoxLayout();

	vlay1->addWidget(spliter);



	OK_btn = new QPushButton("OK");
	OK_btn->setMaximumWidth(90);
	Cancel_btn = new QPushButton("Cancel");
	Cancel_btn->setMaximumWidth(90);
	QHBoxLayout *hlay = new QHBoxLayout();
	hlay->addWidget(botMsgBox);
	hlay->addWidget(Cancel_btn);
	hlay->addWidget(OK_btn);
	vlay1->addLayout(hlay);

	this->setLayout(vlay1);

}

void BlobWidget::makeBaseParamWidget()
{
	QVBoxLayout *vlay = new QVBoxLayout();
	QGroupBox *Image_gb = new QGroupBox("Operator Input");

	QLabel *image_lb = new QLabel("Source Input:");
	QHBoxLayout *hlay1 = new QHBoxLayout();

	Image_cb = new InputBox();
	hlay1->addWidget(image_lb);
	hlay1->addWidget(Image_cb);
	Image_gb->setLayout(hlay1);


	vlay->addLayout(hlay1);

	QGroupBox *Thresh_gb = new QGroupBox("Thresh");
	Thresh_sl = new QSlider();
	Thresh_sp = new QSpinBox();
	Thresh_sl->setOrientation(Qt::Horizontal);
	QHBoxLayout *hlay2 = new QHBoxLayout();
	hlay2->addWidget(Thresh_sl);
	hlay2->addWidget(Thresh_sp);
	Thresh_gb->setLayout(hlay2);
	vlay->addLayout(hlay2);

	QGroupBox *layer_gb = new QGroupBox("Layer Select");
	QLabel *layer_lb = new QLabel("Layer:");
	QHBoxLayout *hlay_layer = new QHBoxLayout();
	Layers_cb = new QComboBox();
	hlay_layer->addWidget(layer_lb);
	hlay_layer->addWidget(Layers_cb);
	layer_gb->setLayout(hlay_layer);

	QGroupBox *Feature_gb = new QGroupBox("Feature Config");
	QVBoxLayout *vlay1 = new QVBoxLayout();
	QLabel *feature_lb = new QLabel("Feature:");
	Feature_cb = new QComboBox();
	QHBoxLayout *hlay3 = new QHBoxLayout();
	hlay3->addWidget(feature_lb);
	hlay3->addWidget(Feature_cb);
	vlay1->addLayout(hlay3);

	QLabel *mode_lb = new QLabel("Mode:");
	Mode_cb = new QComboBox();
	QHBoxLayout *hlay4 = new QHBoxLayout();
	hlay4->addWidget(mode_lb);
	hlay4->addWidget(Mode_cb);
	vlay1->addLayout(hlay4);

	QLabel *min_lb = new QLabel("Minimun:");
	Min_sp = new QDoubleSpinBox();
	QHBoxLayout *hlay5 = new QHBoxLayout();
	hlay5->addWidget(min_lb);
	hlay5->addWidget(Min_sp);
	vlay1->addLayout(hlay5);

	QLabel *max_lb = new QLabel("Maximun:");
	Max_sp = new QDoubleSpinBox();
	QHBoxLayout *hlay6 = new QHBoxLayout();
	hlay6->addWidget(max_lb);
	hlay6->addWidget(Max_sp);
	vlay1->addLayout(hlay6);
	Add_btn = new QPushButton("Add");
	Delete_btn = new QPushButton("Delete");
	QHBoxLayout *hlay7 = new QHBoxLayout();
	hlay7->addWidget(Add_btn);
	hlay7->addWidget(Delete_btn);
	vlay1->addLayout(hlay7);

	QLabel *space = new QLabel();
	space->setMaximumHeight(1.5);
	space->setFrameShadow(QFrame::Sunken);
	QPalette palette;
	palette.setColor(QPalette::Background, QColor(119, 136, 153));
	space->setAutoFillBackground(true);
	space->setPalette(palette);
	vlay1->addWidget(space);

	AddAll_btn = new QPushButton("Add All");
	DeleteAll_btn = new QPushButton("Delete All");
	QHBoxLayout *hlay8 = new QHBoxLayout();
	hlay8->addWidget(AddAll_btn);
	hlay8->addWidget(DeleteAll_btn);
	vlay1->addLayout(hlay8);

	Feature_gb->setLayout(vlay1);
	vlay->addWidget(Image_gb);
	vlay->addWidget(Thresh_gb);
	vlay->addWidget(layer_gb);
	vlay->addWidget(Feature_gb);
	vlay->addWidget(featureTableWidget);
	BaseParamWidget->setLayout(vlay);

}

void BlobWidget::initWidget()
{


	Thresh_sl->setValue(50);
	Thresh_sl->setMaximum(254);
	Thresh_sl->setMinimum(2);
	Thresh_sp->setValue(50);
	Thresh_sp->setMaximum(254);
	Thresh_sp->setMinimum(2);

	Layers_cb->addItems(QStringList() << "All Layers" << "Black" << "White");
	Feature_cb->addItems(QStringList() << "Area"
		<< "Circularity"
		<< "Inertia"
		<< "Convexity"
		<< "Rectangularity"
		<< "Width"
		<< "Height"
		<< "Compactness"
		<< "Contlength");
	Mode_cb->addItems(QStringList() << "Inside" << "Outside");

	Image_cb->setDataType(ToolDataType::eMat);
	
	connect(OK_btn, SIGNAL(clicked()), this, SLOT(on_OK_btn_clicked()));
	connect(Cancel_btn, SIGNAL(clicked()), this, SLOT(on_Cancel_btn_clicked()));
	connect(Image_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(on_image_cb_Changed(int)));
	connect(Thresh_sl, SIGNAL(valueChanged(int)), this, SLOT(on_Thresh_sl_Changed(int)));
	connect(Thresh_sp, SIGNAL(valueChanged(int)), this, SLOT(on_Thresh_Changed(int)));
	connect(Layers_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(on_Layers_cb_Changed(int)));
	connect(Feature_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(on_Feature_cb_Changed(int)));
	connect(Mode_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(on_Mode_cb_Changed(int)));
	connect(Max_sp, SIGNAL(valueChanged(int)), this, SLOT(on_Max_sp_Changed(int)));
	connect(Min_sp, SIGNAL(valueChanged(int)), this, SLOT(on_Min_sp_Changed(int)));

	connect(Add_btn, SIGNAL(clicked()), this, SLOT(on_Add_btn_clicked()));
	connect(Delete_btn, SIGNAL(clicked()), this, SLOT(on_Delete_btn_clicked()));
	connect(AddAll_btn, SIGNAL(clicked()), this, SLOT(on_AddAll_btn_clicked()));
	connect(DeleteAll_btn, SIGNAL(clicked()), this, SLOT(on_DeleteAll_btn_clicked()));

	connect(resultTableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(on_resultTableWidget_cellClicked(int, int)));
	connect(headerView, SIGNAL(sectionDoubleClicked(int)), this, SLOT(on_resultTableHeaderDoubleClicked(int)));

}

void BlobWidget::updateResultTableWidget()
{
}

void BlobWidget::updateFeatureTableWidget()
{
}

void BlobWidget::syncOperator(BlobOperator * AA, BlobOperator * BB)
{
}

void BlobWidget::updateOutput()
{
	//if (CalOperator->input_mat == nullptr)return;
	// if (CalOperator->m_state != NodeState::Success)
	// {
	// return;
	// }
	updateResults(CalOperator->pblob->m_result);
	fr_view->setImage(m_currentImage);
}

void BlobWidget::updateParameter(bool flag)
{
	SIMPLEBLOB_SPACE::Params m_params = CalOperator->getParameters();
	if (flag)
	{
		m_params.threshold = this->Thresh_sp->value();
		switch (Layers_cb->currentIndex())
		{
		case 0:
			m_params.filterByColor = true;
			m_params.blobColor = 0;
			break;
		case 1:
			m_params.filterByColor = true;
			m_params.blobColor = 255;
			break;
		case 2:
			m_params.filterByColor = false;
			break;
		default:
			break;
		}
		//CalOperator->setROI(this->ROI);
		CalOperator->setParams(m_params);
	}
	else
	{
		m_params = CalOperator->getParameters();
		this->Thresh_sp->setValue(m_params.threshold);
		if (m_params.filterByColor)
		{
			if (0 == m_params.blobColor)
				Layers_cb->setCurrentIndex(0);
			else
				Layers_cb->setCurrentIndex(1);
		}
		else
		{
			Layers_cb->setCurrentIndex(2);
		}
		//if (CalOperator->roiChecked)
		//{
		//	setROI(CalOperator->ROI);
		//}
		//setRoiChecked(CalOperator->roiChecked);
		CalOperator->setParams(m_params);
	}
}

void BlobWidget::updateResults(std::vector<SIMPLEBLOB_SPACE::CResult>& m)
{
	using placeholders::_1;
	using placeholders::_2;
	int teyp = -1;
	if (sortIndex < FeaturesIsSlected.size() && sortIndex >= 0)
		teyp = FeaturesIsSlected.at(sortIndex);
	std::sort(m.begin(), m.end(),bind(compareFeatures, _1, _2, teyp));
	resultTableWidget->clear();
	resultTableWidget->setRowCount(m.size());
	resultTableWidget->setColumnCount(selctfeatureNames.size());
	headerView = resultTableWidget->horizontalHeader();
	resultTableWidget->setHorizontalHeaderLabels(selctfeatureNames);
	QString str_temp;
	SIMPLEBLOB_SPACE::Params mparams = CalOperator->getParameters();
	for (size_t i = 0; i < m.size(); i++)
	{
		str_temp = QString("%1").arg(m.at(i).location.x);
		resultTableWidget->setItem(i, 0, new QTableWidgetItem(str_temp));
		str_temp = QString("%1").arg(m.at(i).location.y);
		resultTableWidget->setItem(i, 1, new QTableWidgetItem(str_temp));
		int cloum = 1;
		if (mparams.Area.inuse)
		{
			++cloum;
			str_temp = QString("%1").arg(m.at(i).area);
			resultTableWidget->setItem(i, cloum, new QTableWidgetItem(str_temp));
		}
		if (mparams.Inertia.inuse)
		{
			++cloum;
			str_temp = QString("%1").arg(m.at(i).Inertia, 0, 'a', 3);
			resultTableWidget->setItem(i, cloum, new QTableWidgetItem(str_temp));
		}
		if (mparams.Circularity.inuse)
		{
			++cloum;
			str_temp = QString("%1").arg(m.at(i).Circularity, 0, 'a', 3);
			resultTableWidget->setItem(i, cloum, new QTableWidgetItem(str_temp));
		}
		if (mparams.Convexity.inuse)
		{
			++cloum;
			str_temp = QString("%1").arg(m.at(i).Convexity, 0, 'a', 3);
			resultTableWidget->setItem(i, cloum, new QTableWidgetItem(str_temp));
		}
		if (mparams.Rectangularity.inuse)
		{
			++cloum;
			str_temp = QString("%1").arg(m.at(i).Rectangularity, 0, 'a', 3);
			resultTableWidget->setItem(i, cloum, new QTableWidgetItem(str_temp));
		}
		if (mparams.BolbWide.inuse)
		{
			++cloum;
			str_temp = QString("%1").arg(m.at(i).wide, 0, 'a', 3);
			resultTableWidget->setItem(i, cloum, new QTableWidgetItem(str_temp));
		}
		if (mparams.BlobHeight.inuse)
		{
			++cloum;
			str_temp = QString("%1").arg(m.at(i).heiht, 0, 'a', 3);
			resultTableWidget->setItem(i, cloum, new QTableWidgetItem(str_temp));
		}
		if (mparams.Compactness.inuse)
		{
			++cloum;
			str_temp = QString("%1").arg(m.at(i).Compactness, 0, 'a', 3);
			resultTableWidget->setItem(i, cloum, new QTableWidgetItem(str_temp));
		}
		if (mparams.Conlength.inuse)
		{
			++cloum;
			str_temp = QString("%1").arg(m.at(i).Conlength, 0, 'a', 3);
			resultTableWidget->setItem(i, cloum, new QTableWidgetItem(str_temp));
		}
	}
}


void BlobWidget::initialFeature()
{
}

void BlobWidget::initialFeatureMode()
{
}

void BlobWidget::updateTableWidgetFeatures()
{
	using namespace SIMPLEBLOB_SPACE;
	featureTableWidget->clearContents();
	featureTableWidget->setRowCount(0);
	if (NULL == CalOperator)
		return;
	selctfeatureNames.clear();
	FeaturesIsSlected.clear();
	Params mparams = CalOperator->getParameters();
	int row = 0;
	selctfeatureNames.push_back(tr("X"));
	FeaturesIsSlected.push_back(0);
	selctfeatureNames.push_back(tr("Y"));
	FeaturesIsSlected.push_back(1);
	if (mparams.Area.inuse)
	{
		featureTableWidget->insertRow(row);
		updateRowFeatures(row, mparams.Area);
		selctfeatureNames.push_back(tr("Area"));
		FeaturesIsSlected.push_back(2);
		++row;
	}
	if (mparams.Inertia.inuse)
	{
		featureTableWidget->insertRow(row);
		updateRowFeatures(row, mparams.Inertia);
		selctfeatureNames.push_back(tr("Inertia"));
		FeaturesIsSlected.push_back(3);
		++row;
	}
	if (mparams.Circularity.inuse)
	{
		featureTableWidget->insertRow(row);
		updateRowFeatures(row, mparams.Circularity);
		selctfeatureNames.push_back(tr("Circularity"));
		FeaturesIsSlected.push_back(4);
		++row;
	}
	if (mparams.Convexity.inuse)
	{
		featureTableWidget->insertRow(row);
		updateRowFeatures(row, mparams.Convexity);
		selctfeatureNames.push_back(tr("Convexity"));
		FeaturesIsSlected.push_back(5);
		++row;
	}
	if (mparams.Rectangularity.inuse)
	{
		featureTableWidget->insertRow(row);
		updateRowFeatures(row, mparams.Rectangularity);
		selctfeatureNames.push_back(tr("Rectangularity"));
		FeaturesIsSlected.push_back(6);
		++row;
	}
	if (mparams.BolbWide.inuse)
	{
		featureTableWidget->insertRow(row);
		updateRowFeatures(row, mparams.BolbWide);
		selctfeatureNames.push_back(tr("Wide"));
		FeaturesIsSlected.push_back(7);
		++row;
	}
	if (mparams.BlobHeight.inuse)
	{
		featureTableWidget->insertRow(row);
		updateRowFeatures(row, mparams.BlobHeight);
		selctfeatureNames.push_back(tr("Height"));
		FeaturesIsSlected.push_back(8);
		++row;
	}
	if (mparams.Compactness.inuse)
	{
		featureTableWidget->insertRow(row);
		updateRowFeatures(row, mparams.Compactness);
		selctfeatureNames.push_back(tr("Compactness"));
		FeaturesIsSlected.push_back(9);
		++row;
	}
	if (mparams.Conlength.inuse)
	{
		featureTableWidget->insertRow(row);
		updateRowFeatures(row, mparams.Conlength);
		selctfeatureNames.push_back(tr("Contlength"));
		FeaturesIsSlected.push_back(10);
		++row;
	}
}

void BlobWidget::updateRowFeatures(int row, SIMPLEBLOB_SPACE::feature mfeature)
{
	using namespace SIMPLEBLOB_SPACE;
	QString str_temp;
	str_temp = tr(mfeature.name.c_str());
	featureTableWidget->setItem(row, 0, new QTableWidgetItem(str_temp));
	str_temp = QString("%1").arg(mfeature.minValue);
	featureTableWidget->setItem(row, 1, new QTableWidgetItem(str_temp));
	str_temp = QString("%1").arg(mfeature.maxValue);
	featureTableWidget->setItem(row, 2, new QTableWidgetItem(str_temp));
	if (Mode::INSIDE == mfeature.mode)
	{
		str_temp = tr("INSIDE");
		featureTableWidget->setItem(row, 3, new QTableWidgetItem(str_temp));
	}
	else
	{
		str_temp = tr("OUTSIDE");
		featureTableWidget->setItem(row, 3, new QTableWidgetItem(str_temp));
	}
}

void BlobWidget::updateFeatureValue(bool add)
{
	SIMPLEBLOB_SPACE::feature mfeature;
	mfeature.inuse = add;
	if (add)
	{
		if (0 == Feature_cb->currentIndex())
			mfeature.mode = SIMPLEBLOB_SPACE::Mode::INSIDE;
		else
			mfeature.mode = SIMPLEBLOB_SPACE::Mode::OUTSIDE;
		mfeature.minValue = Min_sp->value();
		mfeature.maxValue = Max_sp->value();
		if (mfeature.minValue >= mfeature.maxValue)
		{
			return;
		}
	}
	int index = Feature_cb->currentIndex();
	if (nullptr == CalOperator)
		return;
	SIMPLEBLOB_SPACE::Params mpara = CalOperator->getParameters();
	switch (index)
	{
	case 0:
		if (!mpara.Area.inuse || !add)
			sortIndex = -1;
		mpara.Area = mfeature;
		mpara.Area.name = "Area";
		break;
	case 1:
		if (!mpara.Circularity.inuse || !add)
			sortIndex = -1;
		mpara.Circularity = mfeature;
		mpara.Circularity.name = "Circularity";
		break;
	case 2:
		if (!mpara.Inertia.inuse || !add)
			sortIndex = -1;
		mpara.Inertia = mfeature;
		mpara.Inertia.name = "Inertia";
		break;
	case 3:
		if (!mpara.Convexity.inuse || !add)
			sortIndex = -1;
		mpara.Convexity = mfeature;
		mpara.Convexity.name = "Convexity";
		break;
	case 4:
		if (!mpara.Rectangularity.inuse || !add)
			sortIndex = -1;
		mpara.Rectangularity = mfeature;
		mpara.Rectangularity.name = "Rectangularity";
		break;
	case 5:
		if (!mpara.BolbWide.inuse || !add)
			sortIndex = -1;
		mpara.BolbWide = mfeature;
		mpara.BolbWide.name = "Wide";
		break;
	case 6:
		if (!mpara.BlobHeight.inuse || !add)
			sortIndex = -1;
		mpara.BlobHeight = mfeature;
		mpara.BlobHeight.name = "Height";
		break;
	case 7:
		if (!mpara.Compactness.inuse || !add)
			sortIndex = -1;
		mpara.Compactness = mfeature;
		mpara.Compactness.name = "Compactness";
		break;
	case 8:
		if (!mpara.Conlength.inuse || !add)
			sortIndex = -1;
		mpara.Conlength = mfeature;
		mpara.Conlength.name = "Contlength";
		break;
	default:
		break;
	}
	CalOperator->setParams(mpara);
}

void BlobWidget::updateHorizonHeaders()
{
}

void BlobWidget::setAllUes(bool flag)
{
	SIMPLEBLOB_SPACE::Params mpara = CalOperator->getParameters();
	mpara.Area.inuse = flag;
	mpara.Circularity.inuse = flag;
	mpara.Compactness.inuse = flag;
	mpara.Conlength.inuse = flag;
	mpara.Convexity.inuse = flag;
	mpara.BlobHeight.inuse = flag;
	mpara.BolbWide.inuse = flag;
	mpara.Inertia.inuse = flag;
	mpara.Rectangularity.inuse = flag;
	CalOperator->setParams(mpara);
	updateTableWidgetFeatures();
	sortIndex = -1;
}

void BlobWidget::showMoveWindows(bool flag)
{

}
