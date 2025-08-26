#ifndef SERIALQRCODE_H_1721804208
#define SERIALQRCODE_H_1721804208

#include "Qrcode_SerialBase.h"

namespace QRCODE {
class SerialQrcode final : public SerialBase {
    Q_OBJECT
public:
    using Self = SerialQrcode;
    static Self& instance() {
        static Self obj{};
        return obj;
    }

private:
    QByteArray m_serialRawData;
    QByteArray m_lastAnalysisedData;
    int        m_timeId = -1;

private:
    SerialQrcode(QObject* parent = nullptr);

public:
    QByteArray Get_lastAnalysisedData() {
        return m_lastAnalysisedData;
    }

public:
    int  Start_periodicEvents(int ms);
    void Stop_periodicEvents();

protected:
    void Reset() override;
    void handle_errorOccurred(QSerialPort::SerialPortError error) override;
    void handle_readyRead() override;

private:
    int analyse_codeType(const QByteArray& bytes);

protected:
    void timerEvent(QTimerEvent* event) override;

signals:
    void signal_qrcode_originalData(const QByteArray& bytes, int type = 0);
};
}  // namespace QRCODE
#endif  // SERIALQRCODE_H_1721804208
