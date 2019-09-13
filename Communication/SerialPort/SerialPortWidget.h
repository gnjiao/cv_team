#pragma once
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSerialPortInfo>
#include <QPushButton>
#include <QTextBrowser>
#include <QTextEdit>
#include <QCheckBox>
#include <QLineEdit>
#include <QSplitter>
#include <QDate>
#include <QMetaEnum>
#include <QScrollBar>
#include "SerialPortIODevice.h"
class SerialPortWidget : public QWidget
{
	Q_OBJECT
public:
	SerialPortWidget(QWidget *parent = Q_NULLPTR);
	~SerialPortWidget();
	private slots:
	void on_send_btn_Clicked();
	void on_open_btn_Clicked();
	void on_refresh_btn_Clicked();
	void on_clear_recv_btn_Clicked();
	void on_clear_send_btn_Clicked();
	void on_ReveiveText(QByteArray text);
	void on_SendText(QByteArray text);
	void on_clear_statistic_btn_Clicked();
private:
	void makeWidget();
	void close();
	void openSerialPort();
	void refresh();
private:
	void refreshRecvData(QByteArray &data, bool isReceivedData);
	QComboBox *SerialPorts_cob;
	QComboBox *BaudRate_cob;
	QComboBox *Databits_cob;
	QComboBox *Stopbits_cob;
	QComboBox *Parity_cob;
	QPushButton *open_btn;
	QPushButton *refresh_btn;
	QPushButton *send_btn;
	QComboBox *input_mode_cob;
	QCheckBox *cycle_time_cb;
	QLineEdit *cycle_time_le;
	QPushButton *read_file_btn;
	QPushButton *clear_input_btn;
	QComboBox *output_mode_cob;
	QCheckBox *output_date_cb;
	QCheckBox *output_time_cb;
	QCheckBox *display_ms_cb;
	QPushButton *save_file_btn;
	QPushButton *clear_output_btn;
	QString outputTextMode;
	QLabel *rxFrames_lb;
	QLabel *txFrames_lb;
	QLabel *rxBytes_lb;
	QLabel *txBytes_lb;
	QPushButton *clear_statistic_btn;
	qlonglong receiveFrameCount = 0;
	qlonglong receiveBytesCount = 0;
	qlonglong sendFrameCount = 0;
	qlonglong sendBytesCount = 0;
	SerialPortIODevice *serial;
	QTextBrowser *text_br;
	QTextEdit *text_te;
};