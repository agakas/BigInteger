#include "biginteger.h"
#include <iostream>
#include <string>

//Конструкторы
BigInteger::BigInteger() = default;

BigInteger::BigInteger(int64_t value) {
    if (value < 0) {
        is_negative = true;
        value = -value;
    }
    while (value > 0) {
        digits.push_back(static_cast<uint32_t>(value % BASE));
        value /= BASE;
    }
}

BigInteger::BigInteger(uint64_t value) {
    while (value > 0) {
        digits.push_back(static_cast<uint32_t>(value % BASE));
        value /= BASE;
    }
}

BigInteger::BigInteger(const std::string& str) {
    is_negative = false;
    digits.clear();

    if (str.empty())
        throw std::invalid_argument("Empty string");

    size_t pos = 0;

    // знак
    if (str[0] == '-') {
        is_negative = true;
        pos++;
    } else if (str[0] == '+') {
        pos++;
    }

    if (pos == str.size())
        throw std::invalid_argument("String has only a sign");

    // пропуск ведущих нулей
    while (pos < str.size() && str[pos] == '0') pos++;

    if (pos == str.size()) {
        // число = 0
        digits.push_back(0);
        is_negative = false;
        return;
    }

    // читаем строку с конца и разбиваем на блоки BASE
    size_t end = str.size();
    while (end > pos) {
        size_t start = (end >= 9) ? end - 9 : pos;
        uint32_t block = 0;
        for (size_t i = start; i < end; ++i) {
            char c = str[i];
            if (!std::isdigit(c))
                throw std::invalid_argument("Invalid character in string");
            block = block * 10 + (c - '0');
        }
        digits.push_back(block);
        end = start;
    }

    normalize();
}
// Вспомогательная функция для удаления ведущих нулей в векторе digits
void BigInteger::normalize() {
    // Убираем нули с конца вектора (старшие разряды)
    while (!digits.empty() && digits.back() == 0) {
        digits.pop_back();
    }

    // Если число стало 0, то оно не может быть отрицательным
    if (digits.empty()) {
        is_negative = false;
        digits.push_back(0);
    }
}

//Реализация перегрузки арифметических операторов
//Сложение и вычитание
BigInteger& BigInteger::operator+=(const BigInteger& other){
    // Одинаковые знаки - просто складываем по модулю
    if (is_negative == other.is_negative) {
        addAbs(other);
    } else {
        // Разные знаки - вычитаем меньшее из большего по модулю
        if (absCompare(*this, other) >= 0) {
            subAbs(other);
        } else {
            BigInteger temp = other;
            temp.subAbs(*this);
            *this = std::move(temp);
        }
    }
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
    return *this += (-other);
}

BigInteger operator+(BigInteger lhs, const BigInteger& rhs) {
    lhs += rhs;
    return lhs;
}

BigInteger operator-(BigInteger lhs, const BigInteger& rhs) {
    lhs -= rhs;
    return lhs;
}

// Умножение и деление
BigInteger& BigInteger::operator*=(const BigInteger& other) {
    // если одно из чисел = 0
    if ((digits.size() == 1 && digits[0] == 0) ||
        (other.digits.size() == 1 && other.digits[0] == 0)) {
        digits = {0};
        is_negative = false;
        return *this;
    }

    std::vector<uint64_t> tmp(digits.size() + other.digits.size(), 0);

    for (size_t i = 0; i < digits.size(); ++i) {
        for (size_t j = 0; j < other.digits.size(); ++j) {
            tmp[i + j] += static_cast<uint64_t>(digits[i]) * other.digits[j];
        }
    }

    digits.resize(tmp.size());
    uint64_t carry = 0;

    for (size_t i = 0; i < tmp.size(); ++i) {
        tmp[i] += carry;
        digits[i] = static_cast<uint32_t>(tmp[i] % BASE);
        carry = tmp[i] / BASE;
    }

    is_negative = (is_negative != other.is_negative);
    normalize();
    return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& other) {
    if (other.digits.size() == 1 && other.digits[0] == 0)
        throw std::runtime_error("Division by zero");

    BigInteger divisor = other;
    divisor.is_negative = false;

    BigInteger current;
    current.digits = {0};

    BigInteger result;
    result.digits.resize(digits.size(), 0);

    for (int i = static_cast<int>(digits.size()) - 1; i >= 0; --i) {
        current.digits.insert(current.digits.begin(), digits[i]);
        current.normalize();

        uint32_t left = 0, right = BASE - 1, best = 0;
        while (left <= right) {
            uint32_t mid = (left + right) / 2;
            BigInteger tmp = divisor;
            tmp.mulByUint(mid);

            if (absCompare(tmp, current) <= 0) {
                best = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        result.digits[i] = best;
        BigInteger tmp = divisor;
        tmp.mulByUint(best);
        current -= tmp;
    }

    *this = result;
    is_negative = (is_negative != other.is_negative);
    normalize();
    return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& other) {
    BigInteger q = *this / other;
    *this -= q * other;
    return *this;
}

BigInteger operator*(BigInteger lhs, const BigInteger& rhs) {
    lhs *= rhs;
    return lhs;
}

BigInteger operator/(BigInteger lhs, const BigInteger& rhs) {
    lhs /= rhs;
    return lhs;
}

BigInteger operator%(BigInteger lhs, const BigInteger& rhs) {
    lhs %= rhs;
    return lhs;
}

// Чисто унарники
BigInteger BigInteger::operator-() const {
    BigInteger res = *this;
    if (!(digits.size() == 1 && digits[0] == 0))
        res.is_negative = !is_negative;
    return res;
}

BigInteger BigInteger::operator+() const {
    return *this;
}

// Вспомогашки для арифметики
// Сравнени по модулю
int BigInteger::absCompare(const BigInteger& a, const BigInteger& b) {
    if (a.digits.size() != b.digits.size())
        return (a.digits.size() < b.digits.size()) ? -1 : 1;

    for (int i = static_cast<int>(a.digits.size()) - 1; i >= 0; --i) {
        if (a.digits[i] != b.digits[i])
            return (a.digits[i] < b.digits[i]) ? -1 : 1;
    }
    return 0;
}
// Сложение по модулю (предполагается, что знаки обоих чисел одинаковые)
void BigInteger::addAbs(const BigInteger& other) {
    uint64_t carry = 0;
    size_t n = std::max(digits.size(), other.digits.size());
    digits.resize(n, 0);

    for (size_t i = 0; i < n; ++i) {
        uint64_t sum = carry + digits[i];
        if (i < other.digits.size())
            sum += other.digits[i];

        digits[i] = static_cast<uint32_t>(sum % BASE);
        carry = sum / BASE;
    }

    if (carry)
        digits.push_back(static_cast<uint32_t>(carry));
}
// Вычитание по модулю (предполагается, что *this >= other по модулю)
void BigInteger::subAbs(const BigInteger& other) {
    int64_t borrow = 0;

    for (size_t i = 0; i < digits.size(); ++i) {
        int64_t cur = static_cast<int64_t>(digits[i]) - borrow;
        if (i < other.digits.size())
            cur -= other.digits[i];

        if (cur < 0) {
            cur += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }

        digits[i] = static_cast<uint32_t>(cur);
    }

    normalize();
}
// Облегченное умножение на uint32_t для оптимизации деления
void BigInteger::mulByUint(uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < digits.size(); ++i) {
        uint64_t cur = carry + static_cast<uint64_t>(digits[i]) * m;
        digits[i] = static_cast<uint32_t>(cur % BASE);
        carry = cur / BASE;
    }
    if (carry) digits.push_back(static_cast<uint32_t>(carry));
}

//Сравнение
bool BigInteger::operator==(const BigInteger& other) const {
    return is_negative == other.is_negative &&
           digits == other.digits;
}

std::strong_ordering operator<=>(const BigInteger& rhs) const{
    if (is_negative != other.is_negative) {
        return is_negative
            ? std::strong_ordering::less
            : std::strong_ordering::greater;
    }
    int cmp = absCompare(*this, other);

    if (cmp == 0)
        return std::strong_ordering::equal;

    if (!is_negative) {
        return cmp < 0
            ? std::strong_ordering::less
            : std::strong_ordering::greater;
    } else {
        return cmp < 0
            ? std::strong_ordering::greater
            : std::strong_ordering::less;
    }
}

// Преобразование к bool
bool BigInteger::operator bool() const {
    for (uint32_t d : digits) {
        if (d != 0) return true;
    }
    return false;
}

// Строковое представление
std::string BigInteger::ToString() const {
    if (digits.empty()) return "0";

    std::string result;
    if (is_negative) result += '-';

    // Старший блок
    result += std::to_string(digits.back());

    // Остальные блоки с ведущими нулями
    for (int i = static_cast<int>(digits.size()) - 2; i >= 0; --i) {
        std::string block_str = std::to_string(digits[i]);
        result += std::string(9 - block_str.size(), '0') + block_str;
    }

    return result;
}
// Вывод/ввод
std::ostream& operator<<(std::ostream& os, const BigInteger& value) {
    os << value.ToString();
    return os;
}

std::istream& operator>>(std::istream& is, BigInteger& value) {
    std::string str;
    is >> str;
    value = BigInteger(str);
    return is;
}