#include "CONTROLLER_Consumable.h"

#include <QMetaEnum>

#include "communication/controller/Event.h"
#include "widget/dialog/DIALOG.h"

namespace CONTROLLER {

Consumable::Consumable() {
    DB::TableEntity::consume dbtable;
    QMetaEnum                qenum = QMetaEnum::fromType<ConsumableType>();

    auto tableAllVals = DB::SqlExe::Get_all_value(dbtable);
    for (auto&& arr : tableAllVals) {
        auto id = arr[0].toInt();
        arr.pop_front();
        assert(arr.size() == entity_attr::Enum_Count);
        const QString type = arr[entity_attr::type];
        if (false) {
        } else if (type == qenum.key(ASubstrat)) {
            cons_ASubstrat.id = id;
            cons_ASubstrat.Set_Values_WithOutId(arr);
        } else if (type == qenum.key(BSubstrat)) {
            cons_BSubstrat.id = id;
            cons_BSubstrat.Set_Values_WithOutId(arr);
        } else if (type == qenum.key(WasteTank)) {
            cons_wasteTank.Set_id(id);
            cons_wasteTank.Set_Values_WithOutId(arr);
        } else {
            assert(false);
        }
    }

    if (cons_ASubstrat.Is_valid() == false) {
        cons_ASubstrat.table[entity_attr::type] = qenum.key(ASubstrat);
        auto id = DB::SqlExe::Insert_oneLine(dbtable, cons_ASubstrat.Get_allValueList());
        cons_ASubstrat.Set_id(id);
    }

    if (cons_BSubstrat.Is_valid() == false) {
        cons_BSubstrat.table[entity_attr::type] = qenum.key(BSubstrat);
        auto id = DB::SqlExe::Insert_oneLine(dbtable, cons_BSubstrat.Get_allValueList());
        cons_BSubstrat.Set_id(id);
    }

    if (cons_wasteTank.Is_valid() == false) {
        cons_wasteTank.table[entity_attr::type] = qenum.key(WasteTank);
        auto id = DB::SqlExe::Insert_oneLine(dbtable, cons_wasteTank.Get_allValueList());
        cons_wasteTank.Set_id(id);
    }

    auto&& qrcode = QRCODE::SerialQrcode::instance();
    connect(
        &qrcode, &QRCODE::SerialQrcode::signal_qrcode_originalData, this,
        [this](const QByteArray& bytes, int type) { this->Qrcode_for_Consumable(bytes, type); });
}

QString Consumable::Get_describe_for_Substrat() {
    auto&& A   = cons_ASubstrat;
    auto&& B   = cons_BSubstrat;
    int    val = qMin(A.Get_surplusCapacity(), B.Get_surplusCapacity());
    return QString("%1").arg(val);
}

QString Consumable::Get_describe_for_WasteTank() {
    auto&& waste = cons_wasteTank;
    return waste.table[entity_attr::usedVal];
}

void Consumable::Qrcode_for_Consumable(const QByteArray& bytes, int type) {
    if (type != (int)QRCODE::QrcodeType::consumable) {
        return;
    }

    QRCODE::EConsumable obj(bytes);
    switch ((QRCODE::ConsumableType)obj.Get_consumableType()) {
    case QRCODE::ConsumableType::ASubstrat: {
        cons_ASubstrat.table[entity_attr::time]    = cons_ASubstrat.Generate_curTime();
        cons_ASubstrat.table[entity_attr::qrcode]  = bytes;
        cons_ASubstrat.table[entity_attr::usedVal] = "0";
        cons_ASubstrat.Update_toDB();
    } break;
    case QRCODE::ConsumableType::BSubstrat: {
        cons_BSubstrat.table[entity_attr::time]    = cons_BSubstrat.Generate_curTime();
        cons_BSubstrat.table[entity_attr::qrcode]  = bytes;
        cons_BSubstrat.table[entity_attr::usedVal] = "0";
        cons_BSubstrat.Update_toDB();
    } break;
    default: {
        sLog_qrcode() << "耗材条码解析异常";
    } break;
    }

    return;
}

void Consumable::Com_for_Consumable(int type, double reduceVal) {
    using Type = COM::DeviceConsumeType;
    switch (static_cast<Type>(type)) {
    case Type::ASubstrate: {
        auto&& A = cons_ASubstrat;
        A.Cumulative_consumption(reduceVal);
        if (A.Get_ratio() <= ENTITY::EInterface_Consume::Min_Rate) {
            WIDGET::DIALOG::Create_DWarningDialog("底物A耗材不足");
        }
    } break;
    case Type::BSubstrate: {
        auto&& B = cons_BSubstrat;
        B.Cumulative_consumption(reduceVal);
        if (B.Get_ratio() <= ENTITY::EInterface_Consume::Min_Rate) {
            WIDGET::DIALOG::Create_DWarningDialog("底物B耗材不足");
        }
    } break;
    case Type::WasteTank: {
        auto&& waste = cons_wasteTank;
        waste.Cumulative_consumption(reduceVal);
    } break;
    }
}

}  // namespace CONTROLLER
