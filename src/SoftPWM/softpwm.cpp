#include "softpwm.hh"

namespace ip {

SoftPWM::SoftPWM(uint8_t pin, uint32_t maxDutyCycle, uint32_t dutyCycle)
    : m_pin(pin), m_maxDutyCycle(maxDutyCycle), m_dutyCycle(dutyCycle) {
  writePin(false);
}

void SoftPWM::setConstant(bool state) {
  if (state)
    m_dutyCycle = m_maxDutyCycle;
  else
    m_dutyCycle = 0;
}

SoftPWM& SoftPWM::operator=(const uint32_t& dutyCycle) {
  if (dutyCycle <= m_maxDutyCycle)
    m_dutyCycle = dutyCycle;
  return *this;
}

}  // namespace ip