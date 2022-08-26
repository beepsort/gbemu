#ifndef __CPU_INSTRUCTION_CONTROL_H__
#define __CPU_INSTRUCTION_CONTROL_H__

#include <stdint.h>

#include "cpu_instruction.h"
#include "cpu_registers.h"
#include "memory.h"

namespace CPU
{
    class JP_HL: public CpuInstruction
    {
    private:
        uint16_t* pc;
        uint16_t* src;
    public:
        JP_HL(uint16_t* pc, uint16_t* src)
        : pc(pc), src(src) {}
        bool tick();
    };

    bool cond_TRUE(CPU::CpuRegisters& registers);
    bool cond_NZ(CPU::CpuRegisters& registers);
    bool cond_NC(CPU::CpuRegisters& registers);
    bool cond_Z(CPU::CpuRegisters& registers);
    bool cond_C(CPU::CpuRegisters& registers);

    class JP_NN: public CpuInstruction
    {
    private:
        bool (*const condition)(CpuRegisters&);
        uint16_t jump_addr = 0;
        uint8_t step = 0;
        CPU::CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
    public:
        JP_NN(CPU::CpuRegisters& registers, MEMORY::AddressDispatcher& memory, bool (*const condition)(CpuRegisters&))
        : registers(registers), memory(memory), condition(condition) {}
        bool tick();
    };

    class JR_N: public CpuInstruction
    {
    private:
        bool (*const condition)(CpuRegisters&);
        int16_t jump_offset = 0;
        uint8_t step = 0;
        CPU::CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
    public:
        JR_N(CPU::CpuRegisters& registers, MEMORY::AddressDispatcher& memory, bool (*const condition)(CpuRegisters&))
        : registers(registers), memory(memory), condition(condition) {}
        bool tick();
    };
};

#endif