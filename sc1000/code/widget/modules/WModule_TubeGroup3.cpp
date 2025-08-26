#include "WModule_TubeGroup3.h"

#include "global/GLOBAL_Type.h"
#include "widget/modules/WModule_ReagentBinSample.h"

namespace WIDGET {

TubeGroup3::TubeGroup3(QWidget *parent) : QObject(parent) {
    auto widget = parent;
    assert(widget);

    auto layout = new QHBoxLayout(widget);
    for (int i = 0; i < COUNT; i += 1) {
        auto item = new TubeUiItem(widget);
        layout->addWidget(item);
        m_tubeList << item;
    }
    for (int i = 0; i < layout->count(); i += 1) {
        layout->setStretch(i, 1);
    }

    for (int i = 0; i < m_tubeList.size(); i += 1) {
        connect(m_tubeList[i], &TubeUiItem::signal_clicked, this, [this](entity_ui_tube tube) {
            qDebug() << "点击tube text" << tube->Get_labelBottomStr() << endl;
            for (auto item : m_tubeList) {
                item->Set_clicked(false);
            }
            tube->Set_clicked(true);

            m_curTube = tube;
            emit signal_clicked(tube);
            signal_tube_select(tube->Get_labelBottomStr());
        });
    }

    Clear_allClicked();
    Switch_groupMark(0);
}

void TubeGroup3::Clear_allClicked() {
    for (int i = 0; i < m_tubeList.size(); i += 1) {
        auto tube = m_tubeList[i];
        tube->Set_clicked(false);
        tube->Set_StateType((int)Type::ETubeState::Idle);
    }

    m_curTube = nullptr;
}

void TubeGroup3::Switch_groupMark(int idx) {
    auto names = Type::Get_TubeSpaceGroupMarkList(idx);
    assert(names.size() == m_tubeList.size());
    for (int i = 0; i < names.size(); i += 1) {
        m_tubeList[i]->Set_labelBottom(names[i]);
    }
}

void TubeGroup3::Switch_groupImage(const QList<int> &tubeStateList) {
    assert(m_tubeList.size() <= tubeStateList.size());
    for (int i = 0; i < m_tubeList.size(); i += 1) {
        m_tubeList[i]->Set_StateType(tubeStateList[i]);
    }
}

}  // namespace WIDGET
