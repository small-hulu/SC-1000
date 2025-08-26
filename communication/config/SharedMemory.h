/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       SharedMemory.h
 * @date       2024-11-04
 * @version    1.0.0
 *
 * @author     cuber-lotus
 * @Email      luyinfeng@smtbio.com
 *
 * @brief
 * 如果外部不设置，而内部又需要使用这些数据
 * 可能导致功能实现异常
 *****************************************************************************/
#ifndef SHAREDMEMORY_H_730690342
#define SHAREDMEMORY_H_730690342

#include <QVector>

#include "../config/CParams.hpp"
#include "../interface/ISingleton.hpp"

namespace COM {
class SharedMemory : public ISingleton<SharedMemory> {
public:
    struct ReagentBinNode {
        /// 试剂的四个孔位[0, 4]
        /// 的残留量（微升）
        QVector<int> liquidResidualList = QVector<int>(ReagentBinHoleReagentCount);
    };

private:
    /// 试剂仓的状态表
    QVector<ReagentBinNode> m_ReagentBinSet = QVector<ReagentBinNode>(ReagentBinBinCount);

public:
    /// 试剂仓自选是否中断服务
    std::atomic_bool flag_autoReagentBinMixingInterruptServicing = false;

public:
    SharedMemory() = default;

public:
    /**
     * @brief Set_ReagentBinHole_liquidResidual
     * 修改第 idxBinGroup 组分的 第 idxBinBack 孔位的当前余量值 val
     * @param idxBinGroup 组号 (0, 12]
     * @param idxBinBack  孔号 (0, 4]
     * @param val         液体余量 （微升）
     */
    void Set_ReagentBinHole_liquidResidual(int idxBinGroup, int idxBinBack, int val);
    void Set_ReagentBinHole_liquidResidual(int idxBinGroup, QVector<int> liquidResidualList);

    /**
     * @brief Get_ReagentBinHole_liquidResidual
     * 获取第 idxBinGroup 组分的 第 idxBinBack 孔位的当前余量值 val
     * @param idxBinGroup 组号 (0, 12]
     * @param idxBinBack  孔号 (0, 4]
     * @return （微升）
     */
    int Get_ReagentBinHole_liquidResidual(int idxBinGroup, int idxBinBack);
};
}  // namespace COM
#endif  // SHAREDMEMORY_H_730690342
