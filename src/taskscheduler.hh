#ifndef PENDULEINVERSE_TASKSCHEDULER_H
#define PENDULEINVERSE_TASKSCHEDULER_H

#include <array>
#include "time.hh"

namespace ip {

template <uint8_t n>
class TaskScheduler {
  using Task = void (*)();

 public:
  /**
   * \brief Planificateur de tâches
   *
   * Construit un planificateur de tâches. Le paramètre n est le nombre de tâche qui
   * seront ajoutées. Pour ajouter des tâches, appeler la fonction add(Task, frequency).
   *
   * \tparam n Le nombre de tâches.
   */
  TaskScheduler() = default;

  void add(Task t, const uint32_t frequency) {
    if (m_nTask >= n)
      return;

    m_tasks[m_nTask]       = t;
    m_delayCycles[m_nTask] = F_CPU / frequency;
    m_lastRan[m_nTask]     = time::ticks();

    m_nTask++;
  }

  inline void run() {
    const uint32_t tick = time::ticks();
    for (uint16_t i = 0; i < m_nTask; i++) {
      if (tick - m_lastRan.at(i) >= m_delayCycles.at(i)) {
        m_lastRan[i] = tick;
        m_tasks.at(i)();
      }
    }
  }

 private:
  uint16_t m_nTask = 0;

  std::array<Task, n>     m_tasks;
  std::array<uint32_t, n> m_lastRan;
  std::array<uint32_t, n> m_delayCycles;
};

}  // namespace ip

#endif  // PENDULEINVERSE_TASKSCHEDULER_H