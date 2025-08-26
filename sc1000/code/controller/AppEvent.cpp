#include "AppEvent.h"

#include "CONTROLLER_Consumable.h"
#include "CONTROLLER_Sample.h"
#include "communication/controller/DispatchController.h"
#include "communication/controller/Event.h"
#include "device/CDevice.hpp"
#include "global/GLOBAL_DeviceState.h"
#include "lte/lte_lbs.h"
#include "utility/UTILITY_Sound.h"
#include "widget/dialog/DIALOG.h"

namespace CONTROLLER {
AppEvent::AppEvent(QObject* parent) : QObject(parent) {
    receive_comSignal();
    receive_expSignal();
}

void AppEvent::receive_comSignal() {
    auto&& comEvent = COM::Event::instance();
    /// 警告，（信息:str, 是否严重:int）
    connect(&comEvent, &COM::Event::signal_waring, this, [](const QString& msg, int flag) {
        qWarning() << "signal_waring" << msg;
        UTILITY::Sound::Speech_Word(msg);
        if (flag) {
            CDevice::DLightBand::Set_LightBandColor(
                GLOBAL::Get_DeviceStateColor(GLOBAL::MachineState::Waring));
            AppEvent::instance().signal_expStateHint(true, msg);
            qWarning() << ("主动停止正在做的实验");
            COM::DispatchController::instance().Stop_dispatch();
        }
    });

    /// muc异常
    connect(&comEvent, &COM::Event::signal_mcuError, this, [](int code, const QString& msg) {
        qWarning() << "signal_mcuError" << QString("code [%1]").arg(code) << msg;
        CDevice::DLightBand::Set_LightBandColor(
            GLOBAL::Get_DeviceStateColor(GLOBAL::MachineState::Waring));
        LTE_LBS::instance().Send_ErrorCode((COM::CLteErrorCode::ErrorCode)code);

        auto warn = WIDGET::DIALOG::Create_DWarningDialog(msg);
        warn->Speech_keep(msg);
        GLOBAL::EquipmentRecord::state = GLOBAL::MachineState::Waring;
    });

    /// 耗材消耗
    connect(&comEvent, &COM::Event::signal_consume, this, [](int type, double reduceVal) {
        qInfo() << "signal_consume" << type << reduceVal;
        Consumable::instance().Com_for_Consumable(type, reduceVal);
    });

    /// 耗材的异常
    connect(&comEvent, &COM::Event::signal_consumeWaring, this, [](const QString& msg) {
        qWarning() << "signal_consumeWaring" << msg;
        auto warn = WIDGET::DIALOG::Create_DWarningDialog(msg);
        warn->Speech_keep(msg);
    });
}

/**
 * 实验相关的信号
 */
void AppEvent::receive_expSignal() {
    auto&& cu = COM::DispatchController::instance();

    connect(&cu, &COM::DispatchController::signal_Accomplish, this, []() {
        WIDGET::DIALOG::Create_DInfoDialog("实验结束");
        CDevice::DLightBand::Set_LightBandColor(
            GLOBAL::Get_DeviceStateColor(GLOBAL::MachineState::Normal));
        AppEvent::instance().signal_expStateHint(false, "实验结束");
        UTILITY::Sound::Speech_Word("实验结束");
    });

    connect(&cu, &COM::DispatchController::signal_ForceStop, this, []() {
        CDevice::DLightBand::Set_LightBandColor(
            GLOBAL::Get_DeviceStateColor(GLOBAL::MachineState::Waring));
        LTE_LBS::instance().Send_ErrorCode(COM::CLteErrorCode::ErrorCode::SCForceStop);

        const QString msg = "实验异常，强制终止";
        qWarning() << msg;
        auto warn = WIDGET::DIALOG::Create_DWarningDialog(msg);
        warn->Speech_keep(msg);
        AppEvent::instance().signal_expStateHint(true, msg);

        auto sampleList = Sample::instance().Get_isLoadList();
        for (auto&& sample : sampleList) {
            if (sample->table[Sample::entity_attr::state].toInt() ==
                (int)Sample::entity_type::TubeState::Testing) {
                sample->table[Sample::entity_attr::state] =
                    QString::number((int)Sample::entity_type::TubeState::Uncompleted);
                sample->Update_toDB();
            }
        }
    });
}
}  // namespace CONTROLLER
