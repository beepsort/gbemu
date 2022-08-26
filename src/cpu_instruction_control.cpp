#include "cpu_instruction_control.h"

bool CPU::JP_HL::tick()
{
    *pc = *src;
    return true;
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

bool CPU::JP_NN::tick()
{
    switch (step++)
    {
    case 0:
        jump_addr = (uint16_t) memory.read(++*registers.PC);
        return false;
    case 1:
        jump_addr |= ((uint16_t) memory.read(++*registers.PC)) << 8;
        return false;
    case 2:
        if (condition(registers))
            return false; // delay instruction prefetching as we will change PC
        else
            return true; // immediately start instruction prefetching
    case 3:
        *registers.PC = jump_addr;
    default:
        return true;
    }
}

bool CPU::JR_N::tick()
{
    switch (step++)
    {
    case 0:
        jump_offset = memory.read(++*registers.PC);
        return false;
    case 1:
        if (condition(registers))
            return false; // delay instruction prefetching as we will change PC
        else
            return true; // immediately start instruction prefetching
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
        return true;
    }
}