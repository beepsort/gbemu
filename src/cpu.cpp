#include "cpu.h"
#include "cpu_instruction_decode.h"

GAMEBOY::Cpu::Cpu(ROMDATA& rom)
    : addressDispatcher(rom)
{}

/**
 * @brief Advance 1 M-Cycle
 * M-cycles are short for memory cycle and are how long it takes
 * for a single memory read or write to take place. An M-cycle
 * totals 4 CPU clock cycles, also called T-cycles
 */
const GAMEBOY::CpuRegisters& GAMEBOY::Cpu::tick()
{
    if (currentInstruction == nullptr &&
        registers.IME &&
        interruptHandler.isQueued(addressDispatcher))
    {
        auto interruptType = interruptHandler.pop(addressDispatcher);
        uint8_t interruptTypeMask = (uint8_t)interruptType;
        uint8_t enabledTypes = addressDispatcher.read(INTERRUPT_ENABLE);
        if (interruptTypeMask & enabledTypes)
        {
            currentInstruction =
                new InterruptHandler::ServiceRoutine(
                    registers,
                    addressDispatcher,
                    interruptType
                );
        }
    }
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
    if (instruction_result != InstructionResult::STOP)
    {
        Timer::getInstance().tick(addressDispatcher);
    }
    return registers;
}

void GAMEBOY::Cpu::report()
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "PC: %04X\n", *registers.PC);
}
