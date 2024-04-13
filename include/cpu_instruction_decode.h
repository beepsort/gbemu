#ifndef __CPU_INSTRUCTION_DECODE_H__
#define __CPU_INSTRUCTION_DECODE_H__

#include <stdint.h>

#include "cpu_instruction.h"
#include "cpu_registers.h"
#include "gbmemory.h"

namespace GAMEBOY
{
    CpuInstruction* decode_opcode(uint8_t opcode, CpuRegisters& registers, AddressDispatcher& memory);
    CpuInstruction* decode_opcode_prefix(uint8_t opcode, CpuRegisters& registers, AddressDispatcher& memory);
};

#endif
