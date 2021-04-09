#include "main_window.h"

#include <sstream>

#include <QHostAddress>
#include <QMessageBox>
#include <QVBoxLayout>

#include "protocol.h"
#include "server_request.h"

using namespace std;

void MainWindow::update()
{
	const auto buffer = Protocol::GetMessages::makeRequest(_messages.size()); // TODO [Client] Индекс определять "умным" способом
	//const auto serverRequest = new ServerRequest(QHostAddress::LocalHost, 5000, buffer, 5000);

	const auto serverRequest = new ServerRequest(_hostAddress, _port, buffer, 5000);

	QObject::connect(serverRequest, &ServerRequest::responded, this, &MainWindow::responded);
	QObject::connect(serverRequest, &ServerRequest::failed, this, &MainWindow::failed);

	//_textEdit->setText(QString());
}

void MainWindow::sendMessage()
{
	const auto buffer = Protocol::SendMessage::makeRequest(_message->toPlainText().toStdString());
	const auto serverRequest = new ServerRequest(_hostAddress, _port, buffer, 5000);

	QObject::connect(serverRequest, &ServerRequest::responded, this, &MainWindow::responded);
	QObject::connect(serverRequest, &ServerRequest::failed, this, &MainWindow::failed);

	//_textEdit->insertPlainText(_message->toPlainText());

	_send->setEnabled(false);
}


void MainWindow::responded(Protocol::QueryType queryType, Protocol::Buffer& response)
{
	switch (queryType)
	{

	case Protocol::QueryType::GetMessages:
	{
		const auto messages = Protocol::GetMessages::parseResponse(response);
		QString response;
		for (auto& message : messages)
		{
			response += QString(message.c_str()) + "\n";
			_messages.push_back(message);
		}
		if(response.size())
			_textEdit->insertPlainText(response);
		break;
	}

	case Protocol::QueryType::SendMessage:
	{
		// TODO [Client] Обработать событие успешной отправки сообщения
		const auto successfuly = Protocol::SendMessage::parseResponse(response);
		if(successfuly)
			_infText->setText(QString("Последнее сообщение было успешно доставлено"));
		auto msg = QString(_message->toPlainText() + "\n");
		_textEdit->insertPlainText(msg);
		_messages.push_back(msg.toStdString());
		_message->setText(QString());
		break;
	}

	default:
	{
		_infText->setText("Неизвестный тип ответа!");
	}

	}


	_send->setEnabled(true);

	sender()->deleteLater();
}

void MainWindow::failed()
{
	_infText->setText(QString("Не удалось отправить запрос или получить ответ!"));
	//_messages.clear();
	_send->setEnabled(true);

	sender()->deleteLater();
}

void MainWindow::updateAddress()
{
	if (_addressW->toPlainText() == "LocalHost")
		_hostAddress = QHostAddress::LocalHost;
	else
		_hostAddress = QHostAddress(_addressW->toPlainText());
	_port = (quint16)_portW->toPlainText().toInt();
}


MainWindow::MainWindow() : _textEdit(new QTextEdit), _message(new QTextEdit), _send(new QPushButton), _updateAddress(new QPushButton), _addressW(new QTextEdit), _portW(new QTextEdit), _infText(new QTextEdit)
 {
	setWindowTitle("Чат");

	_send->setText("Отправить сообщение");
	_updateAddress->setText("Обновить адрес сервера");

	_textEdit->setReadOnly(true);
	_message->setFixedHeight(40);

	_infText->setReadOnly(true);
	_infText->setFixedHeight(23);


	const auto rootLayout = new QVBoxLayout;
	const auto addressLayout = new QHBoxLayout;

	_addressW->setPlainText("LocalHost");
	_addressW->setFixedHeight(23);

	_portW->setPlainText("5000");
	_portW->setFixedHeight(23);

	addressLayout->addWidget(_addressW);
	addressLayout->addWidget(_portW);

	rootLayout->addWidget(_infText);
	rootLayout->addWidget(_textEdit);
	rootLayout->addWidget(_message);
	rootLayout->addWidget(_send);
	rootLayout->addLayout(addressLayout);
	rootLayout->addWidget(_updateAddress);

	rootLayout->setAlignment(_send, Qt::AlignRight);
	setLayout(rootLayout);

	if (_addressW->toPlainText() == "LocalHost")
		_hostAddress = QHostAddress::LocalHost;
	else
		_hostAddress = QHostAddress(_addressW->toPlainText());

	_port = (quint16)_portW->toPlainText().toInt();

	connect(&_updateTimer, &QTimer::timeout, this, &MainWindow::update);

	_updateTimer.start(100);

	QObject::connect(_send, &QAbstractButton::clicked, this, &MainWindow::sendMessage);
	QObject::connect(_updateAddress, &QAbstractButton::clicked, this, &MainWindow::updateAddress);
}
