#include "configuration.hh"

#include "src/sensors/IncrementalEncoder.hh"
#include "src/sensors/SensorFusion.hh"
#include "src/time.h"

using namespace ip::config;
using namespace ip::sensors;

SensorFusion sensors(pinLeftSensor, pinRightSensor, nBits);
IncrementalEncoder encoder;

void changeChA() {
  encoder.handleChangeChA();
}

void changeChB() {
  encoder.handleChangeChB();
}

bool state7 = false, state8 = false;

class Task {
 public:
  Task(void (*task)(), uint32_t f) : m_task(task), m_timer(f) {}

  inline void run() {
    if (m_timer.update())
      m_task();
  }

 private:
  ip::time::Timer m_timer;
  void (*m_task)();
};

Task freq7(fct1, 500);
Task freq8(fct2, 500);

void setup() {
  ip::time::init();

  Serial.begin(115200);

  sensors.setFusionCoefficients(distanceCoefs, nBits);
  encoder.initialize(pinEncA, pinEncB, &changeChA, &changeChB);

  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop() {
  freq7.run();
  freq8.run();
}

void fct1() {}

void fct2() {}