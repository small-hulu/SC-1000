#include "CUIncubate.h"

#include <QDateTime>
#include <QDebug>

#include "../../config/CKeyJson.hpp"
#include "../../utility/JsonConfig.h"
#include "communication/COM_pch.hpp"

namespace COM {

CUIncubate::CUIncubate() {
    Reset_cacheFixtureParams();
}

QString CUIncubate::to_string() const {
    QString msg;
    msg += "Table.size():" + QString::number(m_table.size());
    return msg;
}

void CUIncubate::Reset_cacheFixtureParams() {
    is_load   = false;
    is_dilute = false;
    if (is_load) {
        qWarning() << QString("上料位标志重置 true");
    }
    if (is_dilute) {
        qWarning() << QString("稀释位标志重置 true");
    }

    /** 根据板孔表设置"二维数组" 8x7
     * |    [6]   [5]   [4]   [3]   [2]   [1]   [0]
     * | [0] o     o     o     o     o     o     o
     * | [1] o     o     o     o     o     o     o
     * | [2] o     o     o     o     o     o     o
     * | [3] o     o     o     o     o     o     o
     * | [4] o     o     o     o     o     o     o
     * | [5] o     o     o     o     o     o     o
     * y [6] o     o     o     o     o     o     o
     * v [7]                         o     o     o
     */
    /** 根据板孔表设置"二维数组" 7x8
     * |    [0]   [1]   [2]   [3]   [4]   [5]   [6]   [7]
     * | [0] o     o     o     o     o     o     o     o
     * | [1] o     o     o     o     o     o     o     o
     * | [2] o     o     o     o     o     o     o     o
     * | [3] o     o     o     o     o     o     o
     * | [4] o     o     o     o     o     o     o
     * | [5] o     o     o     o     o     o     o
     * x [6] o     o     o     o     o     o     o
     */
    m_table.clear();
    for (int i = 0; i < 3; i += 1) {
        QVector<entity_cell> line;
        for (int j = 0; j < 8; j += 1) {
            line.push_back(std::make_shared<Cell>());
        }
        m_table.push_back(std::move(line));
    }
    for (int i = 0; i < 4; i += 1) {
        QVector<entity_cell> line;
        for (int j = 0; j < 7; j += 1) {
            line.push_back(std::make_shared<Cell>());
        }
        m_table.push_back(std::move(line));
    }

    auto json         = JsonConfig::instance()[KJ::Gripper];
    auto incubateList = json[KJ::Incubate].toArray();
    for (auto&& item : incubateList) {
        QJsonObject obj            = item.toObject();
        int         i              = obj[KJ::i].toInt();
        int         j              = obj[KJ::j].toInt();
        m_table[i][j]->is_adjusted = true;
    }

    for (int i = 0; i < m_table.size(); i += 1) {
        for (int j = 0; j < m_table[i].size(); j += 1) {
            auto cell   = m_table[i][j];
            cell->state = Cell::State::Unoccupy;
            cell->idx_X = i;
            cell->idx_Y = j;
        }
    }  // for
}

QList<QPair<int, int> > CUIncubate::Get_allPoint() {
    QList<QPair<int, int> > res;
    auto                    idxList = flat_indexIJ();
    for (auto [i, j] : idxList) {
        auto cell = m_table[i][j];
        if (m_table[i][j]->is_adjusted) {
            res << QPair<int, int>{cell->idx_X, cell->idx_Y};
        }
    }
    return res;
}

/**
 * 请求放入
 */
QPair<int, int> CUIncubate::Request_toPutIn() {
    auto idxList = flat_indexIJ();
    for (auto [i, j] : idxList) {
        auto cell = m_table[i][j];
        if (cell->state == Cell::State::Unoccupy && m_table[i][j]->is_adjusted) {
            return {cell->idx_X, cell->idx_Y};
        }
    }
    return {-1, -1};
}

/**
 * 确认放入并开启孵育计时
 */
void CUIncubate::Affirm_toPutIn(Tube::entity_tube tube) {
    auto idxList = flat_indexIJ();
    for (auto [i, j] : idxList) {
        auto& cell = m_table[i][j];
        if (tube->paramPkg.incubateX == cell->idx_X && tube->paramPkg.incubateY == cell->idx_Y) {
            tube->Set_state(Tube::State_InIncubation);
            cell->tube  = tube;
            cell->state = Cell::Occupy;
            sLog_Thermostat() << tube.get() << "确认放入" << QDateTime::currentDateTime().time();

            //孵育时间到后执行的task
            auto task = [cell] {
                qDebug() << "QTimer::singleShot-timeout!";
                //设置可被转移，打开任务的循环 从而能够去执行node(孵育盘-->光电/磁分离)
                cell->tube->Set_state(Tube::State_CanBeTransfer);
                cell->state = Cell::Waiting;
            };
            tube->Set_delayTimeTask(tube->paramPkg.sleepTime, task);
        }

    }  // for
}

/**
 * 确认取出
 */
void CUIncubate::Affirm_fetch(Tube::entity_tube tube) {
    for (int i = 0; i < m_table.size(); i += 1) {
        for (int j = 0; j < m_table[i].size(); j += 1) {
            auto cell = m_table[i][j];
            if (tube->paramPkg.incubateX == cell->idx_X &&
                tube->paramPkg.incubateY == cell->idx_Y) {
                cell->tube  = {};
                cell->state = Cell::Unoccupy;
                sLog_Thermostat() << tube.get() << "确认取出"
                                  << QDateTime::currentDateTime().time();
                return;
            }
        }
    }  // for
}

/**
 * 按照y轴进行排序
 */
QList<QPair<int, int> > CUIncubate::flat_indexIJ() {
    QList<QPair<int, int> > res;
    for (int i = 0; i < m_table.size(); i += 1) {
        for (int j = 0; j < m_table[i].size(); j += 1) {
            res << QPair<int, int>{i, j};
        }
    }
    std::sort(res.begin(), res.end(), [](const QPair<int, int>& a, const QPair<int, int>& b) {
        if (a.second != b.second) {
            return a.second < b.second;
        }
        return a.first < b.first;
    });

    return res;
}

}  // namespace COM
