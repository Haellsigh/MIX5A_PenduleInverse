#include "configuration.h"
#include "SensorFusion.h"

SensorFusion sensors(pinLeftSensor, pinRightSensor, nBits);

void setup()
{
  sensors.setSensorCoefficients(coefsLeftSensor, coefsRightSensor);
  Serial.begin(115200);
}

void loop()
{
  float distance = sensors.update();
  Serial.println(distance);

  delay(50);
}
