#ifndef PENDULEINVERSE_CONTROLLER_PID_H
#define PENDULEINVERSE_CONTROLLER_PID_H

#include "../time.hh"

namespace ip {
namespace controllers {

template <typename T>
class PID {
 public:
  PID(uint32_t frequency);
  void setFrequency(uint32_t frequency);
  void setWrap(T lowWrap, T highWrap);
  void setSetpoint(T setpoint);
  void setP(float kp);
  void setI(float ki);
  void setD(float kd);

  /**
   * \brief Met à jour la sortie du PID.
   * \warning Doit etre appelée à la bonne fréquence (celle donnée dans le constructeur).
   */
  T update(T feedback);

 private:
  uint32_t m_lastTime = 0;

  T m_setpoint = 0;

  float m_lastInput = 0;
  float m_I         = 0;

  float m_dt      = 0;
  float m_disp_ki = 0, m_disp_kd = 0;
  float m_kp = 0, m_ki = 0, m_kd = 0;

  bool m_wrapped = false;
  T    m_lowWrap = 0, m_highWrap = 0;
};

}  // namespace controllers
}  // namespace ip

#include "pid_impl.hh"

#endif  // PENDULEINVERSE_CONTROLLER_PID_H