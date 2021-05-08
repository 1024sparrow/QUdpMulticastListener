#include "gameplay.h"

#include <QDebug>
#include <QNetworkDatagram>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QCoreApplication>

Gameplay::Gameplay(QObject *parent)
{

}

bool Gameplay::start()
{
	const int ports[]{50100,50102,50115,50124,50105,50106};
	for (int oPort : ports)
	{
		_socket = new QUdpSocket(this);
		connect(_socket, &QUdpSocket::readyRead, this, &Gameplay::onPendingDatagrams);

		if (_socket->bind(QHostAddress::AnyIPv4, oPort, QUdpSocket::ShareAddress))
		//if (_socket->bind(QHostAddress(""), oPort, QUdpSocket::ShareAddress))
		{
			qDebug() << "BOUND successfully";
		}
		else
		{
			qDebug() << "CAN NOT BIND:" << _socket->errorString();
			return false;
		}

		//{{
		_socket2 = new QUdpSocket(this);
		connect(_socket2, &QUdpSocket::readyRead, this, &Gameplay::onPendingDatagrams2);

		if (_socket2->bind(QHostAddress::AnyIPv4, oPort, QUdpSocket::ShareAddress))
		{
			qDebug() << "BOUND 2 successfully";
		}
		else
		{
			qDebug() << "CAN NOT BIND 2:" << _socket2->errorString();
			return false;
		}
		//}}

		QUdpSocket sock;

		if (_socket2->joinMulticastGroup(QHostAddress("239.0.0.1"), QNetworkInterface::interfaceFromName("eth1")))
		{
			qDebug() << "SUBSCRIBED 239.0.0.1 first interface";
		}
		else
		{
			qDebug() << "CAN NOT SUBSCRIBE 239.0.0.1 first interface" << _socket->errorString();
			//return false;
		}

		if (_socket2->joinMulticastGroup(QHostAddress("239.0.0.1"), QNetworkInterface::interfaceFromName("enp4s0")))
		{
			qDebug() << "SUBSCRIBED 239.0.0.1 second interface";
		}
		else
		{
			qDebug() << "CAN NOT SUBSCRIBE 239.0.0.1 second interface" << _socket->errorString();
			//return false;
		}

		if (_socket2->joinMulticastGroup(QHostAddress("239.0.0.2"), QNetworkInterface::interfaceFromName("eth1")))
		{
			qDebug() << "SUBSCRIBED 239.0.0.2 first interface";
		}
		else
		{
			qDebug() << "CAN NOT SUBSCRIBE 239.0.0.2 first interfcae" << _socket->errorString();
			//return false;
		}

		if (_socket2->joinMulticastGroup(QHostAddress("239.0.0.2"), QNetworkInterface::interfaceFromName("enp4s0")))
		{
			qDebug() << "SUBSCRIBED 239.0.0.2 second interface";
		}
		else
		{
			qDebug() << "CAN NOT SUBSCRIBE" << _socket->errorString();
			//return false;
		}
	}

	qDebug() << "LISTENING ON PORT 50100 STARTED";
	return true;
}

void Gameplay::onPendingDatagrams()
{
	while (_socket->hasPendingDatagrams())
	{
		QNetworkDatagram datagram = _socket->receiveDatagram();
		if (datagram.isValid())
		{
			//datagram.data();
			qDebug() << "SOME DATA RECEIVED ON PORT " << datagram.destinationPort() << ". Interface " << QNetworkInterface::interfaceFromIndex(datagram.interfaceIndex()).name();
//			  qDebug() << datagram.data().toHex(' ');
		}
	}
}

void Gameplay::onPendingDatagrams2()
{
	while (_socket2->hasPendingDatagrams())
	{
		QNetworkDatagram datagram = _socket2->receiveDatagram();
		if (datagram.isValid())
		{
			//datagram.data();
			qDebug() << "SOME DATA RECEIVED ON PORT " << datagram.destinationPort() << ". Interface " << QNetworkInterface::interfaceFromIndex(datagram.interfaceIndex()).name();
			//qDebug() << "SOME DATA RECEIVED ON PORT 50124" << datagram.senderAddress() << datagram.destinationAddress();
			//qDebug() << datagram.data().toHex(' ');
		}
	}
}
