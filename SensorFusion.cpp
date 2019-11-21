#include "SensorFusion.h"

#include <Arduino.h>
//#include <math.h>

SensorFusion::SensorFusion(uint8_t pinA, uint8_t pinB, uint8_t nBits)
    : m_pinA(pinA),
      m_pinB(pinB),
      m_maxADC(powf(2, nBits))
{
}

void SensorFusion::setSensorCoefficients(float *coefs)
{
    m_coefs = coefs;
}

float SensorFusion::update()
{
    return fusion(m_maxADC - analogRead(m_pinA), analogRead(m_pinB));
}

float SensorFusion::fusion(uint16_t rawA, uint16_t rawB)
{
    // Distance brute fusionnée et normalisée entre 0 et 1.
    const float dRaw = sqrtf(rawA * rawB + rawB * rawB) / m_maxADC;

    // Algorithme pour calculer a_0 + a_1*x + a_2*x^2 + a_3*x^3 + ... sous la 
    // forme d'une boucle.
    // Permet d'éviter de calculer plusieurs fois les puissances.
    float dReal = m_coefsA[0]; // d = a_0
    float dRawPow = 1.;
    for (uint8_t i = 1; i < 7; i++)
    {
        dRawPow *= dRaw;
        dReal += m_coefsA[i] * dRawPow; // d += a_i * x^i
    }

    // \todo: filtrer les mesures (passe bas premier ordre)

    return dReal;
}