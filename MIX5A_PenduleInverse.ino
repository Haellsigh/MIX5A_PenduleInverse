#include "configuration.hh"

#include "src/controllers/pid.hh"
#include "src/sensors/IncrementalEncoder.hh"
#include "src/sensors/SensorFusion.hh"
#include "src/taskscheduler.hh"

#include "src/time.hh"

using namespace ip;
using namespace ip::config;

sensors::SensorFusion       infrared(pinLeftSensor, pinRightSensor, nBits);
sensors::IncrementalEncoder encoder;
controllers::Pid            pid(100);

void changeChA() {
  encoder.handleChangeChA();
}

void changeChB() {
  encoder.handleChangeChB();
}

void task_debugPrint() {
  // Serial.print();
  // Serial.print(",");
  // Serial.println();
}

void task_control() {
  float position = infrared.update();
  float speed    = pid.update(encoder.getRadians());

  float   speedValue     = constrain(fabs(speed), 0, 10);
  uint8_t speedDirection = (speed > 0) ? HIGH : LOW;

  analogWrite(pinSetValueSpeed, speedValue);
  digitalWrite(pinVar12_Direction, speedDirection);

  /*x++;
  static constexpr double mult = M_PI * 2. / 1000.;

  // Amplitude
  static constexpr double amplitude = 2. / 100.;

  // Speed in range [-1; 1]
  float speed = sin(x * 2 * mult);
  // Speed in range [0; 255]
  uint8_t speedValue = amplitude * 255. * ((speed + 1.) / 2.);
  // Direction of speed (true = forward)
  uint8_t speedDirection = (speed > 0) ? HIGH : LOW;

  analogWrite(pinSetValueSpeed, speedValue);
  digitalWrite(pinVar12_Direction, speedDirection);
  digitalWrite(pinVar11_Enable, speedDirection);*/
}

void task_updatePID() {}

ip::TaskScheduler<2> scheduler;

void setup() {
  // Initilisation
  ip::time::init();
  Serial.begin(115200);

  // Ajout des tâches
  scheduler.add(task_control, 100);
  scheduler.add(task_debugPrint, 100);

  // Configuration du régulateur PID
  pid.setP(1);
  pid.setSetpoint(0);

  infrared.setFusionCoefficients(distanceCoefsLeft, distanceCoefsRight, nBits);
  encoder.initialize(pinEncA, pinEncB, &changeChA, &changeChB);

  pinMode(pinVar11_Enable, OUTPUT);
  pinMode(pinVar12_Direction, OUTPUT);
  pinMode(pinSetValueSpeed, OUTPUT);

  digitalWrite(pinVar11_Enable, HIGH);
}

void loop() {
  scheduler.run();
}