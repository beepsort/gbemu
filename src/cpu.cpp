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
const CPU::CpuRegisters& CPU::Cpu::tick()
{
    if (currentInstruction == nullptr)
    {
        uint8_t opcode = addressDispatcher.read(*registers.PC);
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "opcode: %02X\n", opcode);
        currentInstruction = decode_opcode(opcode, registers, addressDispatcher);
    }
    InstructionResult instruction_result = currentInstruction->tick();
    if (instruction_result == InstructionResult::FINISHED)
    {
        delete currentInstruction;
        currentInstruction = nullptr;
    }
    return registers;
}

void CPU::Cpu::report()
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "PC: %04X\n", *registers.PC);
}
