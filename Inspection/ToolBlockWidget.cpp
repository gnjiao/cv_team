#include "ToolBlockWidget.h"

ToolBlockWidget::ToolBlockWidget(QWidget *parent) : DragTreeWidget(parent)
{
	this->setAcceptDrops(true);
	this->setColumnCount(2);;
	this->setHeaderLabels(QStringList()
		<< tr("ToolBlock")
		<< tr("Duration"));
	this->setColumnWidth(0, 160);
	this->setColumnWidth(1, 70);
	this->headerItem()->setTextAlignment(0, Qt::AlignHCenter | Qt::AlignVCenter);
	this->headerItem()->setTextAlignment(1, Qt::AlignHCenter | Qt::AlignVCenter);
	this->setStyleSheet(

		"QTreeWidget::item{ height:40px;}"

		"QTreeWidget::item:hover{ background-color:rgb(119,136,153,50) }"

		"QTreeWidget::item:selected{ background-color:rgb(119,136,153,100) }"

		);

	m_popMenu = new QMenu(this);
	Delete_ac = new QAction(this);
	Delete_ac->setText(tr("Delete"));
	Delete_ac->setIcon(QIcon(":/CV_TEAM/icons/Delete.png"));
	Edit_ac = new QAction(this);
	Edit_ac->setText(tr("Edit"));
	Edit_ac->setIcon(QIcon(":/CV_TEAM/icons/edit.png"));
	m_popMenu->addAction(Delete_ac);
	m_popMenu->addAction(Edit_ac);
	connect(Delete_ac, SIGNAL(triggered()), this, SLOT(on_action_Delete_triggered()));
	connect(Edit_ac, SIGNAL(triggered()), this, SLOT(on_action_Edit_triggered()));
	
}
ToolBlockWidget::~ToolBlockWidget()
{
}
void ToolBlockWidget::updateTree()
{
	this->clear();
	clearOperatorWidgets();
	for each (auto var in ToolBlockMGR->getToolBlock()->Children())
	{
		QTreeWidgetItem* item = new QTreeWidgetItem(this);
		item->setText(0, QString::fromStdString(var->GetNodeShowText())
			+ "." + QString::number(var->GetNodeKey()));
		setTreeItemIcon(item, var->GetNodeName());
		item->setText(1, QString::number(var->getDuration()) + " ms");
		NodeState state = var->GetState();
		switch (state)
		{
		case toolModel::Idel:
		 item->setIcon(0,QIcon(":/CV_TEAM/icons/state_yellow.png"));
		 break;
		case toolModel::Success:
		 item->setIcon(0, QIcon(":/CV_TEAM/icons/state_green.png"));
		 break;
		case toolModel::Failure:
		 item->setIcon(0, QIcon(":/CV_TEAM/icons/state_red.png"));
		 break;
		case toolModel::Running:
		 break;
		}
		this->addTopLevelItem(item);
		OperatorWidget.push_back(creatOperatorWidget(var));
	}
}
void ToolBlockWidget::setInspection(Inspection * inspection)
{
	ToolBlockMGR->setInspection(inspection);
}
void ToolBlockWidget::doSelected(int index)
{
	m_selectedNode = ToolBlockMGR->getToolBlock()->GetChild(index);
}
void ToolBlockWidget::doDraged(int index)
{
}
void ToolBlockWidget::doInserted(int index, QDropEvent * event, QTreeWidgetItem * item)
{
	if (event->mimeData()->hasFormat(typeid(*this).name()))
	{
		swapItem(index, event, item, false);
	}
	else
	{
		string OperatorType = event->mimeData()->text().toStdString();
		BaseOperator* Operator = OperatorMGR->createOperator(OperatorType);
		if (Operator == nullptr)  return;
		QTreeWidgetItem* newItem = new QTreeWidgetItem(QStringList() << item->text(0) << "0 ms");
		newItem->setIcon(0, QIcon(":/CV_TEAM/icons/state_yellow.png"));
		this->addTopLevelItem(newItem);
		ToolBlockMGR->getToolBlock()->AddChild(Operator, true);
		Operator->setInspectionKey(ToolBlockMGR->getInspection()->GetKey());
		Operator->initOutput();
		newItem->setText(0, item->text(0) + "." + QString::number(Operator->GetNodeKey()));
		setTreeItemIcon(newItem, Operator->GetNodeName());
		Operator = nullptr;
	}
	ToolBlockMGR->save();
	emit signalToolBlockChanged(ToolBlockMGR->getToolBlock());
	updateTree();
}
void ToolBlockWidget::contextMenuEvent(QContextMenuEvent * event)
{
	m_popMenu->exec(QCursor::pos());
	event->accept();
}
void ToolBlockWidget::dropEvent(QDropEvent * event)
{
	QTreeWidgetItem* itemOver = itemAt(event->pos());
	QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << QString(event->mimeData()->text()));
	if (itemOver == NULL)
	{
		if (event->mimeData()->hasFormat(typeid(*this).name()))
		{
			swapItem(0, event, item, true);
		}
		else
		{
			string OperatorType = event->mimeData()->text().toStdString();
			BaseOperator* Operator = OperatorMGR->createOperator(OperatorType);
			if (Operator == nullptr)return;
			QTreeWidgetItem* newItem = new QTreeWidgetItem(QStringList() << item->text(0) << "0 ms");
			newItem->setIcon(0, QIcon(":/CV_TEAM/icons/state_yellow.png"));
			this->addTopLevelItem(newItem);
			ToolBlockMGR->getToolBlock()->AddChild(Operator, true);
			Operator->setInspectionKey(ToolBlockMGR->getInspection()->GetKey());
			Operator->initOutput();
			//Operator->registerOutput();
			ToolBlockMGR->initOperator(indexFromItem(newItem).row(), false);
			newItem->setText(0, item->text(0) + "." + QString::number(Operator->GetNodeKey()));
			setTreeItemIcon(newItem, Operator->GetNodeName());
			Operator = nullptr;
		}
		ToolBlockMGR->save();
		emit signalToolBlockChanged(ToolBlockMGR->getToolBlock());
		updateTree();
	}
	else
	{
		QModelIndex index = indexFromItem(itemOver);
		doInserted(index.row() + 1, event, item);
	}
	event->setDropAction(Qt::MoveAction);
	event->accept();
}
void ToolBlockWidget::on_action_Delete_triggered()
{
	if (m_selectedNode->GetNodeKey() == 0)
	{
		return;
	}
	if (m_selectedItem == NULL)
		return;
	delete m_selectedItem;
	ToolBlockMGR->getInspection()->m_toolBlock->RemoveChildByKey(m_selectedNode->GetNodeKey());
	ToolBlockMGR->save();
	emit signalToolBlockChanged(ToolBlockMGR->getToolBlock());
	updateTree();
}
void ToolBlockWidget::on_action_Edit_triggered()
{
	ToolBlockMGR->initOperator(indexOfTopLevelItem(m_selectedItem), true, &OperatorWidget);
}
QDialog * ToolBlockWidget::creatOperatorWidget(BaseNode *toolBase)
{
	if (nullptr == toolBase)
	{
		throw runtime_error("nullptr in creatOperatorWidget");
	}
	if (toolBase->GetNodeName() == "AcquireImage")
	{
		AcquireImageWidget* OperatorWidget = new AcquireImageWidget();
		OperatorWidget->hide();
		return OperatorWidget;
	}
	if (toolBase->GetNodeName() == "findLine")
	{
		FindLineWidget* OperatorWidget = new FindLineWidget();
		OperatorWidget->hide();
		return OperatorWidget;
	}
	if (toolBase->GetNodeName() == "findCircle")
	{
		FindCircleWidget* OperatorWidget = new FindCircleWidget();
		OperatorWidget->hide();
		return OperatorWidget;
	}
	if (toolBase->GetNodeName() == "shapeMatch")
	{
		ShapeMatchWidget* OperatorWidget = new ShapeMatchWidget();
		OperatorWidget->hide();
		return OperatorWidget;
	}
	if (toolBase->GetNodeName() == "enhancement")
	{
		EnhancementWidget* OperatorWidget = new EnhancementWidget();
		OperatorWidget->hide();
		return OperatorWidget;
	}
	if (toolBase->GetNodeName() == "morphology")
	{
		MorphologyWidget* OperatorWidget = new MorphologyWidget();
		OperatorWidget->hide();
		return OperatorWidget;
	}
	if (toolBase->GetNodeName() == "smooth")
	{
		SmoothWidget* OperatorWidget = new SmoothWidget();
		OperatorWidget->hide();
		return OperatorWidget;
	}
	if (toolBase->GetNodeName() == "threshold")
	{
		ThresholdWidget* OperatorWidget = new ThresholdWidget();
		OperatorWidget->hide();
		return OperatorWidget;
	}


	return nullptr;
}
void ToolBlockWidget::swapItem(int index, QDropEvent * event, QTreeWidgetItem * item, bool addOrInsert)
{
	item->setText(1, "0 ms");
	int dotIndex = item->text(0).indexOf('.');
	QString text = item->text(0).left(dotIndex);
	setTreeItemIcon(item, text.toStdString());
	ToolBlockMGR->getToolBlock()->RemoveChildByKey(m_selectedNode->GetNodeKey());
	if (addOrInsert)
	{
		ToolBlockMGR->getToolBlock()->AddChild(m_selectedNode, false);
		this->addTopLevelItem(item);
	}
	else
	{
		ToolBlockMGR->getToolBlock()->InsertChild(index, m_selectedNode, false);
		this->insertTopLevelItem(index, item);
	}
}
void ToolBlockWidget::setTreeItemIcon(QTreeWidgetItem * item, string text)
{
}
void ToolBlockWidget::ReserveSpaceToAddItem()
{
	return;
}
void ToolBlockWidget::clearOperatorWidgets()
{
	for each (auto var in OperatorWidget)
	{
		if (nullptr != var)
		{
			var->hide();
			delete var;
			var = nullptr;
		}
	}
	OperatorWidget.clear();
}