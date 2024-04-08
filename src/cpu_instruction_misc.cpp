#include "cpu_instruction_misc.h"
#include "cpu_instruction_decode.h"

CPU::InstructionResult CPU::NOP::tick()
{
    // Do no useful work, only inc PC
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::CB_PREFIX::tick()
{
    if (instruction == nullptr)
    {
        uint8_t opcode = memory.read(++*registers.PC);
        instruction = CPU::decode_opcode_prefix(opcode, registers, memory);
        return InstructionResult::RUNNING;
    }
    else
    {
        return instruction->tick();
    }
}

CPU::CB_PREFIX::~CB_PREFIX()
{
    if (instruction != nullptr)
    {
        delete instruction;
    }
}

CPU::InstructionResult CPU::RLC::tick()
{
    uint16_t shift_register = (uint16_t)*target;
    shift_register <<= 1;
    *target = (uint8_t)shift_register | (uint8_t)(shift_register>>8);
    registers.set_flag_carry(*target&0x01);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::RLC_absHL::tick()
{
    switch (step++)
    {
        case 0:
            return InstructionResult::RUNNING;
        case 1:
            shift_register = (uint16_t)memory.read(*registers.HL);
            return InstructionResult::RUNNING;
        case 2:
        {
            shift_register <<= 1;
            uint8_t result = (uint8_t)shift_register | (uint8_t)(shift_register>>8);
            registers.set_flag_carry(result&0x01);
            memory.write(*registers.HL, result);
            ++*registers.PC;
        }
        [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::RRC::tick()
{
    uint8_t shift_register = *target>>1;
    *target = shift_register | *target<<7;
    registers.set_flag_carry(*target&0x80);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::RRC_absHL::tick()
{
    switch (step++)
    {
        case 0:
            return InstructionResult::RUNNING;
        case 1:
            shift_register = memory.read(*registers.HL);
            return InstructionResult::RUNNING;
        case 2:
        {
            uint8_t result = shift_register>>1;
            result |= shift_register<<7;
            registers.set_flag_carry(result&0x80);
            memory.write(*registers.HL, result);
            ++*registers.PC;
        }
        [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::RL::tick()
{
    *target <<= 1;
    *target |= registers.get_flag_carry() ? 0x01 : 0x00;
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::RR::tick()
{
    *target >>= 1;
    *target |= registers.get_flag_carry() ? 0x80 : 0x00;
    ++*registers.PC;
    return InstructionResult::FINISHED;
}
