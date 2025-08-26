#ifndef TYPE_HPP_1722577355
#define TYPE_HPP_1722577355

namespace QRCODE {
/**
 * 注意：
 * ini中有逗号
 * qt会自动解析为 QStringList
 */
constexpr char    join_separator = ',';
const char* const time_format    = "yyyy-MM-dd";

enum class QrcodeType : int {
    invalid     = 0,  // 无效
    reagent     = 1,  // 试剂
    consumable  = 2,  // 耗材
    calibration = 3,  // 校准品
    quality     = 4,  // 质控品
};

enum class ConsumableType : int {
    invalid   = 0,  // 无效
    ASubstrat = 1,  // 底物A
    BSubstrat = 2,  // 底物B
};

}  // namespace QRCODE

#endif  // TYPE_HPP_1722577355
