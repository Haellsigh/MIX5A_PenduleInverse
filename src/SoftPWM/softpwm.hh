#ifndef PENDULEINVERSE_SOFTPWM_H
#define PENDULEINVERSE_SOFTPWM_H

#include "../fastio.hh"

namespace ip {

/**
 * \brief MLI Logicielle
 *
 * Cette classe permet de gérer une modulation de largeur d'impulsion de façon logicielle.
 *
 * \note La fréquence d'appel de cette fonction est directement liée à la période de
 * la MLI générée. La période de la MLI sera
 * $T = maxDutyCycle / F_{appel}$
 */
class SoftPWM {
 public:
  /**
   * \brief Initialise la MLI.
   *
   * \note Le rapport cyclique doit être un entier entre 0 et maxDutyCycle.
   *
   * \param pin Le pin de sortie. Voir fastio.hh pour la liste de pins.
   * \param maxDutyCycle Le rapport cyclique maximum.
   * \param dutyCycle Le rapport cyclique.
   */
  SoftPWM(uint8_t pin, uint32_t maxDutyCycle, uint32_t dutyCycle = 0);

  /**
   * \brief Désactive la MLI et change l'état du pin.
   *
   * \note Cette fonction change simplement le rapport cyclique au maximum ou au minimum.
   *
   * \param state L'état (haut ou bas) du pin.
   */
  void setConstant(bool state);

  /**
   * \brief Règle le rapport cyclique de la MLI.
   *
   * \param dutyCycle Le nouveau rapport cyclique.
   */
  SoftPWM& operator=(const uint32_t& dutyCycle);

  /**
   * \brief Met à jour le pin de sortie avec un cache.
   *
   * \note La fréquence d'appel de cette fonction est directement liée à la période de
   * la MLI générée. La période de la MLI sera
   * $T = maxDutyCycle / F_{appel}$.
   */
  inline void tick() {
    if (m_currentTick < m_dutyCycle)
      writePin(true);
    else
      writePin(false);

    m_currentTick++;
    if (m_currentTick >= m_maxDutyCycle) {
      m_currentTick = 0;
    }
  }

 private:
  inline void writePin(bool state) {
    if (m_pinState == state) {
      m_pinState = state;
      digitalWriteFast(m_pin, state);
    }
  }

 private:
  uint8_t m_pin;
  bool    m_pinState = true;
  bool    m_enabled  = true;

  uint32_t m_maxDutyCycle;
  uint32_t m_dutyCycle = 0, m_currentTick = 0;
};
}  // namespace ip

#endif  // PENDULEINVERSE_SOFTPWM_H