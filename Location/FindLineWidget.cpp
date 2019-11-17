#include "FindLineWidget.h"
FindLineWidget::FindLineWidget(QWidget *parent)
	: BaseWidget(parent)
{
	this->setWindowTitle("FindLineWidget");
	this->resize(1000, 800);
	m_CalOperator = new FindLineOperator();
	m_CalOperator->initOutput();
	makeBaseParamWidget();
	makeSetItemWidget();
	makeOutputWidget();
	initWidget();
	initItem();
	showImage();
}
FindLineWidget::~FindLineWidget()
{
}
void FindLineWidget::doPlay()
{
	this->botMsgBox->setText(tr("FindLine start to run ..."));
	cv::Mat src;
	//if (getEditChecked())
	//{
	// src = m_CalOperator->input_refMat->GetValue();
	//}
	//else
	//{
	// src = m_CalOperator->input_mat->GetValue();
	//}
	src = fr_view->getMat();
	m_CalOperator->test(src, m_findLineItem->get_Rects(), m_findLineItem->get_AimRect());
	updateOutput();
}
void FindLineWidget::doOk()
{
	m_CalOperator->setAimRect(m_findLineItem->get_AimRect());
	m_CalOperator->setDivideRects(m_findLineItem->get_Rects());
	m_CalOperator->setCaliperWidth(Cal_Width_sp->value());
	m_CalOperator->setDivideCounts(Caliper_Count_sp->value());
	m_CalOperator->setDistThresh(Distance_Thresh_sp->value());
	m_CalOperator->setFilterType(FittingType_cb->currentIndex());
	m_CalOperator->setGradThresh(Cal_Treshold_sp->value());
	m_CalOperator->setMaxIter(Itr_Count_sp->value());
	m_CalOperator->setPolarProperty(polar_cb->currentIndex());
	m_CalOperator->setResultType(resultType_cb->currentIndex());
	m_CalOperator->setDeleteCount(Delete_Count_sp->value());
	syncOperator(m_CalOperator, m_SaveOperator);
	m_SaveOperator->save();
	this->close();
}
void FindLineWidget::doCancel()
{
	this->close();
}
void FindLineWidget::doEdit(bool b)
{
	m_findLineItem->setVisible(b);
	m_findLineItem->setZValue(100);
	if (!b)
	{
		
	}
	else
	{
		m_findLineItem->setFlags(QGraphicsItem::GraphicsItemFlag::ItemIsSelectable
			| QGraphicsItem::GraphicsItemFlag::ItemIsMovable);
	}
	showImage();
}
void FindLineWidget::doEditSave()
{
}
void FindLineWidget::setOperator(FindLineOperator * Opra)
{
	if (Opra == nullptr) { return; }
	m_SaveOperator = Opra;
	syncOperator(m_SaveOperator, m_CalOperator);
	m_refRect = m_CalOperator->getAimRect();
	m_refDivideRects = m_CalOperator->getDivideRects();
	polar_cb->setCurrentIndex(m_CalOperator->getPolarProperty());
	resultType_cb->setCurrentIndex(m_CalOperator->getResultType());
	FittingType_cb->setCurrentIndex(m_CalOperator->getFilterType());
	Cal_Treshold_sp->setValue(m_CalOperator->getGradThresh());
	Itr_Count_sp->setValue(m_CalOperator->getMaxIter() / 100);
	Distance_Thresh_sp->setValue(m_CalOperator->getDistThresh());
	Caliper_Count_sp->setValue(m_CalOperator->getDivideCounts());
	Cal_Width_sp->setValue(m_CalOperator->getCaliperWidth());
	Delete_Count_sp->setValue(m_CalOperator->getDeleteCount());
	m_findLineItem->setWidth(m_CalOperator->getCaliperWidth());
	m_findLineItem->setCount(m_CalOperator->getDivideCounts());
	m_findLineItem->setAimArea(m_CalOperator->getAimRect());
	m_OKPointsItem->setPoints(m_CalOperator->getPointsOK());
	m_NGPointsItem->setPoints(m_CalOperator->getPointsNG());
	m_ResultLineItem->setLine(m_CalOperator->getResultLine());
	//affine translation
	if (m_CalOperator->input_affineTrans != nullptr)
	{
		m_trans = m_CalOperator->input_affineTrans->GetValue();
		m_findLineItem->setAimArea(m_refRect, m_trans);
	}
	//update output
	updateOutput();
	showImage();
	this->fr_view->getScene()->update();
}
void FindLineWidget::updateOutput()
{
	//if (m_CalOperator->input_mat == nullptr)return;
	if (m_CalOperator->GetState() == NodeState::Success)
	{
		this->botMsgBox->setText(tr("FindLine Running Finished!"));
		geo::Line2D rslLine = m_CalOperator->getResultLine();
		geo::Point2D midPt = geo::Point2D(
			(rslLine.StartPoint().X() + rslLine.EndPoint().X())*0.5f,
			(rslLine.StartPoint().Y() + rslLine.EndPoint().Y())*0.5f);
		vector<geo::Point2D> OKpts = m_CalOperator->getPointsOK();
		vector<geo::Point2D> NGpts = m_CalOperator->getPointsNG();
		double rslLine_K = (rslLine.EndPoint().Y() - rslLine.StartPoint().Y()) / (rslLine.EndPoint().X() - rslLine.StartPoint().X());
		double rslLine_B = rslLine.StartPoint().Y() - rslLine_K*rslLine.StartPoint().X();
		double rslLine_x1 = OKpts[0].X();
		double rslLine_y1 = rslLine_K*rslLine_x1 + rslLine_B;
		double rslLine_x2 = OKpts.back().X();
		double rslLine_y2 = rslLine_K*rslLine_x2 + rslLine_B;
		if (SetEndPoint_cb->isChecked())
		{
			StartPointX_lb->setText(QString::number(rslLine.StartPoint().X()));
			StartPointY_lb->setText(QString::number(rslLine.StartPoint().Y()));
			MidPointX_lb->setText(QString::number(midPt.X()));
			MidPointY_lb->setText(QString::number(midPt.Y()));
			EndPointX_lb->setText(QString::number(rslLine.EndPoint().X()));
			EndPointY_lb->setText(QString::number(rslLine.EndPoint().Y()));
		}
		else
		{
			StartPointX_lb->setText(QString::number(rslLine_x1));
			StartPointY_lb->setText(QString::number(rslLine_y1));
			MidPointX_lb->setText(QString::number(midPt.X()));
			MidPointY_lb->setText(QString::number(midPt.Y()));
			EndPointX_lb->setText(QString::number(rslLine_x2));
			EndPointY_lb->setText(QString::number(rslLine_y2));
		}
		OK_Count_lb->setText(QString::number(OKpts.size()));
		NG_Count_lb->setText(QString::number(NGpts.size()));
		if (!SetEndPoint_cb->isChecked())
		{
			m_ResultLineItem->setLine(rslLine);
		}
		else
		{
			m_ResultLineItem->setLine(rslLine);
		}
		m_ResultLineItem->setVisible(true);
		m_OKPointsItem->setPoints(OKpts);
		m_NGPointsItem->setPoints(NGpts);
		m_OKPointsItem->setVisible(true);
		m_NGPointsItem->setVisible(true);
		this->fr_view->getScene()->update();
	}
	else
	{
		this->botMsgBox->setText("FindLine Running Finished");
		FindLineOperator::ErrorCode ec = m_CalOperator->m_errorCode;
		if (ec == FindLineOperator::ErrorCode::NG_Caliper_Count)
			botMsgBox->setText(tr("Caliper found few point"));
		if (ec == FindLineOperator::ErrorCode::NG_Filter)
			botMsgBox->setText(tr("Filter failed"));
		if (ec == FindLineOperator::ErrorCode::NG_Filter_Count)
			botMsgBox->setText(tr("Too less Points after filter"));
		m_ResultLineItem->setVisible(false);
		m_OKPointsItem->setVisible(false);
		m_NGPointsItem->setVisible(false);
	}
}
InputBox * FindLineWidget::getImageComboBox()
{
	return this->image_cb;
}
InputBox * FindLineWidget::getTransComboBox()
{
	return this->trans_cb;
}
void FindLineWidget::on_image_cb_Changed(int index)
{
	if (m_CalOperator == nullptr) return;
	if (m_CalOperator->input_mat == nullptr)
	{
		return;
	}
	m_CalOperator->input_mat = this->image_cb->getMat(index);
	m_CalOperator->input_refMat = this->image_cb->getRefMat(index);
	showImage();
}
void FindLineWidget::on_trans_cb_Changed(int index)
{
	//if (m_SaveOperator == nullptr) return;
	//m_CalOperator->input_affineTrans = this->trans_cb->getAffineTrans(index);
	//if (m_CalOperator->input_affineTrans)
	//{
	// m_trans = m_CalOperator->input_affineTrans->GetValue();
	//}
	//m_findLineItem->setAimArea(m_refRect, m_trans);
}
void FindLineWidget::on_polar_cb_Changed(int index)
{
	this->m_CalOperator->setPolarProperty(index);
}
void FindLineWidget::on_resultType_cb_Changed(int index)
{
	this->m_CalOperator->setResultType(index);
}
void FindLineWidget::on_Cal_Treshold_sl_Changed(int value)
{
	this->Cal_Treshold_sp->setValue(value);
}
void FindLineWidget::on_Cal_Width_sl_Changed(int value)
{
	this->Cal_Width_sp->setValue(value);
}
void FindLineWidget::on_Cal_Height_sl_Changed(int value)
{
	this->Cal_Height_sp->setValue(value);
}
void FindLineWidget::on_FittingType_cb_Changed(int index)
{
	this->m_CalOperator->setFilterType(index);
}
void FindLineWidget::on_Itr_Count_sl_Changed(int value)
{
	this->Itr_Count_sp->setValue(value);
}
void FindLineWidget::on_Caliper_Count_sl_Changed(int value)
{
	this->Caliper_Count_sp->setValue(value);
	this->m_refDivideRects = m_findLineItem->get_Rects();
}
void FindLineWidget::on_Distance_Thresh_sl_Changed(int value)
{
	this->Distance_Thresh_sp->setValue(value);
}
void FindLineWidget::on_Delete_Count_sl_Changed(int value)
{
	this->Delete_Count_sp->setValue(value);
}
void FindLineWidget::on_Cal_Treshold_Changed(int value)
{
	Cal_Treshold_sl->setValue(value);
	m_CalOperator->setGradThresh(value);
}
void FindLineWidget::on_Cal_Width_Changed(int value)
{
	this->Cal_Width_sl->setValue(value);
	m_findLineItem->setWidth(value);
}
void FindLineWidget::on_Cal_Height_Changed(int value)
{
	this->Cal_Height_sl->setValue(value);
	//m_CalOperator->setCaliperHeight(value);
}
void FindLineWidget::on_Itr_Count_Changed(int value)
{
	this->Itr_Count_sl->setValue(value);
	m_CalOperator->setMaxIter(value * 100);
}
void FindLineWidget::on_Caliper_Count_Changed(int value)
{
	m_CalOperator->setDivideCounts(value);
	m_findLineItem->setCount(value);
	Delete_Count_sp->setMaximum(value - 2);
	Delete_Count_sl->setMaximum(value - 2);
}
void FindLineWidget::on_Distance_Thresh_Changed(int value)
{
	this->Distance_Thresh_sl->setValue(value);
	m_CalOperator->setDistThresh(value);
}
void FindLineWidget::on_Delete_Count_Changed(int value)
{
	this->Delete_Count_sl->setValue(value);
	m_CalOperator->setDeleteCount(value);
}
void FindLineWidget::on_Confirm_btn_Clicked(bool checked)
{
	if ((StartPointX_le->text().isEmpty()) || (StartPointY_le->text().isEmpty()) || (EndPointX_le->text().isEmpty()) || (EndPointY_le->text().isEmpty()))
	{
		return;
	}
	else
	{
		Line2D line(Point2D(StartPointX_le->text().toInt(), StartPointY_le->text().toInt()),
			Point2D(EndPointX_le->text().toInt(), EndPointY_le->text().toInt()));
		m_findLineItem->setLine(line);
	}
}
void FindLineWidget::on_Horizontal_rb_Clicked(bool checked)
{
	if (checked)
	{
		Line2D line = m_findLineItem->getLine();
		Line2D newLine = Line2D(Point2D(line.MidPoint().X() - line.Length() / 2, line.MidPoint().Y()),
			Point2D(line.MidPoint().X() + line.Length() / 2, line.MidPoint().Y()));
		m_findLineItem->setAimArea(newLine);
	}
	else
		return;
}
void FindLineWidget::on_Vertical_rb_Clicked(bool checked)
{
	if (checked)
	{
	}
}
void FindLineWidget::on_Degree45_rb_Clicked(bool checked)
{
	if (checked)
	{
	}
}
void FindLineWidget::on_Degree_45_rb_Clicked(bool checked)
{
	if (checked)
	{
	}
}
void FindLineWidget::on_FindLineItem_cb_Clicked(bool checked)
{
	if (checked)
	{
		this->m_findLineItem->setVisible(true);
	}
	else
	{
		this->m_findLineItem->setVisible(false);
	}
}
void FindLineWidget::on_ResultLine_cb_Clicked(bool checked)
{
	if (checked)
		m_findLineItem->setVisible(true);
	else
		m_findLineItem->setVisible(false);
}
void FindLineWidget::on_OK_Points_cb_Clicked(bool checked)
{
	if (checked)
		m_OKPointsItem->setVisible(true);
	else
		m_OKPointsItem->setVisible(false);
}
void FindLineWidget::on_NG_Points_cb_Clicked(bool checked)
{
	if (checked)
		m_NGPointsItem->setVisible(true);
	else
		m_NGPointsItem->setVisible(false);
}
void FindLineWidget::on_SelectAll_cb_Clicked(bool checked)
{
	if (checked)
	{
		this->FindLineItem_cb->setChecked(true);
		this->ResultLine_cb->setChecked(true);
		this->OK_Points_cb->setChecked(true);
		this->NG_Points_cb->setChecked(true);
		m_findLineItem->setVisible(true);
		m_ResultLineItem->setVisible(true);
		m_OKPointsItem->setVisible(true);
		m_NGPointsItem->setVisible(true);
		this->fr_view->update();
	}
	else
	{
		this->FindLineItem_cb->setChecked(this->FindLineItem_cb->isChecked());
		this->ResultLine_cb->setChecked(this->ResultLine_cb->isChecked());
		this->OK_Points_cb->setChecked(this->OK_Points_cb->isChecked());
		this->NG_Points_cb->setChecked(this->NG_Points_cb->isChecked());
		m_findLineItem->setVisible(this->FindLineItem_cb->isChecked());
		m_ResultLineItem->setVisible(this->ResultLine_cb->isChecked());
		m_OKPointsItem->setVisible(this->OK_Points_cb->isChecked());
		m_NGPointsItem->setVisible(this->NG_Points_cb->isChecked());
		this->fr_view->update();
	}
}
void FindLineWidget::on_FindLineItem_btn_Clicked(bool checked)
{
	m_ColorDialog = new QColorDialog;
	connect(m_ColorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(on_FindLineItem_getcolor(QColor)));
	m_ColorDialog->exec();
}
void FindLineWidget::on_ResultLine_btn_Clicked(bool checked)
{
	m_ColorDialog = new QColorDialog;
	connect(m_ColorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(on_ResultLine_getcolor(QColor)));
	m_ColorDialog->exec();
}
void FindLineWidget::on_OK_Points_btn_Clicked(bool checked)
{
	m_ColorDialog = new QColorDialog;
	connect(m_ColorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(on_OK_Points_getcolor(QColor)));
	m_ColorDialog->exec();
}
void FindLineWidget::on_NG_Points_btn_Clicked(bool checked)
{
	m_ColorDialog = new QColorDialog;
	connect(m_ColorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(on_NG_Points_getcolor(QColor)));
	m_ColorDialog->exec();
}
void FindLineWidget::on_OK_Points_cob_Changed(int value)
{
	if (value == 0)
	{
		m_OKPointsItem->setMarkType(PointSetItem::MarkType::plus);
		this->fr_view->getScene()->update();
	}
	else if (value == 1)
	{
		m_OKPointsItem->setMarkType(PointSetItem::MarkType::cross);
		this->fr_view->getScene()->update();
	}
	else
	{
		m_OKPointsItem->setMarkType(PointSetItem::MarkType::point);
		this->fr_view->getScene()->update();
	}
}
void FindLineWidget::on_NG_Points_cob_Changed(int value)
{
	if (value == 0)
	{
		m_NGPointsItem->setMarkType(PointSetItem::MarkType::plus);
		this->fr_view->getScene()->update();
	}
	else if (value == 1)
	{
		m_NGPointsItem->setMarkType(PointSetItem::MarkType::cross);
		this->fr_view->getScene()->update();
	}
	else
	{
		m_NGPointsItem->setMarkType(PointSetItem::MarkType::point);
		this->fr_view->getScene()->update();
	}
}
void FindLineWidget::on_FindLineItem_getcolor(QColor color)
{
	this->m_findLineItem->setColor(color);
}
void FindLineWidget::on_ResultLine_getcolor(QColor color)
{
	m_ResultLineItem->setColor(color);
}
void FindLineWidget::on_OK_Points_getcolor(QColor color)
{
	m_OKPointsItem->setColor(color);
	this->fr_view->getScene()->update();
}
void FindLineWidget::on_NG_Points_getcolor(QColor color)
{
	m_NGPointsItem->setColor(color);
	this->fr_view->getScene()->update();
}
void FindLineWidget::makeBaseParamWidget()
{
	QPalette pa;
	pa.setColor(QPalette::WindowText, Qt::black);
	QFont ft;
	ft.setPointSize(12);
	QHBoxLayout *hlay = new QHBoxLayout();
	QHBoxLayout *hlay1 = new QHBoxLayout();
	QVBoxLayout *vlay = new QVBoxLayout();
	QGroupBox *image_gb = new QGroupBox("input image");
	//image_label->setPalette(pa);
	//image_label->setFont(ft);
	image_cb = new InputBox();
	image_cb->addItem("null");
	image_cb->addItem("last");
	image_cb->addItem("next");
	hlay->addWidget(image_cb);
	image_gb->setLayout(hlay);
	//hlay->addStretch();
	hlay->setAlignment(Qt::AlignVCenter);
	QGroupBox *trans_gb = new QGroupBox("input trans");
	//trans_label->setFont(ft);
	//trans_label->setPalette(pa);
	trans_cb = new InputBox();
	trans_cb->addItem("null");
	trans_cb->addItem("last");
	trans_cb->addItem("next");
	//hlay1->addWidget(trans_label);
	hlay1->addWidget(trans_cb);
	trans_gb->setLayout(hlay1);
	//hlay1->addStretch();
	hlay1->setAlignment(Qt::AlignVCenter);
	QGroupBox *input_gb = new QGroupBox("Operator Input");
	vlay->addWidget(image_gb);
	vlay->addWidget(trans_gb);
	input_gb->setMaximumHeight(150);
	input_gb->setLayout(vlay);
	ft.setPointSize(12);
	input_gb->setFont(ft);
	pa.setColor(QPalette::WindowText, Qt::blue);
	input_gb->setPalette(pa);
	QGroupBox *CalliperPara_gb = new QGroupBox("Caliper Parameter");
	QVBoxLayout *vlay2 = new QVBoxLayout();
	QHBoxLayout *hlay2 = new QHBoxLayout();
	QGroupBox *polar_gb = new QGroupBox("Polar");
	QGroupBox *resultType_gb = new QGroupBox("Result Type");
	ft.setPointSize(10);
	polar_gb->setFont(ft);
	resultType_gb->setFont(ft);
	polar_cb = new QComboBox();
	resultType_cb = new QComboBox();
	QHBoxLayout *hlay3 = new QHBoxLayout();
	hlay3->addWidget(polar_cb);
	polar_gb->setLayout(hlay3);
	QHBoxLayout *hlay4 = new QHBoxLayout();
	hlay4->addWidget(resultType_cb);
	resultType_gb->setLayout(hlay4);
	QHBoxLayout *hlay5 = new QHBoxLayout();//
	hlay5->addWidget(polar_gb);
	hlay5->addWidget(resultType_gb);
	vlay2->addLayout(hlay5);
	QGroupBox *Caliper_Count_gb = new QGroupBox("Calliper Count");
	Caliper_Count_gb->setMaximumHeight(70);
	Caliper_Count_sp = new QSpinBox();
	Caliper_Count_sl = new QSlider();
	Caliper_Count_sl->setOrientation(Qt::Horizontal);
	QHBoxLayout *hlay_CalCount = new QHBoxLayout();
	hlay_CalCount->addWidget(Caliper_Count_sl);
	hlay_CalCount->addWidget(Caliper_Count_sp);
	Caliper_Count_gb->setLayout(hlay_CalCount);
	vlay2->addWidget(Caliper_Count_gb);
	QGroupBox *cal_thresh_gb = new QGroupBox("Calliper Threshold");
	cal_thresh_gb->setFont(ft);
	Cal_Treshold_sp = new QSpinBox();
	Cal_Treshold_sl = new QSlider();
	Cal_Treshold_sl->setOrientation(Qt::Horizontal);
	QHBoxLayout *hlay6 = new QHBoxLayout();
	hlay6->addWidget(Cal_Treshold_sl);
	hlay6->addWidget(Cal_Treshold_sp);
	cal_thresh_gb->setLayout(hlay6);
	vlay2->addWidget(cal_thresh_gb);
	QGroupBox *Cal_Width_gb = new QGroupBox("Calliper Width");
	Cal_Width_gb->setFont(ft);
	Cal_Width_sp = new QSpinBox();
	Cal_Width_sl = new QSlider();
	Cal_Width_sl->setOrientation(Qt::Horizontal);
	QHBoxLayout *hlay7 = new QHBoxLayout();
	hlay7->addWidget(Cal_Width_sl);
	hlay7->addWidget(Cal_Width_sp);
	Cal_Width_gb->setLayout(hlay7);
	vlay2->addWidget(Cal_Width_gb);
	QGroupBox *Cal_Height_gb = new QGroupBox("Calliper Height");
	Cal_Height_gb->setFont(ft);
	Cal_Height_sp = new QSpinBox();
	Cal_Height_sl = new QSlider();
	Cal_Height_sl->setOrientation(Qt::Horizontal);
	QHBoxLayout *hlay8 = new QHBoxLayout();
	hlay8->addWidget(Cal_Height_sl);
	hlay8->addWidget(Cal_Height_sp);
	Cal_Height_gb->setLayout(hlay8);
	vlay2->addWidget(Cal_Height_gb);
	CalliperPara_gb->setLayout(vlay2);
	QGroupBox *Line_Para_gb = new QGroupBox("Line Parameter");
	QVBoxLayout *vlay3 = new QVBoxLayout();
	QGroupBox *Itr_count_gb = new QGroupBox("Iterate Count");
	Itr_count_gb->setFont(ft);
	Itr_Count_sp = new QSpinBox();
	Itr_Count_sl = new QSlider();
	Itr_Count_sl->setOrientation(Qt::Horizontal);
	QHBoxLayout *hlay9 = new QHBoxLayout();
	hlay9->addWidget(Itr_Count_sl);
	hlay9->addWidget(Itr_Count_sp);
	Itr_count_gb->setLayout(hlay9);
	QGroupBox *FittingType_gb = new QGroupBox("Fit Type");
	QGroupBox *Hollow_gb = new QGroupBox("Hollow");
	ft.setPointSize(10);
	Hollow_gb->setFont(ft);
	FittingType_gb->setFont(ft);
	FittingType_cb = new QComboBox();
	Hollow_cb = new QComboBox();
	QHBoxLayout *hlay10 = new QHBoxLayout();
	hlay10->addWidget(FittingType_gb);
	hlay10->addWidget(Hollow_gb);
	QHBoxLayout *hlay_fittingType = new QHBoxLayout();
	hlay_fittingType->addWidget(FittingType_cb);
	FittingType_gb->setLayout(hlay_fittingType);
	QHBoxLayout *hlay_Hollow = new QHBoxLayout();
	hlay_Hollow->addWidget(Hollow_cb);
	Hollow_gb->setLayout(hlay_Hollow);
	vlay3->addLayout(hlay10);
	vlay3->addWidget(Itr_count_gb);
	QGroupBox *Distance_Thresh_gb = new QGroupBox("Distance Thresh");
	Distance_Thresh_gb->setFont(ft);
	Distance_Thresh_sp = new QSpinBox();
	Distance_Thresh_sl = new QSlider();
	Distance_Thresh_sl->setOrientation(Qt::Horizontal);
	QHBoxLayout *hlay11 = new QHBoxLayout();
	hlay11->addWidget(Distance_Thresh_sl);
	hlay11->addWidget(Distance_Thresh_sp);
	Distance_Thresh_gb->setLayout(hlay11);
	vlay3->addWidget(Distance_Thresh_gb);
	QGroupBox *Delete_Count_gb = new QGroupBox("Delete Count");
	Delete_Count_gb->setFont(ft);
	Delete_Count_sp = new QSpinBox();
	Delete_Count_sl = new QSlider();
	Delete_Count_sl->setOrientation(Qt::Horizontal);
	QHBoxLayout *hlay12 = new QHBoxLayout();
	hlay12->addWidget(Delete_Count_sl);
	hlay12->addWidget(Delete_Count_sp);
	Delete_Count_gb->setLayout(hlay12);
	vlay3->addWidget(Delete_Count_gb);
	Line_Para_gb->setLayout(vlay3);
	QVBoxLayout *BasePara_layout = new QVBoxLayout();
	ft.setPointSize(12);
	CalliperPara_gb->setFont(ft);
	CalliperPara_gb->setPalette(pa);
	Line_Para_gb->setFont(ft);
	Line_Para_gb->setPalette(pa);
	BasePara_layout->addWidget(input_gb);
	BasePara_layout->addWidget(CalliperPara_gb);
	BasePara_layout->addWidget(Line_Para_gb);
	this->BaseParamWidget->setLayout(BasePara_layout);
}
void FindLineWidget::makeSetItemWidget()
{
	QVBoxLayout *vlay = new QVBoxLayout();
	QGroupBox*item_setting_gb = new QGroupBox("Item setting");
	//item_setting_gb.setMaximumHeight(250)
	vlay->addWidget(item_setting_gb);
	QHBoxLayout *hlay = new QHBoxLayout();
	QLabel *start_point_lb = new QLabel("Start Point:");
	StartPointX_le = new QLineEdit();
	StartPointY_le = new QLineEdit();
	hlay->addWidget(start_point_lb);
	hlay->addWidget(StartPointX_le);
	hlay->addWidget(StartPointY_le);
	QHBoxLayout *hlay1 = new QHBoxLayout();
	QLabel *end_point_lb = new QLabel("End Point:");
	EndPointX_le = new QLineEdit();
	EndPointY_le = new QLineEdit();
	hlay1->addWidget(end_point_lb);
	hlay1->addWidget(EndPointX_le);
	hlay1->addWidget(EndPointY_le);
	Confirm_btn = new QPushButton("Confirm");
	Horizontal_rb = new QRadioButton(" Horizontal");
	Vertical_rb = new QRadioButton(" Vertical");
	Degree45_rb = new QRadioButton(" 45 Degree");
	Degree_45_rb = new QRadioButton(" -45 Degree");
	QVBoxLayout *vlay1 = new QVBoxLayout();
	vlay1->addLayout(hlay);
	vlay1->addLayout(hlay1);
	vlay1->addWidget(Confirm_btn);
	vlay1->addWidget(Horizontal_rb);
	vlay1->addWidget(Vertical_rb);
	vlay1->addWidget(Degree45_rb);
	vlay1->addWidget(Degree_45_rb);
	item_setting_gb->setLayout(vlay1);
	QGroupBox *result_display_gb = new QGroupBox("Result display");
	FindLineItem_cb = new QCheckBox(" Find Line Item");
	FindLineItem_btn = new QPushButton("...");
	FindLineItem_btn->setMaximumWidth(50);
	QHBoxLayout *hlay2 = new QHBoxLayout();
	hlay2->addWidget(FindLineItem_cb);
	hlay2->addWidget(FindLineItem_btn);
	ResultLine_cb = new QCheckBox(" Result Line");
	ResultLine_btn = new QPushButton("...");
	ResultLine_btn->setMaximumWidth(50);
	QHBoxLayout *hlay3 = new QHBoxLayout();
	hlay3->addWidget(ResultLine_cb);
	hlay3->addWidget(ResultLine_btn);
	OK_Points_cb = new QCheckBox(" OK Points ");
	OK_Points_cob = new QComboBox();
	OK_Points_cob->setMaximumWidth(70);
	OK_Points_cob->addItems(QStringList() << "Cross" << "Circle" << "Point");
	OK_Points_btn = new QPushButton("...");
	OK_Points_btn->setMaximumWidth(50);
	QHBoxLayout *hlay4 = new QHBoxLayout();
	hlay4->addWidget(OK_Points_cb);
	hlay4->addWidget(OK_Points_cob);
	hlay4->addWidget(OK_Points_btn);
	NG_Points_cb = new QCheckBox(" NG Points ");
	NG_Points_cob = new QComboBox();
	NG_Points_cob->setMaximumWidth(70);
	NG_Points_cob->addItems(QStringList() << "Cross" << "Circle" << "Point");
	NG_Points_btn = new QPushButton("...");
	NG_Points_btn->setMaximumWidth(50);
	QHBoxLayout *hlay5 = new QHBoxLayout();
	hlay5->addWidget(NG_Points_cb);
	hlay5->addWidget(NG_Points_cob);
	hlay5->addWidget(NG_Points_btn);
	SelectAll_cb = new QCheckBox(" Select All");
	SelectAll_btn = new QPushButton("...");
	SelectAll_btn->setMaximumWidth(50);
	QHBoxLayout *hlay6 = new QHBoxLayout();
	hlay6->addWidget(SelectAll_cb);
	hlay6->addWidget(SelectAll_btn);
	QVBoxLayout *vlay2 = new QVBoxLayout();
	vlay2->addLayout(hlay2);
	vlay2->addLayout(hlay3);
	vlay2->addLayout(hlay4);
	vlay2->addLayout(hlay5);
	vlay2->addLayout(hlay6);
	result_display_gb->setLayout(vlay2);
	vlay->addWidget(result_display_gb);
	this->SetItemWidget->setLayout(vlay);
}
void FindLineWidget::makeOutputWidget()
{
	QVBoxLayout *vlay = new QVBoxLayout();
	QGroupBox *line_result_gb = new QGroupBox("Line result");
	line_result_gb->setMinimumHeight(400);
	QLabel *function_word_lb = new QLabel("Line Function: ");
	function_lb = new QLabel("'y = kx + b");
	function_lb->setFrameStyle(QFrame::Panel);
	function_lb->setMaximumHeight(25);
	function_lb->setFrameShadow(QFrame::Sunken);
	function_lb->setStyleSheet(
		"QLabel{background-color:lightgray}"
		);
	QHBoxLayout *hlay_xy = new QHBoxLayout();
	QLabel *space_lb = new QLabel(" ");
	QLabel *x_lb = new QLabel("X");
	x_lb->setAlignment(Qt::AlignCenter);
	QLabel *y_lb = new QLabel("Y");
	y_lb->setAlignment(Qt::AlignCenter);
	hlay_xy->addWidget(space_lb);
	hlay_xy->addWidget(x_lb);
	hlay_xy->addWidget(y_lb);
	QLabel *start_point_lb = new QLabel("Start Point:");
	QLabel *end_point_lb = new QLabel("End Point:");
	QLabel *mid_point_lb = new QLabel("Mid Point:");
	StartPointX_lb = new QLabel("100");
	StartPointX_lb->setFrameStyle(QFrame::Panel);
	StartPointX_lb->setMaximumHeight(25);
	StartPointX_lb->setFrameShadow(QFrame::Sunken);
	StartPointX_lb->setStyleSheet("QLabel{background-color:lightgray}");
	StartPointY_lb = new QLabel("100");
	StartPointY_lb->setFrameStyle(QFrame::Panel);
	StartPointY_lb->setMaximumHeight(25);
	StartPointY_lb->setFrameShadow(QFrame::Sunken);
	StartPointY_lb->setStyleSheet("QLabel{background-color:lightgray}");
	MidPointX_lb = new QLabel("100");
	MidPointX_lb->setFrameStyle(QFrame::Panel);
	MidPointX_lb->setMaximumHeight(25);
	MidPointX_lb->setFrameShadow(QFrame::Sunken);
	MidPointX_lb->setStyleSheet("QLabel{background-color:lightgray}");
	MidPointY_lb = new QLabel("100");
	MidPointY_lb->setFrameStyle(QFrame::Panel);
	MidPointY_lb->setMaximumHeight(25);
	MidPointY_lb->setFrameShadow(QFrame::Sunken);
	MidPointY_lb->setStyleSheet("QLabel{background-color:lightgray}");
	EndPointX_lb = new QLabel("100");
	EndPointX_lb->setFrameStyle(QFrame::Panel);
	EndPointX_lb->setMaximumHeight(25);
	EndPointX_lb->setFrameShadow(QFrame::Sunken);
	EndPointX_lb->setStyleSheet("QLabel{background-color:lightgray}");
	EndPointY_lb = new QLabel("100");
	EndPointY_lb->setFrameStyle(QFrame::Panel);
	EndPointY_lb->setMaximumHeight(25);
	EndPointY_lb->setFrameShadow(QFrame::Sunken);
	EndPointY_lb->setStyleSheet("QLabel{background-color:lightgray}");
	QHBoxLayout *hlay_start = new QHBoxLayout();
	hlay_start->addWidget(start_point_lb);
	hlay_start->addWidget(StartPointX_lb);
	hlay_start->addWidget(StartPointY_lb);
	QHBoxLayout *hlay_mid = new QHBoxLayout();
	hlay_mid->addWidget(mid_point_lb);
	hlay_mid->addWidget(MidPointX_lb);
	hlay_mid->addWidget(MidPointY_lb);
	QHBoxLayout *hlay_end = new QHBoxLayout();
	hlay_end->addWidget(end_point_lb);
	hlay_end->addWidget(EndPointX_lb);
	hlay_end->addWidget(EndPointY_lb);
	QLabel *length_word_lb = new QLabel("Length :");
	Length_lb = new QLabel("100");
	//Length_lb->setMinimumWidth(153);
	Length_lb->setFrameStyle(QFrame::Panel);
	Length_lb->setMaximumHeight(25);
	Length_lb->setFrameShadow(QFrame::Sunken);
	Length_lb->setStyleSheet("QLabel{background-color:lightgray}");
	QLabel *degree_word_lb = new QLabel("Degree :");
	Degree_lb = new QLabel("100");
	//Degree_lb->setMinimumWidth(153);
	Degree_lb->setFrameStyle(QFrame::Panel);
	Degree_lb->setMaximumHeight(25);
	Degree_lb->setFrameShadow(QFrame::Sunken);
	Degree_lb->setStyleSheet("QLabel{background-color:lightgray}");
	QLabel *ok_points_word_lb = new QLabel("Number Of OK Points:");
	OK_Count_lb = new QLabel("100");
	//OK_Count_lb->setMinimumWidth(153);
	OK_Count_lb->setFrameStyle(QFrame::Panel);
	OK_Count_lb->setMaximumHeight(25);
	OK_Count_lb->setFrameShadow(QFrame::Sunken);
	OK_Count_lb->setStyleSheet("QLabel{background-color:lightgray}");
	QLabel *ng_points_word_lb = new QLabel("Number Of NG Points:");
	NG_Count_lb = new QLabel("100");
	//NG_Count_lb->setMinimumWidth(153);
	NG_Count_lb->setFrameStyle(QFrame::Panel);
	NG_Count_lb->setMaximumHeight(25);
	NG_Count_lb->setFrameShadow(QFrame::Sunken);
	NG_Count_lb->setStyleSheet("QLabel{background-color:lightgray}");
	QHBoxLayout *hlay_len = new QHBoxLayout();
	hlay_len->addWidget(length_word_lb);
	hlay_len->addWidget(Length_lb);
	QHBoxLayout *hlay_degree = new QHBoxLayout();
	hlay_degree->addWidget(degree_word_lb);
	hlay_degree->addWidget(Degree_lb);
	QHBoxLayout *hlay_OK = new QHBoxLayout();
	hlay_OK->addWidget(ok_points_word_lb);
	hlay_OK->addWidget(OK_Count_lb);
	QHBoxLayout *hlay_NG = new QHBoxLayout();
	hlay_NG->addWidget(ng_points_word_lb);
	hlay_NG->addWidget(NG_Count_lb);
	SetEndPoint_cb = new QCheckBox("Set Item Endpoint");
	vlay->addWidget(function_word_lb);
	vlay->addWidget(function_lb);
	vlay->addLayout(hlay_xy);
	vlay->addLayout(hlay_start);
	vlay->addLayout(hlay_mid);
	vlay->addLayout(hlay_end);
	vlay->addLayout(hlay_len);
	vlay->addLayout(hlay_degree);
	vlay->addLayout(hlay_OK);
	vlay->addLayout(hlay_NG);
	vlay->addWidget(SetEndPoint_cb);
	//QPalette pa;
	//pa.setColor(QPalette::WindowText, Qt::blue);
	//QFont ft;
	//ft.setPointSize(12);
	//line_result_gb->setFont(ft);
	//line_result_gb->setPalette(pa);
	line_result_gb->setLayout(vlay);
	QVBoxLayout *vlay1 = new QVBoxLayout();
	vlay1->addWidget(line_result_gb);
	QGroupBox *judgment_gb = new QGroupBox("Parameter judgmen");
	QHBoxLayout *hlay_xy1 = new QHBoxLayout();
	QLabel *space_lb1 = new QLabel(" ");
	space_lb1->setMaximumHeight(20);
	QLabel *x_lb1 = new QLabel("Min");
	x_lb1->setAlignment(Qt::AlignCenter);
	QLabel *y_lb1 = new QLabel("Max");
	x_lb1->setMaximumHeight(20);
	y_lb1->setMaximumHeight(20);
	y_lb1->setAlignment(Qt::AlignCenter);
	hlay_xy1->addWidget(space_lb1);
	hlay_xy1->addWidget(x_lb1);
	hlay_xy1->addWidget(y_lb1);
	QVBoxLayout *vlay_jug = new QVBoxLayout();
	QHBoxLayout *hlay_start_x = new QHBoxLayout();
	StartPointX_cb = new QCheckBox("Start Point X:");
	StartPointX_min_le = new QLineEdit();
	StartPointX_max_le = new QLineEdit();
	StartPointX_min_le->setDisabled(true);
	StartPointX_max_le->setDisabled(true);
	hlay_start_x->addWidget(StartPointX_cb);
	hlay_start_x->addWidget(StartPointX_min_le);
	hlay_start_x->addWidget(StartPointX_max_le);
	QHBoxLayout *hlay_start_y = new QHBoxLayout();
	StartPointY_cb = new QCheckBox("Start Point Y:");
	StartPointY_min_le = new QLineEdit();
	StartPointY_max_le = new QLineEdit();
	StartPointY_min_le->setDisabled(true);
	StartPointY_max_le->setDisabled(true);
	hlay_start_y->addWidget(StartPointY_cb);
	hlay_start_y->addWidget(StartPointY_min_le);
	hlay_start_y->addWidget(StartPointY_max_le);
	QHBoxLayout *hlay_end_x = new QHBoxLayout();
	EndPointX_cb = new QCheckBox("End Point X:");
	EndPointX_min_le = new QLineEdit();
	EndPointX_max_le = new QLineEdit();
	EndPointX_min_le->setDisabled(true);
	EndPointX_max_le->setDisabled(true);
	hlay_end_x->addWidget(EndPointX_cb);
	hlay_end_x->addWidget(EndPointX_min_le);
	hlay_end_x->addWidget(EndPointX_max_le);
	QHBoxLayout *hlay_end_y = new QHBoxLayout();
	EndPointY_cb = new QCheckBox("End Point Y:");
	EndPointY_min_le = new QLineEdit();
	EndPointY_max_le = new QLineEdit();
	EndPointY_min_le->setDisabled(true);
	EndPointY_max_le->setDisabled(true);
	hlay_end_y->addWidget(EndPointY_cb);
	hlay_end_y->addWidget(EndPointY_min_le);
	hlay_end_y->addWidget(EndPointY_max_le);
	QVBoxLayout *vlay2 = new QVBoxLayout();
	vlay2->addLayout(hlay_xy1);
	vlay2->addLayout(hlay_start_x);
	vlay2->addLayout(hlay_start_y);
	vlay2->addLayout(hlay_end_x);
	vlay2->addLayout(hlay_end_y);
	judgment_gb->setLayout(vlay2);
	vlay1->addWidget(judgment_gb);
	this->OutputWidget->setLayout(vlay1);
}
void FindLineWidget::initWidget()
{
	this->polar_cb->addItems(QStringList() << "WhitoBlk" << "BlktoWhi" << "Both Side");
	this->resultType_cb->addItems(QStringList() << "First Point" << "Last Point" << "Best Point");
	this->FittingType_cb->addItems(QStringList() << "Ransac" << "Huber" << "Tukey");
	Cal_Treshold_sl->setMinimum(1);
	Cal_Treshold_sl->setMaximum(50);
	Cal_Treshold_sp->setMinimum(1);
	Cal_Treshold_sp->setMaximum(50);
	Cal_Treshold_sl->setValue(20);
	Cal_Treshold_sp->setValue(20);
	Cal_Width_sp->setMinimum(5);
	Cal_Width_sl->setMaximum(30);
	Cal_Width_sp->setMinimum(5);
	Cal_Width_sl->setMaximum(30);
	Cal_Width_sp->setValue(10);
	Cal_Width_sl->setValue(10);
	Cal_Height_sp->setMinimum(5);
	Cal_Height_sl->setMaximum(30);
	Cal_Height_sp->setMinimum(5);
	Cal_Height_sl->setMaximum(30);
	Cal_Height_sp->setValue(20);
	Cal_Height_sl->setValue(20);
	Itr_Count_sp->setMinimum(1);
	Itr_Count_sl->setMaximum(50);
	Itr_Count_sp->setMinimum(1);
	Itr_Count_sl->setMaximum(50);
	Itr_Count_sp->setValue(20);
	Itr_Count_sl->setValue(20);
	Distance_Thresh_sp->setMinimum(0);
	Distance_Thresh_sl->setMaximum(20);
	Distance_Thresh_sp->setMinimum(0);
	Distance_Thresh_sl->setMaximum(20);
	Distance_Thresh_sp->setValue(0);
	Distance_Thresh_sl->setValue(0);
	Caliper_Count_sp->setMinimum(3);
	Caliper_Count_sl->setMaximum(99);
	Caliper_Count_sp->setMinimum(3);
	Caliper_Count_sl->setMaximum(99);
	Caliper_Count_sp->setValue(5);
	Caliper_Count_sl->setValue(5);
	Delete_Count_sp->setMinimum(0);
	Delete_Count_sl->setMaximum(20);
	Delete_Count_sp->setMinimum(0);
	Delete_Count_sl->setMaximum(20);
	Delete_Count_sp->setValue(0);
	Delete_Count_sl->setValue(0);
	StartPointX_le->setValidator(new QIntValidator(1, 10000, this));
	StartPointX_le->setPlaceholderText("X");
	StartPointY_le->setValidator(new QIntValidator(1, 10000, this));
	StartPointY_le->setPlaceholderText("Y");
	EndPointX_le->setValidator(new QIntValidator(1, 10000, this));
	EndPointX_le->setPlaceholderText("X");
	EndPointY_le->setValidator(new QIntValidator(1, 10000, this));
	EndPointY_le->setPlaceholderText("Y");
	FindLineItem_cb->setChecked(true);
	ResultLine_cb->setChecked(true);
	OK_Points_cb->setChecked(true);
	NG_Points_cb->setChecked(true);
	image_cb->setDataType(ToolDataType::eMat);
	trans_cb->setDataType(ToolDataType::eAffineTrans);
	connect(image_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(on_image_cb_Changed(int)));
	connect(trans_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(on_trans_cb_Changed(int)));
	connect(polar_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(on_polar_cb_Changed(int)));
	connect(resultType_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(on_resultType_cb_Changed(int)));
	connect(Cal_Treshold_sl, SIGNAL(valueChanged(int)), this, SLOT(on_Cal_Treshold_sl_Changed(int)));
	connect(Cal_Width_sl, SIGNAL(valueChanged(int)), this, SLOT(on_Cal_Width_sl_Changed(int)));
	connect(Cal_Height_sl, SIGNAL(valueChanged(int)), this, SLOT(on_Cal_Height_sl_Changed(int)));
	connect(FittingType_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(on_FittingType_cb_Changed(int)));
	connect(Itr_Count_sl, SIGNAL(valueChanged(int)), this, SLOT(on_Itr_Count_sl_Changed(int)));
	connect(Caliper_Count_sl, SIGNAL(valueChanged(int)), this, SLOT(on_Caliper_Count_sl_Changed(int)));
	connect(Distance_Thresh_sl, SIGNAL(valueChanged(int)), this, SLOT(on_Distance_Thresh_sl_Changed(int)));
	connect(Delete_Count_sl, SIGNAL(valueChanged(int)), this, SLOT(on_Delete_Count_sl_Changed(int)));
	connect(Cal_Treshold_sp, SIGNAL(valueChanged(int)), this, SLOT(on_Cal_Treshold_Changed(int)));
	connect(Cal_Width_sp, SIGNAL(valueChanged(int)), this, SLOT(on_Cal_Width_Changed(int)));
	connect(Cal_Height_sp, SIGNAL(valueChanged(int)), this, SLOT(on_Cal_Height_Changed(int)));
	connect(Itr_Count_sp, SIGNAL(valueChanged(int)), this, SLOT(on_Itr_Count_Changed(int)));
	connect(Caliper_Count_sp, SIGNAL(valueChanged(int)), this, SLOT(on_Caliper_Count_Changed(int)));
	connect(Distance_Thresh_sp, SIGNAL(valueChanged(int)), this, SLOT(on_Distance_Thresh_Changed(int)));
	connect(Delete_Count_sp, SIGNAL(valueChanged(int)), this, SLOT(on_Delete_Count_Changed(int)));
	connect(Confirm_btn, SIGNAL(clicked(bool)), this, SLOT(on_Confirm_btn_Clicked(bool)));
	connect(Horizontal_rb, SIGNAL(clicked(bool)), this, SLOT(on_Horizontal_rb_Clicked(bool)));
	connect(Vertical_rb, SIGNAL(clicked(bool)), this, SLOT(on_Vertical_rb_Clicked(bool)));
	connect(Degree45_rb, SIGNAL(clicked(bool)), this, SLOT(on_Degree45_rb_Clicked(bool)));
	connect(Degree_45_rb, SIGNAL(clicked(bool)), this, SLOT(on_Degree_45_rb_Clicked(bool)));
	connect(FindLineItem_cb, SIGNAL(clicked(bool)), this, SLOT(on_FindLineItem_cb_Clicked(bool)));
	connect(ResultLine_cb, SIGNAL(clicked(bool)), this, SLOT(on_ResultLine_cb_Clicked(bool)));
	connect(OK_Points_cb, SIGNAL(clicked(bool)), this, SLOT(on_OK_Points_cb_Clicked(bool)));
	connect(NG_Points_cb, SIGNAL(clicked(bool)), this, SLOT(on_NG_Points_cb_Clicked(bool)));
	connect(SelectAll_cb, SIGNAL(clicked(bool)), this, SLOT(on_SelectAll_cb_Clicked(bool)));
	connect(FindLineItem_btn, SIGNAL(clicked(bool)), this, SLOT(on_FindLineItem_btn_Clicked(bool)));
	connect(ResultLine_btn, SIGNAL(clicked(bool)), this, SLOT(on_ResultLine_btn_Clicked(bool)));
	connect(OK_Points_btn, SIGNAL(clicked(bool)), this, SLOT(on_OK_Points_btn_Clicked(bool)));
	connect(NG_Points_btn, SIGNAL(clicked(bool)), this, SLOT(on_NG_Points_btn_Clicked(bool)));
	connect(OK_Points_cob, SIGNAL(currentIndexChanged(int)), this, SLOT(on_OK_Points_cob_Changed(int)));
	connect(NG_Points_cob, SIGNAL(currentIndexChanged(int)), this, SLOT(on_NG_Points_cob_Changed(int)));
}
void FindLineWidget::initItem()
{
	m_findLineItem = new findLineItem();
	m_findLineItem->setZValue(100);
	this->fr_view->getScene()->addItem(m_findLineItem);
	m_OKPointsItem = new PointSetItem();
	m_OKPointsItem->setColor(Qt::green);
	m_OKPointsItem->setDiameter(4);
	m_OKPointsItem->setMarkType(PointSetItem::MarkType::plus);
	m_OKPointsItem->setVisible(false);
	this->fr_view->getScene()->addItem(m_OKPointsItem);
	m_NGPointsItem = new PointSetItem();
	m_NGPointsItem->setColor(Qt::red);
	m_NGPointsItem->setDiameter(4);
	m_NGPointsItem->setMarkType(PointSetItem::MarkType::cross);
	m_NGPointsItem->setVisible(false);
	this->fr_view->getScene()->addItem(m_NGPointsItem);
	m_ResultLineItem = new LineSetItem();
	fr_view->getScene()->addItem(m_ResultLineItem);
	m_ResultLineItem->setVisible(false);
}
void FindLineWidget::syncOperator(FindLineOperator * AA, FindLineOperator * BB)
{
	BB->m_state = AA->m_state;
	BB->m_errorCode = AA->m_errorCode;
	BB->setDuration(AA->getDuration());
	BB->input_mat = AA->input_mat;
	BB->input_refMat = AA->input_refMat;
	BB->input_affineTrans = AA->input_affineTrans;
	BB->setAimRect(AA->getAimRect());
	BB->setDivideRects(AA->getDivideRects());
	BB->setCaliperWidth(AA->getCaliperWidth());
	BB->setDivideCounts(AA->getDivideCounts());
	BB->setDistThresh(AA->getDistThresh());
	BB->setFilterType(AA->getFilterType());
	BB->setGradThresh(AA->getGradThresh());
	BB->setMaxIter(AA->getMaxIter());
	BB->setPolarProperty(AA->getPolarProperty());
	BB->setResultType(AA->getResultType());
	BB->setDeleteCount(AA->getDeleteCount());
	BB->output_line->SetValue(AA->output_line->GetValue());
	BB->output_startPoint->SetValue(AA->output_startPoint->GetValue());
	BB->output_endPoint->SetValue(AA->output_endPoint->GetValue());
	BB->output_midPoint->SetValue(AA->output_midPoint->GetValue());
	BB->output_length->SetValue(AA->output_length->GetValue());
	BB->setPointsOK(AA->getPointsOK());
	BB->setPointNG(AA->getPointsNG());
}
void FindLineWidget::showImage()
{
	if (this->action_edit->isChecked())
	{
		if (m_CalOperator->input_refMat)
		{
			this->fr_view->setImage(m_CalOperator->input_refMat->GetValue());
		}
	}
	else
	{
		if (m_CalOperator->input_mat)
		{
			fr_view->setImage(m_CalOperator->input_mat->GetValue());
		}
	}
}