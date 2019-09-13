#include "DisplayWidget.h"

DisplayWidget::DisplayWidget(QWidget *parent)
	: BaseWidget(parent)
{
	this->resize(1000, 800);
	this->setWindowTitle("DisplayWidget");
	
	makeBaseWidget();

}
DisplayWidget::~DisplayWidget()
{
}
void DisplayWidget::setKey(int key)
{
	m_insKey = key;
	m_inspection = InspectionMGR->FindBy(m_insKey);
	updateDisplayTree();
	showAllOutputItems();
}
void DisplayWidget::doConnect()
{
	
}
void DisplayWidget::doPlay()
{
}
void DisplayWidget::doOk()
{
	emit DisplaySettingOK();
	this->close();
}
void DisplayWidget::doCancel()
{
	this->close();
}

void DisplayWidget::makeBaseWidget()
{
	DisplayTreeWidget = new QTreeWidget();
	QFont ft;
	ft.setPointSize(12);
	QTreeWidgetItem *headerItem = new QTreeWidgetItem();
	headerItem->setText(0, "m_inspection Output");
	headerItem->setFont(0, ft);
	headerItem->setTextAlignment(0, Qt::AlignHCenter | Qt::AlignVCenter);
	headerItem->setIcon(0, QIcon(":/CV_TEAM/icons/display.png"));

	headerItem->setText(1, "Color");
	headerItem->setFont(1, ft);
	headerItem->setTextAlignment(1, Qt::AlignHCenter | Qt::AlignVCenter);
	//headerItem->setIcon(1, QIcon(":/CV_TEAM/icons/color.png"));

	DisplayTreeWidget->setHeaderItem(headerItem);
	DisplayTreeWidget->setColumnCount(2);
	DisplayTreeWidget->setColumnWidth(0, 210);
	DisplayTreeWidget->setColumnWidth(1, 40);
	//DisplayTreeWidget->setColumnWidth(0, 200);
	QVBoxLayout *BasePara_layout = new QVBoxLayout();
	BasePara_layout->addWidget(DisplayTreeWidget);
	this->BaseParamWidget->setLayout(BasePara_layout);
	connect(DisplayTreeWidget, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(on_DisplayTreeWidgetItemChanged(QTreeWidgetItem*, int)));
	//connect(DisplayTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(on_TreeWidgetClicked(QTreeWidgetItem*, int)));
}
std::vector<std::string> DisplayWidget::SplitString(string srcStr, const string & delim)
{

	int nPos = 0;
	vector<string> vec;
	nPos = srcStr.find(delim.c_str());
	while (-1 != nPos)
	{
		string temp = srcStr.substr(0, nPos);
		vec.push_back(temp);
		srcStr = srcStr.substr(nPos + 1);
		nPos = srcStr.find(delim.c_str());
	}
	vec.push_back(srcStr);
	return vec;
}
void DisplayWidget::showAllOutputItems()
{
	
	if (m_inspection == nullptr)
	{
		return;
	}
	cv::Mat image = m_inspection->GetAcquisition()->GetCurrentMat().clone();
	cv::Mat output;
	output = image;
	if ((image.channels() == 1) && (!image.empty()))
	{
		std::vector<cv::Mat> chanels;
		output = cv::Mat::zeros(image.rows, image.cols, CV_8UC3);
		for (size_t i = 0; i < 3; i++)
		{
			chanels.push_back(image);
		}
		cv::merge(chanels, output);
	}
	for each (auto var in m_showItems)
	{
		delete var;
		var = nullptr;
	}
	m_showItems.clear();
	for each (auto child in m_inspection->m_toolBlock->Children())
	{
		BaseOperator* shell = (BaseOperator*)child;
		if (toolModel::Success == shell->GetState())
		{
			for each (auto var in shell->output()->vectorCircle)
			{
				if (!var->GetDisplayChecked())
				{
					CircleSetItem* circleItem = new CircleSetItem();
					circleItem->setFlag(QGraphicsItem::ItemIsMovable, false);
					circleItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
					circleItem->setCircle(var->GetValue());
					QColor color=QColor(var->GetColorR(),var->GetColorG(),var->GetColorB());
					circleItem->setColor(color);
					m_showItems.append(circleItem);
					//double radius = var->GetValue().Radius();
					//std::stringstream s1;
					//s1 << radius;
					//std::stringstream s2;
					//s2 << circle_index;
					////cv::circle(src, cv::Point2d(center_x, center_y), radius, Scalar(0, 255, 0));
					//cv::putText(output, "R" + s2.str() + "=" + s1.str(), Point2d(15, 25 + 25 * index), cv::FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 255, 0), 1);
					//index++;
					//circle_index++;
				}
			}
			for each (auto var in shell->output()->vectorLine)//??
			{
				if (!var->GetDisplayChecked())
				{
					LineSetItem* lineItem = new LineSetItem();
					lineItem->setFlag(QGraphicsItem::ItemIsMovable, false);
					lineItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
					lineItem->setLine(var->GetValue());
					QColor color = QColor(var->GetColorR(), var->GetColorG(), var->GetColorB());
					lineItem->setColor(color);
					m_showItems.append(lineItem);
					//double length = var->GetValue().Length();
					//std::stringstream s1;
					//s1 << length;
					//std::stringstream s2;
					//s2 << line_index;
					////cv::line(src, cv::Point2d(start_point_x, start_point_y), cv::Point2d(end_point_x, end_point_y), Scalar(0, 255, 0));
					//cv::putText(output, "L" + s2.str() + "=" + s1.str(), Point2d(15, 25 + 25 * index), cv::FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 255, 0), 1);
					//index++;
					//line_index++;
				}
			}
			for each (auto var in shell->output()->vectorPoint)
			{
				if (!var->GetDisplayChecked())
				{
					PointSetItem* pointItem = new PointSetItem();
					pointItem->setFlag(QGraphicsItem::ItemIsMovable, false);
					pointItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
					pointItem->setPoint(var->GetValue());
					QColor color = QColor(var->GetColorR(), var->GetColorG(), var->GetColorB());
					pointItem->setColor(color);
					m_showItems.append(pointItem);
					//double point_x = var->GetValue().X();
					//double point_y = var->GetValue().Y();
					//std::stringstream s1;
					//s1 << point_x;
					//std::stringstream s2;
					//s2 << point_y;
					//cv::putText(output, s1.str() + " " + s2.str(), Point2d(15, 25 + 25 * index), cv::FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 255, 0), 1);
					//index++;
				}
			}
			for each (auto var in shell->output()->vectorRegion)
			{
				if (var->GetDisplayChecked())
					shell->drawFeatures(output);
			}
			for each (auto var in shell->output()->vectorFeaturePoints)
			{
				if (var->GetDisplayChecked())
					shell->drawFeatures(output);
				else
				{
					return;
				}
			}
		}
	}
	for each (auto var in m_showItems)
	{
		this->fr_view->getScene()->addItem(var);
	}
	if (output.empty())
		return;
	this->fr_view->setImage(output);
}
void DisplayWidget::getOutputByKey(BaseOperator* BaseOperator, int k, QString & itemText, bool & isEnabled, bool & isChecked)
{
	for each (auto var in BaseOperator->output()->vectorAffineTrans)
	{
		if (var->GetDataKey() == k)
		{
			itemText = QString::fromStdString(var->GetShowText());
			isEnabled = var->GetDisplayEnabled();
			isChecked = var->GetDisplayChecked();
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorAngle)
	{
		if (var->GetDataKey() == k)
		{
			itemText = QString::fromStdString(var->GetShowText());
			isEnabled = var->GetDisplayEnabled();
			isChecked = var->GetDisplayChecked();
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorArea)
	{
		if (var->GetDataKey() == k)
		{
			itemText = QString::fromStdString(var->GetShowText());
			isEnabled = var->GetDisplayEnabled();
			isChecked = var->GetDisplayChecked();
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorBool)
	{
		if (var->GetDataKey() == k)
		{
			itemText = QString::fromStdString(var->GetShowText());
			isEnabled = var->GetDisplayEnabled();
			isChecked = var->GetDisplayChecked();
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorCircle)
	{
		if (var->GetDataKey() == k)
		{
			itemText = QString::fromStdString(var->GetShowText());
			isEnabled = var->GetDisplayEnabled();
			isChecked = var->GetDisplayChecked();
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorDouble)
	{
		if (var->GetDataKey() == k)
		{
			itemText = QString::fromStdString(var->GetShowText());
			isEnabled = var->GetDisplayEnabled();
			isChecked = var->GetDisplayChecked();
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorInt)
	{
		if (var->GetDataKey() == k)
		{
			itemText = QString::fromStdString(var->GetShowText());
			isEnabled = var->GetDisplayEnabled();
			isChecked = var->GetDisplayChecked();
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorLength)
	{
		if (var->GetDataKey() == k)
		{
			itemText = QString::fromStdString(var->GetShowText());
			isEnabled = var->GetDisplayEnabled();
			isChecked = var->GetDisplayChecked();
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorLine)
	{
		if (var->GetDataKey() == k)
		{
			itemText = QString::fromStdString(var->GetShowText());
			isEnabled = var->GetDisplayEnabled();
			isChecked = var->GetDisplayChecked();
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorMat)
	{
		if (var->GetDataKey() == k)
		{
			itemText = QString::fromStdString(var->GetShowText());
			isEnabled = var->GetDisplayEnabled();
			isChecked = var->GetDisplayChecked();
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorPoint)
	{
		if (var->GetDataKey() == k)
		{
			itemText = QString::fromStdString(var->GetShowText());
			isEnabled = var->GetDisplayEnabled();
			isChecked = var->GetDisplayChecked();
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorRefMat)
	{
		if (var->GetDataKey() == k)
		{
			itemText = QString::fromStdString(var->GetShowText());
			isEnabled = var->GetDisplayEnabled();
			isChecked = var->GetDisplayChecked();
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorScore)
	{
		if (var->GetDataKey() == k)
		{
			itemText = QString::fromStdString(var->GetShowText());
			isEnabled = var->GetDisplayEnabled();
			isChecked = var->GetDisplayChecked();
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorString)
	{
		if (var->GetDataKey() == k)
		{
			itemText = QString::fromStdString(var->GetShowText());
			isEnabled = var->GetDisplayEnabled();
			isChecked = var->GetDisplayChecked();
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorFeaturePoints)
	{
		if (var->GetDataKey() == k)
		{
			itemText = QString::fromStdString(var->GetShowText());
			isEnabled = var->GetDisplayEnabled();
			isChecked = var->GetDisplayChecked();
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorRegion)
	{
		if (var->GetDataKey() == k)
		{
			itemText = QString::fromStdString(var->GetShowText());
			isEnabled = var->GetDisplayEnabled();
			isChecked = var->GetDisplayChecked();
			return;
		}
	}
}
void DisplayWidget::setOutputByKey(BaseOperator * BaseOperator, int k, bool isChecked)
{
	for each (auto var in BaseOperator->output()->vectorAffineTrans)
	{
		if (var->GetDataKey() == k)
		{
			var->SetDisplayChecked(isChecked);
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorAngle)
	{
		if (var->GetDataKey() == k)
		{
			var->SetDisplayChecked(isChecked);
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorArea)
	{
		if (var->GetDataKey() == k)
		{
			var->SetDisplayChecked(isChecked);
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorBool)
	{
		if (var->GetDataKey() == k)
		{
			var->SetDisplayChecked(isChecked);
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorCircle)
	{
		if (var->GetDataKey() == k)
		{
			var->SetDisplayChecked(isChecked);
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorDouble)
	{
		if (var->GetDataKey() == k)
		{
			var->SetDisplayChecked(isChecked);
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorInt)
	{
		if (var->GetDataKey() == k)
		{
			var->SetDisplayChecked(isChecked);
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorLength)
	{
		if (var->GetDataKey() == k)
		{
			var->SetDisplayChecked(isChecked);
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorLine)
	{
		if (var->GetDataKey() == k)
		{
			var->SetDisplayChecked(isChecked);
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorMat)
	{
		if (var->GetDataKey() == k)
		{
			var->SetDisplayChecked(isChecked);
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorPoint)
	{
		if (var->GetDataKey() == k)
		{
			var->SetDisplayChecked(isChecked);
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorRefMat)
	{
		if (var->GetDataKey() == k)
		{
			var->SetDisplayChecked(isChecked);
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorScore)
	{
		if (var->GetDataKey() == k)
		{
			var->SetDisplayChecked(isChecked);
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorString)
	{
		if (var->GetDataKey() == k)
		{
			var->SetDisplayChecked(isChecked);
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorFeaturePoints)
	{
		if (var->GetDataKey() == k)
		{
			var->SetDisplayChecked(isChecked);
			return;
		}
	}
	for each (auto var in BaseOperator->output()->vectorRegion)
	{
		if (var->GetDataKey() == k)
		{
			var->SetDisplayChecked(isChecked);
			return;
		}
	}
}
void DisplayWidget::setOutputByString(BaseOperator * baseOpera, std::string str, QColor color)
{
	for each (auto var in baseOpera->output()->vectorAffineTrans)
	{
		if (var->GetShowText() == str)
		{
			var->SetColor(color.red(), color.green(), color.blue());
				return;
		}
	}
	for each (auto var in baseOpera->output()->vectorAngle)
	{
		if (var->GetShowText() == str)
		{
			var->SetColor(color.red(), color.green(), color.blue());
				return;
		}
	}
	for each (auto var in baseOpera->output()->vectorArea)
	{
		if (var->GetShowText() == str)
		{
			var->SetColor(color.red(), color.green(), color.blue());
				return;
		}
	}
	for each (auto var in baseOpera->output()->vectorBool)
	{
		if (var->GetShowText() == str)
		{
			var->SetColor(color.red(), color.green(), color.blue());
				return;
		}
	}
	for each (auto var in baseOpera->output()->vectorCircle)
	{
		if (var->GetShowText() == str)
		{
			var->SetColor(color.red(), color.green(), color.blue());
				return;
		}
	}
	for each (auto var in baseOpera->output()->vectorDouble)
	{
		if (var->GetShowText() == str)
		{
			var->SetColor(color.red(), color.green(), color.blue());
				return;
		}
	}
	for each (auto var in baseOpera->output()->vectorInt)
	{
		if (var->GetShowText() == str)
		{
			var->SetColor(color.red(), color.green(), color.blue());
				return;
		}
	}
	for each (auto var in baseOpera->output()->vectorLength)
	{
		if (var->GetShowText() == str)
		{
			var->SetColor(color.red(), color.green(), color.blue());
				return;
		}
	}
	for each (auto var in baseOpera->output()->vectorLine)
	{
		std::string s1 = var->GetShowText();
		if ( s1== str)
		{
			var->SetColor(color.red(), color.green(), color.blue());
			return;
		}
	}
	for each (auto var in baseOpera->output()->vectorMat)
	{
		if (var->GetShowText() == str)
		{
			var->SetColor(color.red(), color.green(), color.blue());
			return;
		}
	}
	for each (auto var in baseOpera->output()->vectorPoint)
	{
		if (var->GetShowText() == str)
		{
			var->SetColor(color.red(), color.green(), color.blue());
			return;
		}
	}
	for each (auto var in baseOpera->output()->vectorRefMat)
	{
		if (var->GetShowText() == str)
		{
			var->SetColor(color.red(), color.green(), color.blue());
				return;
		}
	}
	for each (auto var in baseOpera->output()->vectorScore)
	{
		if (var->GetShowText() == str)
		{
			var->SetColor(color.red(), color.green(), color.blue());
				return;
		}
	}
	for each (auto var in baseOpera->output()->vectorString)
	{
		if (var->GetShowText() == str)
		{
			var->SetColor(color.red(), color.green(), color.blue());
				return;
		}
	}
	for each (auto var in baseOpera->output()->vectorFeaturePoints)
	{
		if (var->GetShowText() == str)
		{
			var->SetColor(color.red(), color.green(), color.blue());
				return;
		}
	}
	for each (auto var in baseOpera->output()->vectorRegion)
	{
		if (var->GetShowText() == str)
		{
			var->SetColor(color.red(), color.green(), color.blue());
				return;
		}
	}
}
void DisplayWidget::on_TreeWidgetClicked(QTreeWidgetItem * item, int column)
{
	QString ii=item->parent()->text(0);
	QString iii = item->text(0);
	int k = DisplayTreeWidget->indexOfTopLevelItem(item);
}
void DisplayWidget::on_2Color_Changed(int key)
{
	m_pushKey = key;
	m_ColorDialog = new QColorDialog;
	map<int, std::string>::const_iterator it = Button_map.find(m_pushKey);
	if (it == Button_map.end())
	{
		return;
	}
	std::string str = it->second;
	m_ColorDialog->setWindowTitle(QString::fromStdString(str));	
	connect(m_ColorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(on_Display_getcolor(QColor)));
	m_ColorDialog->exec();
}
void DisplayWidget::on_Display_getcolor(QColor color)
{
	map<int, std::string>::const_iterator it = Button_map.find(m_pushKey);
	if (it == Button_map.end())
	{
		return;
	}
	std::string str = it->second;
	vector<std::string> temp;
	temp = SplitString(str, ",");
	if (temp.size() != 2)
	{
		return;
	}
	for each(auto var in m_inspection->m_toolBlock->Children())
	{

		std::string s1 = var->GetNodeShowText() + to_string(var->GetNodeKey());
		std::string s2 = temp[0];
		if (s1==s2)
		{
			BaseOperator* baseOpera = (BaseOperator*)var;
			setOutputByString(baseOpera, temp[1], color);
			break;
		}
	}
	showAllOutputItems();
}
void DisplayWidget::updateDisplayTree()
{
	if (m_inspection == nullptr)
	{
			return;
	}
	if (m_inspection->m_toolBlock == nullptr)
	{
		return;
	}
	DisplayTreeWidget->clear();
	for (int index = 0; index < m_inspection->m_toolBlock->Children().size(); index++)
	{
		BaseOperator* BaseOpera = (BaseOperator*)(m_inspection->m_toolBlock->GetChild(index));
		QTreeWidgetItem* shellRootItem = new QTreeWidgetItem(DisplayTreeWidget);
		shellRootItem->setText(0, QString::fromStdString(BaseOpera->GetNodeShowText())
			+ QString::number(BaseOpera->GetNodeKey()));
		DisplayTreeWidget->addTopLevelItem(shellRootItem);
		for (size_t k = 0; k < BaseOpera->maxDataKey(); k++)
		{
			QTreeWidgetItem* shellOutputItem = new QTreeWidgetItem(shellRootItem);
			shellRootItem->addChild(shellOutputItem);
			QString itemText;
			bool isEnabled = false;
			bool isChecked = false;
			getOutputByKey(BaseOpera, k, itemText, isEnabled, isChecked);
			shellOutputItem->setText(0, itemText);
			if (isEnabled)
			{
				MButton *tempButton = new MButton(m_buttonIndex);
				tempButton->setText("...");
				Button_map.insert(make_pair(m_buttonIndex, shellRootItem->text(0).toStdString() + "," + shellOutputItem->text(0).toStdString()));
				m_buttonIndex++;
				connect(tempButton, SIGNAL(ColorButtonClicked(int)), this, SLOT(on_2Color_Changed(int)));
				DisplayTreeWidget->setItemWidget(shellOutputItem, 1, tempButton);
				
				if (isChecked)
				{
					shellOutputItem->setCheckState(0, Qt::CheckState::Unchecked);
				}
				else
				{
					shellOutputItem->setCheckState(0, Qt::CheckState::Checked);
				}
			}
			else
			{
			}
		}
	}
	showAllOutputItems();
}
void DisplayWidget::on_DisplayTreeWidgetItemChanged(QTreeWidgetItem* item, int column)
{
	int index_shell = -1;
	int index_output = -1;
	bool isChecked = false;
	if (item->checkState(0) == Qt::CheckState::Unchecked)
	{
		isChecked = true;
	}
	QTreeWidgetItem* parentItem = item->parent();
	if (parentItem)
	{
		index_shell = this->DisplayTreeWidget->indexOfTopLevelItem(parentItem);
		index_output = parentItem->indexOfChild(item);
		if (m_inspection == nullptr)
		{
			return;
		}
		BaseOperator* BaseOpera = (BaseOperator*)m_inspection->m_toolBlock->GetChild(index_shell);
		setOutputByKey(BaseOpera, index_output, isChecked);
	}
	showAllOutputItems();
}

MButton::MButton(int k, QWidget *parent)
	:QPushButton(parent)
{
	this->key = k;
}
MButton::~MButton()
{
}


void MButton::setKey(int k)
{
}

int MButton::getKey()
{
	return 0;
}

void MButton::mousePressEvent(QMouseEvent * e)
{
	emit ColorButtonClicked(this->key);
}
