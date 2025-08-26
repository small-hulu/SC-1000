#ifndef WIDGET_MAINTAIN_COMMONLYUSED_H_1717403774
#define WIDGET_MAINTAIN_COMMONLYUSED_H_1717403774

#include <QTableWidget>
#include <QWidget>

namespace Ui {
class CommonlyUsed;
}

namespace WIDGET {
namespace MAINTAIN {

class CommonlyUsed final : public QWidget {
    Q_OBJECT

private:
    Ui::CommonlyUsed* ui    = nullptr;
    QTableWidget*     table = nullptr;

private:
    void init_ui();

public:
    explicit CommonlyUsed(QWidget* parent = nullptr);
    virtual ~CommonlyUsed();

private:
    void append_maintainWork(const QString& txt, std::function<void(void)> task,
                             const QString& key);
    void append_fixtureWork(const QString& txt, std::function<void(void)> task);
};
}  // namespace MAINTAIN
}  // namespace WIDGET
#endif  // WIDGET_MAINTAIN_COMMONLYUSED_H
