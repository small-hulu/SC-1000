#include "ENTITY_EExpCalibration.h"

#include "controller/CONTROLLER_Calibration.h"
#include "controller/CONTROLLER_Reagent.h"

namespace ENTITY {

EExpCalibration::EExpCalibration() {
    table[attr::timeStart] = QDateTime::currentDateTime().toString(time_format);
}

void EExpCalibration::Builder() {
    const int repetitionsCount = [this]() -> int {
        auto calItem = Get_relatedCalibration();
        if (calItem == nullptr) {
            return 1;
        }
        auto qrcodeCal = calItem->Get_qrcodeEntity();
        int  cnt       = qrcodeCal[QRCODE::KeyCal::repetitions].toInt();
        return qMax(1, cnt);
    }();
    auto idxLocationList = Get_c0n_LocationList();

    auto valList = Get_c0n_originalLightValList();
    while (valList.size() < idxLocationList.size()) {
        valList.push_back({});
        while (valList.back().size() < repetitionsCount) {
            valList.back() << "0";
        }
    }

    QList<QString> valStrList;
    for (auto line : valList) {
        valStrList << line.join(join_separator2);
    }
    table[attr::C0nLightValList] = valStrList.join(join_separator);
}

CONTROLLER::Reagent::entity_item EExpCalibration::Get_relatedReagent() {
    QString calId = table[attr::idReagent];
    return CONTROLLER::Reagent::instance().Select_byId_fromDB(calId);
}

CONTROLLER::Calibration::entity_item EExpCalibration::Get_relatedCalibration() {
    QString calId = table[attr::idCalibration];
    return CONTROLLER::Calibration::instance().Select_byId_fromDB(calId);
}

void EExpCalibration::Set_expData(int data, int idxLocation, int idxRepetitions) {
    auto valList = Get_c0n_originalLightValList();
    assert(valList.size() > idxLocation);
    assert(valList[idxLocation].size() > idxRepetitions);
    valList[idxLocation][idxRepetitions] = QString::number(data);

    QList<QString> valStrList;
    for (auto line : valList) {
        valStrList << line.join(join_separator2);
    }
    table[attr::C0nLightValList] = valStrList.join(join_separator);
    table[attr::timeEnd]         = QDateTime::currentDateTime().toString(time_format);

    if (std::any_of(valList.begin(), valList.end(), [](auto&& arr) {
            return std::any_of(arr.begin(), arr.end(), [](auto&& data) { return data == "0"; });
        })) {
    } else {
        table[attr::state] = QString::number((int)ETubeState::Complete);
    }
}

void EExpCalibration::Set_expData(const QList<QString>& data) {
    auto valList = Get_c0n_originalLightValList();
    int  k       = 0;
    for (int i = 0; i < valList.size(); i += 1) {
        for (int j = 0; j < valList[i].size(); j += 1) {
            valList[i][j] = data[k++];
        }
    }
    QList<QString> valStrList;
    for (auto line : valList) {
        valStrList << line.join(join_separator2);
    }
    table[attr::C0nLightValList] = valStrList.join(join_separator);
}

void EExpCalibration::recalculate_4plc() {
    auto idCal = table[attr::idCalibration];
    auto cal   = CONTROLLER::Calibration::instance().Select_byId_fromDB(idCal);
    auto idRea = table[attr::idReagent];
    auto rea   = CONTROLLER::Reagent::instance().Select_byId_fromDB(idRea);
    if (cal == nullptr || rea == nullptr) {
        return;
    }
}

}  // namespace ENTITY
