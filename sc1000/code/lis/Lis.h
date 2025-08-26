#ifndef LIS_H_1742281952
#define LIS_H_1742281952

#include "SerialBase.h"

namespace LIS {

class Lis final : public SerialBase {
    Q_OBJECT
public:
    using Self = Lis;
    static Self& instance() {
        static Self obj{};
        return obj;
    }

public:
    enum class Mode {
        SingleDirection = 0,  // 单向
        Bidirectional   = 1   // 双向
    };

private:
    QByteArray m_serialRawData;
    QByteArray m_lastAnalysisedData;
    Mode       m_mode = Mode::SingleDirection;

private:
    Lis(QObject* parent = nullptr);

public:
    QByteArray Get_lastAnalysisedData() {
        return m_lastAnalysisedData;
    }
    void setMode(Mode mode) {
        m_mode = mode;
    }
    Mode getMode() const {
        return m_mode;
    }

protected:
    void Reset() override;
    void handle_errorOccurred(QSerialPort::SerialPortError error) override;
    void handle_readyRead() override;

signals:
    void signal_lisData(const QByteArray& bytes);
};
}  // namespace LIS

#endif  // LIS_H_1742281952
