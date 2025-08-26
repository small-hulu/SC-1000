/** *************************************************************************
 * Copyright © 2024 luyinfeng.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       COM_Helper.h
 * @date       2024-04-16
 * @version    1.0.0
 *
 * @author     luyinfeng
 * @Email      luyinfeng@smtbio.com
 *
 * @brief
 * 编程中的通用辅助函数
 * 注意模板编程一般都写在头文件中
 * 基本数据类型(Primitive Data Types)
 *
 ***************************************************************************/
#ifndef ANALYSE_H_1703143711
#define ANALYSE_H_1703143711

#include <cassert>
#include <cstdint>
#include <utility>

namespace COM {
/**
 * 判断整形数据第bit位是否为1
 */
template <typename Integer>
bool get_integerBitBool(Integer num, unsigned idx) {
    assert(idx < 8u * sizeof(Integer));
    return num & (1u << idx);
}

/**
 * 设置int的第bit位为0/1
 */
template <typename Integer>
Integer set_integerBit(Integer num, unsigned idx, bool flag) {
    if (flag) {
        num |= 1ull << idx;
    } else {
        num &= ~(1ull << idx);
    }
    return num;
}

/**
 * 合并两个u16为一个32
 * 请注意内存分布和大小端
 */
template <typename Bits32>
Bits32 twou16_to_oneu32(uint16_t low, uint16_t high) {
    union {
        Bits32 b32;
        struct {
            uint16_t low;
            uint16_t high;
        };
    } data;
    static_assert(sizeof(data.b32) == sizeof(uint16_t) * 2, "sizeof is not 32");

    data.low  = low;
    data.high = high;
    return data.b32;
}

/**
 * 拆出一个32为两个u16
 * 请注意内存分布和大小端
 */
template <typename Bits32>
std::pair<uint16_t, uint16_t> oneu32_to_twou16(Bits32 val) {
    union {
        Bits32 b32;
        struct {
            uint16_t low;
            uint16_t high;
        };
    } data;
    static_assert(sizeof(data.b32) == sizeof(uint16_t) * 2, "sizeof is not 32");

    data.b32 = val;
    return {data.low, data.high};
}
}  // namespace COM

#endif  // ANALYSE_H_1703143711
