#ifndef WIDGET_RESULTPARAMSETTINGS_H_1686813004_2024516_1716611386_1722328949
#define WIDGET_RESULTPARAMSETTINGS_H_1686813004_2024516_1716611386_1722328949

#include <QSharedPointer>
#include <QWidget>

#include "InfoSample.h"
#include "widget/Background.h"

class QLineEdit;
class QLabel;
namespace Ui {
class DSampleHistoryFilter;
}

namespace WIDGET::HISTORY {
class DSampleHistoryFilter : public QWidget {
    Q_OBJECT
private:
    QSharedPointer<Ui::DSampleHistoryFilter> ui;

    QVector<QLabel*>    m_uiView_title;
    QVector<QLineEdit*> m_uiView_val;

public:
    DSampleHistoryFilter(QWidget* parent = nullptr);

public:
    InfoSample::HistoryFilterInfo Get_filterInfo();

signals:
    void signal_yes();
};

}  // namespace WIDGET::HISTORY

#endif  // WIDGET_RESULTPARAMSETTINGS_H_1686813004_2024516
