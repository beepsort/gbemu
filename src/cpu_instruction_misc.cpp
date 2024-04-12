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

CPU::InstructionResult CPU::RLC_r::tick()
{
    uint16_t shift_register = (uint16_t)*target;
    shift_register <<= 1;
    *target = (uint8_t)shift_register | (uint8_t)(shift_register>>8);
    registers.set_flag_carry(*target&0x01);
    registers.set_flag_zero(*target==0);
    registers.set_flag_sub(false);
    registers.set_flag_halfcarry(false);
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
            registers.set_flag_zero(result==0);
            registers.set_flag_sub(false);
            registers.set_flag_halfcarry(false);
            memory.write(*registers.HL, result);
            ++*registers.PC;
        }
        [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::RRC_r::tick()
{
    uint8_t shift_register = *target>>1;
    *target = shift_register | *target<<7;
    registers.set_flag_carry(*target&0x80);
    registers.set_flag_zero(*target==0);
    registers.set_flag_sub(false);
    registers.set_flag_halfcarry(false);
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
            registers.set_flag_zero(result==0);
            registers.set_flag_sub(false);
            registers.set_flag_halfcarry(false);
            memory.write(*registers.HL, result);
            ++*registers.PC;
        }
        [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::RL_r::tick()
{
    bool was_carry = registers.get_flag_carry();
    registers.set_flag_carry(*target&0x80);
    *target <<= 1;
    *target |= was_carry ? 0x01 : 0x00;
    registers.set_flag_zero(*target==0);
    registers.set_flag_sub(false);
    registers.set_flag_halfcarry(false);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::RL_absHL::tick()
{
    switch (step++)
    {
        case 0:
            return InstructionResult::RUNNING;
        case 1:
            result = memory.read(*registers.HL);
            return InstructionResult::RUNNING;
        case 2:
        {
            bool was_carry = registers.get_flag_carry();
            registers.set_flag_carry(result&0x80);
            result <<= 1;
            result |= was_carry ? 0x01 : 0x00;
            registers.set_flag_zero(result==0);
            registers.set_flag_sub(false);
            registers.set_flag_halfcarry(false);
            memory.write(*registers.HL, result);
            ++*registers.PC;
        }
        [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::RR_r::tick()
{
    bool was_carry = registers.get_flag_carry();
    registers.set_flag_carry(*target&0x01);
    *target >>= 1;
    *target |= was_carry ? 0x80 : 0x00;
    registers.set_flag_zero(*target==0);
    registers.set_flag_sub(false);
    registers.set_flag_halfcarry(false);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::RR_absHL::tick()
{
    switch (step++)
    {
        case 0:
            return InstructionResult::RUNNING;
        case 1:
            result = memory.read(*registers.HL);
            return InstructionResult::RUNNING;
        case 2:
        {
            bool was_carry = registers.get_flag_carry();
            registers.set_flag_carry(result&0x01);
            result >>= 1;
            result |= was_carry ? 0x80 : 0x00;
            memory.write(*registers.HL, result);
            registers.set_flag_zero(result==0);
            registers.set_flag_sub(false);
            registers.set_flag_halfcarry(false);
            ++*registers.PC;
        }
        [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::SLA_r::tick()
{
    registers.set_flag_carry(*target&0x80);
    *target <<= 1;
    registers.set_flag_zero(*target==0);
    registers.set_flag_sub(false);
    registers.set_flag_halfcarry(false);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::SLA_absHL::tick()
{
    switch (step++)
    {
        case 0:
            return InstructionResult::RUNNING;
        case 1:
            result = memory.read(*registers.HL);
            return InstructionResult::RUNNING;
        case 2:
        {
            registers.set_flag_carry(result&0x80);
            result <<= 1;
            registers.set_flag_zero(result==0);
            registers.set_flag_sub(false);
            registers.set_flag_halfcarry(false);
            memory.write(*registers.HL, result);
            ++*registers.PC;
        }
        [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::SRA_r::tick()
{
    registers.set_flag_carry(*target&0x01);
    uint8_t result = *target >> 1;
    *target = result | (*target & 0x80);
    registers.set_flag_zero(*target==0);
    registers.set_flag_sub(false);
    registers.set_flag_halfcarry(false);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::SRA_absHL::tick()
{
    switch (step++)
    {
        case 0:
            return InstructionResult::RUNNING;
        case 1:
            loaded = memory.read(*registers.HL);
            return InstructionResult::RUNNING;
        case 2:
        {
            registers.set_flag_carry(loaded&0x01);
            uint8_t result = loaded >> 1;
            result |= loaded & 0x80;
            memory.write(*registers.HL, result);
            registers.set_flag_zero(result==0);
            registers.set_flag_sub(false);
            registers.set_flag_halfcarry(false);
            ++*registers.PC;
        }
        [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::SWAP_r::tick()
{
    uint8_t result = *target << 4; // move LSB to MSB
    result |= *target >> 4; // move MSB to LSB
    *target = result;
    registers.set_flag_zero(result==0);
    registers.set_flag_sub(false);
    registers.set_flag_carry(false);
    registers.set_flag_halfcarry(false);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::SWAP_absHL::tick()
{
    switch (step++)
    {
        case 0:
            return InstructionResult::RUNNING;
        case 1:
            loaded = memory.read(*registers.HL);
            return InstructionResult::RUNNING;
        case 2:
        {
            uint8_t result = loaded << 4;
            result |= loaded >> 4;
            memory.write(*registers.HL, result);
            registers.set_flag_zero(result==0);
            registers.set_flag_sub(false);
            registers.set_flag_carry(false);
            registers.set_flag_halfcarry(false);
            ++*registers.PC;
        }
        [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::SRL_r::tick()
{
    registers.set_flag_carry(*target&0x01);
    *target >>= 1;
    registers.set_flag_zero(*target==0);
    registers.set_flag_sub(false);
    registers.set_flag_halfcarry(false);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::SRL_absHL::tick()
{
    switch (step++)
    {
        case 0:
            return InstructionResult::RUNNING;
        case 1:
            result = memory.read(*registers.HL);
            return InstructionResult::RUNNING;
        case 2:
        {
            registers.set_flag_carry(result&0x01);
            result >>= 1;
            memory.write(*registers.HL, result);
            registers.set_flag_zero(result==0);
            registers.set_flag_sub(false);
            registers.set_flag_halfcarry(false);
            ++*registers.PC;
        }
        [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::BIT_r::tick()
{
    uint8_t mask = 0x01 << bitnum;
    registers.set_flag_zero((*target&mask)==0);
    registers.set_flag_sub(false);
    registers.set_flag_halfcarry(true);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::BIT_absHL::tick()
{
    switch (step++)
    {
        case 0:
            return InstructionResult::RUNNING;
        case 1:
        {
            loaded = memory.read(*registers.HL);
            uint8_t mask = 0x01 << bitnum;
            registers.set_flag_zero((loaded&mask)==0);
            registers.set_flag_sub(false);
            registers.set_flag_halfcarry(true);
            ++*registers.PC;
        }
        [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::RES_r::tick()
{
    uint8_t mask = ~(0x01 << bitnum);
    *target &= mask;
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::RES_absHL::tick()
{
    switch (step++)
    {
        case 0:
            return InstructionResult::RUNNING;
        case 1:
            result = memory.read(*registers.HL);
            return InstructionResult::RUNNING;
        case 2:
        {
            uint8_t mask = ~(0x01 << bitnum);
            result &= mask;
            memory.write(*registers.HL, result);
            ++*registers.PC;
        }
        [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::SET_r::tick()
{
    uint8_t mask = 0x01 << bitnum;
    *target |= mask;
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::SET_absHL::tick()
{
    switch (step++)
    {
        case 0:
            return InstructionResult::RUNNING;
        case 1:
            result = memory.read(*registers.HL);
            return InstructionResult::RUNNING;
        case 2:
        {
            uint8_t mask = 0x01 << bitnum;
            result |= mask;
            memory.write(*registers.HL, result);
            ++*registers.PC;
        }
        [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

