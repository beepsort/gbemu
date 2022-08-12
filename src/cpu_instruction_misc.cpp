#include "cpu_instruction_misc.h"

bool CPU::NOP::tick()
{
    // do nothing, finish after first M-cycle
    return true;
}