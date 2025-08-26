#ifndef LIS_NET_CON_H
#define LIS_NET_CON_H

#include <QTcpSocket>
#include <QObject>
#include <QString>
#include "controller/CONTROLLER_ExpSample.h"
#include "widget/dialog/DIALOG.h"
class Lis_net_con : public QObject
{
    Q_OBJECT
public:
    static Lis_net_con& instance();

    bool connectToServer(const QString& host, quint16 port);

    void disconnectFromServer();
    bool sendData(const QByteArray& data);

    bool isConnected() const;

signals:
    void dataReceived(const QByteArray& data);

private:
    Lis_net_con();
    ~Lis_net_con();

    Lis_net_con(const Lis_net_con&) = delete;
    Lis_net_con& operator=(const Lis_net_con&) = delete;

private:
    QTcpSocket socket;
};

#endif // LIS_NET_CON_H
