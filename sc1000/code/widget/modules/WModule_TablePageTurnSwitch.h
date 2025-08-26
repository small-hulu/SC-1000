#ifndef WMODULE_TABLEPAGETURNSWITCH_H_1726131665
#define WMODULE_TABLEPAGETURNSWITCH_H_1726131665

#include <QWidget>

namespace Ui {
class TablePageTurnSwitch;
}

namespace WIDGET {
class TablePageTurnSwitch : public QWidget {
    Q_OBJECT
private:
    Ui::TablePageTurnSwitch *ui = nullptr;

private:
    int all  = 0;  // 所有项的总数
    int each = 0;  // 每页显示的项数

public:
    explicit TablePageTurnSwitch(QWidget *parent = nullptr);
    ~TablePageTurnSwitch();

public:
    void Build_By_ItemCount(int all, int each);
    // void Adjust_style();
private:
    void check_page(int page);

protected:
    void showEvent(QShowEvent *event) override;

signals:
    /**
     * 闭区间范围
     * [1, 5] 表示第 {1，2，3，4，5} 项
     * <= 0 表示无效
     */
    void signal_curRange(int start, int end);
};
}  // namespace WIDGET

#endif  // WMODULE_TABLEPAGETURNSWITCH_H_1726131665
