#ifndef __CPU_INSTRUCTION_DECODE_H__
#define __CPU_INSTRUCTION_DECODE_H__

#include <stdint.h>

#include "cpu_instruction.h"
#include "cpu_registers.h"
#include "gbmemory.h"

namespace CPU
{
    CpuInstruction* decode_opcode(uint8_t opcode, CPU::CpuRegisters& registers, MEMORY::AddressDispatcher& memory);
    CpuInstruction* decode_opcode_prefix(uint8_t opcode, CPU::CpuRegisters& registers, MEMORY::AddressDispatcher& memory);
};

#endif
