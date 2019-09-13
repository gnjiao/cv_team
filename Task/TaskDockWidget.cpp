#include "TaskDockWidget.h"

TaskDockWidget::TaskDockWidget(QWidget *parent)
	: QDockWidget(parent)
{
	this->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	this->setWindowTitle("Task And Inspection Manager");
	makeTreeWidget();
	makeTaskWidget();
	makeLayout();
	initWidget();
	LoadInspection();
}
TaskDockWidget::~TaskDockWidget()
{
}
void TaskDockWidget::on_TabWidget_Changed(int index)
{
}
void TaskDockWidget::on_Add_btn_Clicked(bool)
{
	m_InspectionKey = InspectionItem[TaskWidget->currentIndex()].size();
	QTreeWidgetItem *addItem = new QTreeWidgetItem(TreeWidgetVector[TaskWidget->currentIndex()]);
	addItem->setIcon(0,QIcon(":/CV_TEAM/icons/inspection.png"));
	addItem->setCheckState(0, Qt::CheckState::Checked);
	InspectionItem[TaskWidget->currentIndex()].push_back(addItem);
	InspectionItem[TaskWidget->currentIndex()].at(m_InspectionKey)->setText(0, "Inspection " + QString::number(m_InspectionKey));
	Inspection *ins = new Inspection(m_InspectionKey);
	if (m_InspectionKey<0)
		m_InspectionKey = InspectionMGR->MaxKey() + 1;
	ins->SetName("Inspection" + to_string(m_InspectionKey));
	InspectionMGR->Insert(m_InspectionKey, ins);
	ins->MakeDir();
	AcquireImageOperator *opra = new AcquireImageOperator();
	ins->m_toolBlock->AddChild(opra, true);
	opra->setInspectionKey(ins->GetKey());
	opra->initOutput();
	opra->registerOutput();
	opra->setCoreParams();
	opra->save();
	InspectionMGR->Save();
	InspectionDialog *base = new InspectionDialog();
	base->setInspection(ins);
	base->updateTreeWidget();
	m_InsDialogs.push_back(base);
	m_InspectionKey = m_InspectionKey + 1;
}
void TaskDockWidget::on_Delete_btn_Clicked(bool)
{
	m_InspectionKey = InspectionItem[TaskWidget->currentIndex()].size();
	if (m_InspectionKey == 0)
		return;
	InspectionItem[TaskWidget->currentIndex()].pop_back();
	m_InspectionKey = m_InspectionKey - 1;
	TreeWidgetVector[TaskWidget->currentIndex()]->takeTopLevelItem(m_InspectionKey);
	//InspectionItem[TaskWidget->currentIndex()].at(m_InspectionKey)->setText(0, "Inspection ?" + QString::number(m_InspectionKey));
}
void TaskDockWidget::on_itemDoubleClicked(QTreeWidgetItem *item)
{
	int InspectionIndex = TreeWidgetVector[TaskWidget->currentIndex()]->indexOfTopLevelItem(item);
	Inspection *pinspection = InspectionMGR->FindBy(InspectionIndex);
	if (nullptr == pinspection)
		return;
	QList<InspectionDialog*>::iterator itr = m_InsDialogs.begin();
	for (; itr < m_InsDialogs.end(); ++itr)
	{
		if (nullptr == (*itr))
			continue;
		if ((*itr)->m_inspection == pinspection)
		{
			QString Title = QString::fromStdWString(L"¼ì²âÁ÷³Ì") + QString::number(pinspection->GetKey());
			(*itr)->getFrameView()->setImage(pinspection->GetAcquisition()->GetCurrentMat());
			(*itr)->updateTreeWidget();
			(*itr)->setWindowTitle(Title);
			(*itr)->setTitle(Title);
			(*itr)->show();
			(*itr)->showNormal();
			this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
			return;
		}
	}
}
void TaskDockWidget::makeTaskWidget()
{
	TaskWidget = new QTabWidget(this);
	TaskWidget->setTabPosition(QTabWidget::West);
	TaskWidget->setTabShape(QTabWidget::Triangular);
	TaskWidget->widget(TaskWidget->addTab(TreeWidgetVector[0], tr("Task 1 ")));
	TaskWidget->widget(TaskWidget->addTab(TreeWidgetVector[1], tr("Task 2 ")));
	TaskWidget->widget(TaskWidget->addTab(TreeWidgetVector[2], tr("Task 3 ")));
}
void TaskDockWidget::makeTreeWidget()
{
	for (size_t i = 0; i < 3/*Task.size()*/; i++)
	{
		QTreeWidget *TreeWidget = new QTreeWidget();
		QVector<QTreeWidgetItem*>temp;
		InspectionItem.push_back(temp);
		TreeWidget->setColumnCount(1);
		TreeWidget->setHeaderHidden(true);
		TreeWidget->setStyleSheet(
			"QTreeWidget::item{ height:40px;}"
			"QTreeWidget::item:hover{ background-color:rgb(119,136,153,50) }"
			"QTreeWidget::item:selected{ background-color:rgb(119,136,153,100) }"
			);
		TreeWidgetVector.push_back(TreeWidget);
	}
	//QVector<QTreeWidgetItem*> temp;
	//for (size_t i = 0;i < 3;i++)//Size of Task
	//{?? ?
	//?? ?temp.clear();
	//?? ?for (size_t j = 0;j < 5;j++)//Size of Inspection
	//?? ?{
	//?? ??? ?temp.push_back(new QTreeWidgetItem(TreeWidgetVector[i]));
	//?? ?
	//?? ??? ?QFont ft;
	//?? ??? ?ft.setPointSize(12);
	//?? ??? ?temp[j]->setFont(0,ft);
	//?? ??? ?temp[j]->setText(0,"Inspection ?" + QString::number(j));
	//?? ??? ?
	//?? ?}
	//?? ?InspectionItem.push_back(temp);
	//}
}
void TaskDockWidget::makeLayout()
{
	Add_btn = new QPushButton("+");
	Delete_btn = new QPushButton("-");
	Add_btn->setMinimumWidth(80);
	Delete_btn->setMinimumWidth(80);
	QHBoxLayout *hlay = new QHBoxLayout();
	hlay->addWidget(Add_btn);
	hlay->addWidget(Delete_btn);
	QWidget *widget = new QWidget();
	QVBoxLayout *vlay = new QVBoxLayout();
	vlay->addLayout(hlay);
	vlay->addWidget(TaskWidget);
	TaskWidget->setMinimumWidth(300);
	widget->setLayout(vlay);
	this->setWidget(widget);
}
void TaskDockWidget::initWidget()
{
	connect(TaskWidget, SIGNAL(currentChanged(int)), this, SLOT(on_TabWidget_Changed(int)));
	connect(Add_btn, SIGNAL(clicked(bool)), this, SLOT(on_Add_btn_Clicked(bool)));
	connect(Delete_btn, SIGNAL(clicked(bool)), this, SLOT(on_Delete_btn_Clicked(bool)));
	for (size_t i = 0; i < TreeWidgetVector.size(); i++)
	{
		connect(TreeWidgetVector[i], SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(on_itemDoubleClicked(QTreeWidgetItem*)));
	}
}
void TaskDockWidget::LoadInspection()
{
	InspectionMGR->Load();

	for each (auto var in InspectionMGR->FindAll())
	{
		InspectionItem[TaskWidget->currentIndex()].push_back(new QTreeWidgetItem(TreeWidgetVector[0]));
		InspectionItem[TaskWidget->currentIndex()].at(m_InspectionKey)->setText(0, "Inspection " + QString::number(m_InspectionKey));
		InspectionItem[TaskWidget->currentIndex()].at(m_InspectionKey)->setCheckState(0, Qt::CheckState::Checked);
		InspectionItem[TaskWidget->currentIndex()].at(m_InspectionKey)->setIcon(0, QIcon(":/CV_TEAM/icons/inspection.png"));
		//QPushButton *topLevelButton = new QPushButton("Top Level Button");
		//topLevelButton->setFixedSize(50, 30);
		//TreeWidgetVector[0]->setItemWidget(InspectionItem[TaskWidget->currentIndex()].at(m_InspectionKey), 1, topLevelButton);
		var->GetAcquisition()->LoadReferenceMat(var->GetRefImageFileName());
		InspectionDialog* dialog = new InspectionDialog();
		dialog->setInspection(var);
		dialog->updateTreeWidget();
		m_InsDialogs.push_back(dialog);
		m_InspectionKey++;
	}
	const char*p;
	for each (auto var in InspectionMGR->FindAll())
	{
		var->m_toolBlock = ToolBlockMGR->load(var->GetToolBlockFileName());
		if (var->m_toolBlock)
		{
			try
			{
				ToolBlockMGR->initOperatorsOutput(var->m_toolBlock);
				ToolBlockMGR->initOperatorInput(var->m_toolBlock);
				//ToolBlockMGR->initDisplayOperator(var->m_toolBlock),
				//var->m_shellTreeData->getDisplaySetting());
			}
			catch (const std::exception&e)
			{
				p = e.what();
				InspectionMGR->Remove(var->GetKey());
			}
		}
	}
}