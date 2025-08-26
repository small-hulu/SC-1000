#ifndef WMODULE_TABLEHEADER_H_1726156305
#define WMODULE_TABLEHEADER_H_1726156305

#include <QWidget>

namespace Ui {
class TableHeader;
}

namespace WIDGET {
class TableHeader : public QWidget {
    Q_OBJECT
private:
    Ui::TableHeader *ui = nullptr;

public:
    explicit TableHeader(QWidget *parent = nullptr);
    TableHeader(const QString &msg, QWidget *parent = nullptr);
    ~TableHeader();

public:
    void Set_title(const QString &msg);

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET

#endif  // WMODULE_TABLEHEADER_H_1726156305
