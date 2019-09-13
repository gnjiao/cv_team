#pragma once

#include <QGroupBox>
#include <QVector>

class ExtendedGroupBox : public QGroupBox
{
	Q_OBJECT

public:	
	
	enum State
	{
		STATE_NORMAL,
		STATE_EXPAND
	};

	ExtendedGroupBox(QWidget *parent = Q_NULLPTR, State state = STATE_NORMAL);
	ExtendedGroupBox(const QString &title, QWidget *parent = nullptr, State state = STATE_NORMAL);
	~ExtendedGroupBox();


	void addWidget(QWidget *widget);
	State getState() const;

private slots:
	void onChecked(bool checked);

private:
	QVector<QWidget*> children_;
	State state_;
};
