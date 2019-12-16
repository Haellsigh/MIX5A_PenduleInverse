#ifndef PENDULEINVERSE_TIME_H
#define PENDULEINVERSE_TIME_H

#include <cstdint>

namespace ip {
namespace time {

/**
 * \brief Initialisation des fonctions de delai
 */
static void init() {
#ifdef DWT
  if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  }
#endif
}

/**
 * \brief Delai de t microsecondes.
 */
static void us(uint32_t t) {
#ifdef DWT
  uint32_t startTick = DWT->CYCCNT;
  // 20 cycles of overhead
  uint32_t delayTicks = t * (84000000 / 1000000) - 20;

  while (DWT->CYCCNT - startTick < delayTicks)
    ;
#else
  delayMicroseconds(t);
#endif
}

/**
 * \brief Delai de t millisecondes.
 */
static inline void ms(uint32_t t) {
  us(t * 1000);
}

class Timer {
 public:
  Timer() = default;
  /**
   * \brief Crée un timer de fréquence f.
   */
  Timer(const uint32_t f) { setFrequency(f); }

  void setFrequency(const uint32_t f) {
    m_delayTick = (84000000 / f);
    m_startTick = DWT->CYCCNT;
  }

  /**
   * \brief Renvoie true lorsque T=1/f est dépassé.
   */
  inline bool update() {
    uint32_t tick = DWT->CYCCNT;
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