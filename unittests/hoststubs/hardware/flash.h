//
// Host stand-in for the Pico SDK's hardware/flash.h, so FlashConfigurationStore can be
// compiled and tested on the host. "Flash" is a RAM buffer (hostFlash) the size of the
// persistent block; XIP_BASE/PICO_FLASH_SIZE_BYTES are chosen so the firmware's address
// arithmetic (XIP_BASE + PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE) lands on its start.
//

#ifndef E39_HOSTSTUB_HARDWARE_FLASH_H
#define E39_HOSTSTUB_HARDWARE_FLASH_H

#include <cstddef>
#include <cstdint>

#define FLASH_PAGE_SIZE (1u << 8)
#define FLASH_SECTOR_SIZE (1u << 12)

namespace hoststub {
    extern uint8_t hostFlash[FLASH_SECTOR_SIZE];
    extern int eraseCalls;
    extern int programCalls;
    //Set to 0xFF (erased) and zero the call counters.
    void resetHostFlash();
}

#define PICO_FLASH_SIZE_BYTES FLASH_SECTOR_SIZE
#define XIP_BASE ((uintptr_t) hoststub::hostFlash)

//Same contracts as the SDK: offsets are from the start of flash, erase count must be a
//multiple of FLASH_SECTOR_SIZE, program count a multiple of FLASH_PAGE_SIZE. Violations abort.
void flash_range_erase(uint32_t flash_offs, size_t count);
void flash_range_program(uint32_t flash_offs, const uint8_t *data, size_t count);

#endif
