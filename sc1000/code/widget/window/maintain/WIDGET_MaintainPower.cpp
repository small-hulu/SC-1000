#include "WIDGET_MaintainPower.h"

#include <QLabel>
#include <QPushButton>
#include <QScroller>
#include <QtConcurrent>

#include "communication/fixture/WMainFixture.h"
#include "communication/singlestep/WSingleStepMain.h"
#include "controller/device/CDevice.hpp"
#include "module/WSetting_MaintainExec.h"
#include "ui_WIDGET_MaintainPower.h"
#include "widget/Background.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"
#include "utility/UTILITY_Sound.h"
namespace WIDGET {
namespace MAINTAIN {

void MaintainPower::init_ui() {
    auto init_table = [&](QTableWidget *table) -> void {
        UTILS::init_table_clear(table);

        QScroller::grabGesture(table, QScroller::LeftMouseButtonGesture);
        /// table header
        {
            auto ver = table->verticalHeader();
            auto hor = table->horizontalHeader();

            /// 左侧列属性 false
            /// 顶部行属性 true
            /// todo
            ver->setVisible(false);
            hor->setVisible(true);

            /// 横纵属性不可获取
            ver->setEnabled(false);
            hor->setEnabled(false);

            /// 属性不可编辑
            ver->setSectionsClickable(false);
            hor->setSectionsClickable(false);

            /// 行列表头字体居中
            ver->setDefaultAlignment(Qt::AlignCenter);
            hor->setDefaultAlignment(Qt::AlignCenter);

            /// 设定行属性的高和列属性的宽的最小值 width||height
            hor->setMinimumHeight(70);

            /// 自动拉伸满整个框 （此处全部手动）
            hor->setSectionResizeMode(QHeaderView::Stretch);

        }  // header

        /// 横纵滚动条关闭
        table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        /// 滚动模式
        table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        // table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

        /// 设置不能编辑
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        /// 无焦点
        table->setFocusPolicy(Qt::NoFocus);
        /// 选择模式
        table->setSelectionMode(QAbstractItemView::NoSelection);
        /// 选择行
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        /// 不显示网格
        table->setShowGrid(false);
        /// 设置隔行变色
        table->setAlternatingRowColors(true);
    };

    QList<QString> title;
    title << QString("维护项");
    title << QString("操作");
    title << QString("维护时间");
    init_table(table);
    UTILS::init_table_titles(table, title);
}

MaintainPower::MaintainPower(QWidget *parent) : QWidget(parent), ui(new Ui::MaintainPower) {
    ui->setupUi(this);

    table = ui->tableWidget;
    init_ui();

    {
        struct Node {
            QString               txt;
            std::function<void()> task;
        };
        QList<Node> nodeList;
        nodeList << Node{"开机维护", std::bind(&MaintainPower::task_powerOn, this)};
        nodeList << Node{"关机", std::bind(&MaintainPower::task_powerOff, this)};
        for (auto &&node : nodeList) {
            append_fixtureWork(node.txt, node.task);
        }
    }
}

MaintainPower::~MaintainPower() {
    delete ui;
}

void MaintainPower::append_fixtureWork(const QString &txt, std::function<void()> task) {
    const int curRowCnt = table->rowCount();
    table->insertRow(curRowCnt);
    table->setRowHeight(curRowCnt, UTILS::table_auto_rowHeight);
    auto *item = new QTableWidgetItem(txt);
    item->setTextAlignment(Qt::AlignCenter);
    table->setItem(curRowCnt, 0, item);

    auto *btn = new MaintainExec(table);
    table->setCellWidget(curRowCnt, 1, btn);
    connect(btn, &MaintainExec::signal_clicked, task);
}

void MaintainPower::task_powerOn() {
    auto dialog = DIALOG::Create_DProgressIndicator();
    dialog->Set_words(QString("开机维护中"));

    QFuture<void>         future  = QtConcurrent::run([]() {
        CONTROLLER::CDevice::DMaintain::Homing_allDevice();
        CONTROLLER::CDevice::DMaintain::Maintain_TubleDisCard();
        CONTROLLER::CDevice::DMaintain::Homing_pump();
        CONTROLLER::CDevice::DMaintain::Maintain_WashFill();
        CONTROLLER::CDevice::DMaintain::Maintain_AbleASubstrate();
        CONTROLLER::CDevice::DMaintain::Maintain_AbleASubstrate();
        CONTROLLER::CDevice::DMaintain::Maintain_AbleBSubstrate();
        CONTROLLER::CDevice::DMaintain::Maintain_AbleBSubstrate();
        CONTROLLER::CDevice::DMaintain::Maintain_AbleMagneticSeparationWash();
    });
    QFutureWatcher<void> *watcher = new QFutureWatcher<void>;
    watcher->setFuture(future);
    QObject::connect(watcher, &QFutureWatcher<void>::finished, this, [=] {
        DIALOG::Create_DInfoDialog("处理完成");
        UTILITY::Sound::Speech_Word("开机维护完成");
        dialog->close();
        watcher->deleteLater();
    });
}

void MaintainPower::task_powerOff() {
    auto dialog = DIALOG::Create_DProgressIndicator();
    dialog->Set_words(QString("关机维护中"));

    QFuture<void>         future  = QtConcurrent::run([]() {
        CONTROLLER::CDevice::DMaintain::Homing_allDevice();
        CONTROLLER::CDevice::DMaintain::Maintain_TubleDisCard();
    });
    QFutureWatcher<void> *watcher = new QFutureWatcher<void>;
    watcher->setFuture(future);
    QObject::connect(watcher, &QFutureWatcher<void>::finished, this, [=] {
        DIALOG::Create_DInfoDialog("处理完成");
        dialog->close();
        watcher->deleteLater();
    });
}
}  // namespace MAINTAIN
}  // namespace WIDGET
