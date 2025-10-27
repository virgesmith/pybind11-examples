#pragma once

#include <cstdint>

struct EnumScope {
  enum CEnum { ONE = 1, TWO = 2 };
};

enum GlobalCEnum { ONE = 1, TWO = 2 };

enum class CppEnum: int8_t {
  ONE = 1,
  TWO = 2,
  THREE = 3,
  FOUR = 4
};

