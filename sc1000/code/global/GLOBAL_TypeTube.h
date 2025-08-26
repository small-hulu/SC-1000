#ifndef TYPETUBE_H_1726042807
#define TYPETUBE_H_1726042807

#include <QColor>
#include <QString>

namespace Type {
/**
 * 样本位标记
 */
enum class ETubeSpaceMark : int {
    Mark_1 = 0,
    Mark_2,
    Mark_3,
    Mark_4,
    Mark_5,
    Mark_6,
    Mark_7,
    Mark_8,
    Mark_9,
    Mark_10,
    Mark_11,
    Mark_12,
    Mark_13,
    Mark_14,
    Mark_15,
    Mark_16,
    Mark_17,
    Mark_18,
    Mark_19,
    Mark_20,
    Mark_E1,
    Mark_E2,
    Mark_E3,
    Mark_E4,
    Mark_E5,
    Mark_E6,
    Mark_E7,
    Mark_E8,
    Mark_E9,
    Mark_E10,
};

/**
 * 获取样本位标号字符串
 * ==================
 * 要求传入`enum TubeSpace_Mark`
 * 否则未定义
 */
QString Get_TubeSpaceMark(ETubeSpaceMark idx);

/**
 * 根据字符串获取标号
 * 字符串异常则返回-1
 */
ETubeSpaceMark Get_TubeSpaceMark(const QString& str);

/**
 * 根据组号，获取一个组的样本标号字符串
 * 组号要求 [0, 10] -> 返回序列长度保证==3 如 ["1", "2", "3"]
 * 要求范围则未定义
 */
QList<QString> Get_TubeSpaceGroupMarkList(int idx);

/// 状态类型
enum class ETubeState : int {
    Idle = 0,     // 空闲
    Apply,        // 申请
    Testing,      // 测试中
    SampleAdded,  // 已加样
    Complete,     // 完成
    Uncompleted,  // 未完成
    UnKnown,      // 未知&异常
};

struct InfoTubeState {
    using Enum = ETubeState;

    QColor  color       = QColor("#000000");
    QString nameChinese = "未知";
    QString pathImage;
};

InfoTubeState Get_TubeStateInfo(ETubeState e);

/// 实验类型
enum class ETubeExp : int {
    N_Normal = 0,   // 常规
    E_Emergency,    // 急诊
    Q_Quality,      // 质控
    C_Calibration,  // 校准
    R_Resurvey,     // 重测
    UnKnown,        // 未知&异常
};

struct InfoTubeExp {
    using Enum = ETubeExp;

    QColor  color       = QColor("#000000");
    QString nameChinese = "未知";
    QString pathImage;
};

InfoTubeExp Get_TubeExpInfo(ETubeExp e);
}  // namespace Type

#endif  // TYPETUBE_H_1726042807
