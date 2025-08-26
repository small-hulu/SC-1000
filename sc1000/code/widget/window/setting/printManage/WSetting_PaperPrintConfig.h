#ifndef WIDGET_PaperPrintConfig_Single_H_1724311385
#define WIDGET_PaperPrintConfig_Single_H_1724311385

#include <QEvent>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

namespace Ui {
class PaperPrintConfig;
}

namespace WIDGET::SETTING {

class PaperPrintConfig : public QWidget {
    Q_OBJECT

private:
    Ui::PaperPrintConfig *ui = nullptr;

private:
    QLabel lab_paperPreview;

    QPixmap                testData_curvePlot;
    QVector<QWidget *>     ptr_ui_widget_module;
    QVector<QPushButton *> ptr_ui_btn_able, ptr_ui_btn_moveUp, ptr_ui_btn_moveDown;
    QString css_btn_checked, css_btn_unchecked, css_btn_moveUp_able, css_btn_moveUp_unable,
        css_btn_moveDown_able, css_btn_moveDown_unable;

public:
    QPixmap get_testData_curvePlot() const;

public:
    PaperPrintConfig(QWidget * = Q_NULLPTR);
    ~PaperPrintConfig();

protected:
    void showEvent(QShowEvent *event) override;
};

}  // namespace WIDGET::SETTING

#endif  // WIDGET_PaperPrintConfig_Single_H_1724311385
