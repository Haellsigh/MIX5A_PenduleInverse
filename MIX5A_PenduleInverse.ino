#include "configuration.hh"

#include <Eigen.h>

#include "src/SoftPWM/softpwm.hh"
#include "src/controllers/pid.hh"
#include "src/sensors/IncrementalEncoder.hh"
#include "src/sensors/InfraredFusion.hh"
#include "src/taskscheduler.hh"
#include "src/time.hh"

using namespace ip;
using namespace ip::configuration;

sensors::InfraredFusion     infrared(pin::infrared1, pin::infrared2, nBits);
sensors::IncrementalEncoder encoder;
controllers::PID<float>     pid(100);
SoftPWM                     enablePWM(pin::var11_Enable, 100);

ip::TaskScheduler<3> scheduler;

// Déclaration interruptions
void changeChA();
void changeChB();

// Déclaration tâches
void task_debugPrint();
void task_control();
void task_enableSoftPWM();

//////////////////////////////////////////////////////////////////////////////////////////
// Setup

void setup() {
  // Initilisations
  ip::time::init();
  Serial.begin(115200);

  // Configuration entrées/sorties
  pinMode(pin::var11_Enable, OUTPUT);
  pinMode(pin::var12_Direction, OUTPUT);
  pinMode(pin::var13_Speed, OUTPUT);

  // Ajout des tâches
  scheduler.add(task_control, frequence::control);
  scheduler.add(task_debugPrint, frequence::debug);
  scheduler.add(task_enableSoftPWM, frequence::enableSoftPWM);

  // Configuration du régulateur PID
  pid.setP(1);
  pid.setSetpoint(0);

  // Configuration des capteurs
  infrared.setFusionCoefficients(distanceCoefsLeft, distanceCoefsRight, nBits);
  encoder.initialize(pin::encoderA, pin::encoderB, &changeChA, &changeChB);
}

void loop() {
  scheduler.run();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Interruptions encodeur

inline void changeChA() {
  encoder.handleChangeChA();
}

inline void changeChB() {
  encoder.handleChangeChB();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Tâches périodiques

inline void task_debugPrint() {
  // Serial.print();
  // Serial.print(",");
  // Serial.println();
}

inline void task_control() {
  // float position = infrared.update();
  float feedback = encoder.getRadians();
  float speed    = pid.update(feedback);

  float speedValue = abs(speed);
  // Limit speed to [0; 10]
  speedValue = speedValue > 10 ? 10 : speedValue;

  // Write outputs
  analogWrite(pin::var13_Speed, speedValue);
  digitalWriteFast(pin::var12_Direction, speed > 0);

  // For low speeds, use pwm on the enable pin
  if (speed <= lowSpeedThreshold)
    enablePWM = 100 * speed / lowSpeedThreshold;
  // For higher speeds, enable is always on
  else
    enablePWM = 100;
}

inline void task_enableSoftPWM() {
  enablePWM.tick();
}
