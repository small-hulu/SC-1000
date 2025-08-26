#ifndef WIDGET_DIALOG_MODIFYLOGINUSER_H_1725594623
#define WIDGET_DIALOG_MODIFYLOGINUSER_H_1725594623

#include <QWidget>

namespace Ui {
class DEditDoctor;
}

namespace WIDGET::SETTING {
class DEditDoctor : public QWidget {
    Q_OBJECT
public:
    struct Info {
        QString name;                /// 姓名
        int     canInspect = false;  /// 可送检
        int     canTest    = false;  /// 可测试
        int     canAudit   = false;  /// 可审核
    };

private:
    Ui::DEditDoctor *ui = nullptr;

public:
    explicit DEditDoctor(QWidget *parent = nullptr);
    ~DEditDoctor();

public:
    Info Get_info();

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void signal_yes();
};
}  // namespace WIDGET::SETTING

#endif  // WIDGET_DIALOG_MODIFYLOGINUSER_H_1725594623
