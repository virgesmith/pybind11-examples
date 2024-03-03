
#pragma once

#include <string>
#include <type_traits>
#include <sstream>
#include <iomanip>
#include <map>

#include <pybind11/pybind11.h>

namespace py = pybind11;

using namespace std::string_literals;

namespace std
{
// forces char to be printed (rather than number)
inline std::string to_string(char c)
{
  return std::string(1, c);
}

inline std::string to_string(const std::string& v)
{
  return v;
}

inline std::string to_string(const py::object& o)
{
  return py::str(o);
}

inline std::string to_string(const py::handle& o)
{
  return py::str(o);
}

template<typename K, typename V>
std::string to_string(const std::map<K, V>& dict)
{
  if (dict.empty())
    return "{}";
  std::string result = "{";

  for (const auto& [k, v]: dict)
    result += std::to_string(k) + "=" + std::to_string(v) + " ";
  result += "}";

  return result;
}

}


// need an rvalue ref as might/will be a temporary
template<typename T>
std::string operator%(std::string&& str, T value)
{
  size_t s = str.find("%%");
  if (s != std::string::npos)
  {
    str.replace(s, 2, std::to_string(value));
  }
  return std::move(str);
}


