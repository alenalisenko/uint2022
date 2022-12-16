#include "number.h"
#include <cstring>


uint2022_t from_uint(uint32_t i) {
  uint2022_t res;

  int next = 0;
  while (i) {
    res.digits[next++] = i % uint2022_t::BASE;
    i /= uint2022_t::BASE;

    if(next >= uint2022_t::SIZE and i != 0){
      std::cerr << "Undefined Behavior\n";
    }
  }

  return res;
}

uint2022_t from_string(const char* buff) {
  uint2022_t res;
  size_t str_size = std::strlen(buff);
  if(str_size > uint2022_t::POWER * uint2022_t::SIZE) { // если длина строки больше максимаьного количества цифр в числе
    std::cerr << "Undefined Behavior\n";
  }

  int now = 0;
  for(int i = int(str_size) - 1; i >= 0; i -= uint2022_t::POWER) {
    uint32_t num = 0;
    long long now_ten_power = 1;
    for(long long j = i; j >= 0 and j > i - uint2022_t::POWER; j--) {
      num = (buff[j] - '0') * now_ten_power + num;
      now_ten_power *= 10;
    }
    res.digits[now++] = num;
  }

  return res;
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
  uint2022_t res;

  for(int i = 0; i < uint2022_t::SIZE; i++) {
    res.digits[i] += lhs.digits[i] + rhs.digits[i];
    if(res.digits[i] >= uint2022_t::BASE) {
      res.digits[i] -= uint2022_t::BASE;
      res.digits[i + 1]++;
    }
  }

  if(res.digits[uint2022_t::SIZE - 1] > uint2022_t::BASE) {
    std::cerr << "Undefined Behavior\n";
  }

  return res;
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {
  uint2022_t res;

  for(int i = 0; i < uint2022_t::SIZE; i++) {
    res.digits[i] += lhs.digits[i] - rhs.digits[i];
    if(res.digits[i] < 0) {
      res.digits[i] += uint2022_t::BASE;
      res.digits[i + 1]--;
    }
  }

  if(res.digits[uint2022_t::SIZE - 1] < 0) {
    std::cerr << "Undefined Behavior\n";
  }

  return res;
}

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs) {
  uint2022_t result;

  for (int i = 0; i < uint2022_t::SIZE; i++) {
    int carry = 0;
    for (int j = 0; j < uint2022_t::SIZE - i; j++) {
      long long cur = result.digits[i + j] + lhs.digits[i] * 1ll * rhs.digits[j] + carry;
      result.digits[i + j] = int32_t(cur % uint2022_t::BASE);
      carry = int(cur / uint2022_t::BASE);
    }
    if(carry > 0){
      std::cerr << "Undefined Behavior\n";
    }
  }

  return result;
}

uint2022_t operator/(const uint2022_t& lhs, const int& x) { // деление длинного на короткое
  uint2022_t res;

  for (int i = uint2022_t::SIZE - 1; i >= 0; i--) {
    if (i) {
      res.digits[i - 1] = lhs.digits[i - 1] + (lhs.digits[i] % x) * uint2022_t::BASE;
    }

    res.digits[i] /= x;
  }

  return res;
}

uint2022_t shift(uint2022_t& value) {
  for (size_t i = uint2022_t::SIZE - 1; i > 0; --i) {
    value.digits[i] = value.digits[i - 1];
  }
  value.digits[0] = 0;

  return value;
}

std::pair<uint2022_t, uint2022_t> corner_division(const uint2022_t& lhs, const uint2022_t& rhs){
  uint2022_t modulo = from_uint(0);
  uint2022_t res;
  if(lhs < rhs) {
    return {from_uint(0), lhs};
  }
  for(int i = uint2022_t::SIZE - 1; i >= 0;) {
    int now = 0;
    while(i >= 0 and modulo < rhs) {
      if(i == 0 or lhs.digits[i] > 0 or modulo > from_uint(0)) {
        modulo = shift(modulo);
        modulo.digits[0] += lhs.digits[i];
      }
      now++;
      i--;
    }
    if(modulo == from_uint(0)){
      for(int j = 0; j < now; j++){
        res = shift(res);
      }
      break;
    }
    int32_t quotient = 0;
    int32_t l = 0;
    int32_t r = uint2022_t::BASE;
    while(l <= r){
      int32_t m = (l + r) / 2;
      if(rhs * from_uint(m) <= modulo) {
        quotient = m;
        l = m + 1;
      }
      else {
        r = m - 1;
      }
    }
    modulo = modulo - from_uint(quotient) * rhs;
    res = shift(res);
    res.digits[0] = quotient;
  }

  return {res, modulo};
}

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs) {
  return corner_division(lhs, rhs).first;
}

uint2022_t operator%(const uint2022_t& lhs, const uint2022_t& rhs) {
  return corner_division(lhs, rhs).second;
}

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs) {
  for(int i = 0; i < uint2022_t::SIZE; i++){
    if(lhs.digits[i] != rhs.digits[i]){
      return false;
    }
  }

  return true;
}

bool operator>(const uint2022_t& lhs, const uint2022_t& rhs) {
  for(int i = uint2022_t::SIZE - 1; i >= 0; i--) {
    if(lhs.digits[i] < rhs.digits[i]) {
      return false;
    } else if(lhs.digits[i] > rhs.digits[i]) {
      return true;
    }
  }

  return false;
}

bool operator<(const uint2022_t& lhs, const uint2022_t& rhs) {
  return rhs > lhs;
}

bool operator<=(const uint2022_t& lhs, const uint2022_t& rhs) {
  return !(lhs > rhs);
}

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs) {
  return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value) {
  bool leading_zeros = true;
  char res[uint2022_t::POWER + 1];
  for(int i = uint2022_t::SIZE - 1; i >= 0; i--){
    if(leading_zeros and value.digits[i] != 0){
      leading_zeros = false;
      sprintf(res, "%d", value.digits[i]);
      stream << res;
    } else if(!leading_zeros) {
      sprintf(res, "%09d", value.digits[i]);
      stream << res;
    }
  }
  return stream;
}

