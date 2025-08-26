#ifndef WMODULE_TABLESELECTSINGLE_H_1726196368
#define WMODULE_TABLESELECTSINGLE_H_1726196368

#include <QPushButton>
#include <QTableWidget>
#include <QWidget>

namespace Ui {
class TableSelectSingle;
}

namespace WIDGET {
class TableSelectSingle : public QWidget {
    Q_OBJECT
public:
    enum State : int {
        unseted = 0,  // 未设置
        unselected,   // 设置未选中
        selected      // 选中
    };

private:
    Ui::TableSelectSingle *ui;
    QTableWidget          *table = nullptr;

public:
    explicit TableSelectSingle(QWidget *parent = nullptr);
    ~TableSelectSingle();

public:
    /// 再次构造会全部重置
    void Builder(int row, int col);

public:
    void    Set_tableText(const QList<QString> &txtList);
    QString Get_curChecked();

private:
    QPushButton *get_cellButton(int row, int col);

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void signal_cellSelected(QString txt);
};
}  // namespace WIDGET

#endif  // WMODULE_TABLESELECTSINGLE_H_1726196368
