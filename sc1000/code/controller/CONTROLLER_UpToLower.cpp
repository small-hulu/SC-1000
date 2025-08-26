#include "CONTROLLER_UpToLower.h"

#include "CONTROLLER_Reagent.h"
#include "communication/config/SharedMemory.h"

namespace CONTROLLER {

UpToLower::UpToLower() {
    m_timer.setInterval(1000);
    connect(&m_timer, &QTimer::timeout, this, &UpToLower::onTimeout);
}

void UpToLower::Add_id(const QString& id) {
    if (m_idHash.size() <= MaxSize) {
        m_idHash[id]++;
    } else {
        sLog_other() << "id 存储超过阈值, id = " << id;
    }

    if (!m_idHash.isEmpty() && !m_timer.isActive()) {
        m_timer.start();
    }
}

void UpToLower::Remove_id(const QString& id) {
    if (m_idHash.contains(id)) {
        if (--m_idHash[id] == 0) {
            m_idHash.remove(id);
        }
    } else {
        sLog_other() << "id 移除错误，id = " << id;
    }

    if (m_idHash.isEmpty() && m_timer.isActive()) {
        m_timer.stop();
    }
}

void UpToLower::onTimeout() {
    using namespace CONTROLLER;
    BinInfo info;
    for (auto it = m_idHash.cbegin(); it != m_idHash.cend(); ++it) {
        QString id     = it.key();
        auto    reaSet = Reagent::instance().Select_byId_fromDB(id);
        if (reaSet == nullptr) {
            continue;
        }
        if (reaSet->table[Reagent::entity_attr::isLoad].toInt() == 0) {
            sLog_other() << "试剂id 未装载状态下维护，id = " << id;
            continue;
        }

        QString     fillingtext       = reaSet->table[Reagent::entity_attr::filling];
        QString     liquidConsume     = reaSet->table[Reagent::entity_attr::liquidConsume];
        QStringList fillingList       = fillingtext.split(",", QString::SkipEmptyParts);
        QStringList liquidConsumeList = liquidConsume.split(",", QString::SkipEmptyParts);
        for (int i = 0; i < fillingList.size() && i < info.liquidTotalList.size(); ++i) {
            int value_f             = fillingList[i].toInt();
            int value_l             = liquidConsumeList[i].toInt();
            info.liquidTotalList[i] = value_f * 1000;  // ml --> ul
            info.liquidConsume[i]   = value_l;         // ul
            info.liquidremaining[i] = qMax(0, info.liquidTotalList[i] - info.liquidConsume[i]);
        }

        int idxBinGroup = reaSet->table[Reagent::entity_attr::location].toInt();
        COM::SharedMemory::instance().Set_ReagentBinHole_liquidResidual(idxBinGroup - 1,
                                                                        info.liquidremaining);
    }
}
}  // namespace CONTROLLER
