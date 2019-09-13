#pragma once
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QTextBrowser>
#include <QNetworkInterface>
#include <QSplitter>
#include <QCheckBox>
#include <QScrollBar>
#include <QTime>
#include <QDate>
#include "TcpServerIODevice.h"

class TcpServerWidget : public QWidget
{
	Q_OBJECT
public:
	TcpServerWidget(QWidget *parent = Q_NULLPTR);
	~TcpServerWidget();
	private slots:
	void on_close_btn_Clicked();
	void on_listen_btn_Clicked();
	void changedClients(QList<QTcpSocket*>clients);
	void setCurrentClient(QString text);
	void setServerPort(QString port);
	void on_deviceOpenSuccessfully();
	void on_send_btn_Clicked();
	void on_clear_recv_btn_Clicked();
	void on_clear_send_btn_Clicked();
	void on_ReceiveText(QByteArray text);
	void on_SendText(QByteArray text);
	void on_clear_statistic_btn_Clicked();
private:
	void makeWidget();
	void refreshRecvData(QByteArray &data, bool isReceivedData);
	void close();
	void openSerialPort();
	void refresh();
private:
	QComboBox *ServerHostAddress_cob;
	QComboBox *Clients_cob;
	QLineEdit *ServerPort_le;
	QComboBox *Stopbits_cob;
	QComboBox *Parity_cob;
	QPushButton *listen_btn;
	QPushButton *close_btn;
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
	TcpServerIODevice *m_server;
	QTextBrowser *recv_text_br;
	QTextEdit *send_text_te;
};