#include "cpu.h"
#include "cpu_instruction_decode.h"

CPU::Cpu::Cpu(ROMDATA& rom)
    : addressDispatcher(rom)
{}

/**
 * @brief Advance 1 M-Cycle
 * M-cycles are short for memory cycle and are how long it takes
 * for a single memory read or write to take place. An M-cycle
 * totals 4 CPU clock cycles, also called T-cycles
 */
void CPU::Cpu::tick()
{
    if (currentInstruction == nullptr)
    {
        currentInstruction = decode_opcode(addressDispatcher.read(*(registers.PC++)), registers, addressDispatcher);
    }
}