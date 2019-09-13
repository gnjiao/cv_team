#include "MorphologyWidget.h"

MorphologyWidget::MorphologyWidget(QWidget *parent)
	: BaseWidget(parent)
{
	makeWidget();
	initWidget();
}

MorphologyWidget::~MorphologyWidget()
{
	delete CalOperator;
}

void MorphologyWidget::doPlay()
{
}

void MorphologyWidget::doOk()
{
}

void MorphologyWidget::doCancel()
{
}

void MorphologyWidget::doEdit(bool b)
{
}

void MorphologyWidget::doEditSave()
{
}

void MorphologyWidget::setOperator(MorphologyOperator * Opra)
{
}

void MorphologyWidget::updateOutput()
{
}

InputBox * MorphologyWidget::getImageComboBox()
{
	return nullptr;
}

void MorphologyWidget::on_MorphologyMode_cob_Changed(int index)
{

}

void MorphologyWidget::on_KernelShape_cob_Changed(int index)
{

}

void MorphologyWidget::on_KernelSize_sl_Changed(int value)
{
	this->KernelSize_sp->setValue(value);
}

void MorphologyWidget::on_KernelSize_sp_Changed(int value)
{
	KernelSize_sl->setValue(value);
}

void MorphologyWidget::on_IterCount_sl_Changed(int value)
{
	this->IterCount_sp->setValue(value);
}

void MorphologyWidget::on_IterCount_sp_Changed(int value)
{
	this->IterCount_sl->setValue(value);
}

void MorphologyWidget::makeWidget()
{
	image_cb = new InputBox();
	MorphologyMode_cob = new QComboBox();
	KernelShape_cob = new QComboBox();
	KernelSize_sl = new QSlider(Qt::Horizontal);
	KernelSize_sp = new QSpinBox();
	IterCount_sl = new QSlider(Qt::Horizontal);
	IterCount_sp = new QSpinBox();

	QGroupBox *image_gb = new QGroupBox("Input Image");
	QHBoxLayout *hlay = new QHBoxLayout();
	hlay->addWidget(image_cb);
	image_gb->setLayout(hlay);

	QGroupBox *param_gb = new QGroupBox("Setting Parameter");

	QLabel *Mode_lb = new QLabel("Kernel Shape:");
	QHBoxLayout *hlay_mode = new QHBoxLayout();
	hlay_mode->addWidget(Mode_lb);
	hlay_mode->addWidget(MorphologyMode_cob);

	QLabel *shape_lb = new QLabel("Kernel Shape:");
	QHBoxLayout *hlay1 = new QHBoxLayout();
	hlay1->addWidget(shape_lb);
	hlay1->addWidget(KernelShape_cob);

	QGroupBox *kernelSize_gb = new QGroupBox("Kernel Size");
	QHBoxLayout *hlay2 = new QHBoxLayout();
	hlay2->addWidget(KernelSize_sl);
	hlay2->addWidget(KernelSize_sp);
	kernelSize_gb->setLayout(hlay2);

	QGroupBox *iterCount_gb = new QGroupBox("Iterator Count");
	QHBoxLayout *hlay3 = new QHBoxLayout();
	hlay3->addWidget(IterCount_sl);
	hlay3->addWidget(IterCount_sp);
	iterCount_gb->setLayout(hlay3);

	QVBoxLayout *vlay1 = new QVBoxLayout();
	vlay1->addLayout(hlay_mode);
	vlay1->addLayout(hlay1);
	vlay1->addWidget(kernelSize_gb);
	vlay1->addWidget(iterCount_gb);
	vlay1->setSpacing(30);
	param_gb->setLayout(vlay1);

	QVBoxLayout *vlay = new QVBoxLayout();
	vlay->addWidget(image_gb);
	vlay->addWidget(param_gb);
	vlay->addStretch();
	vlay->setSpacing(30);
	this->BaseParamWidget->setLayout(vlay);

}

void MorphologyWidget::initWidget()
{
	MorphologyMode_cob->addItems(QStringList()
		<< tr("MORPH_ERODE")
		<< tr("MORPH_DILATE")
		<< tr("MORPH_OPEN")
		<< tr("MORPH_CLOSE")
		<< tr("MORPH_GRADIENT")
		<< tr("MORPH_TOPHAT")
		<< tr("MORPH_BLACKHAT")
		<< tr("MORPH_HITMISS"));

	KernelShape_cob->addItems(QStringList()
		<< tr("MORPH_RECT")
		<< tr("MORPH_CROSS")
		<< tr("MORPH_ELLIPSE"));

	connect(MorphologyMode_cob, SIGNAL(currentIndexChanged(int)), this, SLOT(on_MorphologyMode_cob_Changed(int)));
	connect(KernelShape_cob, SIGNAL(currentIndexChanged(int)), this, SLOT(on_KernelShape_cob_Changed(int)));

	connect(KernelSize_sl, SIGNAL(valueChanged(int)), this, SLOT(on_KernelSize_sl_Changed(int)));
	connect(KernelSize_sp, SIGNAL(valueChanged(int)), this, SLOT(on_KernelSize_sp_Changed(int)));

	connect(IterCount_sl, SIGNAL(valueChanged(int)), this, SLOT(on_IterCount_sl_Changed(int)));
	connect(IterCount_sp, SIGNAL(valueChanged(int)), this, SLOT(on_IterCount_sp_Changed(int)));
	

}

void MorphologyWidget::syncOperator(MorphologyOperator * AA, MorphologyOperator * BB)
{
}

void MorphologyWidget::showImage()
{
}
