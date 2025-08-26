#include "WSetting_GeneralSettings.h"

#include "communication/controller/WatchDog.h"
#include "communication/utility/IniConfig.h"
#include "controller/IniSC1000.h"
#include "ui_wsetting_generalsettings.h"

namespace WIDGET::SETTING {
WSetting_GeneralSettings::WSetting_GeneralSettings(QWidget *parent)
    : QWidget(parent), ui(new Ui::WSetting_GeneralSettings) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_StyledBackground);

    {
        // get level tracking config
        auto &ini = CONTROLLER::IniSC1000::instance();

        if (ini[CONTROLLER::KI::setting_level_tracking].toString() == "") {
            // if config is empty
            ini.setValue(CONTROLLER::KI::setting_level_tracking, false);
        }
        // set level tracking icon
        if (ini[CONTROLLER::KI::setting_level_tracking].toBool()) {
            // if setting is true
            ui->pushButton_level_tracking->setStyleSheet(
                "QPushButton {"
                " background-image: url(:/beijing/emergency.png);"
                "}");
            level_tracking_state = true;
        } else {
            // if setting is false
            ui->pushButton_level_tracking->setStyleSheet(
                "QPushButton {"
                " background-image: url(:/beijing/emergency_none.png);"

                "}");
            level_tracking_state = false;
        }
    }
    {
        // get auto max config
        auto &ini = COM::IniConfig::instance();

        if (ini[COM::KI::enable_reagentBinSpinLoop].toInt()) {
            // if setting is true
            ui->auto_mix->setStyleSheet(
                "QPushButton {"
                " background-image: url(:/beijing/emergency.png);"
                "}");
            auto_mix_state = true;

        } else {
            ui->auto_mix->setStyleSheet(
                "QPushButton {"
                " background-image: url(:/beijing/emergency_none.png);"
                "}");
            auto_mix_state = false;
        }
    }
    {
        // get auto load config
        auto &ini = COM::IniConfig::instance();

        if (ini[COM::KI::enable_loopLoadMaterial].toInt()) {
            // if setting is true
            ui->auto_load->setStyleSheet(
                "QPushButton {"
                " background-image: url(:/beijing/emergency.png);"
                "}");
            auto_load_state = true;

        } else {
            ui->auto_load->setStyleSheet(
                "QPushButton {"
                " background-image: url(:/beijing/emergency_none.png);"
                "}");
            auto_load_state = false;
        }
    }
    {
        // get auto fill bottle
        auto &ini = COM::IniConfig::instance();

        if (ini[COM::KI::enable_fillBottle].toInt()) {
            // if setting is true
            ui->auto_fill_bottle->setStyleSheet(
                "QPushButton {"
                " background-image: url(:/beijing/emergency.png);"
                "}");
            auto_fill_bottle_state = true;

        } else {
            ui->auto_fill_bottle->setStyleSheet(
                "QPushButton {"
                " background-image: url(:/beijing/emergency_none.png);"
                "}");
            auto_fill_bottle_state = false;
        }
    }
}

WSetting_GeneralSettings::~WSetting_GeneralSettings() {
    delete ui;
}

void WSetting_GeneralSettings::on_pushButton_level_tracking_clicked() {
    level_tracking_state = !level_tracking_state;

    if (level_tracking_state) {
        ui->pushButton_level_tracking->setStyleSheet(
            "QPushButton {"
            " background-image: url(:/beijing/emergency.png);"

            "}");
    } else {
        ui->pushButton_level_tracking->setStyleSheet(
            "QPushButton {"
            " background-image: url(:/beijing/emergency_none.png);"
            "}");
    }

    // update level tracking config
    auto &ini = CONTROLLER::IniSC1000::instance();
    // set config
    ini.setValue(CONTROLLER::KI::setting_level_tracking, level_tracking_state);
}

void WIDGET::SETTING::WSetting_GeneralSettings::on_auto_mix_clicked() {
    auto_mix_state = !auto_mix_state;
    if (auto_mix_state) {
        ui->auto_mix->setStyleSheet(
            "QPushButton {"
            " background-image: url(:/beijing/emergency.png);"

            "}");
    } else {
        ui->auto_mix->setStyleSheet(
            "QPushButton {"
            " background-image: url(:/beijing/emergency_none.png);"
            "}");
    }
    auto &ini = COM::IniConfig::instance();
    ini.setValue(COM::KI::enable_reagentBinSpinLoop, (int)auto_mix_state);
}

void WIDGET::SETTING::WSetting_GeneralSettings::on_auto_load_clicked() {
    auto_load_state = !auto_load_state;
    if (auto_load_state) {
        ui->auto_load->setStyleSheet(
            "QPushButton {"
            " background-image: url(:/beijing/emergency.png);"

            "}");
    } else {
        ui->auto_load->setStyleSheet(
            "QPushButton {"
            " background-image: url(:/beijing/emergency_none.png);"
            "}");
    }
    auto &ini = COM::IniConfig::instance();
    ini.setValue(COM::KI::enable_loopLoadMaterial, (int)auto_load_state);
}

void WIDGET::SETTING::WSetting_GeneralSettings::on_auto_fill_bottle_clicked() {
    auto_fill_bottle_state = !auto_fill_bottle_state;
    if (auto_fill_bottle_state) {
        ui->auto_fill_bottle->setStyleSheet(
            "QPushButton {"
            " background-image: url(:/beijing/emergency.png);"

            "}");
    } else {
        ui->auto_fill_bottle->setStyleSheet(
            "QPushButton {"
            " background-image: url(:/beijing/emergency_none.png);"
            "}");
    }
    auto               &ini = COM::IniConfig::instance();
    COM::WatchDog_Guard guard;
    ini.setValue(COM::KI::enable_fillBottle, (int)auto_fill_bottle_state);
}

}  // namespace WIDGET::SETTING
