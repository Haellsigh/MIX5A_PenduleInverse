#include "configuration.h"
#include "SensorFusion.h"

#include <stdint.h>

SensorFusion sensors(pinLeftSensor, pinRightSensor, nBits);

int32_t currentStep = 0;

// Unknown initial state
uint8_t EncoderAState = LOW, EncoderBState = LOW;

void stepALow()
{
  EncoderAState = LOW;

  if (EncoderBState == HIGH)
    currentStep++;
  else
    currentStep--;
}

void stepAHigh()
{
  EncoderAState = HIGH;

  if (EncoderBState == LOW)
    currentStep++;
  else
    currentStep--;
}

void stepBLow()
{
  EncoderBState = LOW;

  if (EncoderAState == LOW)
    currentStep++;
  else
    currentStep--;
}

void stepBhigh()
{
  EncoderBState = HIGH;

  if (EncoderAState == HIGH)
    currentStep++;
  else
    currentStep;
}

void setup()
{
  sensors.setSensorCoefficients(coefsLeftSensor, coefsRightSensor);
  Serial.begin(115200);

  pinMode(49, INPUT_PULLUP);
  pinMode(51, INPUT_PULLUP);

  EncoderAState = digitalRead(49);
  EncoderBState = digitalRead(51);

  attachInterrupt(digitalPinToInterrupt(49), stepALow, FALLING);
  attachInterrupt(digitalPinToInterrupt(49), stepAHigh, RISING);
  attachInterrupt(digitalPinToInterrupt(51), stepBLow, FALLING);
  attachInterrupt(digitalPinToInterrupt(51), stepBhigh, RISING);
}

void loop()
{
  float distance = sensors.update();

  Serial.print("steps: ");
  //noInterrupts();
  Serial.println(currentStep);
  //interrupts();

  delay(50);
}
