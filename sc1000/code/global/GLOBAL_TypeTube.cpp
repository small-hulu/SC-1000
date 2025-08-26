#include "GLOBAL_TypeTube.h"

#include <QList>

namespace Type {
QString Get_TubeSpaceMark(ETubeSpaceMark idx) {
    switch (ETubeSpaceMark(idx)) {
    case ETubeSpaceMark::Mark_1  : return "1";
    case ETubeSpaceMark::Mark_2  : return "2";
    case ETubeSpaceMark::Mark_3  : return "3";
    case ETubeSpaceMark::Mark_4  : return "4";
    case ETubeSpaceMark::Mark_5  : return "5";
    case ETubeSpaceMark::Mark_6  : return "6";
    case ETubeSpaceMark::Mark_7  : return "7";
    case ETubeSpaceMark::Mark_8  : return "8";
    case ETubeSpaceMark::Mark_9  : return "9";
    case ETubeSpaceMark::Mark_10 : return "10";
    case ETubeSpaceMark::Mark_11 : return "11";
    case ETubeSpaceMark::Mark_12 : return "12";
    case ETubeSpaceMark::Mark_13 : return "13";
    case ETubeSpaceMark::Mark_14 : return "14";
    case ETubeSpaceMark::Mark_15 : return "15";
    case ETubeSpaceMark::Mark_16 : return "16";
    case ETubeSpaceMark::Mark_17 : return "17";
    case ETubeSpaceMark::Mark_18 : return "18";
    case ETubeSpaceMark::Mark_19 : return "19";
    case ETubeSpaceMark::Mark_20 : return "20";
    case ETubeSpaceMark::Mark_E1 : return "21";
    case ETubeSpaceMark::Mark_E2 : return "22";
    case ETubeSpaceMark::Mark_E3 : return "23";
    case ETubeSpaceMark::Mark_E4 : return "24";
    case ETubeSpaceMark::Mark_E5 : return "25";
    case ETubeSpaceMark::Mark_E6 : return "26";
    case ETubeSpaceMark::Mark_E7 : return "27";
    case ETubeSpaceMark::Mark_E8 : return "28";
    case ETubeSpaceMark::Mark_E9 : return "29";
    case ETubeSpaceMark::Mark_E10: return "30";
    }
    return "Tube-Error";
}

ETubeSpaceMark Get_TubeSpaceMark(const QString &str) {
    for (int i = (int)ETubeSpaceMark::Mark_1; i <= (int)ETubeSpaceMark::Mark_E10; i += 1) {
        if (str == Get_TubeSpaceMark((ETubeSpaceMark)i)) {
            return (ETubeSpaceMark)i;
        }
    }
    return (ETubeSpaceMark)-1;
}

QList<QString> Get_TubeSpaceGroupMarkList(int idx) {
    QList<QString> res;

    switch (idx) {
    case 1: {
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_1);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_2);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_3);
    } break;
    case 2: {
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_4);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_5);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_6);
    } break;
    case 3: {
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_7);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_8);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_9);
    } break;
    case 4: {
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_10);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_11);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_12);
    } break;
    case 5: {
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_13);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_14);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_15);
    } break;
    case 6: {
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_16);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_17);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_18);
    } break;
    case 7: {
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_19);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_20);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_E1);
    } break;
    case 8: {
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_E2);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_E3);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_E4);
    } break;
    case 9: {
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_E5);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_E6);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_E7);
    } break;
    case 10: {
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_E8);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_E9);
        res << Get_TubeSpaceMark(ETubeSpaceMark::Mark_E10);
    } break;
    default: {
        res << Get_TubeSpaceMark((ETubeSpaceMark)-1);
        res << Get_TubeSpaceMark((ETubeSpaceMark)-1);
        res << Get_TubeSpaceMark((ETubeSpaceMark)-1);
    } break;
    }

    return res;
}

InfoTubeState Get_TubeStateInfo(ETubeState e) {
    InfoTubeState info;
    switch (e) {
    case ETubeState::Idle: {
        info.color       = QColor("#ffffff");
        info.nameChinese = "空闲";
        info.pathImage   = ":/suzhou/tube_idle.png";
    } break;
    case ETubeState::Apply: {
        info.color       = QColor("#37adba");
        info.nameChinese = "申请";
        info.pathImage   = ":/suzhou/tube_apply.png";
    } break;
    case ETubeState::Testing: {
        info.color       = QColor("#965bf5");
        info.nameChinese = "测试中";
        info.pathImage   = ":/suzhou/tube_testing.png";
    } break;
    case ETubeState::SampleAdded: {
        info.color       = QColor("#bbe5ea");
        info.nameChinese = "已加样";
        info.pathImage   = ":/suzhou/tube_sampleadded.png";
    } break;
    case ETubeState::Complete: {
        info.color       = QColor("#25b61a");
        info.nameChinese = "完成";
        info.pathImage   = ":/suzhou/tube_complete.png";
    } break;
    case ETubeState::Uncompleted: {
        info.color       = QColor("#ff6f4e");
        info.nameChinese = "未完成";
        info.pathImage   = ":/suzhou/tube_incomplete.png";
    } break;
    case ETubeState::UnKnown:;
    default                 :;
    }

    return info;
}

InfoTubeExp Get_TubeExpInfo(ETubeExp e) {
    InfoTubeExp info;
    switch (e) {
    case ETubeExp::N_Normal: {
        info.color       = QColor("#c3e6ea");
        info.nameChinese = "常规";
        info.pathImage   = QString(":/suzhou/tube_expType_N.png");
    } break;
    case ETubeExp::E_Emergency: {
        info.color       = QColor("#ffcaca");
        info.nameChinese = "急诊";
        info.pathImage   = QString(":/suzhou/tube_expType_E.png");
    } break;
    case ETubeExp::Q_Quality: {
        info.color       = QColor("#cad4ff");
        info.nameChinese = "质控";
        info.pathImage   = QString(":/suzhou/tube_expType_Q.png");
    } break;
    case ETubeExp::C_Calibration: {
        info.color       = QColor("#ffd4b2");
        info.nameChinese = "校准";
        info.pathImage   = QString(":/suzhou/tube_expType_C.png");
    } break;
    case ETubeExp::R_Resurvey: {
        info.color       = QColor("#d4d4d4");
        info.nameChinese = "重测";
        info.pathImage   = QString(":/suzhou/tube_expType_R.png");
    } break;
    case ETubeExp::UnKnown: {
        info.color       = QColor("#000000");
        info.nameChinese = "未知";
        info.pathImage   = "";
    } break;
    default:;
    }

    return info;
}

}  // namespace Type
