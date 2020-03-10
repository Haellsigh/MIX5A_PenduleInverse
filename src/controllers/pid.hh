#ifndef PENDULEINVERSE_CONTROLLER_PID_H
#define PENDULEINVERSE_CONTROLLER_PID_H

#include "../time.hh"

namespace ip {
namespace controllers {

/**
 * \brief Gère un régulateur PID.
 *
 * setFrequency(): gère la fréquence d'appel de update().
 * setWrap(): active la fonctionnalité d'enveloppe d'angle.
 * setSetpoint(): met à jour la consigne du régulateur.
 * setP(): règle le gain kp.
 * setI(): règle le gain ki.
 * setD(): règle le gain kd.
 *
 * update(): Met à jour la sortie du régulateur.
 *
 * \note Les gains kp, ki, kd sont indépendants de la fréquence.
 *
 * \note Temps d'échantillonnage: On suppose que la fréquence d'appel de update() est
 * constante. De cette façon, on peut mettre à jour les gains ki et kd une seule fois afin
 * de prendre en compte le temps d'échantillonage constant et ainsi éviter une
 * multiplication et une division à chaque boucle.
 */
class PID {
 public:
  PID(uint32_t frequency);
  PID(float kp, float ki, float kd, uint32_t frequency);

  void setFrequency(uint32_t frequency);
  void setWrap(float lowWrap, float highWrap);
  void setWrap(bool enabled);
  void setSetpoint(float setpoint);
  void setP(float kp);
  void setI(float ki);
  void setD(float kd);

  /**
   * \brief Met à jour la sortie du régulateur.
   * \warning Doit etre appelée à la bonne fréquence (celle donnée dans le constructeur).
   */
  float update(float measure);

 private:
  uint32_t m_lastTime = 0;

  float m_setpoint = 0;

  float m_lastMeasure = 0;
  float m_I           = 0;

  float m_dt = 0;
  // Valeurs des gains entrés par l'utilisateur
  float m_disp_ki = 0, m_disp_kd = 0;
  // Valeur des gains utilisés en interne (compensation de fréquence)
  float m_kp = 0, m_ki = 0, m_kd = 0;

  bool  m_wrapped = false;
  float m_lowWrap = 0, m_highWrap = 0;
};

}  // namespace controllers
}  // namespace ip

#endif  // PENDULEINVERSE_CONTROLLER_PID_H