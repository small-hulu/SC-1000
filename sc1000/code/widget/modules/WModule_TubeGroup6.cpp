#include "WModule_TubeGroup6.h"

namespace WIDGET {
TubeGroup6::TubeGroup6(QWidget *parent) : QObject(parent) {
    auto widget = parent;
    auto layout = new QGridLayout(widget);
    for (int i = 0; i < ROW; i += 1) {
        QList<entity_ui_tube> tubeList;
        for (int j = 0; j < COL; j += 1) {
            auto item = new TubeUiItem(widget);
            layout->addWidget(item, i, j, 1, 1);

            tubeList << item;
        }
        this->m_tubleList << tubeList;
    }

    for (int i = 0; i < layout->rowCount(); i += 1) {
        layout->setRowStretch(i, 1);
    }
    for (int j = 0; j < layout->columnCount(); j += 1) {
        layout->setColumnStretch(j, 1);
    }

    for (int i = 0, k = 0; i < m_tubleList.size(); i += 1) {
        for (int j = 0; j < m_tubleList[i].size(); j += 1, k += 1) {
            QString txt = Type::Get_TubeStateInfo((Type::ETubeState)k).nameChinese;
            m_tubleList[i][j]->Set_lableRight(txt);
            m_tubleList[i][j]->Set_StateType(k);
        }
    }
}

}  // namespace WIDGET
