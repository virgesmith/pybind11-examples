#pragma once

// python vectorises, typically a is scalar, x, y are vectors
inline double daxpy(double a, double x, double y) {
  return a * x + y;
}