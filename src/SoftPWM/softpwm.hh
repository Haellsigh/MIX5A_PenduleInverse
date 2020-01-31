#ifndef PENDULEINVERSE_SOFTPWM_H
#define PENDULEINVERSE_SOFTPWM_H

#include "../fastio.hh"

namespace ip {
class SoftPWM {
 public:
  SoftPWM(uint8_t pin, uint32_t maxDutyCycle, uint32_t dutyCycle = 0);

  SoftPWM& operator=(const uint32_t& dutyCycle) {
    if (dutyCycle <= m_maxDutyCycle)
      m_dutyCycle = dutyCycle;
    return *this;
  }

  /**
   * \brief Met à jour le SoftPWM. Appeler ticksPerSecond fois par seconde.
   */
  void tick();

 private:
  inline void writePin(bool state) {
    if (m_pinState == state) {
      m_pinState = state;
      digitalWriteFast(m_pin, state);
    }
  }

 private:
  uint8_t m_pin;
  bool    m_pinState = true;

  uint32_t m_maxDutyCycle;
  uint32_t m_dutyCycle = 0, m_currentTick = 0;
};
}  // namespace ip

#endif  // PENDULEINVERSE_SOFTPWM_H