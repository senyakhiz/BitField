#include <iostream>
#include "Set.h"

int main() {
    setlocale(LC_ALL, "Russian");
    
    size_t n;
    std::cout << "Решето Эратосфена" << std::endl;
    std::cout << "Введите верхний предел: ";
    if (!(std::cin >> n)) {
        std::cout << "Ошибка: неверный ввод данных!" << std::endl;
        return 1;
    }
    if (n < 2) {
        std::cout << "В этом диапазоне нет простых чисел." << std::endl;
        return 0;
    }
    Set numbers(n + 1);
    for (size_t i = 2; i <= n; i++) {
        numbers.insert(i);
    }
    for (size_t p = 2; p <= n / p; p++) {
        if (numbers.is_member(p)) {
            for (size_t j = p * p; j <= n; j += p) {
                numbers.erase(j);
            }
        }
    }
    std::cout << "Простые числа от 2 до " << n << ":" << std::endl;
    for (size_t i = 2; i <= n; i++) {
        if (numbers.is_member(i)) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    return 0;
}