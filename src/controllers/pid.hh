#ifndef PENDULEINVERSE_CONTROLLER_PID_H
#define PENDULEINVERSE_CONTROLLER_PID_H

#include "../time.hh"

namespace ip {
namespace controllers {

class Pid {
 private:
  /* data */
 public:
  Pid(uint32_t frequency);
  void setSetpoint(float setpoint);
  void setP(float kp);
  void setI(float ki);
  void setD(float kd);

  void update(float feedback, float* output);

 private:
  uint32_t    m_lastTime = 0;
  time::Timer m_timer;

  float m_setpoint = 0;

  float m_lastInput = 0;
  float m_I         = 0;

  float m_dt      = 0;
  float m_disp_ki = 0, m_disp_kd = 0;
  float m_kp = 0, m_ki = 0, m_kd = 0;
};

}  // namespace controllers
}  // namespace ip

#endif  // PENDULEINVERSE_CONTROLLER_PID_H