#include "softpwm.hh"

namespace ip {

SoftPWM::SoftPWM(uint8_t pin, uint32_t maxDutyCycle, uint32_t dutyCycle)
    : m_pin(pin), m_maxDutyCycle(maxDutyCycle), m_dutyCycle(dutyCycle) {
  writePin(false);
}

void SoftPWM::tick() {
  if (m_currentTick < m_dutyCycle)
    writePin(true);
  else
    writePin(false);

  m_currentTick++;
  if (m_currentTick >= m_maxDutyCycle) {
    m_currentTick = 0;
  }
}  // namespace ip

}  // namespace ip