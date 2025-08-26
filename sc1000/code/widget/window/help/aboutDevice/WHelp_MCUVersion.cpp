#include "WHelp_MCUVersion.h"

#include "communication/COM_Device.hpp"
#include "controller/IniSC1000.h"
#include "ui_WHelp_MCUVersion.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::HELP {
MCUVersion::MCUVersion(QWidget* parent) : QWidget(parent), ui(new Ui::MCUVersion) {
    ui->setupUi(this);

    QList<QString> titles;
    titles << QString("部件");
    titles << QString("信息");

    auto get_version = [](auto&& device) {
        QList<QString> line;
        line << device.objectName() + "固件";
        line << device.Get_mcuVersion();
        return line;
    };
    QList<QList<QString>> content;
    content << (QList<QString>()
                << QString("设备编号")
                << (CONTROLLER::IniSC1000::instance()[CONTROLLER::KI::machine_number].toString()));
    content << (QList<QString>() << QString("软件版本") << ("V1.0.0.5 2025-08-26"));
    content << (QList<QString>() << QString("控制组件") << ("V" + qApp->applicationVersion()));
    content << get_version(COM::DGripper::instance());
    content << get_version(COM::DMixing::instance());
    content << get_version(COM::DReagentBin::instance());
    content << get_version(COM::DPhotoelectric::instance());
    content << get_version(COM::DPipetteNeedle::instance());
    content << get_version(COM::DMagneticSeparation::instance());
    content << get_version(COM::DThermostat::instance());

    auto table = ui->table;
    UTILS::init_table_extend(table);
    UTILS::init_table_titles(table, titles);
    UTILS::init_table_fix_content(table, content);
    for (int i = 0; i < table->rowCount(); i += 1) {
        table->setRowHeight(i, Line_Height);
    }
}

MCUVersion::~MCUVersion() {
    delete ui;
}

void MCUVersion::showEvent(QShowEvent* event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::HELP
