#ifndef PENDULE_SENSORFUSION_H
#define PENDULE_SENSORFUSION_H

#include <cstdint>

class SensorFusion
{
public:
    SensorFusion(uint8_t sensorA, uint8_t sensorB);
    ~SensorFusion() = default;

private:
};

#endif // PENDULE_SENSORFUSION_H