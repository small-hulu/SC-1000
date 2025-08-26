#ifndef TYPETUBE_H_1726042807
#define TYPETUBE_H_1726042807

#include <QString>

namespace UTIL {

struct BinInfo {
    int idxBinGroup = 0;
    int idxBinBack  = 0;

    QString to_string() const {
        return QString("idxBinGroup[%1]idxBinBack[%2]").arg(idxBinGroup).arg(idxBinBack);
    }
};

/**
 * 样本位标记
 */
enum TubeSpaceMark : int {
    Mark_1 = 0,
    Mark_2,
    Mark_E1,
    Mark_3,
    Mark_4,
    Mark_E2,
    Mark_5,
    Mark_6,
    Mark_E3,
    Mark_7,
    Mark_8,
    Mark_E4,
    Mark_9,
    Mark_10,
    Mark_E5,
    Mark_11,
    Mark_12,
    Mark_E6,
    Mark_13,
    Mark_14,
    Mark_E7,
    Mark_15,
    Mark_16,
    Mark_E8,
    Mark_17,
    Mark_18,
    Mark_E9,
    Mark_19,
    Mark_20,
    Mark_E10,
};

/**
 * 获取样本位标号字符串
 * ==================
 * 要求传入`enum TubeSpace_Mark`
 * 否则未定义
 */
QString Get_TubeSpaceMark(int idx);

/**
 * 根据字符串获取标号
 * 字符串异常则返回-1
 */
TubeSpaceMark Get_TubeSpaceMark(QString& str);

/**
 * 将 str(mark) 转化为后来运动控制的标记号
 */
BinInfo Get_MarkToGroup(const QString& flag);

/**
 * 根据组号，获取一个组的样本标号字符串
 * 组号要求 [0, 10] -> 返回序列长度保证==3 如 ["1", "2", "E1"]
 * 要求范围则未定义
 */
QList<QString> Get_TubeSpaceGroupMarkList(int idx);

}  // namespace UTIL

#endif  // TYPETUBE_H_1726042807
