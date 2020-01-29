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
  void setFrequency(uint32_t frequency);
  void setWrap(float lowWrap, float highWrap);
  void setSetpoint(float setpoint);
  void setP(float kp);
  void setI(float ki);
  void setD(float kd);

  /**
   * \brief Met à jour la sortie du PID.
   * \warning Doit etre appelée à la bonne fréquence (celle donnée dans le constructeur).
   */
  float update(float feedback);

 private:
  uint32_t m_lastTime = 0;

  float m_setpoint = 0;

  float m_lastInput = 0;
  float m_I         = 0;

  float m_dt      = 0;
  float m_disp_ki = 0, m_disp_kd = 0;
  float m_kp = 0, m_ki = 0, m_kd = 0;

  bool  m_wrapped = false;
  float m_lowWrap = 0, m_highWrap = 0;
};

}  // namespace controllers
}  // namespace ip

#endif  // PENDULEINVERSE_CONTROLLER_PID_H