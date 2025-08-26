#ifndef INCUBATETABLE_H_1718758926_1111
#define INCUBATETABLE_H_1718758926_1111

#include <QThread>
#include <atomic>

#include "../../entity/Tube.h"
#include "../IDeviceFixture.hpp"

namespace COM {
class CUIncubate final
    : public QObject
    , public IDeviceFixture {
    Q_OBJECT
public:
    struct Cell {
        enum State {
            Unoccupy,  // 未占用
            Occupy,    // 占用
            Waiting,   // 等待  孵育结束等待夹爪
        };
        /// 当前状态
        State state;
        /// 所在位置
        int idx_X;
        int idx_Y;

        bool              is_adjusted = false;
        Tube::entity_tube tube;
    };

private:
    using entity_cell = std::shared_ptr<Cell>;
    QVector<QVector<entity_cell>> m_table;

public:
    std::atomic<bool> is_load   = false;
    std::atomic<bool> is_dilute = false;

public:
    CUIncubate();
    Q_DISABLE_COPY_MOVE(CUIncubate)

public:
    QString to_string() const;

public:
    virtual void Reset_cacheFixtureParams();

public:
    QList<QPair<int, int>> Get_allPoint();

public:
    QPair<int, int> Request_toPutIn();
    void            Affirm_toPutIn(Tube::entity_tube tube);
    void            Affirm_fetch(Tube::entity_tube tube);

private:
    /// 扁平化坐标
    QList<QPair<int, int>> flat_indexIJ();
};
}  // namespace COM
#endif  // INCUBATETABLE_H_1718758926_1111
