#pragma once
#include <cinttypes>
#include <iostream>


struct uint2022_t {
    static const int POWER = 9;
    static const int32_t BASE = 1e9;     //размер разряда
    static const int SIZE = 68;     //количество вмещаемых разрядов
    // итого этот тип данных будет вмещать числа до 10^(68 * 9) = 10^(612) > 2^2022

    int32_t digits[SIZE]{}; // 68 * (32 / 8) = 272 < 300

    uint2022_t() {                                  //стандартный конструктор
        for (int & digit : digits) {
            digit = 0;
        }
    }

    static uint2022_t max() {
        uint2022_t max_uint2022;
        for (int & digit : max_uint2022.digits) {
            digit = BASE;
        }
        return max_uint2022;
    }
};

static_assert(sizeof(uint2022_t) <= 300, "Size of uint2022_t must be no higher than 300 bytes");

uint2022_t from_uint(uint32_t i);

uint2022_t from_string(const char* buff);

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator%(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t shift(uint2022_t& value);

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator>(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator<(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator<=(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs);

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value);
