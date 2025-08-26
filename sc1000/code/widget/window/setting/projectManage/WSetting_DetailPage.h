#ifndef WSETTING_DETAILPAGE_H_1718066512
#define WSETTING_DETAILPAGE_H_1718066512

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTableWidget>
#include <QWidget>

#include "widget/modules/WModule_ButtonBottomGroup.h"

namespace Ui {
class DetailPage;
}

namespace WIDGET::SETTING {
class DetailPage : public QWidget {
    Q_OBJECT

private:
    Ui::DetailPage    *ui            = nullptr;
    ButtonBottomGroup *m_ui_btnGroup = nullptr;

public:
    explicit DetailPage(QWidget *parent = nullptr);
    DetailPage(QString text, QWidget *parent = nullptr);
    ~DetailPage();

private:
    void table_init_1();
    void table_init_2();
    void table_init_3();
    void table_init_4();
    void table_init_5();
    void table_init_6();
    void table_init_7();
    void table_init_8();

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::SETTING
#endif  // WSETTING_DETAILPAGE_H
