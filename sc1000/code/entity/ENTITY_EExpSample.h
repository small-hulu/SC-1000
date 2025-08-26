#ifndef EEXPERIMENT_H_1719970981
#define EEXPERIMENT_H_1719970981

#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>

#include "EInterface_DB.hpp"
#include "controller/CONTROLLER_Reagent.h"
#include "controller/CONTROLLER_Sample.h"
#include "global/GLOBAL_Type.h"

namespace ENTITY {
class EExpSample : public EInterface_DB<::DB::TableEntity::expsample> {
public:
    using ExpType = Type::ETubeExp;

public:
    QJsonObject json;

public:
    EExpSample();

public:
    CONTROLLER::Reagent::entity_item Get_relatedReagent() {
        QString calId = table[attr::idReagent];
        return CONTROLLER::Reagent::instance().Select_byId_fromDB(calId);
    }
    CONTROLLER::Sample::entity_item Get_relatedSample() {
        QString samId = table[attr::idSample];
        return CONTROLLER::Sample::instance().Select_byId_fromDB(samId);
    }

public:
    bool Is_valid() override;

    virtual std::size_t Get_hash() override {
        return static_cast<std::size_t>(id) ^ reinterpret_cast<std::size_t>(this);
    }

public:
    bool Is_inRangeTime(QDateTime startTime, QDateTime endTime) override;

public:
    /// 记录每组实验所关联 `试剂`和`样本`
    /// 这里的编号，不单单是样本，可有校准品，质控品等
    void Set_id_aboutReagentAndSample(int idReagent, int idSample);

    /// 记录原始数据
    /// 同时记录好时间戳
    void    Set_expData(int data);
    QString result_Format(double data);
    int     Get_expData();

public:
    /// 获取浓度值
    double Get_Concentration();

    QPair<int, QString> Judge_scope();
};
}  // namespace ENTITY

#endif  // EEXPERIMENT_H_1719970981
