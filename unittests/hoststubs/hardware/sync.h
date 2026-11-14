//
// Host stand-in for the Pico SDK's hardware/sync.h (interrupts don't exist on the host).
//

#ifndef E39_HOSTSTUB_HARDWARE_SYNC_H
#define E39_HOSTSTUB_HARDWARE_SYNC_H

#include <cstdint>

namespace hoststub { extern int interruptsDisabledDepth; }

inline uint32_t save_and_disable_interrupts() { return (uint32_t) hoststub::interruptsDisabledDepth++; }
inline void restore_interrupts(uint32_t) { hoststub::interruptsDisabledDepth--; }

#endif
