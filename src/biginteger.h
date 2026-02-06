#pragma once

#include <vector>
#include <string>
#include <compare>
#include <cstdint>
#include <iosfwd>

class BigInteger {
    private:
        static constexpr uint32_t BASE = 1'000'000'000; //База дробления числа в векторе для ускорения операций (каждый блок < BASE)
        bool is_negative = false;
        std::vector<uint32_t> digits;
        void normalize();  // убрать ведущие нули
        //static int absCompare(const BigInteger& a, const BigInteger& b);

    public:
        BigInteger();
        BigInteger(int64_t value);
        BigInteger(uint64_t value);
        explicit BigInteger(const std::string& str);     //Чтобы не было неявного преобразования из строки в BigInteger

        //Перегружаем арифметику
        BigInteger& operator+=(const BigInteger& other);
        BigInteger& operator-=(const BigInteger& other);
        BigInteger& operator*=(const BigInteger& other);
        BigInteger& operator/=(const BigInteger& other);
        BigInteger& operator%=(const BigInteger& other);

        friend BigInteger operator+(BigInteger lhs, const BigInteger& rhs);
        friend BigInteger operator-(BigInteger lhs, const BigInteger& rhs);
        friend BigInteger operator*(BigInteger lhs, const BigInteger& rhs);
        friend BigInteger operator/(BigInteger lhs, const BigInteger& rhs);
        friend BigInteger operator%(BigInteger lhs, const BigInteger& rhs);

        // Перегружаем унарние операторы
        BigInteger operator+() const;
        BigInteger operator-() const;

        // Инкремент / декремент
        BigInteger& operator++();    // prefix
        BigInteger operator++(int);  // postfix
        BigInteger& operator--();
        BigInteger operator--(int);

        // Сравнение
        std::strong_ordering operator<=>(const BigInteger& rhs) const;
        bool operator==(const BigInteger& rhs) const = default;

        // Преобразования
        explicit operator bool() const;

        // Строковое представление
        std::string ToString() const;

        // Ввод / вывод
        friend std::ostream& operator<<(std::ostream& os, const BigInteger& value);
        friend std::istream& operator>>(std::istream& is, BigInteger& value);
};
