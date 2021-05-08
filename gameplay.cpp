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
	const QString interfaces[]{
		"eth1",
		"enp4s0"
	};
	const int ports[]{
		50100,
	//	50102,
	//	50115,
	//	50124,
		50105,
	//	50106
	};
	const QString multicastGroups[]{
		"239.0.0.1",
		"239.0.0.2"
	};

	for (auto oInterface : interfaces)
	{
		for (auto oPort : ports)
		{
			QUdpSocket *socket = new QUdpSocket(this);
			connect(socket, &QUdpSocket::readyRead, this, &Gameplay::onPendingDatagrams);
			if (socket->bind(QHostAddress::AnyIPv4, oPort, QUdpSocket::ShareAddress))
			//if (socket->bind(QHostAddress(""), oPort, QUdpSocket::ShareAddress))
			{
				qDebug() << "BOUND successfully";
			}
			else
			{
				qDebug() << "CAN NOT BIND:" << socket->errorString();
				return false;
			}
			for (auto oGroup : multicastGroups)
			{
				if (socket->joinMulticastGroup(QHostAddress(oGroup), QNetworkInterface::interfaceFromName(oInterface)))
				{
					qDebug() << QString("SUBSCRIBED %1 interface '%2', port %3").arg(oGroup).arg(oInterface).arg(oPort);
				}
				else
				{
					qDebug() << QString("CAN NOT SUBSCRIBE  %1 interface '%2', port %3").arg(oGroup).arg(oInterface).arg(oPort);
					//return false;
				}
			}
			qDebug() << QString("LISTENING ON PORT %1 STARTED (interface '%2')").arg(oPort).arg(oInterface);
		}
	}

	return true;
}

void Gameplay::onPendingDatagrams()
{
	auto _socket = static_cast<QUdpSocket *>(sender());
	while (_socket->hasPendingDatagrams())
	{
		QNetworkDatagram datagram = _socket->receiveDatagram();
		if (datagram.isValid())
		{
			//datagram.data();
			qDebug()
				<< "SOME DATA RECEIVED ON PORT "
				<< datagram.destinationPort()
				<< ". Interface "
				<< QNetworkInterface::interfaceFromIndex(datagram.interfaceIndex()).name()
			;
//			  qDebug() << datagram.data().toHex(' ');
		}
	}
}

void Gameplay::onPendingDatagrams2()
{
}
