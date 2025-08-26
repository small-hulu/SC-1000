#include "WSetting_HospitalConfig.h"

#include <QScroller>

#include "WSetting_DEditDoctor.h"
#include "WSetting_HospitalDelete.h"
#include "controller/CONTROLLER_Config.h"
#include "ui_WSetting_HospitalConfig.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::SETTING {

namespace {
QList<QString> doctor_to_strList(const CONTROLLER::Config::Hospital::Doctor &doctor) {
    QList<QString> msg;
    msg << doctor.name;
    msg << (doctor.canInspect ? "是" : "否");
    msg << (doctor.canTest ? "是" : "否");
    msg << (doctor.canAudit ? "是" : "否");
    return msg;
}
}  // namespace

void HospitalConfig::init_ui() {
    QList<QString> title;
    title << QString("科室名");
    title << QString("删除");
    auto departTable = ui->table_depart;
    UTILS::init_table_extend(departTable);
    UTILS::init_table_titles(departTable, title);

    title.clear();
    title << QString("医生名");
    title << QString("可送检");
    title << QString("可测试");
    title << QString("可审核");
    title << QString("删除");
    auto doctorTable = ui->table_doctor;
    UTILS::init_table_extend(doctorTable);
    UTILS::init_table_titles(doctorTable, title);
}

HospitalConfig::HospitalConfig(QWidget *parent) : QWidget(parent), ui(new Ui::HospitalConfig{}) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    init_ui();

    /// ui-message
    {
        CONTROLLER::Config::Hospital hospital;
        ui->edit_hospitalName->setText(hospital.hospitalName);
        ui->edit_departmentName->setText(hospital.departmentName);

        auto departMsgList = hospital.departmentList;
        for (auto s : departMsgList) {
            append_departTableLine({s});
        }

        auto doctorMsgList = hospital.doctorList;
        for (auto doctor : doctorMsgList) {
            QList<QString> msg = doctor_to_strList(doctor);
            append_doctorTableLine(msg);
        }
    }

    /// 按钮组
    {
        m_ui_btnGroup = ButtonBottomGroup::Get();
        m_ui_btnGroup->show();
        m_ui_btnGroup->Set_refreshFunc({});
        QList<QString> titles;
        titles << QString("保存");
        QList<std::function<void()>> taskList;
        taskList << std::bind(&HospitalConfig::save_total, this);
        m_ui_btnGroup->Set_BtnList(titles, taskList);
    }

    /// connect
    {
        connect(ui->btn_addDepart, &QPushButton::clicked, this, &HospitalConfig::append_depart);
        connect(ui->btn_addDoctor, &QPushButton::clicked, this, &HospitalConfig::append_doctor);
    }
}

HospitalConfig::~HospitalConfig() {
    m_ui_btnGroup->hide();
    delete ui;
}

void HospitalConfig::save_total() {
    CONTROLLER::Config::Hospital hospital;
    hospital.hospitalName   = ui->edit_hospitalName->text();
    hospital.departmentName = ui->edit_departmentName->text();
    hospital.Save();
    DIALOG::Create_DInfoDialog("保存完成");
}

void HospitalConfig::append_depart() {
    auto dialog = DIALOG::Create_DInputLine();
    connect(dialog, &DIALOG::DInputLine::signal_yes, this, [dialog, this]() {
        auto                         msg = dialog->Get_messge();
        CONTROLLER::Config::Hospital hospital;
        hospital.Append_department(msg);
        hospital.Save();
        append_departTableLine({msg});
    });
}

void HospitalConfig::append_doctor() {
    auto dialog = new DEditDoctor(Background::Get());
    connect(dialog, &DEditDoctor::signal_yes, this, [dialog, this]() {
        auto                                 info = dialog->Get_info();
        CONTROLLER::Config::Hospital::Doctor doctor;
        doctor.name       = info.name;
        doctor.canTest    = info.canTest;
        doctor.canAudit   = info.canAudit;
        doctor.canInspect = info.canInspect;
        CONTROLLER::Config::Hospital hospital;
        hospital.Append_doctor(doctor);
        hospital.Save();

        QList<QString> msg = doctor_to_strList(doctor);
        append_doctorTableLine(msg);
    });
    dialog->show();
}

void HospitalConfig::append_departTableLine(const QList<QString> &line) {
    auto table = ui->table_depart;
    assert(line.size() == table->columnCount() - 1);

    const int curRowCnt = table->rowCount();
    table->insertRow(curRowCnt);
    table->setRowHeight(curRowCnt, Line_Height);
    for (int i = 0; i < line.size(); i += 1) {
        auto *item = new QTableWidgetItem(line[i]);
        item->setTextAlignment(Qt::AlignCenter);
        table->setItem(curRowCnt, i, item);
    }

    auto widget = new HospitalDelete{};
    widget->Set_message(line);
    connect(widget, &HospitalDelete::signal_clicked, this,
            [this, widget]() { remove_depart(widget->Get_message()); });
    table->setCellWidget(curRowCnt, line.size(), widget);
}

void HospitalConfig::append_doctorTableLine(const QList<QString> &line) {
    auto table = ui->table_doctor;
    assert(line.size() == table->columnCount() - 1);

    const int curRowCnt = table->rowCount();
    table->insertRow(curRowCnt);
    table->setRowHeight(curRowCnt, Line_Height);
    for (int i = 0; i < line.size(); i += 1) {
        auto *item = new QTableWidgetItem(line[i]);
        item->setTextAlignment(Qt::AlignCenter);
        table->setItem(curRowCnt, i, item);
    }

    auto widget = new HospitalDelete{};
    widget->Set_message(line);
    connect(widget, &HospitalDelete::signal_clicked, this,
            [this, widget]() { remove_doctor(widget->Get_message()); });
    table->setCellWidget(curRowCnt, line.size(), widget);
}

void HospitalConfig::remove_depart(const QList<QString> &line) {
    auto table = ui->table_depart;
    for (int i = 0; i < table->rowCount(); i += 1) {
        QList<QString> msg;
        for (int j = 0; j < table->columnCount() - 1; j += 1) {
            auto item = table->item(i, j);
            if (item) {
                msg << item->text();
            }
        }
        if (line == msg) {
            table->removeRow(i);
            CONTROLLER::Config::Hospital hospital;
            hospital.Remove_department(line[0]);
            hospital.Save();
            break;
        }
    }
}

void HospitalConfig::remove_doctor(const QList<QString> &line) {
    auto table = ui->table_doctor;
    for (int i = 0; i < table->rowCount(); i += 1) {
        QList<QString> msg;
        for (int j = 0; j < table->columnCount() - 1; j += 1) {
            auto item = table->item(i, j);
            if (item) {
                msg << item->text();
            }
        }
        if (line == msg) {
            table->removeRow(i);
            CONTROLLER::Config::Hospital hospital;
            hospital.Remove_doctorByName(line[0]);
            hospital.Save();
            break;
        }
    }
}

void HospitalConfig::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}

}  // namespace WIDGET::SETTING
