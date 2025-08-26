#include "GLOBAL_Type.h"

QList<QString> Type::TypeList_of_detect() {
    QList<QString> types;
    types << QString("血清");
    types << QString("尿液");
    types << QString("血浆");
    types << QString("全血");
    types << QString("末梢血");
    types << QString("脑脊液");
    types << QString("其他");
    return types;
}

QList<QString> Type::TypeList_of_tube_type() {
    QList<QString> types;
    types << QString("标准杯");
    types << QString("微量杯");
    types << QString("");
    return types;
}

QList<QString> Type::TypeList_of_ageUnit() {
    QList<QString> types;
    types << QString("岁");
    types << QString("月");
    types << QString("天");
    return types;
}

QList<QString> Type::TypeList_of_sex() {
    QList<QString> types;
    types << QString("");
    types << QString("男");
    types << QString("女");
    return types;
}
