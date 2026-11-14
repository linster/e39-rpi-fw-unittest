//
// Host stand-in for the Pico SDK's pico/multicore.h lockout API. Core 1 is "not running"
// unless a test sets hoststub::core1VictimInitialized.
//

#ifndef E39_HOSTSTUB_PICO_MULTICORE_H
#define E39_HOSTSTUB_PICO_MULTICORE_H

#include <cstdint>

namespace hoststub {
    extern bool core1VictimInitialized;
    extern bool lockoutStartSucceeds;
    extern int lockoutStarts;
    extern int lockoutEnds;
}

inline void multicore_lockout_victim_init() { hoststub::core1VictimInitialized = true; }
inline bool multicore_lockout_victim_is_initialized(unsigned core) { return core == 1 && hoststub::core1VictimInitialized; }
inline bool multicore_lockout_start_timeout_us(uint64_t) { hoststub::lockoutStarts++; return hoststub::lockoutStartSucceeds; }
inline void multicore_lockout_end_blocking() { hoststub::lockoutEnds++; }

#endif
