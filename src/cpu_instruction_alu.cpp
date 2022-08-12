#include <limits.h>

#include "cpu_instruction_alu.h"

bool is_add_carry(uint8_t a, uint8_t b)
{
    return b > 0 && a > UINT8_MAX - b;
}

bool is_add_halfcarry(uint8_t a, uint8_t b)
{
    return (((a & 0x0F) + (b & 0x0F)) & 0x10) == 0x10;
}

bool is_add_carry(uint16_t a, uint16_t b)
{
    return b > 0 && a > UINT16_MAX - b;
}

bool is_add_halfcarry(uint16_t a, uint16_t b)
{
    return (((a & 0x0F00) + (b & 0x0F00)) & 0x1000) == 0x1000;
}

bool is_sub_carry(uint16_t a, uint16_t b)
{
    return a < b;
}

bool is_sub_halfcarry(uint16_t a, uint16_t b)
{
    return (a & 0x0F00) < (b & 0x0F00);
}

bool CPU::ADD_r_r::tick()
{
    registers.set_flag_carry(is_add_carry(*dest, *src));
    registers.set_flag_halfcarry(is_add_halfcarry(*dest, *src));
    *dest += *src;
    if (check_carry && registers.get_flag_carry())
    {
        if (is_add_carry(*dest, 1)) registers.set_flag_carry(true);
        if (is_add_halfcarry(*dest, 1)) registers.set_flag_halfcarry(true);
        ++*dest;
    }
    registers.set_flag_zero(*dest == 0);
    registers.set_flag_sub(false);
    return true;
}

bool CPU::ADD_r_n::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(++*pc);
        registers.set_flag_carry(is_add_carry(*dest, src));
        registers.set_flag_halfcarry(is_add_halfcarry(*dest, src));
        *dest += src;
        if (check_carry && registers.get_flag_carry())
        {
            if (is_add_carry(*dest, 1)) registers.set_flag_carry(true);
            if (is_add_halfcarry(*dest, 1)) registers.set_flag_halfcarry(true);
            ++*dest;
        }
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(false);
        return false;
    }
    return true;
}

bool CPU::ADD_r_absrr::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(*src_addr);
        registers.set_flag_carry(is_add_carry(*dest, src));
        registers.set_flag_halfcarry(is_add_halfcarry(*dest, src));
        *dest += src;
        if (check_carry && registers.get_flag_carry())
        {
            if (is_add_carry(*dest, 1)) registers.set_flag_carry(true);
            if (is_add_halfcarry(*dest, 1)) registers.set_flag_halfcarry(true);
            ++*dest;
        }
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(false);
        return false;
    }
    return true;
}

bool is_sub_carry(uint8_t a, uint8_t b)
{
    return a < b;
}

bool is_sub_halfcarry(uint8_t a, uint8_t b)
{
    return (a & 0x0F) < (b & 0x0F);
}

bool CPU::SUB_r_r::tick()
{
    registers.set_flag_carry(is_sub_carry(*dest, *src));
    registers.set_flag_halfcarry(is_sub_halfcarry(*dest, *src));
    *dest -= *src;
    if (check_carry && registers.get_flag_carry())
    {
        if (is_sub_carry(*dest, 1)) registers.set_flag_carry(true);
        if (is_sub_halfcarry(*dest, 1)) registers.set_flag_halfcarry(true);
        --*dest;
    }
    registers.set_flag_zero(*dest == 0);
    registers.set_flag_sub(true);
    return true;
}

bool CPU::SUB_r_n::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(++*pc);
        registers.set_flag_carry(is_sub_carry(*dest, src));
        registers.set_flag_halfcarry(is_sub_halfcarry(*dest, src));
        *dest -= src;
        if (check_carry && registers.get_flag_carry())
        {
            if (is_sub_carry(*dest, 1)) registers.set_flag_carry(true);
            if (is_sub_halfcarry(*dest, 1)) registers.set_flag_halfcarry(true);
            --*dest;
        }
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(true);
        return false;
    }
    return true;
}

bool CPU::SUB_r_absrr::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(*src_addr);
        registers.set_flag_carry(is_sub_carry(*dest, src));
        registers.set_flag_halfcarry(is_sub_halfcarry(*dest, src));
        *dest -= src;
        if (check_carry && registers.get_flag_carry())
        {
            if (is_sub_carry(*dest, 1)) registers.set_flag_carry(true);
            if (is_sub_halfcarry(*dest, 1)) registers.set_flag_halfcarry(true);
            --*dest;
        }
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(true);
        return false;
    }
    return true;
}

bool CPU::AND_r_r::tick()
{
    *dest &= *src;
    registers.set_flag_zero(*dest == 0);
    registers.set_flag_sub(false);
    registers.set_flag_halfcarry(true);
    registers.set_flag_carry(false);
    return true;
}

bool CPU::AND_r_n::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(++*pc);
        *dest &= src;
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(false);
        registers.set_flag_halfcarry(true);
        registers.set_flag_carry(false);
        return false;
    }
    return true;
}

bool CPU::AND_r_absrr::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(*src_addr);
        *dest &= src;
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(false);
        registers.set_flag_halfcarry(true);
        registers.set_flag_carry(false);
        return false;
    }
    return true;
}

bool CPU::XOR_r_r::tick()
{
    *dest ^= *src;
    registers.set_flag_zero(*dest == 0);
    registers.set_flag_sub(false);
    registers.set_flag_halfcarry(true);
    registers.set_flag_carry(false);
    return true;
}

bool CPU::XOR_r_n::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(++*pc);
        *dest ^= src;
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(false);
        registers.set_flag_halfcarry(true);
        registers.set_flag_carry(false);
        return false;
    }
    return true;
}

bool CPU::XOR_r_absrr::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(*src_addr);
        *dest ^= src;
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(false);
        registers.set_flag_halfcarry(true);
        registers.set_flag_carry(false);
        return false;
    }
    return true;
}

bool CPU::OR_r_r::tick()
{
    *dest |= *src;
    registers.set_flag_zero(*dest == 0);
    registers.set_flag_sub(false);
    registers.set_flag_halfcarry(true);
    registers.set_flag_carry(false);
    return true;
}

bool CPU::OR_r_n::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(++*pc);
        *dest |= src;
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(false);
        registers.set_flag_halfcarry(true);
        registers.set_flag_carry(false);
        return false;
    }
    return true;
}

bool CPU::OR_r_absrr::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(*src_addr);
        *dest |= src;
        registers.set_flag_zero(*dest == 0);
        registers.set_flag_sub(false);
        registers.set_flag_halfcarry(true);
        registers.set_flag_carry(false);
        return false;
    }
    return true;
}

bool CPU::CP_r_r::tick()
{
    registers.set_flag_carry(is_sub_carry(*dest, *src));
    registers.set_flag_halfcarry(is_sub_halfcarry(*dest, *src));
    uint8_t result = *dest - *src;
    registers.set_flag_zero(result == 0);
    registers.set_flag_sub(true);
    return true;
}

bool CPU::CP_r_n::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(++*pc);
        registers.set_flag_carry(is_sub_carry(*dest, src));
        registers.set_flag_halfcarry(is_sub_halfcarry(*dest, src));
        uint8_t result = *dest - src;
        registers.set_flag_zero(result == 0);
        registers.set_flag_sub(true);
        return false;
    }
    return true;
}

bool CPU::CP_r_absrr::tick()
{
    if (step++ == 0)
    {
        uint8_t src = memory.read(*src_addr);
        registers.set_flag_carry(is_sub_carry(*dest, src));
        registers.set_flag_halfcarry(is_sub_halfcarry(*dest, src));
        uint8_t result = *dest - src;
        registers.set_flag_zero(result == 0);
        registers.set_flag_sub(true);
        return false;
    }
    return true;
}

bool CPU::INC_r::tick()
{
    registers.set_flag_halfcarry(is_add_halfcarry(*dest, 1));
    *dest = *dest != UINT8_MAX ? *dest+1 : 0;
    registers.set_flag_zero(*dest == 0);
    registers.set_flag_sub(false);
    return true;
}

bool CPU::INC_absrr::tick()
{
    switch (step++)
    {
        case 0:
            result = memory.read(*dest_addr);
            return false;
        case 1:
            registers.set_flag_halfcarry(is_add_halfcarry(result, 1));
            result = result != UINT8_MAX ? result+1 : 0;
            registers.set_flag_zero(result == 0);
            registers.set_flag_sub(false);
            return false;
        default:
            return true;
    }
}

bool CPU::DEC_r::tick()
{
    registers.set_flag_halfcarry(is_sub_halfcarry(*dest, 1));
    *dest = *dest != 0 ? *dest-1 : UINT8_MAX;
    registers.set_flag_zero(*dest == 0);
    registers.set_flag_sub(true);
    return true;
}

bool CPU::DEC_absrr::tick()
{
    switch (step++)
    {
        case 0:
            result = memory.read(*dest_addr);
            return false;
        case 1:
            registers.set_flag_halfcarry(is_sub_halfcarry(result, 1));
            result = result != 0 ? result-1 : UINT8_MAX;
            registers.set_flag_zero(result == 0);
            registers.set_flag_sub(true);
            return false;
        default:
            return true;
    }
}

bool CPU::DAA::tick()
{
    uint8_t adjust_val = 0;
    if (registers.get_flag_halfcarry())
    {
        adjust_val += 6;
    }
    if (registers.get_flag_carry())
    {
        adjust_val += 60;
    }
    registers.set_flag_halfcarry(false);
    registers.set_flag_carry(is_add_carry(*registers.A, adjust_val));
    *registers.A += adjust_val;
    registers.set_flag_zero(*registers.A == 0);
    return true;
}

bool CPU::CPL::tick()
{
    *(registers.A) = ~*(registers.A);
    registers.set_flag_sub(true);
    registers.set_flag_halfcarry(true);
    return true;
}

/*
 * 16-bit ALU Insutrctions
 */

bool CPU::ADD_HL_rr::tick()
{
    registers.set_flag_carry(is_add_carry(*dest, *src));
    registers.set_flag_halfcarry(is_add_halfcarry(*dest, *src));
    *dest += *src;
    registers.set_flag_sub(false);
    return true;
}

bool CPU::INC_rr::tick()
{
    registers.set_flag_halfcarry(is_add_halfcarry(*dest, 1));
    *dest = *dest != UINT16_MAX ? *dest+1 : 0;
    registers.set_flag_zero(*dest == 0);
    registers.set_flag_sub(false);
    return true;
}

bool CPU::DEC_rr::tick()
{
    registers.set_flag_halfcarry(is_sub_halfcarry(*dest, 1));
    *dest = *dest != 0 ? *dest-1 : UINT16_MAX;
    registers.set_flag_zero(*dest == 0);
    registers.set_flag_sub(true);
    return true;
}
