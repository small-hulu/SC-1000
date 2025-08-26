#ifndef WSETTING_PAPERPRINTPARAMS_H_1725604880
#define WSETTING_PAPERPRINTPARAMS_H_1725604880

#include <QWidget>

#include "printer/PRINTER.hpp"
#include "widget/modules/WModule_ButtonBottomGroup.h"
namespace Ui {
class PaperPrintParams;
}

namespace WIDGET::SETTING {
class PaperPrintParams : public QWidget {
    Q_OBJECT
private:
    Ui::PaperPrintParams *ui = nullptr;

private:
    ButtonBottomGroup      *m_ui_btnGroup = nullptr;
    bool                    preview_sign;
    PRINTER::PrinterSample *printer;
    QWidget                *widget;

private:
    QMap<int, QString> mpPageSize;

public:
    void loadWidgetSettingPreview(const QString &text);
    explicit PaperPrintParams(QWidget *parent = nullptr);
    ~PaperPrintParams();

public:
    bool get_preview_sign();
    void onComboBoxTextChanged(const QString &text);

private:
    void save_total();

private slots:
    void on_pushButton_resolution_decrease_clicked();
    void on_pushButton_resolution_add_clicked();

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::SETTING

#endif  // WSETTING_PAPERPRINTPARAMS_H_1725604880
