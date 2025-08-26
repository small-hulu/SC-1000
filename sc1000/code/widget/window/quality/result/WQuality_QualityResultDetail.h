#ifndef WQUALITY_QUALITYRESULTDETAIL_H_1720096139
#define WQUALITY_QUALITYRESULTDETAIL_H_1720096139

#include <QWidget>

namespace Ui {
class QualityResultDetail;
}

namespace WIDGET::QUALITY {
class QualityResultDetail : public QWidget {
    Q_OBJECT
private:
    Ui::QualityResultDetail *ui;

public:
    explicit QualityResultDetail(QWidget *parent = nullptr);
    ~QualityResultDetail();

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::QUALITY

#endif  // WQUALITY_QUALITYRESULTDETAIL_H_1720096139
