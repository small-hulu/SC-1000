#ifndef CONTROLLER_UPTOLOWER_H_1744955924
#define CONTROLLER_UPTOLOWER_H_1744955924

#include <QHash>
#include <QObject>
#include <QTimer>

namespace CONTROLLER {
class UpToLower : public QObject {
public:
    using Self = UpToLower;
    static Self& instance() {
        static Self obj{};
        return obj;
    }

public:
    static constexpr int MaxSize = 12;

    struct BinInfo {
        /// [后台] 试剂仓的组号
        int idxBinGroup              = 0;
        /// [后台] 孔位
        int idxBinBack               = 0;
        /// 试剂仓内的
        QVector<int> liquidTotalList = QVector<int>(4);
        QVector<int> liquidConsume   = QVector<int>(4);
        QVector<int> liquidremaining = QVector<int>(4);
    };

private:
    QTimer              m_timer;
    QHash<QString, int> m_idHash;

private:
    UpToLower();

public:
    void Add_id(const QString& id);
    void Remove_id(const QString& id);

private:
    void onTimeout();
};
}  // namespace CONTROLLER
#endif  // CONTROLLER_UPTOLOWER_H_1744955924
