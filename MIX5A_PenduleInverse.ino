#include "configuration.h"
#include "SensorFusion.h"

#include <stdint.h>

//SensorFusion sensors(pinLeftSensor, pinRightSensor, nBits);

int32_t currentStep = 0;

// Unknown initial state
uint8_t EncoderAState, EncoderBState;

void stepAChange()
{
  EncoderAState = ((EncoderAState == LOW) ? HIGH : LOW);

  if (EncoderAState == HIGH) {
    if (EncoderBState == LOW)
      currentStep++;
    else
      currentStep--;
  } else {
    if (EncoderBState == HIGH)
      currentStep++;
    else
      currentStep--;
  }
}

void stepBChange()
{
  EncoderBState = ((EncoderBState == LOW) ? HIGH : LOW);

  if (EncoderBState == HIGH) {
    if (EncoderAState == HIGH)
      currentStep++;
    else
      currentStep--;
  } else {
    if (EncoderAState == LOW)
      currentStep++;
    else
      currentStep--;
  }
}

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
  //sensors.setSensorCoefficients(coefsLeftSensor, coefsRightSensor);
  Serial.begin(115200);

  pinMode(49, INPUT_PULLUP);
  pinMode(51, INPUT_PULLUP);

  delay(10);

  noInterrupts();
  
  attachInterrupt(digitalPinToInterrupt(49), stepAChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(51), stepBChange, CHANGE);
  
  EncoderAState = digitalRead(49);
  EncoderBState = digitalRead(51);
  
  interrupts();
}

void loop()
{
  float distance = sensors.update();
  //Serial.println(distance);

  int stepn;
  noInterrupts();
  stepn = currentStep % 2048;
  interrupts();

  Serial.println(360. * stepn / 4096.);
  delay(10);
}
