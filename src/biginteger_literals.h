#pragma once

#include <cstddef>
#include "biginteger.h"

namespace bi_literals {

    BigInteger operator""_bi(unsigned long long value);
    BigInteger operator""_bi(const char* str, std::size_t);

}