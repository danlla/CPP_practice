#pragma once

#include <QTextEdit>
#include <QPushButton>
#include <QWidget>
#include <QTimer>
#include <QHostAddress>

#include "protocol.h"

class MainWindow: public QWidget {
	Q_OBJECT

private:
	QTextEdit* _textEdit;
	QTextEdit* _message;
	QPushButton* _send;
	QPushButton* _updateAddress;
	QTextEdit* _addressW;
	QTextEdit* _portW;
	QTextEdit* _infText;

	QTimer _updateTimer;

	std::vector<std::string> _messages;

	QHostAddress _hostAddress;
	quint16 _port;

private slots:
	void update();
	void sendMessage();
	void responded(Protocol::QueryType queryType, Protocol::Buffer& response);
	void failed();
	void updateAddress();

public:
	MainWindow();

	MainWindow(const MainWindow&) = delete;
	MainWindow& operator=(const MainWindow&) = delete;
};
