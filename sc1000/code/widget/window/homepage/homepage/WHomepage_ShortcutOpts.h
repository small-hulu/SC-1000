#ifndef WHOMEPAGE_SHORTCUTOPTS_H_1715398124
#define WHOMEPAGE_SHORTCUTOPTS_H_1715398124

#include <QPushButton>
#include <QWidget>

namespace Ui {
class ShortcutOpts;
}

namespace WIDGET::HOMEPAGE {
class ShortcutOpts : public QWidget {
    Q_OBJECT
private:
    Ui::ShortcutOpts *ui = nullptr;

public:
    explicit ShortcutOpts(QWidget *parent = nullptr);
    ~ShortcutOpts();

private:
    void btnfunc_BootMaintain();
    void btnfunc_SampleConsumable();
    void btnfunc_History();
    void btnfunc_CloseMaintain();

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::HOMEPAGE

#endif  // WHOMEPAGE_SHORTCUTOPTS_H
