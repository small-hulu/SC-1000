#include "WIDGET_Factory_SwitchPageHistory.h"

#include "widget/window/history/WIDGET_HistorySample.h"

namespace WIDGET {
namespace {
enum ESubPage { Page_HistorySample, EnumCount };
}

QVector<QString> FSwitchPageHistory::Get_subNameList() {
    QVector<QString> names(EnumCount);
    names[Page_HistorySample] = "测试结果";
    return names;
}

QWidget *FSwitchPageHistory::Create_bySwitchEnum(int index, QWidget *parent) {
    assert(index < EnumCount);

    switch (static_cast<ESubPage>(index)) {
        using namespace HISTORY;
    case Page_HistorySample: return new HistorySample(parent);
    case EnumCount         :;
    }

    return nullptr;
}

}  // namespace WIDGET
