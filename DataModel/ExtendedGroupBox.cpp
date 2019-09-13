#include "ExtendedGroupBox.h"

ExtendedGroupBox::ExtendedGroupBox(QWidget *parent /*= nullptr*/, State state /*= STATE_NORMAL*/)
	: QGroupBox(parent)
{
	setObjectName("ExtendedGroupBox");
	setCheckable(true);
	state_ = state;
	if (state_ == STATE_NORMAL)
	{
		//Òþ²Ø´¹Ö±±ß¿ò
		setFlat(true);
		setChecked(false);
	}
	connect(this, SIGNAL(clicked(bool)), this, SLOT(onChecked(bool)));

}

ExtendedGroupBox::ExtendedGroupBox(const QString &title, QWidget *parent /*= nullptr*/, State state /*= STATE_NORMAL*/)
	: QGroupBox(title, parent)
{
	setObjectName("ExtendedGroupBox");
	setCheckable(true);
	state_ = state;
	if (state_ == STATE_NORMAL)
	{
		//Òþ²Ø´¹Ö±±ß¿ò
		setFlat(true);
		setChecked(false);
	}
	connect(this, SIGNAL(clicked(bool)), this, SLOT(onChecked(bool)));
}

ExtendedGroupBox::~ExtendedGroupBox()
{
}


void ExtendedGroupBox::addWidget(QWidget *widget)
{
	if (widget != nullptr)
	{
		if (state_ == STATE_NORMAL)
		{
			widget->setVisible(false);
		}
		children_.push_back(widget);
	}
}


void ExtendedGroupBox::onChecked(bool checked)
{
	if (checked)
	{
		//ÏÔÊ¾´¹Ö±±ß¿ò
		setFlat(false);
		for (auto iter = children_.begin(); iter != children_.end(); ++iter)
		{
			(*iter)->setVisible(true);
		}
		state_ = STATE_EXPAND;
	}
	else
	{
		//Òþ²Ø´¹Ö±±ß¿ò
		setFlat(true);
		for (auto iter = children_.begin(); iter != children_.end(); ++iter)
		{
			(*iter)->setVisible(false);
		}
		state_ = STATE_NORMAL;
	}
}



ExtendedGroupBox::State ExtendedGroupBox::getState() const
{
	return state_;
}
