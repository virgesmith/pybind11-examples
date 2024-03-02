
#include <thread>
#include <chrono>

class Timer final
{
public:
  Timer() { reset(); }
  ~Timer() { }

  void reset()
  {
    m_start = std::chrono::system_clock::now();
  }

  double elapsed_ms() const
  {
    return (std::chrono::system_clock::now() - m_start).count() / 1'000'000;
  }

private:
  std::chrono::system_clock::time_point m_start;
};

void sleep_thread_ms(int ms)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}