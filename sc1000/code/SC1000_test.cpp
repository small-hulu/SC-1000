#include "SC1000_test.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>

#include "controller/CONTROLLER_Project.h"
#include "lis/Lis_SC1000.h"
#include "lte/lte_sc.h"
#include "widget/modules/WModule_ReagentBinSample.h"

namespace TEST {

void test_controller_project() {
    auto info    = CONTROLLER::Project::ExtraInfo{};
    info.project = "OC1";
    auto json    = CONTROLLER::Project::Merge_FlowAndLocation(info, {}, {});
    qDebug().noquote().nospace() << QString::fromUtf8(
        QJsonDocument(json).toJson(QJsonDocument::Indented));
}

void test_lte_sc() {
    for (int i : {250, 216, 215, 81, 80, 79}) {
        auto item = CONTROLLER::ExpSample::instance().Select_byId_fromDB(i);
        /// 测试连发送后端可能会处理不来，但实际实验中数据之间是有间隔的
        LTE_SC::Send_SC_Data(item);
    }
}

void test_lis() {
    LIS::Lis_SC1000::Send_singleMessage("中文测试");

    for (int id : {382, 383, 390, 391}) {
        auto x = CONTROLLER::ExpSample::instance().Select_byId_fromDB(id);
        LIS::Lis_SC1000::Send_ExpData(x);
    }
}

void btn_trigger() {
    std::function<void(void)> task = [&]() { test_lis(); };

    auto btn = new QPushButton("单机触发");
    QObject::connect(btn, &QPushButton::clicked, btn, task);
    btn->show();
}

void test() {
#ifdef QT_DEBUG
    //    auto w = new WIDGET::ReagentBinSample();
    //    w->show();

    //    btn_trigger();
#endif
}

}  // namespace TEST
