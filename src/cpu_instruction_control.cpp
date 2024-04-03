#include "cpu_instruction_control.h"

CPU::InstructionResult CPU::HALT::tick()
{
    uint8_t IE = memory.read(MEMORY::INTERRUPT_ENABLE);
    uint8_t IF = memory.read(MEMORY::INTERRUPT_FLAG);
    bool interrupt_pending = IE & IF;
    if (registers.IME)
    {
        return InstructionResult::HALT;
    }
    else if (interrupt_pending)
    {
        if (step == 0)
        {
            // Waste an M-cycle. This is a replication of a hardware bug
            return InstructionResult::RUNNING;
        }
        else if (step == 1)
        {
            // We have either waited an extra M-cycle with an already pending interrupt
            // or we have waited until a new interrupt arrived
            // Interrupt is not handled as the Interrupt Master Enable is not set
            ++*registers.PC;
            return InstructionResult::FINISHED;
        }
        step = 1;
    }
    // Wait until an interrupt is available to be handled
    return InstructionResult::RUNNING;
}

CPU::InstructionResult CPU::STOP::tick()
{
    ++*registers.PC;
    return InstructionResult::STOP;
}

CPU::InstructionResult CPU::JP_HL::tick()
{
    *pc = *src;
    return InstructionResult::FINISHED;
}

bool CPU::cond_TRUE(CPU::CpuRegisters& registers)
{
    return true;
}

bool CPU::cond_NZ(CPU::CpuRegisters& registers)
{
    return !registers.get_flag_zero();
}

bool CPU::cond_NC(CPU::CpuRegisters& registers)
{
    return !registers.get_flag_carry();
}

bool CPU::cond_Z(CPU::CpuRegisters& registers)
{
    return registers.get_flag_zero();
}

bool CPU::cond_C(CPU::CpuRegisters& registers)
{
    return registers.get_flag_carry();
}

CPU::InstructionResult CPU::JP_NN::tick()
{
    switch (step++)
    {
    case 0:
        jump_addr = (uint16_t) memory.read(++*registers.PC);
        return InstructionResult::RUNNING;
    case 1:
        jump_addr |= ((uint16_t) memory.read(++*registers.PC)) << 8;
        return InstructionResult::RUNNING;
    case 2:
        if (condition(registers))
            return InstructionResult::RUNNING; // delay instruction prefetching as we will change PC
        else
            return InstructionResult::FINISHED; // immediately start instruction prefetching
    case 3:
        *registers.PC = jump_addr;
    default:
        return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::JR_N::tick()
{
    switch (step++)
    {
    case 0:
        jump_offset = memory.read(++*registers.PC);
        return InstructionResult::RUNNING;
    case 1:
        if (condition(registers))
            return InstructionResult::RUNNING; // delay instruction prefetching as we will change PC
        else
            return InstructionResult::FINISHED; // immediately start instruction prefetching
    case 2:
        if (jump_offset >= 0)
        {
            *registers.PC += jump_offset;
        }
        else
        {
            uint16_t abs_offset = (uint16_t) -jump_offset;
            *registers.PC += abs_offset;
        }
    default:
        return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::SCF::tick()
{
    registers.set_flag_carry(true);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::CCF::tick()
{
    registers.set_flag_carry(false);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::DI::tick()
{
    registers.IME = false;
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::EI::tick()
{
    registers.IME = true;
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::RET::tick()
{
    switch(step++)
    {
        case 0:
            return CPU::InstructionResult::RUNNING;
        case 1:
            jump_addr = memory.read((*registers.SP)++);
            return CPU::InstructionResult::RUNNING;
        case 2:
            jump_addr |= memory.read((*registers.SP)++) << 8;
            return CPU::InstructionResult::RUNNING;
        case 3:
            *registers.PC = jump_addr;
        default:
            return CPU::InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::RET_CC::tick()
{
    switch(step++)
    {
        case 0:
            return InstructionResult::RUNNING;
        case 1:
            if (condition(registers))
                return InstructionResult::RUNNING; // delay instruction prefetching as we will change PC
            else
                return InstructionResult::FINISHED; // immediately start instruction prefetching
        case 2:
            jump_addr = memory.read((*registers.SP)++);
        case 3:
            jump_addr |= memory.read((*registers.SP)++) << 8;
        case 4:
        default:
            *registers.PC = jump_addr;
            return InstructionResult::FINISHED;
    }
}
