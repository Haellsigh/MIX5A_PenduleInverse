#include "configuration.hh"

#include "src/controllers/pid.hh"
#include "src/sensors/IncrementalEncoder.hh"
#include "src/sensors/SensorFusion.hh"

#include "src/time.hh"

using namespace ip::config;
using namespace ip::sensors;
using namespace ip::controllers;

SensorFusion       sensors(pinLeftSensor, pinRightSensor, nBits);
IncrementalEncoder encoder;
Pid                pid(100);

float position = 0;

void changeChA() {
  encoder.handleChangeChA();
}

void changeChB() {
  encoder.handleChangeChB();
}

class Task {
 public:
  Task(void (*task)(), uint32_t f) : m_task(task), m_timer(f) {}

  inline void run() {
    if (m_timer.update())
      m_task();
  }

 private:
  void (*m_task)();
  ip::time::Timer m_timer;
};

Task task_updatePosition(updatePosition, 100);
Task task_serialPrint(serialPrint, 100);

void setup() {
  ip::time::init();

  pid.setP(1);
  pid.setSetpoint(0);

  Serial.begin(115200);

  sensors.setFusionCoefficients(distanceCoefsLeft, distanceCoefsRight, nBits);
  encoder.initialize(pinEncA, pinEncB, &changeChA, &changeChB);

  pinMode(pinVar11_Enable, OUTPUT);
  pinMode(pinVar12_Direction, OUTPUT);
  pinMode(pinSetValueSpeed, OUTPUT);

  digitalWrite(pinVar11_Enable, HIGH);
}

float pidoutput = 0;

void loop() {
  task_updatePosition.run();
  task_serialPrint.run();
}

int x = 0;

void serialPrint() {
  Serial.println(pidoutput);
}

void updatePosition() {
  position = sensors.update();
  pid.update(encoder.getSteps(), &pidoutput);

  float   speed          = pidoutput;
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