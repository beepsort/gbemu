#include <limits.h>

#include "gameboy/cpu_instruction_alu.h"

bool is_add_carry(uint8_t a, uint8_t b)
{
    return b > 0 && a > (UINT8_MAX - b);
}

bool is_add_halfcarry(uint8_t a, uint8_t b)
{
    return (((a & 0x0F) + (b & 0x0F)) & 0x10) == 0x10;
}

bool is_sub_carry(uint8_t a, uint8_t b)
{
    return a < b;
}

bool is_sub_halfcarry(uint8_t a, uint8_t b)
{
    return (a & 0x0F) < (b & 0x0F);
}

bool is_add_carry(uint16_t a, uint16_t b)
{
    return b > 0 && a > UINT16_MAX - b;
}

bool is_add_halfcarry(uint16_t a, uint16_t b)
{
    return (((a & 0x0FFF) + (b & 0x0FFF)) & 0x1000) == 0x1000;
}

bool is_sub_carry(uint16_t a, uint16_t b)
{
    return a < b;
}

bool is_sub_halfcarry(uint16_t a, uint16_t b)
{
    return (a & 0x0FFF) < (b & 0x0FFF);
}

GAMEBOY::InstructionResult GAMEBOY::ADD_r_r::tick()
{
    bool was_carry = registers.get_flag_carry();
    registers.set_flag_carry(is_add_carry(*dest, *src));
    registers.set_flag_halfcarry(is_add_halfcarry(*dest, *src));
    *dest += *src;
    if (check_carry && was_carry)
    {
        if (is_add_carry(*dest, 1)) registers.set_flag_carry(true);
        if (is_add_halfcarry(*dest, 1)) registers.set_flag_halfcarry(true);
        ++*dest;
    }
    registers.set_flag_zero(*dest == 0);
    registers.set_flag_sub(false);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::ADD_r_n::tick()
{
    if (step++ == 0)
    {
        bool was_carry = registers.get_flag_carry();
        uint8_t src = memory.read(++*registers.PC);
        registers.set_flag_carry(is_add_carry(*dest, src));
        registers.set_flag_halfcarry(is_add_halfcarry(*dest, src));
        *dest += src;
        if (check_carry && was_carry)
        {
            if (is_add_carry(*dest, 1)) registers.set_flag_carry(true);
            if (is_add_halfcarry(*dest, 1)) registers.set_flag_halfcarry(true);
            ++*dest;
        }
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(false);
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::ADD_r_absrr::tick()
{
    if (step++ == 0)
    {
        bool was_carry = registers.get_flag_carry();
        uint8_t src = memory.read(*src_addr);
        registers.set_flag_carry(is_add_carry(*dest, src));
        registers.set_flag_halfcarry(is_add_halfcarry(*dest, src));
        *dest += src;
        if (check_carry && was_carry)
        {
            if (is_add_carry(*dest, 1)) registers.set_flag_carry(true);
            if (is_add_halfcarry(*dest, 1)) registers.set_flag_halfcarry(true);
            ++*dest;
        }
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(false);
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::SUB_r_r::tick()
{
    bool was_carry = registers.get_flag_carry();
    registers.set_flag_carry(is_sub_carry(*dest, *src));
    registers.set_flag_halfcarry(is_sub_halfcarry(*dest, *src));
    *dest -= *src;
    if (check_carry && was_carry)
    {
        if (is_sub_carry(*dest, 1)) registers.set_flag_carry(true);
        if (is_sub_halfcarry(*dest, 1)) registers.set_flag_halfcarry(true);
        --*dest;
    }
    registers.set_flag_zero(*dest == 0);
    registers.set_flag_sub(true);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::SUB_r_n::tick()
{
    if (step++ == 0)
    {
        bool was_carry = registers.get_flag_carry();
        uint8_t src = memory.read(++*registers.PC);
        registers.set_flag_carry(is_sub_carry(*dest, src));
        registers.set_flag_halfcarry(is_sub_halfcarry(*dest, src));
        *dest -= src;
        if (check_carry && was_carry)
        {
            if (is_sub_carry(*dest, 1)) registers.set_flag_carry(true);
            if (is_sub_halfcarry(*dest, 1)) registers.set_flag_halfcarry(true);
            --*dest;
        }
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(true);
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::SUB_r_absrr::tick()
{
    if (step++ == 0)
    {
        bool was_carry = registers.get_flag_carry();
        uint8_t src = memory.read(*src_addr);
        registers.set_flag_carry(is_sub_carry(*dest, src));
        registers.set_flag_halfcarry(is_sub_halfcarry(*dest, src));
        *dest -= src;
        if (check_carry && was_carry)
        {
            if (is_sub_carry(*dest, 1)) registers.set_flag_carry(true);
            if (is_sub_halfcarry(*dest, 1)) registers.set_flag_halfcarry(true);
            --*dest;
        }
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(true);
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::AND_r_r::tick()
{
    *dest &= *src;
    registers.set_flag_zero(*dest == 0);
    registers.set_flag_sub(false);
    registers.set_flag_halfcarry(true);
    registers.set_flag_carry(false);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::AND_r_n::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(++*registers.PC);
        *dest &= src;
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(false);
        registers.set_flag_halfcarry(true);
        registers.set_flag_carry(false);
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::AND_r_absrr::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(*src_addr);
        *dest &= src;
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(false);
        registers.set_flag_halfcarry(true);
        registers.set_flag_carry(false);
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::XOR_r_r::tick()
{
    *dest ^= *src;
    registers.set_flag_zero(*dest == 0);
    registers.set_flag_sub(false);
    registers.set_flag_halfcarry(false);
    registers.set_flag_carry(false);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::XOR_r_n::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(++*registers.PC);
        *dest ^= src;
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(false);
        registers.set_flag_halfcarry(false);
        registers.set_flag_carry(false);
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::XOR_r_absrr::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(*src_addr);
        *dest ^= src;
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(false);
        registers.set_flag_halfcarry(false);
        registers.set_flag_carry(false);
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::OR_r_r::tick()
{
    *dest |= *src;
    registers.set_flag_zero(*dest == 0);
    registers.set_flag_sub(false);
    registers.set_flag_halfcarry(false);
    registers.set_flag_carry(false);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::OR_r_n::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(++*registers.PC);
        *dest |= src;
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(false);
        registers.set_flag_halfcarry(false);
        registers.set_flag_carry(false);
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::OR_r_absrr::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(*src_addr);
        *dest |= src;
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(false);
        registers.set_flag_halfcarry(false);
        registers.set_flag_carry(false);
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::CP_r_r::tick()
{
    registers.set_flag_carry(is_sub_carry(*dest, *src));
    registers.set_flag_halfcarry(is_sub_halfcarry(*dest, *src));
    uint8_t result = *dest - *src;
    registers.set_flag_zero(result == 0);
    registers.set_flag_sub(true);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::CP_r_n::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(++*registers.PC);
        registers.set_flag_carry(is_sub_carry(*dest, src));
        registers.set_flag_halfcarry(is_sub_halfcarry(*dest, src));
        uint8_t result = *dest - src;
        registers.set_flag_zero(result == 0);
        registers.set_flag_sub(true);
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::CP_r_absrr::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(*src_addr);
        registers.set_flag_carry(is_sub_carry(*dest, src));
        registers.set_flag_halfcarry(is_sub_halfcarry(*dest, src));
        uint8_t result = *dest - src;
        registers.set_flag_zero(result == 0);
        registers.set_flag_sub(true);
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::INC_r::tick()
{
    registers.set_flag_halfcarry(is_add_halfcarry(*dest, 1));
    *dest = *dest != UINT8_MAX ? *dest+1 : 0;
    registers.set_flag_zero(*dest == 0);
    registers.set_flag_sub(false);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::INC_absrr::tick()
{
    switch (step++)
    {
        case 0:
            result = memory.read(*dest_addr);
            return InstructionResult::RUNNING;
        case 1:
            registers.set_flag_halfcarry(is_add_halfcarry(result, 1));
            result = result != UINT8_MAX ? result+1 : 0;
            memory.write(*dest_addr, result);
            registers.set_flag_zero(result == 0);
            registers.set_flag_sub(false);
            return InstructionResult::RUNNING;
        case 2:
            ++*registers.PC;
            [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

GAMEBOY::InstructionResult GAMEBOY::DEC_r::tick()
{
    registers.set_flag_halfcarry(is_sub_halfcarry(*dest, 1));
    *dest = *dest != 0 ? *dest-1 : UINT8_MAX;
    registers.set_flag_zero(*dest == 0);
    registers.set_flag_sub(true);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::DEC_absrr::tick()
{
    switch (step++)
    {
        case 0:
            result = memory.read(*dest_addr);
            return InstructionResult::RUNNING;
        case 1:
            registers.set_flag_halfcarry(is_sub_halfcarry(result, 1));
            result = result != 0 ? result-1 : UINT8_MAX;
            memory.write(*dest_addr, result);
            registers.set_flag_zero(result == 0);
            registers.set_flag_sub(true);
            return InstructionResult::RUNNING;
        case 2:
            ++*registers.PC;
            [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

GAMEBOY::InstructionResult GAMEBOY::DAA::tick()
{
    if (!registers.get_flag_sub() && *registers.A >= 0x9A)
    {
        registers.set_flag_carry(true);
    }
    if (!registers.get_flag_sub() && (*registers.A & 0x0F) >= 0x0A)
    {
        registers.set_flag_halfcarry(true);
    }
    uint8_t adjust_val = 0;
    if (registers.get_flag_halfcarry())
    {
        adjust_val += 0x06;
    }
    if (registers.get_flag_carry())
    {
        adjust_val += 0x60;
    }
    uint16_t adjusted_result;
    if (registers.get_flag_sub() == false)
    {
        adjusted_result = (*registers.A) + adjust_val;
    }
    else // registers.get_flag_sub() === true
    {
        adjusted_result = (*registers.A) - adjust_val;
    }
    registers.set_flag_halfcarry(false);
    *registers.A = (uint8_t) adjusted_result;
    registers.set_flag_zero(*registers.A == 0);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::CPL::tick()
{
    *(registers.A) = ~*(registers.A);
    registers.set_flag_sub(true);
    registers.set_flag_halfcarry(true);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

/*
 * 16-bit ALU Insutrctions
 */

GAMEBOY::InstructionResult GAMEBOY::ADD_HL_rr::tick()
{
    if (step++ == 0)
    {
        registers.set_flag_carry(is_add_carry(*dest, *src));
        registers.set_flag_halfcarry(is_add_halfcarry(*dest, *src));
        *dest += *src;
        registers.set_flag_sub(false);
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::ADD_SP_n::tick()
{
    switch (step++)
    {
        case 0:
            return InstructionResult::RUNNING;
        case 1:
            offset = (int8_t)memory.read(++*registers.PC);
            return InstructionResult::RUNNING;
        case 2:
        {
            uint32_t target = *registers.SP + offset;
            registers.set_flag_zero(false);
            registers.set_flag_sub(false);
            registers.set_flag_carry(is_add_carry((uint8_t)*registers.SP, (uint8_t)offset));
            registers.set_flag_halfcarry(is_add_halfcarry((uint8_t)*registers.SP, (uint8_t)offset));
            *registers.SP = (uint16_t)target;
            return InstructionResult::RUNNING;
        }
        case 3:
            ++*registers.PC;
            [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

GAMEBOY::InstructionResult GAMEBOY::LD_HL_SP_n::tick()
{
    switch (step++)
    {
        case 0:
            return InstructionResult::RUNNING;
        case 1:
            offset = (int8_t)memory.read(++*registers.PC);
            return InstructionResult::RUNNING;
        case 2:
        {
            uint32_t target = *registers.SP + offset;
            registers.set_flag_zero(false);
            registers.set_flag_sub(false);
            registers.set_flag_carry(is_add_carry((uint8_t)*registers.SP, (uint8_t)offset));
            registers.set_flag_halfcarry(is_add_halfcarry((uint8_t)*registers.SP, (uint8_t)offset));
            *registers.HL = (uint16_t)target;
            ++*registers.PC;
        }
        [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

GAMEBOY::InstructionResult GAMEBOY::INC_rr::tick()
{
    if (step++ == 0)
    {
        *dest = *dest != UINT16_MAX ? *dest+1 : 0;
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::DEC_rr::tick()
{
    if (step++ == 0)
    {
        *dest = *dest != 0 ? *dest-1 : UINT16_MAX;
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::RLA_r::tick()
{
    uint8_t carry = (*registers.A) >> 7;
    uint8_t lsb;
    if (throughCarry)
    {
        lsb = registers.get_flag_carry() ? 1 : 0;
    }
    else
    {
        lsb = carry;
    }
    *registers.A = (*registers.A) << 1 | lsb;
    // TODO: confirm conflicting information, may be set as a result of the computation
    registers.set_flag_zero(0); //*registers.A == 0);
    registers.set_flag_carry(carry);
    registers.set_flag_halfcarry(0);
    registers.set_flag_sub(0);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

GAMEBOY::InstructionResult GAMEBOY::RRA_r::tick()
{
    uint8_t carry = (*registers.A) << 7;
    uint8_t msb;
    if (throughCarry)
    {
        msb = registers.get_flag_carry() ? 1<<7 : 0;
    }
    else
    {
        msb = carry;
    }
    *registers.A = msb | (*registers.A) >> 1;
    // TODO: confirm conflicting information, may be set as a result of the computation
    registers.set_flag_zero(0); //*registers.A == 0);
    registers.set_flag_carry(carry);
    registers.set_flag_halfcarry(0);
    registers.set_flag_sub(0);
    ++*registers.PC;
    return InstructionResult::FINISHED;
}
