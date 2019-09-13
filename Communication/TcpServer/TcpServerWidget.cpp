#include "TcpServerWidget.h"

TcpServerWidget::TcpServerWidget(QWidget *parent)
	: QWidget(parent)
{
	makeWidget();
	m_server = new TcpServerIODevice();
	m_server->start();
	connect(m_server, SIGNAL(clientsChanged(QList<QTcpSocket*>)), this, SLOT(changedClients(QList<QTcpSocket*>)));
	connect(m_server, SIGNAL(bytesRead(QByteArray)), this, SLOT(on_ReceiveText(QByteArray)));
	connect(m_server, SIGNAL(bytesWritten(QByteArray)), this, SLOT(on_SendText(QByteArray)));
	connect(m_server, SIGNAL(deviceOpenSuccessfully()), this, SLOT(on_deviceOpenSuccessfully()));
}
TcpServerWidget::~TcpServerWidget()
{
}
void TcpServerWidget::on_close_btn_Clicked()
{
	m_server->close();
	listen_btn->setEnabled(true);
	listen_btn->setText("Listen");
}
void TcpServerWidget::on_listen_btn_Clicked()
{
	m_server->open(ServerHostAddress_cob->currentText(), ServerPort_le->text());
}
void TcpServerWidget::changedClients(QList<QTcpSocket*> clients)
{
	disconnect(Clients_cob, SIGNAL(currentTextChanged(QString)), this, SLOT(setCurrentClient(QString)));
	if (clients.isEmpty())
	{
		Clients_cob->clear();
		Clients_cob->addItem(QString::fromStdWString(L"请连接客户端"));
	}
	else {
		Clients_cob->clear();
		foreach(QTcpSocket *client, clients) {
			Clients_cob->addItem(client->peerAddress().toString() + ":" + QString::number(client->peerPort()));
		}
	}
	connect(Clients_cob, SIGNAL(currentTextChanged(QString)), this, SLOT(setCurrentClient(QString)));
}
void TcpServerWidget::setCurrentClient(QString text)
{
	QString address, port;
	QStringList list = text.split(':');
	address = list.at(0);
	port = list.at(1);
	m_server->changedCurrentScoket(address, port);
}
void TcpServerWidget::setServerPort(QString port)
{
}
void TcpServerWidget::on_deviceOpenSuccessfully()
{
	this->listen_btn->setText(QString::fromStdWString(L"服务器已启动监听"));
	listen_btn->setEnabled(false);
}
void TcpServerWidget::on_send_btn_Clicked()
{
	m_server->writeBytes(this->send_text_te->toPlainText().toLocal8Bit());
}
void TcpServerWidget::on_clear_recv_btn_Clicked()
{
	this->recv_text_br->clear();
}
void TcpServerWidget::on_clear_send_btn_Clicked()
{
	this->send_text_te->clear();
}
void TcpServerWidget::on_ReceiveText(QByteArray text)
{
	receiveFrameCount = rxFrames_lb->text().toLongLong();
	receiveFrameCount += 1;
	rxFrames_lb->setText(QString::number(receiveFrameCount));
	receiveBytesCount = rxBytes_lb->text().toLongLong();
	receiveBytesCount += text.length();
	rxBytes_lb->setText(QString::number(receiveBytesCount));
	refreshRecvData(text, true);
}
void TcpServerWidget::on_SendText(QByteArray text)
{
	sendFrameCount = txFrames_lb->text().toLongLong();
	sendFrameCount += 1;
	txFrames_lb->setText(QString::number(sendFrameCount));
	sendBytesCount = txBytes_lb->text().toLongLong();
	sendBytesCount += text.length();
	txBytes_lb->setText(QString::number(sendBytesCount));
	refreshRecvData(text, false);
}
void TcpServerWidget::on_clear_statistic_btn_Clicked()
{
	receiveFrameCount = 0;
	receiveBytesCount = 0;
	sendFrameCount = 0;
	sendBytesCount = 0;
	rxFrames_lb->setText(QString::number(receiveFrameCount));
	rxBytes_lb->setText(QString::number(receiveBytesCount));
	txFrames_lb->setText(QString::number(sendFrameCount));
	txBytes_lb->setText(QString::number(sendBytesCount));
}
void TcpServerWidget::makeWidget()
{
	Clients_cob = new QComboBox();
	recv_text_br = new QTextBrowser();
	send_text_te = new QTextEdit();
	ServerHostAddress_cob = new QComboBox();
	ServerPort_le = new QLineEdit();
	ServerPort_le->setText("9003");
	listen_btn = new QPushButton("Listen ");
	close_btn = new QPushButton("Close Server");
	send_btn = new QPushButton("Send Text");
	connect(listen_btn, SIGNAL(clicked()), this, SLOT(on_listen_btn_Clicked()));
	connect(close_btn, SIGNAL(clicked()), this, SLOT(on_close_btn_Clicked()));
	connect(send_btn, SIGNAL(clicked()), this, SLOT(on_send_btn_Clicked()));
	QVBoxLayout *vlay = new QVBoxLayout();
	vlay->addWidget(new QLabel("Client Address List:"));
	vlay->addWidget(Clients_cob);
	vlay->addWidget(new QLabel("Local Server Address:"));
	vlay->addWidget(ServerHostAddress_cob);
	vlay->addWidget(new QLabel("Local Server Port:"));
	vlay->addWidget(ServerPort_le);
	QGroupBox *connect_gb = new QGroupBox("Connect Settings");
	connect_gb->setLayout(vlay);
	connect_gb->setMaximumWidth(200);
	QVBoxLayout *TcpManager_vlay = new QVBoxLayout();
	QGroupBox *TcpManager_gb = new QGroupBox("Serial Port Manager");
	TcpManager_gb->setLayout(TcpManager_vlay);
	TcpManager_vlay->addWidget(listen_btn);
	TcpManager_vlay->addWidget(close_btn);
	TcpManager_vlay->addWidget(send_btn);
	QVBoxLayout *left_vlay = new QVBoxLayout();
	left_vlay->addWidget(TcpManager_gb);
	left_vlay->addWidget(connect_gb);
	QGroupBox *recv_gb = new QGroupBox("Recv Msg");
	QGroupBox *send_gb = new QGroupBox("Send Msg");
	QHBoxLayout *recv_hlay = new QHBoxLayout();
	QHBoxLayout *send_hlay = new QHBoxLayout();
	recv_gb->setLayout(recv_hlay);
	send_gb->setLayout(send_hlay);
	recv_hlay->addWidget(recv_text_br);
	send_hlay->addWidget(send_text_te);
	QVBoxLayout *mid_vlay = new QVBoxLayout();
	QSplitter *split = new QSplitter(Qt::Vertical);
	split->setHandleWidth(0);
	split->addWidget(recv_gb);
	split->addWidget(send_gb);
	QList<int> list;
	list.append(400);
	list.append(220);
	split->setSizes(list);
	mid_vlay->addWidget(split);
	QGroupBox *InputSetting_gb = new QGroupBox("Recv Data Settings");
	QGroupBox *OutputSetting_gb = new QGroupBox("Send Data Settings");
	QGroupBox *DataStatistic_gb = new QGroupBox("Data Statistic");
	InputSetting_gb->setMaximumWidth(200);
	OutputSetting_gb->setMaximumWidth(200);
	DataStatistic_gb->setMaximumWidth(200);
	QVBoxLayout *InputSetting_vlay = new QVBoxLayout();
	QVBoxLayout *OutputSetting_vlay = new QVBoxLayout();
	QVBoxLayout *DataStatistic_vlay = new QVBoxLayout();
	//????????????
	QHBoxLayout*inputmode_hlay = new QHBoxLayout();
	inputmode_hlay->addWidget(new QLabel("Input Mode:"));
	input_mode_cob = new QComboBox();
	input_mode_cob->addItems(QStringList() << "Ascii" << "Bin" << "Oct" << "Dec" << "Hex" << "Utf8");
	inputmode_hlay->addWidget(input_mode_cob);
	read_file_btn = new QPushButton("Read File");
	clear_input_btn = new QPushButton("Clear");
	QHBoxLayout *read_file_hlay = new QHBoxLayout();
	read_file_hlay->addWidget(read_file_btn);
	read_file_hlay->addWidget(clear_input_btn);
	InputSetting_vlay->addLayout(inputmode_hlay);
	cycle_time_cb = new QCheckBox("Use Cycle Time");
	cycle_time_le = new QLineEdit();
	InputSetting_vlay->addWidget(cycle_time_cb);
	InputSetting_vlay->addWidget(cycle_time_le);
	InputSetting_vlay->addLayout(read_file_hlay);
	InputSetting_gb->setLayout(InputSetting_vlay);
	QHBoxLayout*outputmode_hlay = new QHBoxLayout();
	outputmode_hlay->addWidget(new QLabel("Output Mode:"));
	output_mode_cob = new QComboBox();
	output_mode_cob->addItems(QStringList() << "Ascii" << "Bin" << "Oct" << "Dec" << "Hex" << "Utf8");
	outputmode_hlay->addWidget(output_mode_cob);
	QHBoxLayout *date_time_hlay = new QHBoxLayout();
	output_date_cb = new QCheckBox("Show Date");
	output_time_cb = new QCheckBox("Show Time");
	display_ms_cb = new QCheckBox("Show ms");
	date_time_hlay->addWidget(output_date_cb);
	date_time_hlay->addWidget(output_time_cb);
	save_file_btn = new QPushButton("Save File");
	clear_output_btn = new QPushButton("Clear");
	QHBoxLayout *save_file_hlay = new QHBoxLayout();
	save_file_hlay->addWidget(save_file_btn);
	save_file_hlay->addWidget(clear_output_btn);
	OutputSetting_vlay->addLayout(outputmode_hlay);
	OutputSetting_vlay->addLayout(date_time_hlay);
	OutputSetting_vlay->addLayout(save_file_hlay);
	OutputSetting_gb->setLayout(OutputSetting_vlay);
	//???????
	rxFrames_lb = new QLabel();
	txFrames_lb = new QLabel();
	rxBytes_lb = new QLabel();
	txBytes_lb = new QLabel();
	rxFrames_lb->setText(QString::number(receiveFrameCount));
	rxBytes_lb->setText(QString::number(receiveBytesCount));
	txFrames_lb->setText(QString::number(sendFrameCount));
	txBytes_lb->setText(QString::number(sendBytesCount));
	QHBoxLayout *hlay1 = new QHBoxLayout();
	hlay1->addWidget(new QLabel("Rx Frames:"));
	hlay1->addWidget(rxFrames_lb);
	QHBoxLayout *hlay2 = new QHBoxLayout();
	hlay2->addWidget(new QLabel("Tx Frames:"));
	hlay2->addWidget(txFrames_lb);
	QHBoxLayout *hlay3 = new QHBoxLayout();
	hlay3->addWidget(new QLabel("Rx Bytes:"));
	hlay3->addWidget(rxBytes_lb);
	QHBoxLayout *hlay4 = new QHBoxLayout();
	hlay4->addWidget(new QLabel("Tx Bytes:"));
	hlay4->addWidget(txBytes_lb);
	clear_statistic_btn = new QPushButton("Clear Statistic Data");
	connect(clear_statistic_btn, SIGNAL(clicked()), this, SLOT(on_clear_statistic_btn_Clicked()));
	QVBoxLayout *statistic_vlay = new QVBoxLayout();
	statistic_vlay->addLayout(hlay1);
	statistic_vlay->addLayout(hlay3);
	statistic_vlay->addLayout(hlay2);
	statistic_vlay->addLayout(hlay4);
	statistic_vlay->addWidget(clear_statistic_btn);
	DataStatistic_gb->setLayout(statistic_vlay);
	QVBoxLayout *right_vlay = new QVBoxLayout();
	right_vlay->addWidget(InputSetting_gb);
	right_vlay->addWidget(DataStatistic_gb);
	right_vlay->addWidget(OutputSetting_gb);
	QHBoxLayout *main_hlay = new QHBoxLayout();
	main_hlay->addLayout(left_vlay);
	main_hlay->addLayout(mid_vlay);
	main_hlay->addLayout(right_vlay);
	this->setLayout(main_hlay);
	refresh();
}
void TcpServerWidget::refreshRecvData(QByteArray & data, bool isReceivedData)
{
	QString str;
	str.append("<font color=silver>[ </font>");
	if (output_date_cb->isChecked())
	{
		str.append(QDate::currentDate().toString("yyyy/MM/dd"));
		str = QString("<font color=silver>%1</font>").arg(str);
	}
	if (output_time_cb->isChecked())
	{
		if (display_ms_cb->isChecked())
		{
			str.append(QTime::currentTime().toString("hh:mm:ss.z "));
		}
		else
		{
			str.append(QTime::currentTime().toString("hh:mm:ss "));
		}
		str = QString("<font color=silver>%1</font>").arg(str);
	}
	if (isReceivedData)
	{
		str.append("<font color=blue>Rx</font>");
	}
	else
	{
		str.append("<font color=purple>Tx</font>");
	}
	str.append("<font color=silver>] </font>");
	outputTextMode = output_mode_cob->currentText();
	if (outputTextMode == "Bin") {
		for (int i = 0; i < data.length(); i++) {
			str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 2), 8, '0'));
		}
	}
	else if (outputTextMode == "Oct") {
		for (int i = 0; i < data.length(); i++) {
			str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 8), 3, '0'));
		}
	}
	else if (outputTextMode == "Dec") {
		for (int i = 0; i < data.length(); i++) {
			str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 10)));
		}
	}
	else if (outputTextMode == "Hex") {
		for (int i = 0; i < data.length(); i++) {
			str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 16), 2, '0'));
		}
	}
	else if (outputTextMode == "Ascii") {
		str.append(QString(data));
	}
	else if (outputTextMode == "Local8bit") {
		str.append(QString::fromLocal8Bit(data));
	}
	else
	{
		Q_ASSERT_X(false, __FUNCTION__, "Unknown output mode");
	}
	recv_text_br->append(str);
	recv_text_br->verticalScrollBar()->setSliderPosition(recv_text_br->verticalScrollBar()->maximum());
}
void TcpServerWidget::close()
{
}
void TcpServerWidget::openSerialPort()
{
}
void TcpServerWidget::refresh()
{
	QList<QHostAddress> addressList = QNetworkInterface::allAddresses();
	ServerHostAddress_cob->clear();
	if (addressList.isEmpty())
	{
		ServerHostAddress_cob->addItem(tr("请连接客户端"));
	}
	else
	{
		QList<QHostAddress> ipv4AddressList;
		foreach(QHostAddress address, addressList)
		{
			if (address.protocol() == QAbstractSocket::IPv4Protocol)
			{
				ipv4AddressList.append(address);
			}
		}
		foreach(QHostAddress address, ipv4AddressList)
		{
			ServerHostAddress_cob->addItem(address.toString());
		}
	}
}