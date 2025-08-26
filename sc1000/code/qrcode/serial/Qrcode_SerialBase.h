#ifndef SERIALBASE_H_1721802200
#define SERIALBASE_H_1721802200

#include <QMap>
#include <QObject>
#include <QSerialPort>
#include <QSettings>
#include <QThread>

namespace QRCODE {
class SerialBase : public QObject {
    Q_OBJECT
public:
    struct Info {
        QString port;
        qint16  vid = -1;
        qint16  pid = -1;
    };

protected:
    QSerialPort m_serial;
    Info        m_info;

public:
    explicit SerialBase(QObject *parent = nullptr);

public:
    void Set_info(Info info) {
        m_info = info;
    }

protected:
    virtual void Reset();

public:
    /// 判断是否打开，纯代理
    bool isOpen();
    /// 默认基于串口号打开
    bool open();
    /// 关闭，阻塞，必然true
    bool close();

protected:
    virtual void handle_errorOccurred(QSerialPort::SerialPortError error) = 0;
    virtual void handle_readyRead()                                       = 0;
};
}  // namespace QRCODE

#endif  // SERIALBASE_H_1721802200
