#include "SerialPortWidget.h"
#include <QApplication>
SerialPortWidget::SerialPortWidget(QWidget *parent)
	: QWidget(parent)
{
	makeWidget();
	serial = new SerialPortIODevice();
	connect(serial, SIGNAL(bytesRead(QByteArray)), this, SLOT(on_ReveiveText(QByteArray)));
	connect(serial, SIGNAL(bytesWritten(QByteArray)), this, SLOT(on_SendText(QByteArray)));
	serial->start();
	connect(open_btn, SIGNAL(clicked()), this, SLOT(on_open_btn_Clicked()));
	connect(refresh_btn, SIGNAL(clicked()), this, SLOT(on_refresh_btn_Clicked()));
	connect(send_btn, SIGNAL(clicked()), this, SLOT(on_send_btn_Clicked()));
}
SerialPortWidget::~SerialPortWidget()
{
}
void SerialPortWidget::on_send_btn_Clicked()
{
	serial->writeBytes(this->text_te->toPlainText().toLocal8Bit());
}
void SerialPortWidget::on_open_btn_Clicked()
{
	if (serial->isOpen())
	{
		serial->close();
		SerialPorts_cob->setEnabled(true);
		BaudRate_cob->setEnabled(true);
		Databits_cob->setEnabled(true);
		Stopbits_cob->setEnabled(true);
		Parity_cob->setEnabled(true);
		this->open_btn->setText("Open Serial Port");
	}
	else
	{
		this->openSerialPort();
		SerialPorts_cob->setEnabled(false);
		BaudRate_cob->setEnabled(false);
		Databits_cob->setEnabled(false);
		Stopbits_cob->setEnabled(false);
		Parity_cob->setEnabled(false);
		this->open_btn->setText("Close Serial Port");
	}
}
void SerialPortWidget::on_refresh_btn_Clicked()
{
	refresh();
}
void SerialPortWidget::on_clear_recv_btn_Clicked()
{
	this->text_br->clear();
}
void SerialPortWidget::on_clear_send_btn_Clicked()
{
	this->text_te->clear();
}
void SerialPortWidget::on_ReveiveText(QByteArray text)
{
	//
	receiveFrameCount = rxFrames_lb->text().toLongLong();
	receiveFrameCount += 1;
	rxFrames_lb->setText(QString::number(receiveFrameCount));
	receiveBytesCount = rxBytes_lb->text().toLongLong();
	receiveBytesCount += text.length();
	rxBytes_lb->setText(QString::number(receiveBytesCount));
	refreshRecvData(text, true);
	//this->text_br->append(text);
}
void SerialPortWidget::on_SendText(QByteArray text)
{
	sendFrameCount = txFrames_lb->text().toLongLong();
	sendFrameCount += 1;
	txFrames_lb->setText(QString::number(sendFrameCount));
	sendBytesCount = txBytes_lb->text().toLongLong();
	sendBytesCount += text.length();
	txBytes_lb->setText(QString::number(sendBytesCount));
	refreshRecvData(text, false);
}
void SerialPortWidget::on_clear_statistic_btn_Clicked()
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
void SerialPortWidget::makeWidget()
{
	SerialPorts_cob = new QComboBox();
	refresh();
	BaudRate_cob = new QComboBox();
	Databits_cob = new QComboBox();
	Stopbits_cob = new QComboBox();
	Parity_cob = new QComboBox();
	text_br = new QTextBrowser();
	text_te = new QTextEdit();
	QVBoxLayout *vlay = new QVBoxLayout();
	vlay->addWidget(new QLabel("Select Serial Port:"));
	vlay->addWidget(SerialPorts_cob);
	vlay->addWidget(new QLabel("BaudRate:"));
	vlay->addWidget(BaudRate_cob);
	vlay->addWidget(new QLabel("Data Bit:"));
	vlay->addWidget(Databits_cob);
	vlay->addWidget(new QLabel("Stop Bit:"));
	vlay->addWidget(Stopbits_cob);
	vlay->addWidget(new QLabel("Parity:"));
	vlay->addWidget(Parity_cob);
	QGroupBox *connect_gb = new QGroupBox("Connect Settings");
	connect_gb->setLayout(vlay);
	connect_gb->setMaximumWidth(200);
	open_btn = new QPushButton("Open Serial Port");
	refresh_btn = new QPushButton("Refresh ");
	send_btn = new QPushButton("Send Msg ");
	QVBoxLayout *serialPortManager_vlay = new QVBoxLayout();
	QGroupBox *serialPortManager_gb = new QGroupBox("Serial Port Manager");
	serialPortManager_gb->setLayout(serialPortManager_vlay);
	serialPortManager_vlay->addWidget(open_btn);
	serialPortManager_vlay->addWidget(refresh_btn);
	serialPortManager_vlay->addWidget(send_btn);
	QVBoxLayout *left_vlay = new QVBoxLayout();
	left_vlay->addWidget(serialPortManager_gb);
	left_vlay->addWidget(connect_gb);
	QGroupBox *recv_gb = new QGroupBox("Recv Msg");
	QGroupBox *send_gb = new QGroupBox("Send Msg");
	QHBoxLayout *recv_hlay = new QHBoxLayout();
	QHBoxLayout *send_hlay = new QHBoxLayout();
	recv_gb->setLayout(recv_hlay);
	send_gb->setLayout(send_hlay);
	recv_hlay->addWidget(text_br);
	send_hlay->addWidget(text_te);
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
	//
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
	//
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
	//
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
	QList <qint32> baudRateList = QSerialPortInfo::standardBaudRates();
	this->BaudRate_cob->clear();
	foreach(qint32 baudRate, baudRateList) {
		this->BaudRate_cob->addItem(QString::number(baudRate));
	}
	this->BaudRate_cob->setCurrentText("9600");
	QStringList dataBitsList;
	dataBitsList << "8" << "7" << "6" << "5";
	this->Databits_cob->addItems(dataBitsList);
	QStringList stopBitsList;
	stopBitsList << "1";
#ifdef Q_OS_WIN
	stopBitsList << "1.5";
#endif
	stopBitsList << "2";
	this->Stopbits_cob->addItems(stopBitsList);
	QStringList parityList;
	parityList << "NoParity" << "EvenParity" << "OddParity" << "SpaceParity" << "MarkParity";
	this->Parity_cob->addItems(parityList);
}
void SerialPortWidget::close()
{
	this->serial->close();
}
void SerialPortWidget::openSerialPort()
{
	QString portName = this->SerialPorts_cob->currentText().split(' ').first();
	QString baudRate = this->BaudRate_cob->currentText();
	QString dataBits = this->Databits_cob->currentText();
	QString stopBits = this->Stopbits_cob->currentText();
	QString parity = this->Parity_cob->currentText();
	this->serial->open(portName, baudRate, dataBits, stopBits, parity);
}
void SerialPortWidget::refreshRecvData(QByteArray &data, bool isReceivedData)
{
	// if (ui->checkBoxPause->isChecked()){
	// return;
	// }
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
	// if (ui->checkBoxOutputReceiveDataOnly->isChecked()){
	// if (!isReceivedData){
	// return;
	// }
	// /**
	// * 
	//if (!text_br->toPlainText().isEmpty())
	//{
	// str = text_br->toPlainText() + " " + str;
	//}
	//else
	//{
	text_br->append(str);
	//}
	//text_br->setText(QString("<font color=blue>%1</font>").arg(str));
	text_br->verticalScrollBar()->setSliderPosition(text_br->verticalScrollBar()->maximum());
}
void SerialPortWidget::refresh()
{
	QList <QSerialPortInfo> infoList = QSerialPortInfo::availablePorts();
	this->SerialPorts_cob->clear();
	if (infoList.isEmpty()) {
		this->SerialPorts_cob->addItem(QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning), QString::fromStdWString(L"无可用端口"));
	}
	else
	{
		foreach(QSerialPortInfo info, infoList) {
			this->SerialPorts_cob->addItem(info.portName() + " " + info.description());
		}
	}
}