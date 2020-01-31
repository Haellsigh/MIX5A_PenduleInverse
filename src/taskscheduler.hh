#ifndef PENDULEINVERSE_TASKSCHEDULER_H
#define PENDULEINVERSE_TASKSCHEDULER_H

#include "time.hh"

namespace ip {

/**
 * \brief Planificateur de tâches
 *
 * Construit un planificateur de tâches. Le paramètre n est le nombre de tâche qui
 * seront ajoutées. Pour ajouter des tâches, appeler la fonction add(Task, frequency).
 * Appeller run() dans une boucle infinie.
 *
 * \tparam n Le nombre de tâches.
 */
template <uint8_t n>
class TaskScheduler {
  using Task = void (*)();

 public:
  /**
   * \brief Ajoute une tâche au planificateur.
   */
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
      if (tick - m_lastRan[i] >= m_delayCycles[i]) {
        m_lastRan[i] = tick;
        m_tasks[i]();
      }
    }
  }

 private:
  uint16_t m_nTask = 0;

  Task     m_tasks[n];
  uint32_t m_lastRan[n];
  uint32_t m_delayCycles[n];
};

}  // namespace ip

#endif  // PENDULEINVERSE_TASKSCHEDULER_H