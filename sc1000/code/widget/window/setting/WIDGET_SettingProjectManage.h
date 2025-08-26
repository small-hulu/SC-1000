#ifndef WIDGET_SETTINGS_PM_H_1717144358
#define WIDGET_SETTINGS_PM_H_1717144358

#include <QListWidget>
#include <QWidget>

namespace Ui {
class ProjectManage;
}

namespace WIDGET::SETTING {
class ProjectManagement final : public QWidget {
    Q_OBJECT

private:
    Ui::ProjectManage *ui          = nullptr;
    QWidget           *m_subWidget = nullptr;

public:
    explicit ProjectManagement(QWidget *parent = nullptr);
    virtual ~ProjectManagement();

private:
    void build_optionList(QListWidget *list, QVector<QString> btnList);

    void switch_widget(int idx);
};

}  // namespace WIDGET::SETTING

#endif  // WIDGET_SETTINGS_PM_H
