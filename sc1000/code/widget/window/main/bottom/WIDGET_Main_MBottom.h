#ifndef WIDGET_MAIN_MBOTTOM_H_1696737344
#define WIDGET_MAIN_MBOTTOM_H_1696737344

#include <QWidget>

namespace Ui {
class MBottom;
}

namespace WIDGET {
namespace MAIN {

class MBottom final : public QWidget {
    Q_OBJECT
private:
    Ui::MBottom *ui = nullptr;

public:
    explicit MBottom(QWidget *parent = nullptr);
    virtual ~MBottom();

private:
    void modify_expState(bool flag, const QString &msg);

protected:
    void timerEvent(QTimerEvent *event) override;
};

}  // namespace MAIN
}  // namespace WIDGET

#endif  // WIDGET_MAIN_MBOTTOM_H_1696737344
