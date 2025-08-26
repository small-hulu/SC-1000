#ifndef WIDGET_HOSPITALCONFIG_H_1724311285
#define WIDGET_HOSPITALCONFIG_H_1724311285

#include <QWidget>

#include "widget/modules/WModule_ButtonBottomGroup.h"

namespace Ui {
class HospitalConfig;
}

namespace WIDGET::SETTING {
class HospitalConfig : public QWidget {
    Q_OBJECT
private:
    static constexpr int Line_Height = 70;

private:
    Ui::HospitalConfig* ui = nullptr;

private:
    ButtonBottomGroup* m_ui_btnGroup = nullptr;

private:
    void init_ui();

public:
    HospitalConfig(QWidget* parent = nullptr);
    ~HospitalConfig();

private:
    void save_total();
    void append_depart();
    void append_doctor();
    void append_departTableLine(const QList<QString>& line);
    void append_doctorTableLine(const QList<QString>& line);
    void remove_depart(const QList<QString>& line);
    void remove_doctor(const QList<QString>& line);

protected:
    void showEvent(QShowEvent* event) override;
};
}  // namespace WIDGET::SETTING

#endif  // WIDGET_HOSPITALCONFIG_H_1724311285
