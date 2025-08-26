#ifndef TUBEGROUP6_H_1721030223
#define TUBEGROUP6_H_1721030223

#include "widget/modules/WModule_TubeUiItem.h"

namespace WIDGET {
class TubeGroup6 : public QObject {
    Q_OBJECT
private:
    constexpr static int ROW = 2;
    constexpr static int COL = 3;

private:
    QList<QList<entity_ui_tube>> m_tubleList;

public:
    TubeGroup6(QWidget* parent);
};
}  // namespace WIDGET

#endif  // TUBEGROUP6_H_1721030223
