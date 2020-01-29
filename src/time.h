#ifndef PENDULEINVERSE_TIME_H
#define PENDULEINVERSE_TIME_H

#include <cstdint>

namespace ip {
namespace time {

volatile uint32_t* DWT_CYCCNT = (volatile uint32_t*)0xE0001004;
volatile uint32_t* DWT_CONTROL = (volatile uint32_t*)0xE0001000;
volatile uint32_t* SCB_DEMCR = (volatile uint32_t*)0xE000EDFC;

/**
 * \brief Initialisation des fonctions de delai
 */
static void init() {
  *SCB_DEMCR |= 0x01000000;
  *DWT_CYCCNT = 0;    // reset the counter
  *DWT_CONTROL |= 1;  // enable the counter
}

inline uint32_t ticks() {
  return *DWT_CYCCNT;
}

inline uint32_t us() {
  static constexpr uint16_t cycles_per_us = F_CPU / 1000 / 1000;
  return ticks() / cycles_per_us;
}

inline uint32_t ms() {
  static constexpr uint16_t cycles_per_ms = F_CPU / 1000;
  return ticks() / cycles_per_ms;
}

inline void us(uint32_t t) {
  const uint32_t start = ticks();
  // 20 cycles of overhead
  constexpr uint32_t cycles_per_us = (F_CPU / 1000 / 1000) - 20;

  const uint32_t delay = t * cycles_per_us;

  while (ticks() - start < delay)
    ;
}

inline void ms(uint32_t t) {
  const uint32_t start = ticks();
  // 20 cycles of overhead
  constexpr uint32_t cycles_per_ms = (F_CPU / 1000) - 20;

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
    if (tick - m_startTick > m_delayTick) {
      m_startTick = tick;
      return true;
    }
    return false;
  }

 private:
  uint32_t m_delayTick;
  uint32_t m_startTick;
};

}  // namespace time

}  // namespace ip

#endif  // PENDULEINVERSE_TIME_H