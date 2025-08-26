#include "WIDGET_SettingDeviceConfig.h"

#include "communication/fixture/FIXTURE.hpp"
#include "deviceConfig/WSetting_DeviceFix.h"
#include "modules/WSetting_SettingOption.h"
#include "ui_WIDGET_SettingDeviceConfig.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::SETTING {

DeviceConfig::DeviceConfig(QWidget* parent) : QWidget(parent), ui(new Ui::DeviceConfig) {
    ui->setupUi(this);

    /// init-ui
    {
        QVector<QString> btnList;
        btnList << "1.项目列表";
        btnList << "2.项目列表";
        btnList << "3.项目列表";
        btnList << "4.项目列表";
        btnList << "5.项目列表";
        btnList << "6.项目列表";
        btnList << "7.项目列表";
        btnList << "8.项目列表";
        btnList << "9.项目列表";
        btnList << "10.项目列表";
        //        auto list = ui->list_left;
        //        build_optionList(list, btnList);

        new COM::WFIXTURE::MainFixture(this);
        //        new DeviceFix(this);
    }
}

DeviceConfig::~DeviceConfig() {
    delete ui;
}

void DeviceConfig::build_optionList(QListWidget* list, QVector<QString> btnList) {
    list->setStyleSheet("QListWidget::item { height: 70px; }");
    for (int i = 0; i < btnList.size(); i += 1) {
        auto item   = new QListWidgetItem();
        auto widget = new SettingOption(btnList[i], list);
        list->addItem(item);
        list->setItemWidget(item, widget);
    }
}

}  // namespace WIDGET::SETTING
