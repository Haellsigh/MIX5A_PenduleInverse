#ifndef PENDULEINVERSE_INCREMENTALENCODER_H
#define PENDULEINVERSE_INCREMENTALENCODER_H

#include <cstdint>

namespace ip {
namespace sensors {

class IncrementalEncoder {
 public:
  IncrementalEncoder() = default;
  IncrementalEncoder(uint32_t stepsPerRevolution) {
    m_stepsToRad = stepsPerRevolution * 2 * M_PI;
  }
  ~IncrementalEncoder() = default;

  template <typename Function>
  void initialize(uint8_t  pinLeft,
                  uint8_t  pinRight,
                  Function changeChA,
                  Function changeChB) {
    pinMode(pinLeft, INPUT_PULLUP);
    pinMode(pinRight, INPUT_PULLUP);

    delay(10);

    noInterrupts();
    // Attache les interruptions aux fonctions fournies. Celles-ci doivent
    // appeler handleChangeChx.
    attachInterrupt(digitalPinToInterrupt(pinLeft), changeChA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pinRight), changeChB, CHANGE);
    m_stateChA = digitalRead(pinLeft);
    m_stateChB = digitalRead(pinRight);
    interrupts();
  }

  inline int32_t getSteps() {
    noInterrupts();
    int32_t pos = m_currentPosition;
    interrupts();
    return pos;
  }

  inline float getRadians() { return getSteps() * m_stepsToRad; }

  inline void handleChangeChA() {
    // Register the change
    m_stateChA = !m_stateChA;

    if (m_stateChA) {
      if (!m_stateChB)
        m_currentPosition++;
      else
        m_currentPosition--;
    } else {
      if (m_stateChB)
        m_currentPosition++;
      else
        m_currentPosition--;
    }
  }

  inline void handleChangeChB() {
    // Register the change
    m_stateChB = !m_stateChB;

    if (m_stateChB) {
      if (m_stateChA)
        m_currentPosition++;
      else
        m_currentPosition--;
    } else {
      if (!m_stateChA)
        m_currentPosition++;
      else
        m_currentPosition--;
    }
  }

 private:
  int32_t m_currentPosition = 0;
  float   m_stepsToRad      = 0;
  bool    m_stateChA, m_stateChB;
};

}  // namespace sensors
}  // namespace ip

#endif  // PENDULEINVERSE_INCREMENTALENCODER_H