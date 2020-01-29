#ifndef PENDULEINVERSE_TIME_H
#define PENDULEINVERSE_TIME_H

#include <cstdint>

namespace ip {
namespace time {

extern volatile uint32_t* DWT_CYCCNT;
extern volatile uint32_t* DWT_CONTROL;
extern volatile uint32_t* SCB_DEMCR;

/**
 * \brief Initialisation des fonctions de delai
 */
void init();

inline uint32_t ticks() {
  return *DWT_CYCCNT;
}

inline uint32_t us() {
  static constexpr uint32_t cycles_per_us = F_CPU / 1000 / 1000;
  return ticks() / cycles_per_us;
}

inline uint32_t ms() {
  static constexpr uint32_t cycles_per_ms = F_CPU / 1000;
  return ticks() / cycles_per_ms;
}

inline void us(uint32_t t) {
  const uint32_t start = ticks();
  // 20 cycles of overhead, todo
  constexpr uint32_t cycles_per_us = (F_CPU / 1000 / 1000);

  const uint32_t delay = t * cycles_per_us;

  while (ticks() - start < delay)
    ;
}

inline void ms(uint32_t t) {
  const uint32_t start = ticks();
  // 20 cycles of overhead, todo
  constexpr uint32_t cycles_per_ms = (F_CPU / 1000);

  uint32_t delay = t * cycles_per_ms;

  while (ticks() - start < delay)
    ;
}

class Timer {
 public:
  Timer() = default;
  /**
   * \brief Crée un timer de fréquence f.
   */
  Timer(const uint32_t f) { setFrequency(f); }

  void setFrequency(const uint32_t f) {
    // 20 cycles of overhead
    m_delayTick = (F_CPU / f) - 20;
    m_startTick = ticks();
  }

  /**
   * \brief Renvoie true tout les temps T=1/f.
   */
  inline bool update() {
    const uint32_t tick = ticks();
    if (tick - m_startTick >= m_delayTick) {
      m_dt        = tick - m_startTick;
      m_startTick = tick;
      return true;
    }
    return false;
  }

  inline uint32_t getDelta() { return m_dt / F_CPU; }

 private:
  uint32_t m_delayTick = 0;
  uint32_t m_startTick = 0;
  uint32_t m_dt        = 0;
};

}  // namespace time

}  // namespace ip

#endif  // PENDULEINVERSE_TIME_H