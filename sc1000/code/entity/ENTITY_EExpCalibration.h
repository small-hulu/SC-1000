#ifndef EEXPCALIBRATION_H_1732171813
#define EEXPCALIBRATION_H_1732171813

#include <QJsonObject>

#include "EInterface_DB.hpp"
#include "controller/CONTROLLER_Calibration.h"
#include "controller/CONTROLLER_Reagent.h"
#include "global/GLOBAL_TypeTube.h"

namespace ENTITY {
class EExpCalibration : public EInterface_DB<::DB::TableEntity::expcalibration> {
public:
    using ETubeState = Type::ETubeState;

public:
    EExpCalibration();

    void Builder();

public:
    CONTROLLER::Reagent::entity_item     Get_relatedReagent();
    CONTROLLER::Calibration::entity_item Get_relatedCalibration();

public:
    /// [][]
    /// [0, n-1][0, n-1]
    /// [第几个校准点][第几次重复次数]
    void Set_expData(int data, int idxLocation, int idxRepetitions);
    void Set_expData(const QList<QString>& data);

public:
    /// 重新矫正计算4plc参数
    void recalculate_4plc();

public:
    virtual std::size_t Get_hash() override {
        return UTILITY::get_hash(id, reinterpret_cast<std::size_t>(this));
    }

public:
    /**
     * location 和 val
     * 两个List的长度应该保持一致
     */
    QList<QString> Get_c0n_LocationList() {
        return table[attr::C0nLocationList].split(join_separator, QString::SkipEmptyParts);
    }

    void Set_c0n_LocationList(const QList<QString>& list) {
        table[attr::C0nLocationList] = list.join(join_separator);
    }

    QList<QString> Get_c0n_avgLightValList() {
        QList<QString> res;
        for (auto line :
             table[attr::C0nLightValList].split(join_separator, QString::SkipEmptyParts)) {
            int sum = 0;
            int cnt = 0;
            for (auto node : line.split(join_separator2, QString::SkipEmptyParts)) {
                sum += node.toDouble();
                cnt += 1;
            }
            res << QString::number(sum / cnt);
        }
        return res;
    }


    QList<QList<QString>> Get_c0n_originalLightValList() {
        QList<QList<QString>> res;
        for (auto line :
             table[attr::C0nLightValList].split(join_separator, QString::SkipEmptyParts)) {
            res << line.split(join_separator2, QString::SkipEmptyParts);
        }
        return res;
    }
};
}  // namespace ENTITY

#endif  // EEXPCALIBRATION_H_1732171813
