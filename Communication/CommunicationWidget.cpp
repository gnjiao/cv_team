#include "CommunicationWidget.h"

CommunicationWidget::CommunicationWidget(QWidget *parent)
	: LTwindow(parent)
{
	this->setObjectName("InspectionDialog");
	this->resize(1100, 750);
	this->setWindowIcon(QIcon(":/main/logo"));
	this->setWindowTitle("Communication Widget");
	m_customTab = new CustomTabWidget();
	m_TcpServerWidget = new TcpServerWidget();
	UdpWidget = new QWidget();
	m_SerialPortWidget = new SerialPortWidget();
	ModbusWidget = new QWidget();
	m_customTab->addTab(m_TcpServerWidget, tr("TCP"));
	QFont ft;
	ft.setPointSize(16);
	ft.setBold(true);
	m_customTab->setFont(ft);
	m_customTab->addTab(UdpWidget, tr("UDP"));
	m_customTab->addTab(m_SerialPortWidget, tr("Serial Port"));
	m_customTab->addTab(ModbusWidget, tr("Modbus"));
	QWidget *w = new QWidget();
	QHBoxLayout *hlay_main = new QHBoxLayout();
	hlay_main->addWidget(m_customTab);
	hlay_main->setContentsMargins(20, 20, 20, 0);
	w->setLayout(hlay_main);
	this->setCentralWidget(w);

}

CommunicationWidget::~CommunicationWidget()
{
}

void CommunicationWidget::makeTabWidget()
{
	//m_tabWidget = new QTabWidget(this);
	//m_tabWidget->setTabPosition(QTabWidget::North);
	//m_tabWidget->setTabShape(QTabWidget::Triangular);
	//m_tabWidget->widget(m_tabWidget->addTab(TcpWidget, tr("TCP")));
	//m_tabWidget->widget(m_tabWidget->addTab(UdpWidget, tr("UDP")));
	//m_tabWidget->widget(m_tabWidget->addTab(SerialPortWidget, tr("Serial Port")));
	//m_tabWidget->widget(m_tabWidget->addTab(ModbusWidget, tr("Modbus")));
}

void CommunicationWidget::makeMainLayout()
{

	//m_title = new TitleBar(this);
	//m_title->setButtonType(ButtonType::MIN_BUTTON);
	//m_title->raise();
	//m_title->setBackgroundColor(QColor(115, 115, 115, 70));
	//m_title->setTitleContent("InspectionDialog", 16);
	//m_title->setTitleIcon(":/CV_TEAM/icons/inspection.png");
	//location = this->geometry();
	//max = false;


}

