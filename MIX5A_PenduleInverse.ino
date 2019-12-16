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

template <typename Function>
class Task {
 public:
  Task(Function task, uint32_t f) : m_task(task) { m_timer.setFrequency(f); }

  inline void run() {
    if (m_timer.update())
      m_task();
  }

 private:
  ip::time::Timer m_timer;
  Function m_task;
};

Task<void (*)()> freq7(fct1, 500);
Task<void (*)()> freq8(fct2, 500);

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

void fct1() {
  digitalWrite(7, state7);
  state7 = !state7;
}

void fct2() {
  digitalWrite(8, state8);
  state8 = !state8;
}