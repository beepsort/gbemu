#ifndef __CPU_INTERRUPT_H__
#define __CPU_INTERRUPT_H__

#include "cpu_instruction.h"
#include "cpu_registers.h"
#include "gbmemory.h"

#include <stdint.h>

namespace GAMEBOY
{
    enum class InterruptType: uint8_t
    {
        VBLANK = 0x1,
        LCD = 0x2,
        TIMER = 0x4,
        SERIAL = 0x8,
        JOYPAD = 0x10
    };
    class InterruptHandler
    {
    public:
        bool isQueued(GAMEBOY::AddressDispatcher& memory);
        InterruptType pop(GAMEBOY::AddressDispatcher& memory);

        class ServiceRoutine: public CpuInstruction
        {
        private:
            CpuRegisters& registers;
            GAMEBOY::AddressDispatcher& memory;
            uint8_t step = 0;
            InterruptType interruptType;
        public:
            ServiceRoutine(CpuRegisters& registers, GAMEBOY::AddressDispatcher& memory, InterruptType interruptType)
            : registers(registers), memory(memory), interruptType(interruptType) {}
            GAMEBOY::InstructionResult tick();
        };
    };
}

#endif
