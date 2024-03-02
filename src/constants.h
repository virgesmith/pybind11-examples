
#include <map>
#include <pybind11/pybind11.h>


class Constants final {
public:
  static Constants& instance() {
    static Constants constants;
    return constants;
  }

  py::object get(const std::string& name) {
    if (auto it = m_store.find(name); it == m_store.end()) {
      throw py::value_error(name + " not found");
    } else {
      return it->second;
    }
  }

  void set(const std::string& name, py::object value) {
    if (m_store.find(name) != m_store.end()) {
      throw py::value_error(name + " already defined");
    } else {
      m_store[name] = value;
    }
  }

  void repr() {
    for (const auto& item: m_store) {
      py::print(item.first, ": ", item.second);
    }
  }

private:
  Constants() {};
  Constants(const Constants&) = delete;
  Constants& operator=(const Constants&) = delete;
  Constants(Constants&&) = delete;
  Constants& operator=(Constants&&) = delete;
  
  std::map<std::string, py::object> m_store;
};