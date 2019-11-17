#include "AcquireImageWidget.h"

AcquireImageWidget::AcquireImageWidget()
{
	this->resize(1000, 800);
	makeBaseWidget();
	initWidget();

}
AcquireImageWidget::~AcquireImageWidget()
{
}
void AcquireImageWidget::setOperator(AcquireImageOperator * opra)
{
	this->m_operator = opra;
	m_Mat = m_operator->output_mat->GetValue().clone();
	int insKey = m_operator->getInspectionKey();
	m_refMat= InspectionMGR->FindBy(insKey)->GetAcquisition()->LoadReferenceMat(InspectionMGR->FindBy(insKey)->GetRefImageFileName());
	InspectionMGR->FindBy(insKey)->GetAcquisition()->SetReferenceMat(m_refMat);
	showImage();

}
void AcquireImageWidget::doOpenFile(cv::Mat img)
{
	img.copyTo(m_Mat);
	showImage();
}
void AcquireImageWidget::doEdit(bool b)
{
	action_openFile->setEnabled(b);
	showImage();
}
void AcquireImageWidget::doEditSave()
{
}
void AcquireImageWidget::doEditCancel()
{
}
void AcquireImageWidget::doPlay()
{
	m_operator->execute();
}
void AcquireImageWidget::doOk()
{
	this->close();
}
void AcquireImageWidget::doCancel()
{
	this->close();
}
void AcquireImageWidget::on_Register_btn_Clicked()
{
	QString title = tr("Register Reference Image");
	Inspection* inspection = InspectionMGR->FindBy(m_operator->getInspectionKey());
	if (inspection == nullptr)
	{
		QString strInfo = tr("Inspection is NULL!");
		QMessageBox::warning(this, title, strInfo, QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	if (!m_Mat.data)
	{
		QString strInfo = tr("Image is NULL!");
		QMessageBox::warning(this, title, strInfo, QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	inspection->GetAcquisition()->SetReferenceMat(m_Mat);
	m_refMat = m_Mat;
	inspection->GetAcquisition()->SaveReferenceMat(inspection->GetRefImageFileName());
	this->doPlay();
	m_operator->setCoreParams();
	inspection = nullptr;
	QString strInfo = tr("Register Done!");
	QMessageBox::information(this, title, strInfo, QMessageBox::Ok, QMessageBox::NoButton);
	showImage();
}
void AcquireImageWidget::makeBaseWidget()
{
	QGroupBox *resister_gb = new QGroupBox("Register Reffrence Iamge");
	QVBoxLayout *vlay = new QVBoxLayout();
	m_Register_btn = new QPushButton("Register");
	vlay->addWidget(m_Register_btn);
	resister_gb->setLayout(vlay);
	QGroupBox *frontSightSetting_gb = new QGroupBox("Front Sight Item Setting");
	QVBoxLayout *vlay1 = new QVBoxLayout();
	QLabel *left = new QLabel("ON");
	QLabel *right = new QLabel("OFF");
	m_OnOff_cb = new QCheckBox();
	QHBoxLayout *hlay = new QHBoxLayout();
	hlay->addWidget(left);
	hlay->addWidget(m_OnOff_cb);
	hlay->addWidget(right);
	QLabel *type_lb = new QLabel("Front Sight Type:");
	m_Type_cb = new QComboBox();
	QHBoxLayout *hlay1 = new QHBoxLayout();
	hlay1->addWidget(type_lb);
	hlay1->addWidget(m_Type_cb);
	QLabel *color_lb = new QLabel("Color Setting:");
	m_FrontSightColor_btn = new QPushButton("...");
	QHBoxLayout *hlay2 = new QHBoxLayout();
	hlay2->addWidget(color_lb);
	hlay2->addWidget(m_FrontSightColor_btn);
	QLabel *size_lb = new QLabel("Front Sight Size:");
	m_CenterSize_sp = new QDoubleSpinBox();
	QHBoxLayout *hlay3 = new QHBoxLayout();
	hlay3->addWidget(size_lb);
	hlay3->addWidget(m_CenterSize_sp);
	QLabel*ratio_lb = new QLabel("Ratio :");
	m_Ratio_lb = new QLabel();
	m_Ratio_lb->setStyleSheet("QLabel{background-color:lightgray}");
	QHBoxLayout *hlay4 = new QHBoxLayout();
	hlay4->addWidget(ratio_lb);
	hlay4->addWidget(m_Ratio_lb);
	vlay1->addLayout(hlay);
	vlay1->addLayout(hlay1);
	vlay1->addLayout(hlay2);
	vlay1->addLayout(hlay3);
	vlay1->addLayout(hlay4);
	frontSightSetting_gb->setLayout(vlay1);
	QVBoxLayout *vlay2 = new QVBoxLayout();
	vlay2->addWidget(resister_gb);
	vlay2->addWidget(frontSightSetting_gb);
	this->BaseParamWidget->setLayout(vlay2);
	this->action_edit->setChecked(false);
}
void AcquireImageWidget::initWidget()
{

	connect(m_Register_btn, SIGNAL(clicked()), this, SLOT(on_Register_btn_Clicked()));
}
void AcquireImageWidget::showImage()
{
	if (!this->action_edit->isChecked())
	{
		this->fr_view->setImage(m_Mat);
	}
	else
	{
		this->fr_view->setImage(m_refMat);
	}
	
}
