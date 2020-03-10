#include "pid.hh"

namespace ip {
namespace controllers {

/**
 * \brief Construit un régulateur PID avec des gains nulls et une fréquence.
 */
PID::PID(uint32_t frequency) {
  setFrequency(frequency);
}

/**
 * \brief Construit un régulateur PID avec des gains et une fréquence positifs.
 */
PID::PID(float kp, float ki, float kd, uint32_t frequency) {
  setP(kp);
  setI(ki);
  setD(kd);
  setFrequency(frequency);
}

/**
 * \brief Met à jour la fréquence d'appel de update() du régulateur.
 */
void PID::setFrequency(uint32_t frequency) {
  m_dt = 1.f / frequency;
  // Met à jour les gains pour que la régulation ne change pas avec la fréquence.
  m_ki = m_disp_ki * m_dt;
  m_kd = m_disp_kd / m_dt;
}

/**
 * \brief Active l'enveloppe d'angle.
 *
 * Permet de limiter l'angle d'entrée dans le domaine [lowWrap; highWrap).
 */
void PID::setWrap(float lowWrap, float highWrap) {
  m_wrapped  = true;
  m_lowWrap  = lowWrap;
  m_highWrap = highWrap;
}

/**
 * \brief Gère l'activation de l'enveloppe d'angle.
 */
void PID::setWrap(bool enabled) {
  m_wrapped = enabled;
}

/**
 * \brief Met à jour la consigne du régulateur.
 */
void PID::setSetpoint(float setpoint) {
  m_setpoint = setpoint;
}

/**
 * \brief Met à jour le gain kp du régulateur.
 */
void PID::setP(float kp) {
  m_kp = kp;
}

/**
 * \brief Met à jour le gain ki du régulateur.
 */
void PID::setI(float ki) {
  m_disp_ki = ki;
  m_ki      = ki * m_dt;
}

/**
 * \brief Met à jour le gain kd du régulateur.
 */
void PID::setD(float kd) {
  m_disp_kd = kd;
  m_kd      = kd / m_dt;
}

/**
 * \brief Calcule une nouvelle sortie du régulateur.
 *
 * \warning Doit être appelé à la fréquence donnée par la fonction setFrequency().
 */
float PID::update(float measure) {
  // On calcule l'erreur
  float error = m_setpoint - measure;
  // Enveloppage de l'erreur entre m_lowWrap et m_highWrap.
  if (m_wrapped) {
    const float width  = m_highWrap - m_lowWrap;
    const float offset = error - m_lowWrap;

    error = offset - (floor(offset / width) * width) + m_lowWrap;
  }

  // On dérive uniquement la mesure afin d'éviter les dérivées infinies de la consigne.
  float dMeasure = m_lastMeasure - measure;

  m_I += error * m_ki;

  return m_kp * error + m_I + m_kd * dMeasure;
}

}  // namespace controllers
}  // namespace ip