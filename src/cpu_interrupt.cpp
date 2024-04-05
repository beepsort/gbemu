#include "cpu_interrupt.h"

bool CPU::InterruptHandler::isQueued(MEMORY::AddressDispatcher& memory)
{
    uint8_t queue = memory.read(MEMORY::INTERRUPT_FLAG);
    uint8_t enabled = memory.read(MEMORY::INTERRUPT_ENABLE);
    return queue & enabled;
}

CPU::InterruptType CPU::InterruptHandler::pop(MEMORY::AddressDispatcher& memory)
{
    uint8_t current_flags = memory.read(MEMORY::INTERRUPT_FLAG);
    uint8_t enabled = memory.read(MEMORY::INTERRUPT_ENABLE);
    uint8_t enabled_active_flags = current_flags & enabled;
    for (uint8_t type = 1; type<=0x10; type<<=1)
    {
        if (enabled_active_flags & type)
        {
            uint8_t updated_flags = current_flags & ~type;
            memory.write(MEMORY::INTERRUPT_FLAG, updated_flags);
            return static_cast<InterruptType>(type);
        }
    }
    return {};
}

CPU::InstructionResult CPU::InterruptHandler::ServiceRoutine::tick()
{
    switch (step++)
    {
        case 0:
        case 1:
            return InstructionResult::RUNNING;
        case 2:
        {
            uint8_t pc_msb = (uint8_t)((*registers.PC & 0xFF00) >> 8);
            memory.write(--*registers.SP, pc_msb);
            return InstructionResult::RUNNING;
        }
        case 3:
        {
            uint8_t pc_lsb = (uint8_t)(*registers.PC & 0x00FF);
            memory.write(--*registers.SP, pc_lsb);
            return InstructionResult::RUNNING;
        }
        case 4:
        {
            uint8_t handlerAddress = 0;
            switch (interruptType)
            {
                case InterruptType::VBLANK:
                    handlerAddress = 0x40;
                    break;
                case InterruptType::LCD:
                    handlerAddress = 0x48;
                    break;
                case InterruptType::TIMER:
                    handlerAddress = 0x50;
                    break;
                case InterruptType::SERIAL:
                    handlerAddress = 0x58;
                    break;
                case InterruptType::JOYPAD:
                    handlerAddress = 0x60;
                    break;
            }
            *registers.PC = (uint8_t) handlerAddress;
            [[fallthrough]];
        }
        default:
            return InstructionResult::FINISHED;
    }
}
