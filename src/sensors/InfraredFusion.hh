#ifndef PENDULEINVERSE_INFRAREDFUSION_H
#define PENDULEINVERSE_INFRAREDFUSION_H

#include <cstdint>

namespace ip {
namespace sensors {

class InfraredFusion {
 public:
  /**
   * \brief Initialise la classe.
   * \param pin1 Pin où lire les données analogiques du capteur droite.
   * \param pin2 Pin où lire les données analogiques du capteur gauche.
   * \param nBits Nombre de bits de l'ADC.
   */
  InfraredFusion(const uint8_t pin1, const uint8_t pin2, const uint8_t nBits);
  ~InfraredFusion() = default;

  /**
   * \brief Règle les coefficients du polynôme de la fonction distance.
   * \param c1 Tableau contenant les n coefficients du capteur 1.
   * \param c1 Tableau contenant les n coefficients du capteur 2.
   * \param n Nombre de coefficients.
   */
  void setFusionCoefficients(float* c1, float* c2, const uint8_t n);

  /**
   * \brief Renvoie une nouvelle valeur fusionnée des capteurs.
   * \warning Aucune vérification n'est effectuée sur les tableaux de
   * coefficients. Il faut donc appeller setFusionCoefficients() auparavant.
   */
  float update() const;

 private:
  /**
   * \brief Exécute l'algorithme de fusion des données à partir des mesures
   *        brutes de capteurs.
   * \param raw1 Donnée brute du capteur 1.
   * \param raw2 Donnée brute du capteur 2.
   */
  float fusion(const uint16_t raw1, const uint16_t raw2) const;

  /**
   * \brief Evalue le polynome poly de degré n au point x.
   *
   * \param x    Le point x.
   * \param poly Tableau contenant les coefficients du polynome, du plus petit au plus
   *             grand ordre.
   * \param n    Nombre de coefficients dans le tableau.
   */
  float polyInterp(const uint16_t x, float* poly, const uint8_t n) const;

 private:
  uint8_t  m_pin1, m_pin2;
  uint8_t  m_nBits = 0;
  uint16_t m_maxADC;

  float * m_coefs1 = nullptr, *m_coefs2 = nullptr;
  uint8_t m_nCoefs = 0;
};

}  // namespace sensors
}  // namespace ip

#endif  // PENDULEINVERSE_INFRAREDFUSION_H