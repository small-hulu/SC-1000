#include "WIDGET_ReagentReagent.h"

#include <QDebug>

#include "controller/CONTROLLER_Reagent.h"
#include "controller/device/CDevice_DReagentBin.h"
#include "entity/ENTITY_EReagent.h"
#include "reagent/WReagent_DReagentParams.h"
#include "reagent/WReagent_ReagentTable.h"
#include "ui_WIDGET_ReagentReagent.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::REAGENT {

ReagentReagent::ReagentReagent(QWidget* parent) : QWidget(parent), ui(new Ui::ReagentReagent) {
    ui->setupUi(this);

    /// ui=init
    {
        m_ui_btnGroup = ButtonBottomGroup::Get();
        m_ui_btnGroup->show();
        m_reagentBin = new ReagentBinShip(ui->widget_up_left);
    }

    /// init-exe
    {
        m_ui_btnGroup->Set_refreshFunc(std::bind(&ReagentReagent::refresh_bottomBtnList, this));
        show_reagentTable();
        refresh_bottomBtnList();
    }
}

ReagentReagent::~ReagentReagent() {
    m_ui_btnGroup->hide();
    delete ui;
}

void ReagentReagent::refresh_bottomBtnList() {
    QList<QString>                   names;
    QList<std::function<void(void)>> taskList;

    if (false) {
    } else if (nullptr != m_ui_reagentTable) {
        names << QString("装载");
        taskList << [this]() {
            const int idx = ReagentBinCache::GroupIdx_Cache;
            m_ui_reagentTable->Entity_install(idx);
        };
        names << QString("卸载");
        taskList << std::bind(&ReagentTable::Entity_uninstall, m_ui_reagentTable);
        names << QString("修改");
        taskList << std::bind(&ReagentTable::Entity_modify, m_ui_reagentTable);
        names << QString("详情");
        taskList << std::bind(&ReagentTable::Entity_details, m_ui_reagentTable);

        static const QVector<QString> btnNames   = {"启动混匀", "停止混匀"};
        auto                          switch_fun = [this, names]() {
            const auto open =
                COM::IniConfig::instance()[COM::KI::enable_reagentBinSpinLoop].toInt();
            CONTROLLER::CDevice::DReagentBin::Switch_spinLoop(!open);
            auto newNames = names;
            newNames << btnNames[open];
            m_ui_btnGroup->Set_BtnTextList(newNames);
        };

        const auto open = COM::IniConfig::instance()[COM::KI::enable_reagentBinSpinLoop].toInt();
        names << btnNames[open];
        taskList << switch_fun;
    }

    m_ui_btnGroup->Set_BtnList(names, taskList);
}

void ReagentReagent::show_reagentTable() {
    assert(m_ui_reagentTable == nullptr);
    m_ui_reagentTable = new ReagentTable(ui->widget_up_right);
    m_ui_reagentTable->setAttribute(Qt::WA_DeleteOnClose);
    m_ui_reagentTable->show();
}
}  // namespace WIDGET::REAGENT
