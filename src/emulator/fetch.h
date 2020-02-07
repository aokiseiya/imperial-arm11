#ifndef ARM11_35_FETCH_H
#define ARM11_35_FETCH_H

#include "emulator.h"

inline void fetch(Emulator *emulator) {
  emulator->fetched = get_memory(emulator, emulator->registers[PC]);
  emulator->registers[PC] += 4;
  emulator->decode_flag = true;
}

#endif //ARM11_35_FETCH_H
