#include "cpu_instruction_misc.h"

bool CPU::NOP::tick()
{
    // Do no useful work, only inc PC
    ++*registers.PC;
    return true;
}