#include "cpu_instruction_misc.h"

CPU::InstructionResult CPU::NOP::tick()
{
    // Do no useful work, only inc PC
    ++*registers.PC;
    return InstructionResult::FINISHED;
}
