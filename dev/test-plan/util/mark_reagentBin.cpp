#include "mark_reagentBin.h"

#include <QList>

namespace UTIL {
QString Get_TubeSpaceMark(int idx) {
    switch (TubeSpaceMark(idx)) {
    case Mark_1  : return "1";
    case Mark_2  : return "2";
    case Mark_E1 : return "E1";
    case Mark_3  : return "3";
    case Mark_4  : return "4";
    case Mark_E2 : return "E2";
    case Mark_5  : return "5";
    case Mark_6  : return "6";
    case Mark_E3 : return "E3";
    case Mark_7  : return "7";
    case Mark_8  : return "8";
    case Mark_E4 : return "E4";
    case Mark_9  : return "9";
    case Mark_10 : return "10";
    case Mark_E5 : return "E5";
    case Mark_11 : return "11";
    case Mark_12 : return "12";
    case Mark_E6 : return "E6";
    case Mark_13 : return "13";
    case Mark_14 : return "14";
    case Mark_E7 : return "E7";
    case Mark_15 : return "15";
    case Mark_16 : return "16";
    case Mark_E8 : return "E8";
    case Mark_17 : return "17";
    case Mark_18 : return "18";
    case Mark_E9 : return "E9";
    case Mark_19 : return "19";
    case Mark_20 : return "20";
    case Mark_E10: return "E10";
    }
    return "Tube-Error";
}

TubeSpaceMark Get_TubeSpaceMark(QString& str) {
    for (int i = Mark_1; i <= Mark_E10; i += 1) {
        if (str == Get_TubeSpaceMark(i)) {
            return (TubeSpaceMark)i;
        }
    }
    return (TubeSpaceMark)-1;
}

BinInfo Get_MarkToGroup(const QString& flag) {
    BinInfo info;
    if (flag[0] == 'E') {
        int idx = flag.mid(1).toInt();
        assert(idx >= 1 && idx <= 10);
        info.idxBinGroup = idx - 1;
        info.idxBinBack  = 0;
    } else {
        int idx = flag.toInt();
        assert(idx >= 1 && idx <= 20);
        info.idxBinGroup = (idx - 1) / 2;
        if (idx & 1) {
            info.idxBinBack = 2;
        } else {
            info.idxBinBack = 1;
        }
    }
    return info;
}

QList<QString> Get_TubeSpaceGroupMarkList(int idx) {
    QList<QString> res;

    switch (idx) {
    case 1: {
        res << Get_TubeSpaceMark(Mark_1);
        res << Get_TubeSpaceMark(Mark_2);
        res << Get_TubeSpaceMark(Mark_E1);
    } break;
    case 2: {
        res << Get_TubeSpaceMark(Mark_3);
        res << Get_TubeSpaceMark(Mark_4);
        res << Get_TubeSpaceMark(Mark_E2);
    } break;
    case 3: {
        res << Get_TubeSpaceMark(Mark_5);
        res << Get_TubeSpaceMark(Mark_6);
        res << Get_TubeSpaceMark(Mark_E3);
    } break;
    case 4: {
        res << Get_TubeSpaceMark(Mark_7);
        res << Get_TubeSpaceMark(Mark_8);
        res << Get_TubeSpaceMark(Mark_E4);
    } break;
    case 5: {
        res << Get_TubeSpaceMark(Mark_9);
        res << Get_TubeSpaceMark(Mark_10);
        res << Get_TubeSpaceMark(Mark_E5);
    } break;
    case 6: {
        res << Get_TubeSpaceMark(Mark_11);
        res << Get_TubeSpaceMark(Mark_12);
        res << Get_TubeSpaceMark(Mark_E6);
    } break;
    case 7: {
        res << Get_TubeSpaceMark(Mark_13);
        res << Get_TubeSpaceMark(Mark_14);
        res << Get_TubeSpaceMark(Mark_E7);
    } break;
    case 8: {
        res << Get_TubeSpaceMark(Mark_15);
        res << Get_TubeSpaceMark(Mark_16);
        res << Get_TubeSpaceMark(Mark_E8);
    } break;
    case 9: {
        res << Get_TubeSpaceMark(Mark_17);
        res << Get_TubeSpaceMark(Mark_18);
        res << Get_TubeSpaceMark(Mark_E9);
    } break;
    case 10: {
        res << Get_TubeSpaceMark(Mark_19);
        res << Get_TubeSpaceMark(Mark_20);
        res << Get_TubeSpaceMark(Mark_E10);
    } break;
    default: {
        res << Get_TubeSpaceMark(-1);
        res << Get_TubeSpaceMark(-1);
        res << Get_TubeSpaceMark(-1);
    } break;
    }

    return res;
}

}  // namespace UTIL
