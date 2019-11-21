#ifndef PENDULE_SENSORFUSION_H
#define PENDULE_SENSORFUSION_H

#include <stdint.h>

class SensorFusion
{
public:
    /**
     * \brief Initializes the SensorFusion class.
     * \param pinA The analog input pin to read sensor A data.
     * \param pinB The analog input pin to read sensor B data.
     * \param nBits The number of bits of the ADC.
     */
    SensorFusion(uint8_t pinA, uint8_t pinB, uint8_t nBits);
    ~SensorFusion() = default;

    /**
     * \brief Sets the polynomial coefficients for the distance function.
     * \param coefs An array containing the 6 coefficients for the distance 
     * function.
     */
    void setDistanceCoefficients(float *coefs);

    /**
     * \brief Returns a new fused value from the sensors.
     * \returns The newest fused sensor value.
     */
    float update();

private:
    float fusion(uint16_t rawA, uint16_t rawB);

private:
    uint8_t m_pinA, m_pinB;
    uint16_t m_maxADC;

    float *m_coefs = nullptr;
};

#endif // PENDULE_SENSORFUSION_H