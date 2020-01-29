#include "time.hh"

namespace ip {
namespace time {

volatile uint32_t* DWT_CYCCNT  = (volatile uint32_t*)0xE0001004;
volatile uint32_t* DWT_CONTROL = (volatile uint32_t*)0xE0001000;
volatile uint32_t* SCB_DEMCR   = (volatile uint32_t*)0xE000EDFC;

void init() {
  *SCB_DEMCR |= 0x01000000;
  *DWT_CYCCNT = 0;    // reset the counter
  *DWT_CONTROL |= 1;  // enable the counter
}

}  // namespace time
}  // namespace ip