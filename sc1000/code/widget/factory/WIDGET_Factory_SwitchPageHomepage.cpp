/**
 * !!!!!!!!!!!!!!!!
 * ! [deprecated] !
 * !!!!!!!!!!!!!!!!
 */
/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       WIDGET_Factory_SwitchPageHomepage.cpp
 * @date       2024-08-21
 * @version    1.0.0
 *
 * @author     cuber-lotus
 * @Email      luyinfeng@smtbio.com
 *
 * @brief
 * 本模块 -> 废弃
 *
 *****************************************************************************/
#include "WIDGET_Factory_SwitchPageHomepage.h"

#include "widget/window/homepage/WIDGET_HomepageHomepage.h"
#include "widget/window/homepage/WIDGET_HomepageInstrumentStatus.h"

namespace WIDGET {

/**
 * 注意，第一版只显示主页部分
 * 但后续还是要加上其他部分的，因此先注释
 * TODO
 */
namespace {
enum ESubPage_Homepage {
    ESubPage_Homepage_Homepage,  // 主页
    // ESubPage_Homepage_InstrumentStatus,  // 仪器状态

    EnumCount
};
}

QVector<QString> FSwitchPageHomepage::Get_subNameList() {
    QVector<QString> names(EnumCount);
    names[ESubPage_Homepage_Homepage] = "主页";
    // names[ESubPage_Homepage_InstrumentStatus] = "仪器状态";
    return names;
}

QWidget *FSwitchPageHomepage::Create_bySwitchEnum(int index, QWidget *parent) {
    assert(index < EnumCount);

    switch (static_cast<ESubPage_Homepage>(index)) {
        using namespace HOMEPAGE;
    case ESubPage_Homepage_Homepage: return new Homepage(parent);
    // case ESubPage_Homepage_InstrumentStatus: return new InstrumentStatus(parent);
    case EnumCount                 :;
    }

    return nullptr;
}

}  // namespace WIDGET
