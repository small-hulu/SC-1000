#include "Lis_net_con.h"

Lis_net_con& Lis_net_con::instance()
{
    static Lis_net_con obj;
    return obj;
}

Lis_net_con::Lis_net_con()
{ 
    connect(&socket, &QTcpSocket::readyRead, this, [this]() {
        emit dataReceived(socket.readAll());
    });
}

Lis_net_con::~Lis_net_con()
{
    disconnectFromServer();
}

bool Lis_net_con::connectToServer(const QString& host, quint16 port)
{
    if (socket.state() == QAbstractSocket::ConnectedState) {
            return true;
        }

        socket.connectToHost(host, port);
        if (!socket.waitForConnected(3000)) {
            return false;
        }
        return true;
}

void Lis_net_con::disconnectFromServer()
{
    if (socket.state() == QAbstractSocket::ConnectedState) {
        socket.disconnectFromHost();
        socket.waitForDisconnected(3000);
    }
}

bool Lis_net_con::sendData(const QByteArray& data)
{
    if (socket.state() == QAbstractSocket::ConnectedState) {
        socket.write(data);
        return socket.waitForBytesWritten(3000);
    }
    return false;
}

bool Lis_net_con::isConnected() const
{
    return socket.state() == QAbstractSocket::ConnectedState;
}
