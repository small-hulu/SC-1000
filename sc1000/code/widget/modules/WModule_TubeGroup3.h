#ifndef TUBEGROUP_H_1721029180
#define TUBEGROUP_H_1721029180

#include <QWidget>

#include "WModule_TubeUiItem.h"

namespace WIDGET {
class TubeGroup3 : public QObject {
    Q_OBJECT
private:
    constexpr static int COUNT = 3;

private:
    QList<entity_ui_tube> m_tubeList;
    entity_ui_tube        m_curTube = nullptr;

public:
    TubeGroup3(QWidget* parent);

public:
    QList<entity_ui_tube> Get_tubeList() {
        return m_tubeList;
    }
    entity_ui_tube Get_curTube() {
        return m_curTube;
    }

    void Clear_allClicked();

public:
    /**
     * 切换组号 [1, 10]
     * 比如第一组则显示 ["1", "2", "E1"]
     */
    void Switch_groupMark(int idx);

    /**
     * 切换状态图片
     */
    void Switch_groupImage(const QList<int>& tubeStateList);

signals:
    void signal_clicked(entity_ui_tube);
    void signal_tube_select(QString btn_number);
};
}  // namespace WIDGET

#endif  // TUBEGROUP_H_1721029180
