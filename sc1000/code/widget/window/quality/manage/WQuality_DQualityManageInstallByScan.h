#ifndef WQUALITY_DQUALITYMANAGEINSTALLBYSCAN_H_1722494086
#define WQUALITY_DQUALITYMANAGEINSTALLBYSCAN_H_1722494086

#include <QWidget>

#include "controller/CONTROLLER_Quality.h"

namespace Ui {
class DQualityManageInstallByScan;
}

namespace WIDGET::QUALITY {
class DQualityManageInstallByScan : public QWidget {
    Q_OBJECT
    using entity_item = CONTROLLER::Quality::entity_item;
    using entity_attr = entity_item::element_type::attr;

private:
    Ui::DQualityManageInstallByScan *ui;
    QByteArray                       m_barcode;

public:
    explicit DQualityManageInstallByScan(QWidget *parent = nullptr);
    ~DQualityManageInstallByScan();

    void Reset();

public:
    QByteArray Get_barcode() {
        return m_barcode;
    }

    QList<entity_item> Get_curEntityItem();

private:
    void refresh_ui();

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void signal_yes();
};
}  // namespace WIDGET::QUALITY

#endif  // WQUALITY_DQUALITYMANAGEINSTALLBYSCAN_H
