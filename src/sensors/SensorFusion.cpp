#include "SensorFusion.hh"

#include <Arduino.h>

namespace ip {
namespace sensors {

SensorFusion::SensorFusion(const uint8_t pinLeft,
                           const uint8_t pinRight,
                           const uint8_t nBits)
    : m_pinLeft(pinLeft), m_pinRight(pinRight), m_maxADC(1 << nBits) {
  pinMode(m_pinLeft, INPUT);
  pinMode(m_pinRight, INPUT);
}

void SensorFusion::setFusionCoefficients(float*        coefsLeft,
                                         float*        coefsRight,
                                         const uint8_t n) {
  m_fCoefsLeft  = coefsLeft;
  m_fCoefsRight = coefsRight;
  m_nCoefs      = n;
}

float SensorFusion::update() const {
  return fusion(analogRead(m_pinLeft), analogRead(m_pinRight));
}

float SensorFusion::fusion(const uint16_t rawLeft, const uint16_t rawRight) const {
  // Distance brute, normalisée entre 0 et 1.
  // const float dRaw = sqrtf(rawLeft * rawLeft + rawRight * rawRight) / m_maxADC;

  const float dRawLeft  = static_cast<float>(rawLeft) / m_maxADC;
  const float dRawRight = static_cast<float>(rawRight) / m_maxADC;

  // Calcul de a_0 + a_1*x + ... + a_n*x^n optimisé
  float dLeft   = m_fCoefsLeft[0];  // d = a_0
  float dRawPow = 1.f;
  for (uint8_t i = 1; i <= 4; i++) {
    dRawPow *= dRawLeft;
    dLeft += m_fCoefsLeft[i] * dRawPow;  // d += a_i * x^i
  }

  // Calcul de a_0 + a_1*x + ... + a_n*x^n optimisé
  float dRight = m_fCoefsRight[0];  // d = a_0
  dRawPow      = 1.f;
  for (uint8_t i = 1; i <= 4; i++) {
    dRawPow *= dRawRight;
    dRight += m_fCoefsRight[i] * dRawPow;  // d += a_i * x^i
  }

  // const float poidsDroite = (constrain(rawLeft - 250, 0, 600)) / 700 + 0.5;

  // return dLeft * poidsGauche + dRight * (1. - poidsGauche);

  if (dRight < 40) {
    return dRight;
  } else if (dLeft > 65) {
    return dLeft;
  } else {
    return (dLeft + dRight) / 2.;
  }

  /*
    // Calcul de a_0 + a_1*x + ... + a_n*x^n optimisé
    float d       = m_fCoefs[0];  // d = a_0
    float dRawPow = 1.;
    for (uint8_t i = 1; i <= 6; i++) {
      dRawPow *= dRaw;
      d += m_fCoefs[i] * dRawPow;  // d += a_i * x^i
    }
    */

  // \todo: filtrer les mesures (passe bas premier ordre)
  // return d;
}

}  // namespace sensors
}  // namespace ip