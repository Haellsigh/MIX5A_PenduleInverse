#include "SensorFusion.h"

SensorFusion::SensorFusion(uint8_t pinA, uint8_t pinB)
    : m_pinA(pinA),
      m_pinB(pinB)
{
}

void SensorFusion::setSensorCoefficients(float *coefsA, float coefsB)
{
    m_coefsA = coefsA;
    m_coefsB = coefsB;
}

float SensorFusion::update()
{
    return fusion(analogRead(m_pinA), analogRead(m_pinB));
}

double SensorFusion::fusion(uint16_t rawA, uint16_t rawB)
{
    return 0.;
}