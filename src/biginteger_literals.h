#pragma once
#include "biginteger.h"

// Объявления
BigInteger operator"" _bi(unsigned long long);
BigInteger operator"" _bi(const char*, std::size_t);