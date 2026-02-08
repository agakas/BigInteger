#include "biginteger.h"
#include <ostream>
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
/*BigInteger::BigInteger& operator+=(const BigInteger& other){
    
}*/

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