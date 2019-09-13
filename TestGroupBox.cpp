#include "ExtendedGroupBox.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QCheckBox>
#include <QProgressBar>
#include <QFile>

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QFile file(":/CV_TEAM/stylesheets/style.qss");
	bool s = file.open(QFile::ReadOnly);
	a.setStyleSheet(file.readAll());
	file.close();

	QDialog w;
	QVBoxLayout *vbox_layout = new QVBoxLayout();

	//设置窗口的layout的SizeConstraint为SetFixedSize，
	//否则在ExtendedGroupBox收缩时，无法动态调整大小。
	vbox_layout->setSizeConstraint(QLayout::SetFixedSize);
	w.setLayout(vbox_layout);

	QGroupBox *setting_group_box = new QGroupBox(("Setting"));
	QHBoxLayout *setting_layout = new QHBoxLayout();
	setting_group_box->setLayout(setting_layout);

	QCheckBox *check_box1 = new QCheckBox(("CheckBox1"));
	QCheckBox *check_box2 = new QCheckBox(("CheckBox2"));
	QCheckBox *check_box3 = new QCheckBox(("CheckBox3"));
	setting_layout->addWidget(check_box1);
	setting_layout->addWidget(check_box2);
	setting_layout->addWidget(check_box3);

	ExtendedGroupBox *advanced_setting_group_box = new ExtendedGroupBox(("Advanced Setting"), &w, ExtendedGroupBox::STATE_NORMAL);
	QHBoxLayout *advanced_setting_layout = new QHBoxLayout();
	advanced_setting_group_box->setLayout(advanced_setting_layout);

	QCheckBox *check_box4 = new QCheckBox(("CheckBox4"));
	QCheckBox *check_box5 = new QCheckBox(("CheckBox5"));
	QCheckBox *check_box6 = new QCheckBox(("CheckBox6"));
	advanced_setting_layout->addWidget(check_box4);
	advanced_setting_layout->addWidget(check_box5);
	advanced_setting_layout->addWidget(check_box6);

	//将子控件添加到ExtendedGroupBox中
	advanced_setting_group_box->addWidget(check_box4);
	advanced_setting_group_box->addWidget(check_box5);
	advanced_setting_group_box->addWidget(check_box6);

	vbox_layout->addWidget(setting_group_box);
	vbox_layout->addWidget(advanced_setting_group_box);

	w.show();
	return a.exec();
}