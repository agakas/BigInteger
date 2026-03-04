#include <iostream>
#include "biginteger.h"
#include "biginteger_literals.h"

int main() {
    std::cout << "===== BigInteger FULL SPEC DEMO =====\n\n";

    // ------------------------------------------------------------
    // 1. Конструирование
    // ------------------------------------------------------------
    BigInteger a(int64_t(123));
    BigInteger b(int64_t(-456));
    BigInteger c(uint64_t(789));
    BigInteger d(std::string("0000123400"));

    std::cout << "[Construction]\n";
    std::cout << "a = " << a << '\n';
    std::cout << "b = " << b << '\n';
    std::cout << "c = " << c << '\n';
    std::cout << "d = " << d << " (from \"0000123400\")\n\n";

    // ------------------------------------------------------------
    // 2. Арифметика
    // ------------------------------------------------------------
    BigInteger x(std::string("12345678"));
    BigInteger y(std::string("98765432108"));

    std::cout << "[Arithmetic]\n";
    std::cout << "x = " << x << '\n';
    std::cout << "y = " << y << '\n';
    std::cout << "x + y = " << (x + y) << '\n';
    std::cout << "y - x = " << (y - x) << '\n';
    std::cout << "x * y = " << (x * y) << '\n';
    std::cout << "y / x = " << (y / x) << '\n';
    std::cout << "y % x = " << (y % x) << "\n\n";

    // ------------------------------------------------------------
    // 3. Составное присваивание
    // ------------------------------------------------------------
    BigInteger ca(int64_t(1000));

    std::cout << "[Compound assignment]\n";
    ca += BigInteger(uint64_t(25));
    std::cout << "ca += 25  -> " << ca << '\n';
    ca -= BigInteger(uint64_t(5));
    std::cout << "ca -= 5   -> " << ca << '\n';
    ca *= BigInteger(uint64_t(2));
    std::cout << "ca *= 2   -> " << ca << '\n';
    ca /= BigInteger(uint64_t(10));
    std::cout << "ca /= 10  -> " << ca << '\n';
    ca %= BigInteger(uint64_t(7));
    std::cout << "ca %= 7   -> " << ca << "\n\n";

    // ------------------------------------------------------------
    // 4. Унарные операции и инкременты
    // ------------------------------------------------------------
    BigInteger u(int64_t(42));

    std::cout << "[Unary operations]\n";
    std::cout << "+u  = " << (+u) << '\n';
    std::cout << "-u  = " << (-u) << '\n';
    std::cout << "++u = " << (++u) << '\n';
    std::cout << "u++ = " << (u++) << " (after: " << u << ")\n";
    std::cout << "--u = " << (--u) << '\n';
    std::cout << "u-- = " << (u--) << " (after: " << u << ")\n\n";

    // ------------------------------------------------------------
    // 5. Сравнение (через <=>)
    // ------------------------------------------------------------
    BigInteger s1(int64_t(100));
    BigInteger s2(int64_t(100));
    BigInteger s3(int64_t(200));

    std::cout << "[Comparison]\n";
    if (s1 == s2) std::cout << "s1 == s2\n";
    if (s1 < s3)  std::cout << "s1 < s3\n";
    if (s3 > s2)  std::cout << "s3 > s2\n\n";

    // ------------------------------------------------------------
    // 6. ToString()
    // ------------------------------------------------------------
    std::cout << "[ToString]\n";
    std::cout << "x.ToString() = \"" << x.ToString() << "\"\n\n";

    // ------------------------------------------------------------
    // 7. Преобразование в bool
    // ------------------------------------------------------------
    std::cout << "[Conversion to bool]\n";
    if (BigInteger(int64_t(0)))
        std::cout << "ERROR\n";
    else
        std::cout << "0 is false (OK)\n";

    if (BigInteger(int64_t(5)))
        std::cout << "5 is true (OK)\n\n";

    // ------------------------------------------------------------
    // 8. Ввод
    // ------------------------------------------------------------
    std::cout << "[Input]\nEnter BigInteger:\n> ";
    BigInteger input;
    std::cin >> input;
    std::cout << "You entered: " << input << "\n\n";

    // ------------------------------------------------------------
    // 9. _bi literal (NOT IMPLEMENTED YET)
    // ------------------------------------------------------------
    std::cout << "[User-defined literal _bi]\n";
    BigInteger l1 = 37183123_bi;
    BigInteger l2 = "37183123127313234632762740326741"_bi;

    std::cout << "l1 = " << l1 << '\n';
    std::cout << "l2 = " << l2 << " (from string literal)\n\n";

    std::cout << "===== DEMO FINISHED =====\n";
    return 0;
}