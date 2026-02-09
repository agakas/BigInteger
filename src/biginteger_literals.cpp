#include "biginteger_literals.h"

namespace bi_literals {

BigInteger operator""_bi(unsigned long long value) {
    return BigInteger(static_cast<uint64_t>(value));
}

BigInteger operator""_bi(const char* str, std::size_t len) {
    return BigInteger(std::string(str, len));
}

}