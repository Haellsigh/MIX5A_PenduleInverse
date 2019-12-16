#ifndef PENDULEINVERSE_SENSORFUSION_H
#define PENDULEINVERSE_SENSORFUSION_H

#include <cstdint>

namespace ip {
namespace sensors {

class SensorFusion {
 public:
  /**
   * \brief Initialise la classe.
   * \param pinLeft Pin où lire les données analogiques du capteur droite.
   * \param pinRight Pin où lire les données analogiques du capteur gauche.
   * \param nBits Nombre de bits de l'ADC.
   */
  SensorFusion(const uint8_t pinLeft,
               const uint8_t pinRight,
               const uint8_t nBits);
  ~SensorFusion() = default;

  /**
   * \brief Règle les coefficients du polynôme de la fonction distance.
   * \param coefs Tableau contenant les n coefficients.
   * \param n Nombre de coefficients.
   */
  void setFusionCoefficients(float* coefs, const uint8_t n);

  /**
   * \brief Renvoie une nouvelle valeur fusionnée des capteurs.
   * \warning Aucune vérification n'est effectuée sur le tableau de
   * coefficients. Il faut donc appeller setFusionCoefficients() auparavant.
   */
  float update() const;

 private:
  /**
   * \brief Exécute l'algorithme de fusion des données à partir des mesures
   *        brutes de capteurs.
   * \param rawLeft Donnée brute du capteur gauche.
   * \param rawRight Donnée bruite du capteur droite.
   */
  float fusion(const uint16_t rawLeft, const uint16_t rawRight) const;

 private:
  uint8_t m_pinLeft, m_pinRight;
  uint16_t m_maxADC;

  float* m_fCoefs = nullptr;
  uint8_t m_nCoefs = 0;
};

}  // namespace sensors
}  // namespace ip

#endif  // PENDULEINVERSE_SENSORFUSION_H