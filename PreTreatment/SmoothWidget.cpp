#include "SmoothWidget.h"

SmoothWidget::SmoothWidget(QWidget *parent)
	: BaseWidget(parent)
{
	makeWidget();

}

SmoothWidget::~SmoothWidget()
{
	delete CalOperator;
}

InputBox * SmoothWidget::getImageComboBox()
{
	return nullptr;
}

void SmoothWidget::setOperator(SmoothOperator * opra)
{
}

void SmoothWidget::doPlay()
{
}

void SmoothWidget::doOk()
{
}

void SmoothWidget::doCancel()
{
}

void SmoothWidget::on_image_cb_Changed(int index)
{
}

void SmoothWidget::syncOperator(SmoothOperator * AA, SmoothOperator * BB)
{
}

void SmoothWidget::showImage()
{
}

void SmoothWidget::makeWidget()
{


}

