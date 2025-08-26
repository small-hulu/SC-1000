#ifndef WMODULE_TABLECHECK_H_1720171959
#define WMODULE_TABLECHECK_H_1720171959

#include <QPushButton>
#include <QTableWidget>
#include <QWidget>

namespace Ui {
class TableSelect;
}

namespace WIDGET {
class TableSelect : public QWidget {
    Q_OBJECT
public:
    enum State : int {
        unseted = 0,  // 未设置
        unselected,   // 设置未选中
        selected,     // 选中
        unEnabled     // 不可作用
    };

public:
    struct CellInfo {
        int     x   = -1;
        int     y   = -1;
        QString txt = "";
    };

private:
    Ui::TableSelect *ui    = nullptr;
    QTableWidget    *table = nullptr;

private:
    CellInfo m_lastInfo;

public:
    explicit TableSelect(QWidget *parent = nullptr);
    ~TableSelect();

public:
    /// 构造table
    void Builder(int row, int col);

public:
    int Get_tableRowCount();
    int Get_tableColCount();
    /// 获取所有被选中的坐标
    QList<QPair<int, int>> Get_tableCheckedIndex();
    QList<QString>         Get_tableCheckedString();

public:
    QString Get_cellString(int row, int col);
    void    Set_cellString(int row, int col, const QString &txt);
    bool    Is_cellChecked(int row, int col);

public:
    void Set_tableText(const QList<QString> &txtList);
    void Set_tableClickedList(const QList<QString> &txtList);
    void Set_tableEnableList(const QList<QString> &txtList);
    void Clear_tableCellClicked(int x, int y);

public:
    CellInfo Get_lastSeletedInfo() {
        return m_lastInfo;
    }

private:
    QPushButton *get_cellButton(int row, int col);

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void signal_cellSelected(int row, int col, QString txt);
};
}  // namespace WIDGET

#endif  // WMODULE_TABLECHECK_H
