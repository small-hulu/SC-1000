#include "WIDGET_MaintainCommonlyUsed.h"

#include <QPushButton>
#include <QScroller>
#include <QtConcurrent>

#include "communication/fixture/WMainFixture.h"
#include "communication/singlestep/WSingleStepMain.h"
#include "controller/IniSC1000.h"
#include "controller/device/CDevice_DMaintain.h"
#include "module/WSetting_MaintainExec.h"
#include "ui_WIDGET_MaintainCommonlyUsed.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET {
namespace MAINTAIN {

void CommonlyUsed::init_ui() {
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

CommonlyUsed::CommonlyUsed(QWidget *parent) : QWidget(parent), ui(new Ui::CommonlyUsed{}) {
    ui->setupUi(this);

    table = ui->tableWidget;
    init_ui();

    {
        struct Node {
            QString               txt;
            std::function<void()> task;
            QString               key;
        };
        QList<Node> nodeList;
        using CONMaintain = CONTROLLER::CDevice::DMaintain;
        namespace KI      = CONTROLLER::KI;
        nodeList << Node{"全部件归位", &CONMaintain::Homing_allDevice,
                         KI::maintain_homingAllDevice};
        nodeList << Node{"一键清洗", &CONMaintain::Maintain_WashOneClick,
                         KI::maintain_washoneclick};
        nodeList << Node{"泵归位", &CONMaintain::Homing_pump, KI::maintain_homingAllPump};
        nodeList << Node{"清管", &CONMaintain::Maintain_TubleDisCard, KI::maintain_discardTube};
        nodeList << Node{"底物A灌注", &CONMaintain::Maintain_AbleASubstrate,
                         KI::maintain_ableASubstrate};
        nodeList << Node{"底物B灌注", &CONMaintain::Maintain_AbleBSubstrate,
                         KI::maintain_ableBSubstrate};
        nodeList << Node{"磁分离清洗液灌注", &CONMaintain::Maintain_AbleMagneticSeparationWash,
                         KI::maintain_ableMagneticSeparationWash};
        nodeList << Node{"排空清洗液", &CONMaintain::Maintain_WashDrain, KI::maintain_washDrain};
        nodeList << Node{"加注清洗液", &CONMaintain::Maintain_WashFill, KI::maintain_washFill};
        nodeList << Node{"强化清洗液", &CONMaintain::Maintain_WashIntensive,
                         KI::maintain_washIntensive};
        for (auto &&node : nodeList) {
            append_maintainWork(node.txt, node.task, node.key);
        }
    }

    {
        struct Node {
            QString               txt;
            std::function<void()> task;
        };
        QList<Node> nodeList;
        nodeList << Node{"单步调试", []() {
                             auto widget = new COM::SINGLEWSTEP::SingleStepMain();
                             widget->show();
                         }};
        for (auto &&node : nodeList) {
            append_fixtureWork(node.txt, node.task);
        }
    }
}

CommonlyUsed::~CommonlyUsed() {
    delete ui;
}

void CommonlyUsed::append_maintainWork(const QString &txt, std::function<void()> task,
                                       const QString &key) {
    const int curRowCnt = table->rowCount();
    table->insertRow(curRowCnt);
    table->setRowHeight(curRowCnt, UTILS::table_auto_rowHeight);

    /// 任务
    auto *item = new QTableWidgetItem(txt);
    item->setTextAlignment(Qt::AlignCenter);
    table->setItem(curRowCnt, 0, item);

    /// 时间
    namespace KI = CONTROLLER::KI;
    auto &ini    = CONTROLLER::IniSC1000::instance();
    item         = new QTableWidgetItem(ini[key].toString());
    item->setTextAlignment(Qt::AlignCenter);
    table->setItem(curRowCnt, 2, item);

    /// btn
    /// 可以执行这个task，也可以写一个lambda方便出个弹窗
    auto *btn = new MaintainExec(table);
    table->setCellWidget(curRowCnt, 1, btn);
    connect(btn, &MaintainExec::signal_clicked, [=]() {
        auto dialog = DIALOG::Create_DProgressIndicator();
        dialog->Set_words(txt + QString("中"));

        QFuture<void>         future  = QtConcurrent::run(task);
        QFutureWatcher<void> *watcher = new QFutureWatcher<void>;
        watcher->setFuture(future);
        QObject::connect(watcher, &QFutureWatcher<void>::finished, this, [=] {
            /// 记录维护时间
            const QString curtime = UTILS::time_generate();
            table->item(curRowCnt, 2)->setText(curtime);
            auto &ini = CONTROLLER::IniSC1000::instance();
            ini.setValue(key, curtime);

            DIALOG::Create_DInfoDialog("处理完成");
            dialog->close();
            watcher->deleteLater();
        });
    });
}

void CommonlyUsed::append_fixtureWork(const QString &txt, std::function<void()> task) {
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
}  // namespace MAINTAIN
}  // namespace WIDGET
