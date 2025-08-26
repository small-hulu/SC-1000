#ifndef WIDGET_BYPATIENT_H_1724219106
#define WIDGET_BYPATIENT_H_1724219106

#include <QCheckBox>
#include <QJsonDocument>
#include <QTableWidget>
#include <QWidget>

#include "WIDGET_PrintViewSample.h"
#include "WSample_DSampleHistoryFilter.h"
#include "WSample_DSampleHistoryModify.h"
#include "WSample_SampleHistoryDetail.h"
#include "communication/controller/DispatchController.h"
#include "controller/CONTROLLER_ExpSample.h"
#include "controller/CONTROLLER_Project.h"
#include "controller/CONTROLLER_Reagent.h"
#include "controller/CONTROLLER_Sample.h"
#include "global/GLOBAL_KeyProject.h"
#include "global/GLOBAL_Setting.h"
#include "printer/PRINTER.hpp"
#include "widget/modules/WModule_ButtonBottomGroup.h"
#include "widget/window/setting/printManage/WSetting_PaperPrintParams.h"
#include "widget/window/setting/lisManage/Lis_net_con.h"
#include "lte/lte_lbs.h"
namespace Ui {
class HistorySample;
}

namespace WIDGET::HISTORY {
class HistorySample : public QWidget {
    Q_OBJECT
public:
    using entity_item = CONTROLLER::ExpSample::entity_item;
    using entity_attr = entity_item::element_type::attr;

public:
    constexpr static const int Sync_IntervalTime = 5000;

public:
    struct LineEntityNode {
        entity_item entity;
        QCheckBox  *checkBox = nullptr;
    };

public:
    static QList<QString> Get_lineStr(HistorySample::entity_item entity);

private:
    Ui::HistorySample *ui            = nullptr;
    QTableWidget      *table         = nullptr;
    ButtonBottomGroup *m_ui_btnGroup = nullptr;

    SampleHistoryDetail    *m_ui_historyDetail = nullptr;
    WIDGET_PrintViewSample *print_preview      = nullptr;
    PRINTER::PrinterSample *printer            = nullptr;

private:
    QList<LineEntityNode> m_items;

public:
    explicit HistorySample(QWidget *parent = nullptr);
    ~HistorySample();

private:
    void Clear();
    void Filter_data(const QList<entity_item> &items);
    void Append_tableLine(HistorySample::entity_item entity);

private:
    void fill_tableLine(int idx, HistorySample::entity_item entity);
    void refresh_bottomBtnList();
    void show_HistoryTable();

    void Btn_delete();
    void Btn_showDetail();
    void Btn_retest();
    void Btn_print();
    void Brn_lisSend();
    void Btn_modify();
    void Btn_query();
    void Btn_usbExport();
    void Btn_selectAllOrCancel();

protected:
    void timerEvent(QTimerEvent *event) override;
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::HISTORY

#endif  // WIDGET_BYPATIENT_H_1724219106
