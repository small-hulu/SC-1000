#ifndef WSAMPLE_SAMPLEAPPLYMAIN_H_1721028656
#define WSAMPLE_SAMPLEAPPLYMAIN_H_1721028656

#include <QWidget>

#include "../InfoSample.h"
#include "controller/CONTROLLER_Sample.h"
#include "widget/modules/WModule_SwitchList.h"
#include "widget/modules/WModule_TableSelect.h"
#include "widget/modules/WModule_TableSelectSingle.h"

namespace Ui {
class SampleApplyMain;
}

namespace WIDGET::SAMPLE {
class SampleApplyMain : public QWidget {
    Q_OBJECT
public:
    using entity_item = CONTROLLER::Sample::entity_item;
    using entity_attr = entity_item::element_type::attr;

private:
    static constexpr int Up_Row_Count   = 4;
    static constexpr int Up_Col_Count   = 6;
    static constexpr int Down_Row_Count = 2;
    static constexpr int Down_Col_Count = 6;

private:
    int all       = 0;  // 所有项的总数
    int each      = 0;  // 每页显示的项数
    int all_down  = 0;  // 所有项的总数
    int each_down = 0;  // 每页显示的项数

public:
    struct UiInfo {
        QString        location;
        QString        number;
        QString        detectType;
        QString        sampleTube;
        QString        barCode;
        QList<QString> projectList;
    };

private:
    Ui::SampleApplyMain *ui = nullptr;

private:
    TableSelect       *m_ui_tableSelect         = nullptr;
    TableSelectSingle *m_ui_tableSelect_Merge   = nullptr;
    SwitchList        *m_ui_switchList          = nullptr;
    SwitchList        *m_ui_switchList_tubetype = nullptr;

public:
    explicit SampleApplyMain(QWidget *parent = nullptr);
    ~SampleApplyMain();

public:
    UiInfo Get_UiInfo();
    void   Get_UiInfo(InfoSample::ApplyInfo &info);
    void   check_page(int page);
    void   check_page_down(int page);

public:
    entity_item Btn_save(const InfoSample::ApplyInfo &info);
    entity_item Btn_save_Merage(const InfoSample::ApplyInfo &info);
    void        Multiple_Apply();

private:
    QString priority       = "常规";
    bool    emergency_sign = false;

public slots:
    void on_pushButton_emergency_clicked();

private:
    void show_range(int start, int end);
    void show_range_down(int start, int end);
    void Build_By_ItemCount(int all, int each);
    void Build_By_ItemCount_down(int all, int each);

protected:
    void showEvent(QShowEvent *event) override;

signals:
    /**
     * 闭区间范围
     * [1, 5] 表示第 {1，2，3，4，5} 项
     * <= 0 表示无效
     */
    void signal_curRange(int start, int end);
    void signal_curRange_down(int start, int end);
};
}  // namespace WIDGET::SAMPLE

#endif  // WSAMPLE_SAMPLEAPPLYMAIN_H_1721028656
