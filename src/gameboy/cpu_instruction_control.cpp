#include "gameboy/cpu_instruction_control.h"
#include "gameboy/memory_io.h"

GAMEBOY::InstructionResult GAMEBOY::HALT::tick()
{
    uint8_t IE = memory.read(INTERRUPT_ENABLE);
    uint8_t IF = memory.read(INTERRUPT_FLAG);
    bool interrupt_pending = IE & IF;
    if (registers.IME)
    {
        ++*registers.PC;
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

GAMEBOY::InstructionResult GAMEBOY::STOP::tick()
{
    if (step == 0)
    {
        ++*registers.PC;
        memory.write(GAMEBOY::IOHandler::TIMER_REG_DIV, 0);
        step++;
    }
    return InstructionResult::STOP;
}

GAMEBOY::InstructionResult GAMEBOY::JP_HL::tick()
{
    *pc = *src;
    return InstructionResult::FINISHED;
}

bool GAMEBOY::cond_TRUE(GAMEBOY::CpuRegisters&)
{
    return true;
}

bool GAMEBOY::cond_NZ(GAMEBOY::CpuRegisters& registers)
{
    return !registers.get_flag_zero();
}

bool GAMEBOY::cond_NC(GAMEBOY::CpuRegisters& registers)
{
    return !registers.get_flag_carry();
}

bool GAMEBOY::cond_Z(GAMEBOY::CpuRegisters& registers)
{
    return registers.get_flag_zero();
}

bool GAMEBOY::cond_C(GAMEBOY::CpuRegisters& registers)
{
    return registers.get_flag_carry();
}

GAMEBOY::InstructionResult GAMEBOY::JP_NN::tick()
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
        {
            return InstructionResult::RUNNING; // delay instruction prefetching as we will change PC
        }
        else
        {
            ++*registers.PC;
            return InstructionResult::FINISHED; // immediately start instruction prefetching
        }
    case 3:
        *registers.PC = jump_addr;
        [[fallthrough]];
    default:
        return InstructionResult::FINISHED;
    }
}

GAMEBOY::InstructionResult GAMEBOY::JR_N::tick()
{
    switch (step++)
    {
    case 0:
        jump_offset = (int8_t)memory.read(++*registers.PC);
        return InstructionResult::RUNNING;
    case 1:
        ++*registers.PC;
        if (condition(registers))
        {
            return InstructionResult::RUNNING; // delay instruction prefetching as we will change PC
        }
        else
        {
            return InstructionResult::FINISHED; // immediately start instruction prefetching
        }
    case 2:
        *registers.PC += jump_offset;
        [[fallthrough]];
    default:
        return InstructionResult::FINISHED;
    }
}

GAMEBOY::InstructionResult GAMEBOY::SCF::tick()
{
    registers.set_flag_carry(true);
    registers.set_flag_halfcarry(false);
    registers.set_flag_sub(false);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::CCF::tick()
{
    registers.set_flag_carry(!registers.get_flag_carry());
    registers.set_flag_halfcarry(false);
    registers.set_flag_sub(false);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::DI::tick()
{
    registers.IME = false;
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::EI::tick()
{
    registers.IME = true;
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::RET::tick()
{
    switch(step++)
    {
        case 0:
            return InstructionResult::RUNNING;
        case 1:
            jump_addr = memory.read((*registers.SP)++);
            return InstructionResult::RUNNING;
        case 2:
            jump_addr |= memory.read((*registers.SP)++) << 8;
            return InstructionResult::RUNNING;
        case 3:
            *registers.PC = jump_addr;
            [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

GAMEBOY::InstructionResult GAMEBOY::RET_CC::tick()
{
    switch(step++)
    {
        case 0:
            return InstructionResult::RUNNING;
        case 1:
            if (condition(registers))
            {
                return InstructionResult::RUNNING; // delay instruction prefetching as we will change PC
            }
            else
            {
                step = 10;
                ++*registers.PC;
                return InstructionResult::FINISHED; // immediately start instruction prefetching
            }
        case 2:
            jump_addr = memory.read((*registers.SP)++);
            return InstructionResult::RUNNING;
        case 3:
            jump_addr |= memory.read((*registers.SP)++) << 8;
            return InstructionResult::RUNNING;
        case 4:
            *registers.PC = jump_addr;
            [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

GAMEBOY::InstructionResult GAMEBOY::RETI::tick()
{
    switch(step++)
    {
        case 0:
            return InstructionResult::RUNNING;
        case 1:
            jump_addr = memory.read((*registers.SP)++);
            return InstructionResult::RUNNING;
        case 2:
            jump_addr |= memory.read((*registers.SP)++) << 8;
            return InstructionResult::RUNNING;
        case 3:
            *registers.PC = jump_addr;
            registers.IME = true;
            [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

GAMEBOY::InstructionResult GAMEBOY::CALL_NN::tick()
{
    switch (step++)
    {
    case 0:
        return InstructionResult::RUNNING;
    case 1:
        jump_addr = (uint16_t) memory.read(++*registers.PC);
        return InstructionResult::RUNNING;
    case 2:
        jump_addr |= ((uint16_t) memory.read(++*registers.PC)) << 8;
        ++*registers.PC;
        if (condition(registers))
        {
            return InstructionResult::RUNNING; // delay instruction prefetching as we will change PC
        }
        else
        {
            return InstructionResult::FINISHED; // immediately start instruction prefetching
        }
    case 3:
        return InstructionResult::RUNNING;
    case 4:
        memory.write(--*registers.SP, (uint8_t)((*registers.PC) >> 8));
        return InstructionResult::RUNNING;
    case 5:
        memory.write(--*registers.SP, (uint8_t)*registers.PC);
        *registers.PC = jump_addr;
        [[fallthrough]];
    default:
        return InstructionResult::FINISHED;
    }
}

GAMEBOY::InstructionResult GAMEBOY::RST::tick()
{
    switch (step++)
    {
    case 0:
        return InstructionResult::RUNNING;
    case 1:
        memory.write(--*registers.SP, (uint8_t)((++*registers.PC) >> 8));
        return InstructionResult::RUNNING;
    case 2:
        memory.write(--*registers.SP, (uint8_t)*registers.PC);
        *registers.PC = (uint16_t)jump_lsb;
        return InstructionResult::RUNNING;
    case 3:
        [[fallthrough]];
    default:
        return InstructionResult::FINISHED;
    }
}

