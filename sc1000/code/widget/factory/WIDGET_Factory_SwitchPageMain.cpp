#include "WIDGET_Factory_SwitchPageMain.h"

#include "WIDGET_Factory_SwitchPageCalibration.h"
#include "WIDGET_Factory_SwitchPageHelp.h"
#include "WIDGET_Factory_SwitchPageHistory.h"
#include "WIDGET_Factory_SwitchPageMaintain.h"
#include "WIDGET_Factory_SwitchPageQuality.h"
#include "WIDGET_Factory_SwitchPageReagent.h"
#include "WIDGET_Factory_SwitchPageSample.h"
#include "WIDGET_Factory_SwitchPageSettings.h"
#include "widget/window/main/WIDGET_SubPage.h"

namespace WIDGET {

namespace {
/**
 * 顺序的决定是这里保证的
 */
enum ESubPage_Top : int {
    Top_Sample = 0,   // 样本->测试
    Top_History,      // 主页
    Top_Reagent,      // 试剂
    Top_Calibration,  // 校准->定标
    Top_Quality,      // 质控
    Top_Maintain,     // 维护
    Top_Setting,      // 设置
    Top_Help,         // 帮助

    Enum_Count
};
}  // namespace

QVector<QString> FSwitchPageMain::Get_subPageList(int index) {
    assert(index >= 0 && index < Enum_Count);

    switch (static_cast<ESubPage_Top>(index)) {
    case Top_Sample     : return FSwitchPageSample().Get_subNameList();
    case Top_History    : return FSwitchPageHistory().Get_subNameList();
    case Top_Reagent    : return FSwitchPageReagent().Get_subNameList();
    case Top_Calibration: return FSwitchPageCalibration().Get_subNameList();
    case Top_Quality    : return FSwitchPageQuality().Get_subNameList();
    case Top_Maintain   : return FSwitchPageMaintain().Get_subNameList();
    case Top_Setting    : return FSwitchPageSettings().Get_subNameList();
    case Top_Help       : return FSwitchPageHelp().Get_subNameList();
    case Enum_Count     :;
    }

    return {};
}

QWidget* FSwitchPageMain::Create_bySwitchEnum(int index, QWidget* parent) {
    assert(index < ESubPage_Top::Enum_Count);

    auto titles = Get_subPageList(static_cast<ESubPage_Top>(index));
    switch (static_cast<ESubPage_Top>(index)) {
    case Top_Sample     : return new SubPage(titles, new FSwitchPageSample(__LINE__), parent);
    case Top_History    : return new SubPage(titles, new FSwitchPageHistory{__LINE__}, parent);
    case Top_Reagent    : return new SubPage(titles, new FSwitchPageReagent(__LINE__), parent);
    case Top_Calibration: return new SubPage(titles, new FSwitchPageCalibration(__LINE__), parent);
    case Top_Quality    : return new SubPage(titles, new FSwitchPageQuality(__LINE__), parent);
    case Top_Maintain   : return new SubPage(titles, new FSwitchPageMaintain(__LINE__), parent);
    case Top_Setting    : return new SubPage(titles, new FSwitchPageSettings(__LINE__), parent);
    case Top_Help       : return new SubPage(titles, new FSwitchPageHelp(__LINE__), parent);
    case Enum_Count     :;
    }

    return nullptr;
}

}  // namespace WIDGET
