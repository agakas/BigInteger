#include "biginteger_literals.h"
#include <string>

// Реализация
BigInteger operator"" _bi(unsigned long long value) {
    return BigInteger(uint64_t(value));
}

BigInteger operator"" _bi(const char* str, std::size_t len) {
    return BigInteger(std::string(str, len));
}