#include "WModule_ReagentBinSample.h"

#include <QButtonGroup>
#include <QSignalMapper>

#include "controller/CONTROLLER_Consumable.h"
#include "controller/CONTROLLER_Sample.h"
#include "controller/device/CDevice.hpp"
#include "database/extend/DB_TableEntity_sample.h"
#include "entity/ENTITY_ESample.h"
#include "global/GLOBAL_DeviceState.h"
#include "global/GLOBAL_TypeTube.h"
#include "ui_WModule_ReagentBinSample.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS_Resize.h"

int ReagentBinCache::GroupIdx_Cache = 1;
namespace WIDGET {

ReagentBinSample::ReagentBinSample(QWidget* parent)
    : QWidget(parent), ui(new Ui::ReagentBinSample) {
    ui->setupUi(this);

    /// init
    {
        using namespace CONTROLLER;
        ui->label_jifaye_num->setText(Consumable::instance().Get_describe_for_Substrat());
        ui->label_feiliao_num->setText(Consumable::instance().Get_describe_for_WasteTank());

        for (int group = 1; group <= Dial_Max; group += 1) {
            m_btnTxtTable << Type::Get_TubeSpaceGroupMarkList(group);
        }

        QList<QPushButton*> btnList;
        btnList << nullptr;
        btnList << nullptr;
        btnList << nullptr;

        btnList[0] = ui->pushButton_E1;
        btnList[1] = ui->pushButton_2;
        btnList[2] = ui->pushButton_1;
        m_btnList << btnList;
        btnList[0] = ui->pushButton_E2;
        btnList[1] = ui->pushButton_4;
        btnList[2] = ui->pushButton_3;
        m_btnList << btnList;
        btnList[0] = ui->pushButton_E3;
        btnList[1] = ui->pushButton_6;
        btnList[2] = ui->pushButton_5;
        m_btnList << btnList;
        btnList[0] = ui->pushButton_E4;
        btnList[1] = ui->pushButton_8;
        btnList[2] = ui->pushButton_7;
        m_btnList << btnList;
        btnList[0] = ui->pushButton_E5;
        btnList[1] = ui->pushButton_10;
        btnList[2] = ui->pushButton_9;
        m_btnList << btnList;
        btnList[0] = ui->pushButton_E6;
        btnList[1] = ui->pushButton_12;
        btnList[2] = ui->pushButton_11;
        m_btnList << btnList;
        btnList[0] = ui->pushButton_E7;
        btnList[1] = ui->pushButton_14;
        btnList[2] = ui->pushButton_13;
        m_btnList << btnList;
        btnList[0] = ui->pushButton_E8;
        btnList[1] = ui->pushButton_16;
        btnList[2] = ui->pushButton_15;
        m_btnList << btnList;
        btnList[0] = ui->pushButton_E9;
        btnList[1] = ui->pushButton_18;
        btnList[2] = ui->pushButton_17;
        m_btnList << btnList;
        btnList[0] = ui->pushButton_E10;
        btnList[1] = ui->pushButton_20;
        btnList[2] = ui->pushButton_19;
        m_btnList << btnList;

        for (auto btnList : m_btnList) {
            for (auto btn : btnList) {
                connect(btn, &QPushButton::clicked, this, &ReagentBinSample::handle_btnClicked);
            }
        }
    }

    /// connect
    {
        connect(ui->btn_rotate, &QPushButton::clicked, this, [this]() {
            if (m_selectedGroupIdx == -1) {
                return;
            }
            ReagentBinCache::GroupIdx_Cache = m_selectedGroupIdx + 1;
            rotate_reagentBin(ReagentBinCache::GroupIdx_Cache);
            Update_dial();
        });

        connect(ui->btn_rotate_left, &QPushButton::clicked, this, [this]() {
            /// [1, 10] -> [0, 10)
            int next                        = ReagentBinCache::GroupIdx_Cache - 1;
            next                            = ((next - 1) + Dial_Max) % Dial_Max;
            /// [0, 10) -> [1, 10]
            ReagentBinCache::GroupIdx_Cache = next + 1;
            rotate_reagentBin(ReagentBinCache::GroupIdx_Cache);
            Update_dial();
        });

        connect(ui->btn_rotate_right, &QPushButton::clicked, this, [this]() {
            /// [1, 10] -> [0, 10)
            int next                        = ReagentBinCache::GroupIdx_Cache - 1;
            next                            = ((next + 1) + Dial_Max) % Dial_Max;
            /// [0, 10) -> [1, 10]
            ReagentBinCache::GroupIdx_Cache = next + 1;
            rotate_reagentBin(ReagentBinCache::GroupIdx_Cache);
            Update_dial();
        });
    }

    Update_dial();
}

ReagentBinSample::~ReagentBinSample() {
    delete ui;
}

void ReagentBinSample::Update_dial() {
    auto updateCellBtn = [this](QPushButton* btn) -> void {
        QString qss      = R"(
width: 38px;
height: 38px;
background: %2;
border-radius: 18px;
border: 4px solid %1;
font-weight: bold;
font-size: 19px;
color: %1;
line-height: 60px;
)";
        int     location = btn->text().toInt();
        int     group    = (location - 1) / 3;
        QString border   = "#A0CAD0";
        if (location == m_selectedCellIdx || group == m_selectedGroupIdx) {
            border = "#02C9DF";
        }
        qss = qss.arg(border);

        QString background = Type::Get_TubeStateInfo(ENTITY::ESample::TubeState::Idle).color.name();
        auto item = CONTROLLER::Sample::instance().Get_isLoadByLocation(QString::number(location));
        if (item) {
            int  state = item->table[DB::TableEntity::sample::state].toInt();
            auto info  = Type::Get_TubeStateInfo((ENTITY::ESample::TubeState)state);
            background = info.color.name();
        }
        qss = qss.arg(background);

        btn->setStyleSheet(qss);
    };

    for (int i = 0; i < m_btnList.size(); i += 1) {
        int group = (i + (ReagentBinCache::GroupIdx_Cache - 1) + Dial_Max) % Dial_Max;
        for (int j = 0; j < m_btnList[i].size(); j += 1) {
            m_btnList[i][j]->setText(m_btnTxtTable[group][j]);
            updateCellBtn(m_btnList[i][j]);
        }
    }
}

void ReagentBinSample::Select_onebtn(QString btnTxt) {
    m_selectedGroupIdx = -1;
    m_selectedCellIdx  = btnTxt.toInt();
    Update_dial();
}

void ReagentBinSample::handle_btnClicked() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button == nullptr) {
        return;
    }

    m_selectedGroupIdx = (button->text().toInt() - 1) / 3;
    m_selectedCellIdx  = -1;

    Update_dial();
}

void ReagentBinSample::rotate_reagentBin(int idx) {
    if (idx < 0 || idx > Dial_Max) {
        DIALOG::Create_DWarningDialog("指定组号错误");
        return;
    }

    auto task = [=]() {
        CONTROLLER::CDevice::DReagentBin::ToFront_forSample(idx);
        m_selectedGroupIdx = -1;
        m_selectedCellIdx  = -1;
        emit signal_curGroup(idx);
    };

    if (GLOBAL::EquipmentRecord::state == GLOBAL::MachineState::Waring) {
        auto dialog = DIALOG::Create_DIsVerify("仪器异常，是否继续操作？");
        connect(dialog, &DIALOG::DIsVerify::signal_yes, this, [task]() { task(); });
    } else {
        task();
    }
}

void ReagentBinSample::showEvent(QShowEvent* event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET
