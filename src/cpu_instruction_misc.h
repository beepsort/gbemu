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

    class RLC_absHL: public CpuInstruction
    {
        private:
            CpuRegisters& registers;
            MEMORY::AddressDispatcher& memory;
            uint16_t shift_register;
            uint8_t step = 0;
        public:
            RLC_absHL(CpuRegisters& registers, MEMORY::AddressDispatcher& memory)
            : registers(registers), memory(memory) {}
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

    class RRC_absHL: public CpuInstruction
    {
        private:
            CpuRegisters& registers;
            MEMORY::AddressDispatcher& memory;
            uint8_t shift_register;
            uint8_t step = 0;
        public:
            RRC_absHL(CpuRegisters& registers, MEMORY::AddressDispatcher& memory)
            : registers(registers), memory(memory) {}
            InstructionResult tick();
    };

    class RL: public CpuInstruction
    {
        private:
            CpuRegisters& registers;
            uint8_t* target;
        public:
            RL(CpuRegisters& registers, uint8_t* target)
            : registers(registers), target(target) {}
            InstructionResult tick();
    };

    class RL_absHL: public CpuInstruction
    {
        private:
            CpuRegisters& registers;
            MEMORY::AddressDispatcher& memory;
            uint8_t result;
            uint8_t step = 0;
        public:
            RL_absHL(CpuRegisters& registers, MEMORY::AddressDispatcher& memory)
            : registers(registers), memory(memory) {}
            InstructionResult tick();
    };

    class RR: public CpuInstruction
    {
        private:
            CpuRegisters& registers;
            uint8_t* target;
        public:
            RR(CpuRegisters& registers, uint8_t* target)
            : registers(registers), target(target) {}
            InstructionResult tick();
    };

    class RR_absHL: public CpuInstruction
    {
        private:
            CpuRegisters& registers;
            MEMORY::AddressDispatcher& memory;
            uint8_t result;
            uint8_t step = 0;
        public:
            RR_absHL(CpuRegisters& registers, MEMORY::AddressDispatcher& memory)
            : registers(registers), memory(memory) {}
            InstructionResult tick();
    };

    class SLA_r: public CpuInstruction
    {
        private:
            CpuRegisters& registers;
            uint8_t* target;
        public:
            SLA_r(CpuRegisters& registers, uint8_t* target)
            : registers(registers), target(target) {}
            InstructionResult tick();
    };

    class SRA_r: public CpuInstruction
    {
        private:
            CpuRegisters& registers;
            uint8_t* target;
        public:
            SRA_r(CpuRegisters& registers, uint8_t* target)
            : registers(registers), target(target) {}
            InstructionResult tick();
    };
};

#endif
