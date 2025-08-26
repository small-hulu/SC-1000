#ifndef TRAYTABLE_H_1715657890
#define TRAYTABLE_H_1715657890
/*
 * 控制磁分离盘上的管（tube）在各个处理阶段（针头操作、底物加注、出盘）之间的状态变迁与位置管理。
 */


#include <QDebug>
#include <QMetaEnum>
#include <QQueue>
#include <QThread>
#include <QVector>
#include <atomic>

#include "../../config/CParams.hpp"
#include "../../entity/Tube.h"
#include "../../interface/ICOMThread.h"

namespace COM {
class CUMagneticSeparation final : public ICOMThread {
    Q_OBJECT
public:
    static constexpr int Cell_Count = MagneticSeparationCount;

    static int offset(int idx) {
        idx %= Cell_Count;
        return (idx + Cell_Count) % Cell_Count;
    }

public:
    enum class State : int {
        Unoccupy,        // 未占用
        NeedNeedle0,     // 需要第1次排液
        NeedNeedle1,     // 需要第2次排液
        NeedNeedle2,     // 需要第3次排液
        NeedASubstrate,  // 需要底物A
        CanTransferOut,  // 可以转移出去
    };
    Q_ENUM(State);

    struct Cell {
        /// 当前状态
        State state    = State::Unoccupy;
        /// 物理层面的，盘上的第几个孔（仅初始化一次，不准动）
        int idxReality = -1;
        /// 逻辑层面的编号（随轮盘转动改变）
        int idxLogic   = -1;
        /// 管对象
        Tube::entity_tube tube;

        QString to_string() const {
            QString msg;
            msg += QString(" state:%1").arg(QMetaEnum::fromType<State>().key((int)state));
            msg += QString(" idxReality:%1").arg(idxReality);
            msg += QString(" idxLogic:%1").arg(idxLogic);
            return msg;
        }
    };
    using CellRef = Cell&;

    /**
     * |0   1   2   3   4   5   6|
     *      针      针          针
     */
    struct Border {
        enum : int {
            //! 注意起始位置的设置(0是逻辑上的)
            IdxInBorder  = 10,               /// 进入边界
            IdxNeedle0   = IdxInBorder + 1,  /// 第1次排液
            IdxNeedle1   = IdxNeedle0 + 2,   /// 第2次排液
            IdxNeedle2   = IdxNeedle1 + 3,   /// 第3次排液
            IdxOutBorder = IdxNeedle2,       /// 出去边界
        };
        const int inBorder  = offset(IdxInBorder);
        const int outBorder = offset(IdxOutBorder);
        const int needle0   = offset(IdxNeedle0);
        const int needle1   = offset(IdxNeedle1);
        const int needle2   = offset(IdxNeedle2);

        constexpr int get_borderLength() const {
            return IdxOutBorder - IdxInBorder + 1;
        }

        QString to_string() const {
            QString msg;
            msg += QString(" inBorder:%1").arg(inBorder);
            msg += QString(" outBorder:%1").arg(outBorder);
            msg += QString(" needle0:%1").arg(needle0);
            msg += QString(" needle1:%1").arg(needle1);
            msg += QString(" needle2:%1").arg(needle2);
            return msg;
        }
    };

private:  // 磁分离轮询辅助信息
    /// 轮询周期
    int ms                     = 15000;
    /// 外部接入标志
    std::atomic_bool exInvolve = 0;

private:  // 磁分离盘状态标志
    Border        border;
    QVector<Cell> table; //保存所有有效孔位的 Cell 信息
    Cell          inValidCell;

public:
    CUMagneticSeparation();

public:
    QString to_string() const;
    void    Reset();

public:
    /// 请求放入 外部请求放入一个新的管（找到一个空的孔位并返回其逻辑位置）。
    int Request_putIn();
    /// 确认放入 确认某个位置放入了具体的 tube，会更新 state 和存储对象。
    void Affirm_putIn(Tube::entity_tube tube);
    /// 确认取出 确认某个管已经被取出，从磁盘中移除它。
    void Affirm_fetch(Tube::entity_tube tube);

private:
    CellRef get_RealityCell(int idx) {
        for (CellRef cell : table) {
            if (cell.idxReality == idx) {
                return cell;
            }
        }
        qCritical() << idx << "磁分离自旋 下标维护异常";
        return inValidCell;
    }

    CellRef get_logicCell(int idx) {
        for (CellRef cell : table) {
            if (cell.idxLogic == idx) {
                return cell;
            }
        }
        qCritical() << idx << "磁分离自旋 下标维护异常";
        return inValidCell;
    }

    /// 转盘与逻辑标号对齐
    void align_Logictable(const char* const func = __builtin_FUNCTION());
    void poll_spin();    //处理转盘旋转的对齐逻辑
    void poll_state();   //处理当前各孔的状态变迁

protected:
    virtual void run() override;
};
}  // namespace COM
#endif  // TRAYTABLE_H_1715657890
