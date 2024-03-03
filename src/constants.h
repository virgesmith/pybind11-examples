#include "print.h"

#include <pybind11/pybind11.h>

#include <map>
#include <ranges>


class Constants final {
public:
  typedef py::handle mapped_type;
  typedef std::pair<std::string, mapped_type> value_type;
  typedef std::map<std::string, mapped_type> store_type;

  static Constants& instance(py::kwargs kwargs = py::kwargs()) {
    static Constants constants;
    for (auto [k, v]: kwargs) {
      constants.set(k.cast<std::string>(), v);
    }
    return constants;
  }

  mapped_type get(const std::string& name) const {
    if (auto it = m_store.find(name); it == m_store.end()) {
      throw py::value_error(name + " not found");
    } else {
      return it->second;
    }
  }

  void set(const std::string& name, mapped_type value) {
    if (m_store.find(name) != m_store.end()) {
      throw py::value_error(name + " already defined");
    } else {
      m_store[name] = value;
    }
  }

  auto keys_begin() const { return std::views::keys(m_store).begin(); }

  auto keys_end() const { return std::views::keys(m_store).end(); }

  auto store_begin() const { return m_store.begin(); }

  auto store_end() const { return m_store.end(); }

  std::string repr() {
    return "<Constants %%>"_s % m_store;
  }

private:
  Constants() {}
  Constants(const Constants&) = delete;
  Constants& operator=(const Constants&) = delete;
  Constants(Constants&&) = delete;
  Constants& operator=(Constants&&) = delete;

  ~Constants() {}
  
  std::map<std::string, mapped_type> m_store;
};