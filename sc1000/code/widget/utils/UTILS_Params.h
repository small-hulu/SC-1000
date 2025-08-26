#ifndef PARAMS_H_1720245024
#define PARAMS_H_1720245024

#include <QList>

namespace WIDGET::UTILS {

/// [常量区]

/**
 * 自动扩展性表格的默认高度
 */
constexpr int     table_auto_rowHeight = 80;
constexpr char    join_separator       = ',';
const char* const time_format          = "yyyy-MM-dd hh:mm:ss";
/// [常量区]

inline QString time_generate() {
    return QDateTime::currentDateTime().toString(time_format);
}

/**
 * 拉升比例：垂直部分主框架
 * 1. 主体内容
 * 2. 按钮组
 */
inline QList<int> params_stretch_mainFrame() {
    return {12, 1};
}

/**
 * 拉升比例：
 * 1. 顶部提示
 * 2. 转盘
 */
inline QList<int> params_stretch_reagentBin_ver() {
    return {1, 6};
}

/**
 * 拉升比例：
 * (左右)
 * 1. 转盘
 * 2. 其他
 */
inline QList<int> params_stretch_reagentBin_hor() {
    return {502, 1182};
}

/**
 * 设置部分
 * 三级菜单的问题
 */
inline QList<int> params_stretch_settings() {
    return {2, 8};
}

}  // namespace WIDGET::UTILS
#endif  // PARAMS_H_1720245024
