#include "WSetting_PaperPrintParams.h"

#include <QPrinter>
#include <QPrinterInfo>

#include "controller/CONTROLLER_Config.h"
#include "global/GLOBAL_Setting.h"
#include "ui_WSetting_PaperPrintParams.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace ST = Setting;
namespace WIDGET::SETTING {
PaperPrintParams::PaperPrintParams(QWidget *parent)
    : QWidget(parent), ui(new Ui::PaperPrintParams) {
    ui->setupUi(this);

    ui->combo_print_preview->addItem("打开");
    ui->combo_print_preview->addItem("关闭");

    /// ui-init
    {
        UTILS::init_combo_reset(ui->combo_PrinterName);
        UTILS::init_combo_reset(ui->combo_Prin_mode);

        auto combo = ui->combo_PrinterName;
        combo->addItem("");
        for (auto info : QPrinterInfo::availablePrinters()) {
            combo->addItem(info.printerName());
        }
        auto Prin_mode = ui->combo_Prin_mode;
        Prin_mode->addItem(ST::Sample_Report);
        Prin_mode->addItem(ST::Inspection_Report);
        Prin_mode->addItem(ST::Thermal_Report);

        CONTROLLER::Config::Printer printer;
        ENTITY::PrinterSettings     settings = printer.LoadFromJson();
        ui->combo_PrinterName->setCurrentText(printer.printer.printerName());
        ui->combo_Prin_mode->setCurrentText(settings.printmode);
        ui->combo_print_preview->setCurrentText(settings.preview_mode);
        ui->lab_resolution->setText(settings.res);
        onComboBoxTextChanged(settings.printmode);
        connect(ui->combo_Prin_mode,
                QOverload<const QString &>::of(&QComboBox::currentIndexChanged), this,
                &PaperPrintParams::onComboBoxTextChanged);
    }

    {
        m_ui_btnGroup = ButtonBottomGroup::Get();  // 按钮组
        m_ui_btnGroup->show();
        m_ui_btnGroup->Set_refreshFunc({});
        QList<QString> titles;
        titles << QString("保存");
        QList<std::function<void()>> taskList;
        taskList << std::bind(&PaperPrintParams::save_total, this);
        m_ui_btnGroup->Set_BtnList(titles, taskList);
    }
}

void PaperPrintParams::onComboBoxTextChanged(const QString &text) {
    QTimer::singleShot(0, this, [this, text]() { loadWidgetSettingPreview(text); });
}
void PaperPrintParams::loadWidgetSettingPreview(const QString &text) {
    QJsonObject json;
    using namespace PRINTER;
    printer    = new PrinterSample();
    double res = ui->lab_resolution->text().toDouble();
    widget     = printer->Create_widget_setting_preview(text, res, json, ui->widget);

    QSize targetSize = ui->widget->size();
    widget->resize(targetSize);
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    widget->show();
}
PaperPrintParams::~PaperPrintParams() {
    m_ui_btnGroup->hide();
    delete ui;
}

bool PaperPrintParams::get_preview_sign() {
    if (ui->combo_print_preview->currentText() == "打开") {
        return true;
    } else {
        return false;
    }
}

void PaperPrintParams::save_total() {
    CONTROLLER::Config::Printer printer;
    printer.setPrint_mode(ui->combo_Prin_mode->currentText());
    printer.setPreview_mode(ui->combo_print_preview->currentText());
    printer.setRes(ui->lab_resolution->text());
    printer.printer.setPrinterName(ui->combo_PrinterName->currentText());
    printer.Save();
    DIALOG::Create_DInfoDialog("保存完成");
}

void PaperPrintParams::on_pushButton_resolution_decrease_clicked() {
    QString current_res = ui->lab_resolution->text();
    double  temp_res    = current_res.toDouble() - 0.1;
    ui->lab_resolution->setText(QString::number(temp_res));
    // onComboBoxTextChanged(ui->combo_print_preview->currentText());
}

void PaperPrintParams::on_pushButton_resolution_add_clicked() {
    QString current_res = ui->lab_resolution->text();
    double  temp_res    = current_res.toDouble() + 0.1;
    ui->lab_resolution->setText(QString::number(temp_res));
    // onComboBoxTextChanged(ui->combo_print_preview->currentText());
}

void PaperPrintParams::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::SETTING
