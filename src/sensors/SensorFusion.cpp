#include "SensorFusion.hh"

#include <Arduino.h>
//#include <math.h>

namespace ip {
namespace sensors {

SensorFusion::SensorFusion(const uint8_t pinLeft,
                           const uint8_t pinRight,
                           const uint8_t nBits)
    : m_pinLeft(pinLeft), m_pinRight(pinRight), m_maxADC(1 << nBits) {}

void SensorFusion::setFusionCoefficients(float* coefs, const uint8_t n) {
  m_fCoefs = coefs;
  m_nCoefs = n;
}

float SensorFusion::update() const {
  return fusion(m_maxADC - analogRead(m_pinLeft), analogRead(m_pinRight));
}

float SensorFusion::fusion(const uint16_t rawLeft,
                           const uint16_t rawRight) const {
  // Distance brute, normalisée entre 0 et 1.
  const float dRaw = sqrtf(rawLeft * rawLeft + rawRight * rawRight) / m_maxADC;

  // Calcul de a_0 + a_1*x + ... + a_n*x^n optimisé
  float d = m_fCoefs[0];  // d = a_0
  float dRawPow = 1.;
  for (uint8_t i = 1; i <= 6; i++) {
    dRawPow *= dRaw;
    d += m_fCoefs[i] * dRawPow;  // d += a_i * x^i
  }

  // \todo: filtrer les mesures (passe bas premier ordre)

  return d;
}

}  // namespace sensors
}  // namespace ip