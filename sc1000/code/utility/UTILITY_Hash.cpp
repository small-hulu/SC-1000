#include "UTILITY_Hash.h"

namespace UTILITY {

std::size_t get_hash(std::size_t x, std::size_t y) {
    return x ^ y;
}
}  // namespace UTILITY
