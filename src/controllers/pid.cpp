#include "pid.hh"

namespace ip {
namespace controllers {

Pid::Pid(uint32_t frequency) : m_dt(1.f / frequency) {}

void Pid::setFrequency(uint32_t frequency) {
  m_dt = 1.f / frequency;
  // Update gains
  m_ki = m_disp_ki * m_dt;
  m_kd = m_disp_kd / m_dt;
}

void Pid::setWrap(float lowWrap, float highWrap) {
  m_wrapped  = true;
  m_lowWrap  = low;
  m_highWrap = high;
}

void Pid::setSetpoint(float setpoint) {
  m_setpoint = setpoint;
}

void Pid::setP(float kp) {
  m_kp = kp;
}

void Pid::setI(float ki) {
  m_disp_ki = ki;
  m_ki      = ki * m_dt;
}

void Pid::setD(float kd) {
  m_disp_kd = kd;
  m_kd      = kd / m_dt;
}

float Pid::update(float feedback, ) {
  float error = m_setpoint - feedback;

  if (m_wrapped) {
    const float range = m_highWrap - m_lowWrap;

    error = (error + m_highWrap) / range;
    error = ((error - std::floor(error)) * range) + m_lowWrap;
  }

  // Derivative only on input to avoid setpoint d
  float dFeedback = feedback - m_lastInput;

  m_I += error * m_ki;

  return m_kp * error + m_I + m_kd * dFeedback;
}

}  // namespace controllers
}  // namespace ip