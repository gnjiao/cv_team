#pragma once
#include <QTreeWidget>
#include <QColorDialog>
#include "DataModel\BaseWidget.h"
#include "DataModel\BaseOperator.h"
#include "GraphicsItem\PointSetItem.h"
#include "GraphicsItem\LineSetItem.h"
#include "GraphicsItem\CircleSetItem.h"
#include "GraphicsItem\TextItem.h"
#include <map>
#include <QMouseEvent>
#include <string>
#include <vector>
class MButton :public QPushButton
{
	Q_OBJECT
public:
	MButton(int k,QWidget *parent = Q_NULLPTR);
	~MButton();
	void setKey(int k);
	int getKey();
protected:
	virtual void mousePressEvent(QMouseEvent *e);
private:
	int key;
signals:
	void ColorButtonClicked(int);
};
class DisplayWidget : public BaseWidget
{
	Q_OBJECT
public:
	DisplayWidget(QWidget *parent = Q_NULLPTR);
	~DisplayWidget();
	void setKey(int key);
	void doConnect();
	void doPlay();
	void doOk();
	void doCancel();
	void updateDisplayTree();
private:
	void makeBaseWidget();
	std::vector<std::string>SplitString(string srcStr, const string& delim);
	void showAllOutputItems();
	QList<QGraphicsItem*> m_showItems;
	int m_insKey=0;
	int m_buttonIndex = 0;
	int m_pushKey = 0;
	std::map<int, std::string> Button_map;
	QTreeWidget* DisplayTreeWidget;
	QColorDialog *m_ColorDialog;
	Inspection *m_inspection;
	void getOutputByKey(BaseOperator* baseOpera, int key, QString &text, bool &isEnabled, bool &isChecked);
	void setOutputByKey(BaseOperator* baseOpera, int key, bool isChecked);
	void setOutputByString(BaseOperator* baseOpera, std::string, QColor);
private slots:
	void on_DisplayTreeWidgetItemChanged(QTreeWidgetItem* item, int column);
	void on_TreeWidgetClicked(QTreeWidgetItem* item, int column);
	void on_2Color_Changed(int);
	void on_Display_getcolor(QColor);
signals:
	void DisplaySettingOK();
};