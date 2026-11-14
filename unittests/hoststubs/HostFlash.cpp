#include "hardware/flash.h"
#include "hardware/sync.h"
#include "pico/multicore.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace hoststub {
    uint8_t hostFlash[FLASH_SECTOR_SIZE];
    int eraseCalls = 0;
    int programCalls = 0;
    int interruptsDisabledDepth = 0;
    bool core1VictimInitialized = false;
    bool lockoutStartSucceeds = true;
    int lockoutStarts = 0;
    int lockoutEnds = 0;

    void resetHostFlash() {
        memset(hostFlash, 0xFF, sizeof hostFlash);
        eraseCalls = programCalls = 0;
        interruptsDisabledDepth = 0;
        core1VictimInitialized = false;
        lockoutStartSucceeds = true;
        lockoutStarts = lockoutEnds = 0;
    }
}

//The linker-script symbol flash_utils.h refers to.
uint8_t ADDR_PERSISTENT[FLASH_SECTOR_SIZE];

static void requireOrAbort(bool ok, const char *what) {
    if (!ok) { fprintf(stderr, "hoststub flash contract violated: %s\n", what); abort(); }
}

void flash_range_erase(uint32_t flash_offs, size_t count) {
    requireOrAbort(flash_offs % FLASH_SECTOR_SIZE == 0, "erase offset not sector aligned");
    requireOrAbort(count % FLASH_SECTOR_SIZE == 0 && count > 0, "erase count not a multiple of FLASH_SECTOR_SIZE");
    requireOrAbort(flash_offs + count <= sizeof hoststub::hostFlash, "erase past end of flash");
    requireOrAbort(hoststub::interruptsDisabledDepth > 0, "erase with interrupts enabled");
    memset(hoststub::hostFlash + flash_offs, 0xFF, count);
    hoststub::eraseCalls++;
}

void flash_range_program(uint32_t flash_offs, const uint8_t *data, size_t count) {
    requireOrAbort(flash_offs % FLASH_PAGE_SIZE == 0, "program offset not page aligned");
    requireOrAbort(count % FLASH_PAGE_SIZE == 0 && count > 0, "program count not a multiple of FLASH_PAGE_SIZE");
    requireOrAbort(flash_offs + count <= sizeof hoststub::hostFlash, "program past end of flash");
    requireOrAbort(hoststub::interruptsDisabledDepth > 0, "program with interrupts enabled");
    //Real NOR flash can only clear bits: programming ANDs into the erased state.
    for (size_t i = 0; i < count; i++) hoststub::hostFlash[flash_offs + i] &= data[i];
    hoststub::programCalls++;
}
