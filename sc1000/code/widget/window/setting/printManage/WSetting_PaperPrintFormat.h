#ifndef WIDGET_PAPERPRINTPORMAT_H_1724311363
#define WIDGET_PAPERPRINTPORMAT_H_1724311363

#include <QWidget>

namespace Ui {
class PaperPrintFormat;
}

namespace WIDGET::SETTING {

class PaperPrintFormat : public QWidget {
    Q_OBJECT

private:
    Ui::PaperPrintFormat *ui = nullptr;

public:
    PaperPrintFormat(QWidget *parent = nullptr);
    ~PaperPrintFormat();

protected:
    void showEvent(QShowEvent *event) override;
};

}  // namespace WIDGET::SETTING

#endif  // WIDGET_PAPERPRINTPORMAT_H_1724311363
