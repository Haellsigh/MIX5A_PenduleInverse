#include "InfraredFusion.hh"

#include <Arduino.h>

namespace ip {
namespace sensors {

InfraredFusion::InfraredFusion(const uint8_t pin1,
                               const uint8_t pin2,
                               const uint8_t nBits)
    : m_pin1(pin1), m_pin2(pin2), m_nBits(nBits), m_maxADC(1 << nBits) {
  pinMode(m_pin1, INPUT);
  pinMode(m_pin2, INPUT);
}

void InfraredFusion::setFusionCoefficients(float* c1, float* c2, const uint8_t n) {
  m_coefs1 = c1;
  m_coefs2 = c2;
  m_nCoefs = n;
}

float InfraredFusion::update() const {
  return fusion(analogRead(m_pin1), analogRead(m_pin2));
}

float InfraredFusion::fusion(const uint16_t raw1, const uint16_t raw2) const {
  const float d1 = polyInterp(static_cast<float>(raw1) / m_maxADC, m_coefs1, m_nCoefs);
  const float d2 = polyInterp(static_cast<float>(raw2) / m_maxADC, m_coefs2, m_nCoefs);

  if (d2 < 40) {
    return d2;
  } else if (d1 > 65) {
    return d1;
  } else {
    return (d1 + d2) / 2.;
  }

  // \todo: filtrer les mesures (passe bas premier ordre)
}

float InfraredFusion::polyInterp(const uint16_t x, float* poly, const uint8_t n) const {
  // Calcul de a_0 + a_1*x + ... + a_n*x^n optimisé
  float p    = poly[0];  // p = a_0
  float xPow = 1.f;      // Use float because uint32_t would overflow (x^n >> 2^32)
  for (uint8_t i = 1; i < n; i++) {
    xPow *= x;
    p += poly[i] * xPow;  // p += a_i * x^i
  }
  return p;
}

}  // namespace sensors
}  // namespace ip