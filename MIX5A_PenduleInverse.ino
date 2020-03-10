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
controllers::PID            pid(frequency::control);

ip::TaskScheduler<2> scheduler;

// Déclaration interruptions
void changeChA();
void changeChB();

// Déclaration tâches
void task_debugPrint();
void task_control();

//////////////////////////////////////////////////////////////////////////////////////////
// Setup

void setup() {
  // Initilisations
  ip::time::init();
  Serial.begin(115200);

  // Configuration entrées/sorties
  pinMode(pin::var11_Enable, OUTPUT);
  pinMode(pin::var13_Current, OUTPUT);

  // Ajout des tâches
  scheduler.add(task_control, frequency::control);
  scheduler.add(task_debugPrint, frequency::debug);

  // Configuration du régulateur PID
  pid.setWrap(-180, 180);
  pid.setSetpoint(0);
  pid.setP(1);
  pid.setI(1);
  pid.setD(1);

  // Configuration des capteurs
  infrared.setFusionCoefficients(distanceCoefsLeft, distanceCoefsRight, nBits);
  encoder.initialize(pin::encoderA, pin::encoderB, &changeChA, &changeChB);
}

void loop() {
  // Exécute le gestionnaire de tâches aussi vite que possible.
  // Celui-ci s'occupe de lancer les tâches au bon moments.
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

// Fréquence: 100 Hz
inline void task_debugPrint() {
  // Serial.print();
  // Serial.print(",");
  // Serial.println();
}

// Fréquence: 500 Hz
inline void task_control() {
  float position = infrared.update();
  float mesure   = encoder.getRadians();
  float current  = pid.update(position);

  float currentValue = abs(current);
  // Limit speed to [0; 10] out of [0; 255].
  currentValue = currentValue > 10 ? 10 : currentValue;

  // Mise à jour de la consigne en courant du variateur
  analogWrite(pin::var13_Current, currentValue);
}