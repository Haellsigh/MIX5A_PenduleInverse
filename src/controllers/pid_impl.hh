#include "pid.hh"

namespace ip {
namespace controllers {

template <typename T>
PID<T>::PID(uint32_t frequency) {
  setFrequency(frequency);
}

template <typename T>
void PID<T>::setFrequency(uint32_t frequency) {
  m_dt = 1.f / frequency;
  // Update gains
  m_ki = m_disp_ki * m_dt;
  m_kd = m_disp_kd / m_dt;
}

template <typename T>
void PID<T>::setWrap(T lowWrap, T highWrap) {
  m_wrapped  = true;
  m_lowWrap  = lowWrap;
  m_highWrap = highWrap;
}

template <typename T>
void PID<T>::setSetpoint(T setpoint) {
  m_setpoint = setpoint;
}

template <typename T>
void PID<T>::setP(float kp) {
  m_kp = kp;
}

template <typename T>
void PID<T>::setI(float ki) {
  m_disp_ki = ki;
  m_ki      = ki * m_dt;
}

template <typename T>
void PID<T>::setD(float kd) {
  m_disp_kd = kd;
  m_kd      = kd / m_dt;
}

/**
 * Détails de l'implémentation:
 *
 * Temps d'échantillonnage: On suppose que la fréquence d'appel de update(feedback) est
 * constante. De cette façon, on peut mettre à jour les gains ki et kd pour prendre en
 * compte le temps d'échantillonage constant et ainsi éviter une multiplication et une
 * division.
 *
 * m_wrapped: Permet
 */
template <typename T>
T PID<T>::update(T feedback) {
  float error = m_setpoint - feedback;

  if (m_wrapped) {
    const T width  = m_highWrap - m_lowWrap;
    const T offset = error - m_lowWrap;

    error = offset - (floor(offset / width) * width) + m_lowWrap;
  }

  // Derivative only on input to avoid setpoint d
  float dFeedback = feedback - m_lastInput;

  m_I += error * m_ki;

  return m_kp * error + m_I + m_kd * dFeedback;
}

}  // namespace controllers
}  // namespace ip