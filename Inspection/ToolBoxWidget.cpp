#include "ToolBoxWidget.h"



ToolBoxWidget::ToolBoxWidget(QWidget *parent)

: DragTreeWidget(parent)

{

	this->setAcceptDrops(false);

	this->setColumnCount(1);

	//this->setHeaderLabel(tr("ToolBox"));

	QFont ft;


	ft.setPointSize(12);

	QTreeWidgetItem *headerItem = new QTreeWidgetItem();

	headerItem->setText(0, "ToolBox");

	headerItem->setFont(0,ft);

	headerItem->setTextAlignment(0, Qt::AlignHCenter | Qt::AlignVCenter);

	headerItem->setIcon(0, QIcon(":/CV_TEAM/icons/Toolbox.png"));

	this->setHeaderItem(headerItem);

	this->setColumnWidth(0, 200);


	m_root_pretreatment = new QTreeWidgetItem(this);

	m_root_pretreatment->setFont(0,ft);

	m_root_pretreatment->setText(0, tr("pretreatment"));

	//m_root_pretreatment->setIcon(0, QIcon(":/CV_TEAM/icons/pretreatment.png"));

	this->addTopLevelItem(m_root_pretreatment);



	m_root_location = new QTreeWidgetItem(this);

	m_root_location->setText(0, tr("location"));

	m_root_location->setFont(0, ft);

	//m_root_location->setIcon(0, QIcon(":/CV_TEAM/icons/location.png"));

	this->addTopLevelItem(m_root_location);



	m_root_measurement = new QTreeWidgetItem(this);

	m_root_measurement->setText(0, tr("measurement"));

	m_root_measurement->setFont(0, ft);

	//m_root_measurement->setIcon(0, QIcon(":/CV_TEAM/icons/measument.png"));

	this->addTopLevelItem(m_root_measurement);



	QTreeWidgetItem* child = new QTreeWidgetItem(m_root_pretreatment);

	child->setText(0, QString("threshold"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/thresh.png"));

	m_root_pretreatment->addChild(child);

	child = new QTreeWidgetItem(m_root_pretreatment);

	child->setText(0, QString("smooth"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/measument.png"));

	m_root_pretreatment->addChild(child);

	child = new QTreeWidgetItem(m_root_pretreatment);

	child->setText(0, QString("morphology"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/measument.png"));

	m_root_pretreatment->addChild(child);

	child = new QTreeWidgetItem(m_root_pretreatment);

	child->setText(0, QString("enhancement"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/measument.png"));

	m_root_pretreatment->addChild(child);

	child = new QTreeWidgetItem(m_root_pretreatment);

	child->setText(0, QString("rgbToGray"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/measument.png"));

	m_root_pretreatment->addChild(child);



	child = new QTreeWidgetItem(m_root_location);

	child->setText(0, QString("shapeMatch"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/shapematch.png"));

	m_root_location->addChild(child);

	child = new QTreeWidgetItem(m_root_location);

	child->setText(0, QString("grayTemplateMatch"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/match.png"));

	m_root_location->addChild(child);

	child = new QTreeWidgetItem(m_root_location);

	child->setText(0, QString("blob"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/blob.png"));

	m_root_location->addChild(child);

	child = new QTreeWidgetItem(m_root_location);

	child->setText(0, QString("caliper"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/caliper.png"));

	m_root_location->addChild(child);

	child = new QTreeWidgetItem(m_root_location);

	child->setText(0, QString("findLine"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/findline.png"));

	m_root_location->addChild(child);

	child = new QTreeWidgetItem(m_root_location);

	child->setText(0, QString("findCircle"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/findCircle.png"));

	m_root_location->addChild(child);

	child = new QTreeWidgetItem(m_root_location);

	child->setText(0, QString("findArc"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/findArc.png"));

	m_root_location->addChild(child);





	child = new QTreeWidgetItem(m_root_measurement);

	child->setText(0, QString("pointToPoint"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/PointToPoint.png"));

	m_root_measurement->addChild(child);

	child = new QTreeWidgetItem(m_root_measurement);

	child->setText(0, QString("pointToLine"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/lineTopoint.png"));

	m_root_measurement->addChild(child);

	child = new QTreeWidgetItem(m_root_measurement);

	child->setText(0, QString("lineToLine"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/lineToline.png"));

	m_root_measurement->addChild(child);

	child = new QTreeWidgetItem(m_root_measurement);

	child->setText(0, QString("lineWithLine"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/linewithline.png"));

	m_root_measurement->addChild(child);

	child = new QTreeWidgetItem(m_root_measurement);

	child->setText(0, QString("angleBy3Points"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/angle3point.png"));

	m_root_measurement->addChild(child);



	child = new QTreeWidgetItem(m_root_measurement);

	child->setText(0, QString("angleBy4Points"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/angle4point.png"));

	m_root_measurement->addChild(child);



	child = new QTreeWidgetItem(m_root_measurement);

	child->setText(0, QString("check"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/check.png"));

	m_root_measurement->addChild(child);



	m_root_recognition = new QTreeWidgetItem(this);

	m_root_recognition->setFont(0, ft);

	m_root_recognition->setText(0, tr("recognition"));

	//m_root_pretreatment->setIcon(0, QIcon(":/CV_TEAM/icons/pretreatment.png"));

	this->addTopLevelItem(m_root_recognition);

	child = new QTreeWidgetItem(m_root_recognition);

	child->setText(0, QString("Barcode"));

	child->setIcon(0, QIcon(":/CV_TEAM/icons/code.png"));

	m_root_recognition->addChild(child);

	this->setStyleSheet(

		"QTreeWidget::item{ height:40px;}"

		"QTreeWidget::item:hover{ background-color:rgb(119,136,153,50) }"

		"QTreeWidget::item:selected{ background-color:rgb(119,136,153,100) }"

		);

}



ToolBoxWidget::~ToolBoxWidget()

{

}



