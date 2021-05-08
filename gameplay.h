#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <QObject>
#include <QUdpSocket>

class Gameplay : public QObject
{
    Q_OBJECT
public:
    Gameplay(QObject *parent = nullptr);
    bool start();

private slots:
    void onPendingDatagrams();
    void onPendingDatagrams2();

private:
    QUdpSocket *_socket, *_socket2;
};

#endif // GAMEPLAY_H
