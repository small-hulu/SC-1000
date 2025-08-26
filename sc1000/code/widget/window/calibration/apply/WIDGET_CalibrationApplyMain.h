#ifndef WIDGET_CALIBRATIONAPPLYMAIN_H_1720233746
#define WIDGET_CALIBRATIONAPPLYMAIN_H_1720233746

#include <QWidget>

#include "widget/modules/WModule_SwitchList.h"
#include "widget/modules/WModule_TableSelect.h"

namespace Ui {
class CalibrationApplyMain;
}

namespace WIDGET::CALIBRATION {
class CalibrationApplyMain : public QWidget {
    Q_OBJECT
private:
    struct UiInfo {
        QString        projectName;   // 项目名称
        QString        reagentBatch;  // 试剂批号
        QString        calBatch;      // 校准批号
        QList<QString> c0nList;       // c0~cn的信息

        bool Is_valid() const {
            if (c0nList.size() == 0) {
                return false;
            }
            if (projectName.size() == 0) {
                return false;
            }
            if (reagentBatch.size() == 0) {
                return false;
            }
            if (calBatch.size() == 0) {
                return false;
            }
            return true;
        }
    };

private:
    Ui::CalibrationApplyMain *ui                     = nullptr;
    TableSelect              *m_ui_projectTable      = nullptr;
    QTableWidget             *m_ui_reagentBatchTable = nullptr;
    SwitchList               *m_ui_calBatch          = nullptr;

public:
    explicit CalibrationApplyMain(QWidget *parent = nullptr);
    ~CalibrationApplyMain();

public:
    /// 获取ui上的数据集合
    UiInfo Get_uiInfo();

public:  /// 提供上层的操作
    void Btn_save(UiInfo info);

public:
    /**
     * 外部设置管的信息
     */
    void Set_tubeIndex(QString msg);

private:
    /// 刷新试剂编号列表
    /// 在点击项目的列表按钮后触发
    void refresh_reagentBatchNumber();

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::CALIBRATION
#endif  // WIDGET_CALIBRATIONAPPLYMAIN_H
