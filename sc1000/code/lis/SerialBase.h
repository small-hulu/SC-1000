#ifndef SERIALBASE_H_1742281952
#define SERIALBASE_H_1742281952

#include <QObject>
#include <QSerialPort>

namespace LIS {

class SerialBase : public QObject {
    Q_OBJECT
public:
    struct ComInfo {
        QString                  port_name;
        qint32                   BaudRate;
        QSerialPort::DataBits    DataBits;
        QSerialPort::Parity      Checkbit;
        QSerialPort::StopBits    Stopbit;
        QSerialPort::FlowControl FlowControl;
    };

protected:
    QSerialPort m_serial;
    ComInfo     m_info;

public:
    explicit SerialBase(QObject *parent = nullptr);

public:
    void Set_info(ComInfo info) {
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

public:
    virtual void Write_bySequenced(const QByteArray &data);
    virtual void Write_byParallel(const QByteArray &data);

protected:
    virtual void handle_errorOccurred(QSerialPort::SerialPortError error) = 0;
    virtual void handle_readyRead()                                       = 0;
};
}  // namespace LIS

#endif  // SERIALBASE_H_1742281952
