#include "SC1000_StartUp.h"

#include <QDebug>

#include "controller/CONTROLLER_Config.h"
#include "controller/IniSC1000.h"
#include "controller/device/CDevice_DInit.h"
#include "controller/device/CDevice_DLightBand.h"
#include "global/GLOBAL_DeviceState.h"
#include "lis/Lis_SC1000.h"
#include "lte/lte_lbs.h"
#include "./lte/controller/Parse.h"
#include "./lte/config/CKeyJson.hpp"
#include "lte/controller/Lte_Scheduler.h"
#include "qrcode/QRCODE.hpp"
#include "widget/Background.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS_Resize.h"
#include "widget/window/main/WIDGET_Login.h"
#include "widget/window/main/WIDGET_Main.h"
#include "widget/window/sample/WIDGET_SampleApply.h"
#include "widget/window/setting/lisManage/Lis_net_con.h"
/**
 * 主要用于
 * - 管理界面
 * - 通信链接
 */
StartUp::StartUp() {
    m_widgetList.resize((int)BaseWidget::StartUp_BWCount);

    /// [login]
    auto widgetLogin = new WIDGET::Login{WIDGET::Background::Get()};
    widgetLogin->show();
    m_widgetList[(int)BaseWidget::BW_Login] = widgetLogin;
    /// [login]

    /// [connect]
    connect(widgetLogin, &WIDGET::Login::signal_loginOk, this, &StartUp::login_ok);
    /// [connect]

    qInfo() << __func__ << "Success";
}

/**
 * 主要打个日志
 * 用于记录软件程序声明周期的完整性
 * 不用管理内存
 * 因为这里代表整个程序的结束
 * =========================
 * 但是，目前使用了background的设置方案
 * 这里的作用就更小了
 */
StartUp::~StartUp() {
    qInfo() << __func__;
    CONTROLLER::CDevice::DLightBand::Set_LightBandColor(
        GLOBAL::Get_DeviceStateColor((GLOBAL::MachineState)-1));
    exit(0);
}

/**
 * 登录成功，
 */
void StartUp::login_ok() {
    if (m_widgetList[(int)BaseWidget::BW_Login]) {
        m_widgetList[(int)BaseWidget::BW_Login]->close();
        m_widgetList[(int)BaseWidget::BW_Login] = nullptr;
    }

    if (nullptr == m_widgetList[(int)BaseWidget::BW_Main]) {
        auto widgetMain = new WIDGET::Main{WIDGET::Background::Get()};
        widgetMain->show();
        m_widgetList[(int)BaseWidget::BW_Main] = widgetMain;

        QTimer::singleShot(2850, this, &StartUp::link_communication);
    }
}

/**
 * 通信连接性操作
 * ================================================
 * 为什么通信的连接不在 startup-instace 前面的init
 * 因为最初的init是需要保证整个app程序能启动的
 * 而通信连接失败，很多操作还是可以操作的，如查看历史数据等等
 * 且通讯连接的相关问题可能耗时和操作复杂，因此放于这里
 */
void StartUp::link_communication() {
    using namespace CONTROLLER;

    bool success = CDevice::DInit::Init();
    if (false == success) {
        qWarning() << "仪器通信连接异常";
        // WIDGET::DIALOG::Create_DWarningDialog("仪器通信连接异常");
    }

    auto&& ini = IniSC1000::instance();

    QRCODE::SerialQrcode::Info info;
    info.vid      = ini[KI::qrcode_vid].toInt();
    auto&& qrcode = QRCODE::SerialQrcode::instance();
    qrcode.Set_info(info);
    qrcode.Start_periodicEvents(1500);

    LIS::Lis::ComInfo comInfo;
    LIS::Lis_SC1000::Set_defaultInfo(comInfo);
    auto& lis = LIS::Lis::instance();
    lis.Set_info(comInfo);
    CONTROLLER::Config::Network network;
    lis.setMode((LIS::Lis::Mode)network.lisConfig.direction);
    lis.open();

    qint16 port = static_cast<qint16>(network.networkConfig.serverPort.toInt());
    auto& conn = Lis_net_con::instance();
    if (conn.connectToServer(network.networkConfig.serverIP, port)) {
        conn.sendData("Hello TCP Server This SC-1000\n");
    }

    auto&& lte = LTE_LBS::instance();
    lte.Set_vid_pid(ini[KI::lte_vid].toString(), ini[KI::lte_pid].toString());
    lte.Start();

    auto &lte_con = LTE::Lte_Scheduler::instance();
    lte_con.Start();

    ///init
    auto lte_controler  = LTE_ML302::get_instance();
    connect(lte_controler, &LTE_ML302::receive_from_tcp,
            this, [&lte_con](const QJsonObject &json){
        qDebug() << "lambda 收到:" << QJsonDocument(json).toJson(QJsonDocument::Compact);
        LTE_LBS::instance().Send_Reply(LTE::KJ::ControlOK,json[LTE::KJ::hash].toString());
        /// parse && append
        bool        ok   = true;
        lte_con.Append_Task(LTE::Parse::json_to_task(json, ok));
        if (ok == false) {
            qWarning() << "解析错误";
        }
    });
}
