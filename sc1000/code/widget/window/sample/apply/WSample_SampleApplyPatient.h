#ifndef WSAMPLE_SAMPLEAPPLYPATIENT_H_1722311218
#define WSAMPLE_SAMPLEAPPLYPATIENT_H_1722311218

#include <QWidget>

#include "../InfoSample.h"
#include "controller/CONTROLLER_Sample.h"
#include "widget/modules/WModule_SwitchList.h"
namespace Ui {
class SampleApplyPatient;
}

namespace WIDGET::SAMPLE {
class SampleApplyPatient : public QWidget {
    Q_OBJECT
private:
    Ui::SampleApplyPatient *ui;

private:
    //! ！这两的数据是可选择的！
    /// 年龄单位
    /// 性别
    SwitchList *m_ui_ageUnit = nullptr;
    SwitchList *m_ui_sex     = nullptr;

public:
    explicit SampleApplyPatient(QWidget *parent = nullptr);
    ~SampleApplyPatient();

public:
    /// 设置到ui中
    void Set_info(const InfoSample::ApplyInfo &info);

public:
    /// 将当前界面的信息保存到上层的info中
    void Btn_confirm(InfoSample::ApplyInfo &info);

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::SAMPLE

#endif  // WSAMPLE_SAMPLEAPPLYPATIENT_H_1722311218
