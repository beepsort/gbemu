#ifndef __CPU_INSTRUCTION_DECODE_H__
#define __CPU_INSTRUCTION_DECODE_H__

#include <stdint.h>

#include "gameboy/cpu_instruction.h"
#include "gameboy/cpu_registers.h"
#include "gameboy/memory.h"

namespace GAMEBOY
{
    CpuInstruction* decode_opcode(uint8_t opcode, CpuRegisters& registers, AddressDispatcher& memory);
    CpuInstruction* decode_opcode_prefix(uint8_t opcode, CpuRegisters& registers, AddressDispatcher& memory);
};

#endif
