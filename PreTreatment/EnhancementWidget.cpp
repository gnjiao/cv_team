#include "EnhancementWidget.h"

EnhancementWidget::EnhancementWidget(QWidget *parent)
	: BaseWidget(parent)
{
	makeWidget();
	CalOperator = new EnhancementOperator();
	CalOperator->initOutput();
	this->resize(1000, 800);
}
EnhancementWidget::~EnhancementWidget()
{
	delete CalOperator;
}

InputBox * EnhancementWidget::getImageComboBox()
{
	return this->Image_cb;
}

void EnhancementWidget::setOperator(EnhancementOperator * opra)
{
	SaveOperator = opra;
	syncOperator(SaveOperator, CalOperator);
	on_Mode_cb_Changed(CalOperator->m_mode);
	Mode_cb->setCurrentIndex(CalOperator->m_mode);
	showImage();
}

void EnhancementWidget::doPlay()
{
	if (action_edit->isChecked())
	{
		CalOperator->test(MatType::Reference);
	}
	else
	{
		CalOperator->test(MatType::Current);
	}
	showImage();
}

void EnhancementWidget::doOk()
{
	syncOperator(CalOperator, SaveOperator);
	this->close();

}

void EnhancementWidget::doCancel()
{
}

void EnhancementWidget::on_image_cb_Changed(int index)
{
	if (SaveOperator == nullptr) 
		return;
	CalOperator->input_mat = Image_cb->getMat(index);
	CalOperator->input_refMat = Image_cb->getRefMat(index);
	showImage();
}

void EnhancementWidget::on_Mode_cb_Changed(int index)
{
	CalOperator->m_mode = (EnhancementOperator::EnhancementMode)index;
	doPlay();
	showImage();
}

void EnhancementWidget::syncOperator(EnhancementOperator * AA, EnhancementOperator * BB)
{
	BB->m_state = AA->m_state;
	BB->setDuration(AA->getDuration());
	BB->input_mat = AA->input_mat;
	BB->input_refMat = AA->input_refMat;
	BB->m_mode = AA->m_mode;
	BB->output_refMat->SetValue(AA->output_refMat->GetValue());
	BB->output_mat->SetValue(AA->output_mat->GetValue());
}

void EnhancementWidget::showImage()
{
	if (action_edit->isChecked())
	{
		if (CalOperator->output_refMat)
		{
			this->fr_view->setImage(CalOperator->output_refMat->GetValue());
		}
	}
	else
	{
		if (CalOperator->output_mat)
		{
			this->fr_view->setImage(CalOperator->output_mat->GetValue());
		}
	}
}

void EnhancementWidget::makeWidget()
{
	Image_cb = new InputBox();
	Mode_cb = new QComboBox();

	Image_cb->setDataType(ToolDataType::eMat);
	Mode_cb->addItems(QStringList() << tr("EqualizeHist") << tr("Laplace") << "Gamma" << "LOG");
	QFont ft;
	ft.setPointSize(14);
	QGroupBox*Image_gb = new QGroupBox("Source Image");
	Image_gb->setFont(ft);
	QLabel *Image_lb = new QLabel("Source Image:");
	ft.setPointSize(10);
	Image_lb->setFont(ft);
	QHBoxLayout *hlay = new QHBoxLayout(this);
	//hlay->addWidget(Image_lb);
	hlay->addWidget(Image_cb);
	Image_gb->setLayout(hlay);

	ft.setPointSize(14);
	QGroupBox*Mode_gb = new QGroupBox("Enhance Mode");
	Mode_gb->setFont(ft);
	QLabel *Mode_lb = new QLabel(" Mode:");
	ft.setPointSize(10);
	Mode_lb->setFont(ft);
	QHBoxLayout *hlay1 = new QHBoxLayout();
	//hlay1->addWidget(Mode_lb);
	hlay1->addWidget(Mode_cb);
	Mode_gb->setLayout(hlay1);

	QVBoxLayout *vlay = new QVBoxLayout(this);
	vlay->addWidget(Image_gb);
	vlay->addWidget(Mode_gb);
	vlay->addStretch();
	this->BaseParamWidget->setLayout(vlay);

	connect(Image_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(on_image_cb_Changed(int)));
	connect(Mode_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(on_Mode_cb_Changed(int)));
}

