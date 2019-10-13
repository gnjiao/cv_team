#include "BarcodeWidget.h"

BarcodeWidget::BarcodeWidget(QWidget *parent)
	: BaseWidget(parent)
{
	makeWidget();
	CalOperator = new BarcodeOperator();
	CalOperator->initOutput();
	this->resize(1000, 800);
}
BarcodeWidget::~BarcodeWidget()
{
	delete CalOperator;
}
InputBox * BarcodeWidget::getImageComboBox()
{
	return this->Image_cb;
}
void BarcodeWidget::setOperator(BarcodeOperator * opra)
{
	if (opra == nullptr) { return; }
	SaveOperator = opra;
	syncOperator(SaveOperator, CalOperator);
	//update output
	updateOutput();
	showImage();
	this->fr_view->getScene()->update();
}
void BarcodeWidget::doPlay()
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
	updateOutput();
}
void BarcodeWidget::doOk()
{
	syncOperator(CalOperator, SaveOperator);
	this->close();
}
void BarcodeWidget::doCancel()
{
}
void BarcodeWidget::doEdit(bool checked)
{
	showImage();
}
void BarcodeWidget::on_image_cb_Changed(int index)
{
	if (SaveOperator == nullptr)
		return;
	CalOperator->input_mat = Image_cb->getMat(index);
	CalOperator->input_refMat = Image_cb->getRefMat(index);
	showImage();
}
void BarcodeWidget::on_Mode_cb_Changed(int index)
{
	//CalOperator->m_mode = (BarcodeOperator::EnhancementMode)index;
	doPlay();
	showImage();
}
void BarcodeWidget::syncOperator(BarcodeOperator * AA, BarcodeOperator * BB)
{
	BB->m_state = AA->m_state;
	BB->setDuration(AA->getDuration());
	BB->input_mat = AA->input_mat;
	BB->input_refMat = AA->input_refMat;
	//BB->m_mode = AA->m_mode;
	//BB->output_refMat->SetValue(AA->output_refMat->GetValue());
	BB->output_string->SetValue(AA->output_string->GetValue());
}
void BarcodeWidget::showImage()
{
	if (CalOperator->input_mat)
	{
		fr_view->setImage(CalOperator->input_mat->GetValue());
	}
}
void BarcodeWidget::makeWidget()
{
	Image_cb = new InputBox();
	Mode_cb = new QComboBox();
	Result_lb = new QLabel("empty result");
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
	vlay->addWidget(Result_lb);
	vlay->addStretch();
	this->BaseParamWidget->setLayout(vlay);
	resultItem = new TextItem(100, 100, 12);
	//resultItem->setText("this is test for textItem");
	this->fr_view->getScene()->addItem(resultItem);
	connect(Image_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(on_image_cb_Changed(int)));
	connect(Mode_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(on_Mode_cb_Changed(int)));
}
void BarcodeWidget::updateOutput()
{
	Result_lb->setText(QString::fromStdString(CalOperator->output_string->GetValue()));
}