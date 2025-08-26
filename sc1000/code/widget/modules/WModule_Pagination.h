#ifndef WMODULE_PAGINATION_H_1720254598
#define WMODULE_PAGINATION_H_1720254598

#include <QWidget>

namespace Ui {
class Pagination;
}

namespace WIDGET {
class Pagination : public QWidget {
    Q_OBJECT
private:
    Ui::Pagination *ui;

public:
    explicit Pagination(QWidget *parent = nullptr);
    ~Pagination();

public:
    /// no-signal emit
    void Set_pageAll(int num);
    int  Get_pageAll();
    void Set_pageCur(int num);
    int  Get_pageCur();

private:
    /// 切换第几页
    bool switch_page(int num);

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void signal_pageSwitched(int);
};
}  // namespace WIDGET

#endif  // WMODULE_PAGINATION_H_1720254598
