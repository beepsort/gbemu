#include "gameboy/cpu.h"
#include "gameboy/cpu_instruction_decode.h"

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
        interruptHandler.isQueued(memory))
    {
        auto interruptType = interruptHandler.pop(memory);
        uint8_t interruptTypeMask = (uint8_t)interruptType;
        uint8_t enabledTypes = memory.read(INTERRUPT_ENABLE);
        if (interruptTypeMask & enabledTypes)
        {
            currentInstruction =
                new InterruptHandler::ServiceRoutine(
                    registers,
                    memory,
                    interruptType
                );
        }
    }
    if (currentInstruction == nullptr)
    {
        uint8_t opcode = memory.read(*registers.PC);
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "opcode: %02X\n", opcode);
        currentInstruction = decode_opcode(opcode, registers, memory);
    }
    InstructionResult instruction_result = currentInstruction->tick();
    if (instruction_result == InstructionResult::FINISHED)
    {
        delete currentInstruction;
        currentInstruction = nullptr;
    }
    if (instruction_result != InstructionResult::STOP)
    {
        Timer::getInstance().tick(memory);
    }
    // Exit HALT on button press
    if (instruction_result == InstructionResult::HALT)
    {
        uint8_t irq = memory.read(INTERRUPT_FLAG);
        if (irq & 0x10)
        {
            delete currentInstruction;
            currentInstruction = nullptr;
        }
    }
    return registers;
}

