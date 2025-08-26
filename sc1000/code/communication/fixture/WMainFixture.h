#ifndef WMAINFIXTURE_H_1723266488
#define WMAINFIXTURE_H_1723266488

#include <QWidget>

#include "WOperationPanel.h"
#include "widget/modules/WModule_ButtonBottomGroup.h"

namespace Ui {
class MainFixture;
}

namespace COM::WFIXTURE {

class MainFixture : public QWidget {
    Q_OBJECT
private:
    Ui::MainFixture *ui = nullptr;

private:
    OperationPanel *m_ui_panel = nullptr;

private:
    WIDGET::ButtonBottomGroup *m_ui_btnGroup = nullptr;

public:
    explicit MainFixture(QWidget *parent = nullptr);
    ~MainFixture();

private:
    void switch_page(QString title);

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace COM::WFIXTURE

#endif  // WMAINFIXTURE_H_1723266488
