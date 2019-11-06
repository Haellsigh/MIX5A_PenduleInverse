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
     */
    SensorFusion(uint8_t pinA, uint8_t pinB);
    ~SensorFusion() = default;

    /**
     * \brief Sets the fitted coefficients for each sensor.
     * \param coefsA An array containing the 4 coefficients for the sensor A.
     * \param coefsB An array containing the 4 coefficients for the sensor B.
     */
    void setSensorCoefficients(float *coefsA, float coefsB);

    float update();

private:
    float fusion(uint16_t rawA, uint16_t rawB);

private:
    uint8_t m_pinA, m_pinB;

    float *m_coefsA, *m_coefsB;
};

#endif // PENDULE_SENSORFUSION_H