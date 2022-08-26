#include "cpu_instruction_load.h"

/* 8-bit load instructions */

bool CPU::LD_r_r::tick()
{
    *dest = *src;
    return true;
}

bool CPU::LD_r_n::tick()
{
    if (step++ == 0)
    {
        *dest = (uint8_t) memory.read(++*registers.PC);
        return false;
    }
    ++*registers.PC;
    return true;
}

bool CPU::LD_r_absrr::tick()
{
    if (step++ == 0)
    {
        *dest = memory.read(*src_addr);
        switch (post_operation)
        {
            case AddressMutOperation::INC:
                *src_addr++;
                break;
            case AddressMutOperation::DEC:
                *src_addr--;
                break;
            case AddressMutOperation::NONE:
                break;
        }
        return false;
    }
    ++*registers.PC;
    return true;
}

bool CPU::LD_absrr_r::tick()
{
    if (step++ == 0)
    {
        memory.write(*dest_addr, *src);
        switch (post_operation)
        {
            case AddressMutOperation::INC:
                *dest_addr++;
                break;
            case AddressMutOperation::DEC:
                *dest_addr--;
                break;
            case AddressMutOperation::NONE:
                break;
        }
        return false;
    }
    ++*registers.PC;
    return true;
}

bool CPU::LD_absrr_n::tick()
{
    if (step++ == 0)
    {
        uint8_t data = memory.read(++*registers.PC);
        memory.write(*dest_addr, data);
        return false;
    }
    ++*registers.PC;
    return true;
}

bool CPU::LD_r_absnn::tick()
{
    switch (step++)
    {
        case 0:
            load_addr = memory.read(++*registers.PC);
            return false;
        case 1:
            load_addr |= memory.read(++*registers.PC) << 8;
            return false;
        case 2:
            *dest = memory.read(load_addr);
            return false;
        default:
            ++*registers.PC;
            return true;
    }
}

bool CPU::LD_absnn_r::tick()
{
    switch (step++)
    {
        case 0:
            write_addr = memory.read(++*registers.PC);
            return false;
        case 1:
            write_addr |= memory.read(++*registers.PC) << 8;
            return false;
        case 2:
            memory.write(write_addr, *src);
            return false;
        default:
            ++*registers.PC;
            return true;
    }
}

bool CPU::LD_r_relr::tick()
{
    if (step++ == 0)
    {
        uint16_t read_addr = 0xFF << 8;
        read_addr |= *src_addr_lsb;
        *dest = memory.read(read_addr);
        return false;
    }
    ++*registers.PC;
    return true;
}

bool CPU::LD_relr_r::tick()
{
    if (step++ == 0)
    {
        uint16_t write_addr = 0xFF << 8;
        write_addr |= *dest_addr_lsb;
        memory.write(write_addr, *src);
        return false;
    }
    ++*registers.PC;
    return true;
}

bool CPU::LD_r_reln::tick()
{
    switch (step++)
    {
        case 0:
            load_addr = 0xFF << 8;
            load_addr |= ++*registers.PC;
            return false;
        case 1:
            *dest = memory.read(load_addr);
            return false;
        default:
            ++*registers.PC;
            return true;
    }
}

bool CPU::LD_reln_r::tick()
{
    switch (step++)
    {
        case 0:
            write_addr = 0xFF << 8;
            write_addr = memory.read(++*registers.PC);
            return false;
        case 1:
            memory.write(write_addr, *src);
            return false;
        default:
            ++*registers.PC;
            return true;
    }
}

/* 16-bit load instructions */

bool CPU::LD_rr_nn::tick()
{
    switch (step++)
    {
        case 0:
            // Load LSB from operand
            operand = memory.read(++*registers.PC);
            return false;
        case 1:
            // Load MSB from operand
            operand |= memory.read(++*registers.PC) << 8;
            // Store in 16-bit register
            *dest = operand;
            return false;
        default:
            ++*registers.PC;
            return true;
    }
}

bool CPU::LD_absnn_rr::tick()
{
    switch (step++)
    {
        case 0:
            dest_addr = memory.read(++*registers.PC);
            return false;
        case 1:
            dest_addr |= memory.read(++*registers.PC) << 8;
            return false;
        case 2:
        {
            uint8_t register_lsb = (uint8_t)(*src & 0x00FF);
            memory.write(dest_addr, register_lsb);
            return false;
        }
        case 3:
        {
            uint8_t register_msb = (uint8_t)((*src & 0xFF00) >> 8);
            memory.write(dest_addr+1, register_msb);
            return false;
        }
        default:
            ++*registers.PC;
            return true;
    }
}

bool CPU::LD_rr_rr::tick()
{
    switch (step++)
    {
        case 0:
            *dest = *src;
            return false;
        default:
            ++*registers.PC;
            return true;
    }
}

bool CPU::PUSH_rr::tick()
{
    switch (step++)
    {
        case 0:
            return false;
        case 1:
        {
            uint8_t register_msb = (uint8_t)((*src & 0xFF00) >> 8);
            memory.write(--*sp, register_msb);
            return false;
        }
        case 2:
        {
            uint8_t register_lsb = (uint8_t)(*src & 0x00FF);
            memory.write(--*sp, register_lsb);
            return false;
        }
        default:
            ++*registers.PC;
            return true;
    }
}

bool CPU::POP_rr::tick()
{
    switch (step++)
    {
        case 0:
        {
            *dest = memory.read((*sp)++);
            return false;
        }
        case 1:
        {
            *dest |= memory.read((*sp)++) << 8;
            return false;
        }
        default:
            ++*registers.PC;
            return true;
    }
}