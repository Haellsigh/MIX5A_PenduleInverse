#include "pid.hh"

namespace ip {
namespace controllers {

Pid::Pid(uint32_t frequency) : m_timer(frequency), m_dt(1 / frequency) {}

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

void Pid::update(float feedback, float* output) {
  if (m_timer.update()) {
    // float dt = m_timer.getDelta();

    float error = m_setpoint - feedback;
    // Derivative only on input to avoid setpoint d
    float dFeedback = feedback - m_lastInput;

    m_I += error * m_ki;

    *output = m_kp * error + m_I + m_kd * dFeedback;
  }
}

}  // namespace controllers
}  // namespace ip