#ifndef __CPU_INSTRUCTION_MISC_H__
#define __CPU_INSTRUCTION_MISC_H__

#include "cpu_instruction.h"
#include "cpu_registers.h"
#include "gbmemory.h"

namespace CPU
{
    class NOP: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
    public:
        NOP(CpuRegisters& registers)
        : registers(registers) {}
        InstructionResult tick();
    };

    class CB_PREFIX: public CpuInstruction
    {
        private:
            CpuRegisters& registers;
            MEMORY::AddressDispatcher& memory;
            CpuInstruction* instruction = nullptr;
        public:
            CB_PREFIX(CpuRegisters& registers, MEMORY::AddressDispatcher& memory)
            : registers(registers), memory(memory) {}
            ~CB_PREFIX();
            InstructionResult tick();
    };

    class RLC: public CpuInstruction
    {
        private:
            CpuRegisters& registers;
            uint8_t* target;
        public:
            RLC(CpuRegisters& registers, uint8_t* target)
            : registers(registers), target(target) {}
            InstructionResult tick();
    };

    class RRC: public CpuInstruction
    {
        private:
            CpuRegisters& registers;
            uint8_t* target;
        public:
            RRC(CpuRegisters& registers, uint8_t* target)
            : registers(registers), target(target) {}
            InstructionResult tick();
    };
};

#endif
