#ifndef __CPU_INTERRUPT_H__
#define __CPU_INTERRUPT_H__

#include "cpu_instruction.h"
#include "cpu_registers.h"
#include "gbmemory.h"

#include <stdint.h>

namespace CPU
{
    enum class InterruptType: uint8_t
    {
        VBLANK = 0x1,
        LCD = 0x2,
        TIMER = 0x4,
        SERIAL = 0x8,
        JOYPAD = 0x16
    };
    class InterruptHandler
    {
    public:
        bool isQueued(MEMORY::AddressDispatcher& memory);
        InterruptType pop(MEMORY::AddressDispatcher& memory);

        class ServiceRoutine: public CpuInstruction
        {
        private:
            CpuRegisters& registers;
            MEMORY::AddressDispatcher& memory;
            uint8_t step = 0;
            InterruptType interruptType;
        public:
            ServiceRoutine(CpuRegisters& registers, MEMORY::AddressDispatcher& memory, InterruptType interruptType)
            : registers(registers), memory(memory), interruptType(interruptType) {}
            CPU::InstructionResult tick();
        };
    };
}

#endif
