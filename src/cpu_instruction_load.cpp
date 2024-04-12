#include "cpu_instruction_load.h"

/* 8-bit load instructions */

CPU::InstructionResult CPU::LD_r_r::tick()
{
    *dest = *src;
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::LD_r_n::tick()
{
    if (step++ == 0)
    {
        *dest = (uint8_t) memory.read(++*registers.PC);
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::LD_r_absrr::tick()
{
    if (step++ == 0)
    {
        *dest = memory.read(*src_addr);
        switch (post_operation)
        {
            case AddressMutOperation::INC:
                ++*src_addr;
                break;
            case AddressMutOperation::DEC:
                --*src_addr;
                break;
            case AddressMutOperation::NONE:
                break;
        }
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::LD_absrr_r::tick()
{
    if (step++ == 0)
    {
        memory.write(*dest_addr, *src);
        switch (post_operation)
        {
            case AddressMutOperation::INC:
                ++*dest_addr;
                break;
            case AddressMutOperation::DEC:
                --*dest_addr;
                break;
            case AddressMutOperation::NONE:
                break;
        }
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::LD_absrr_n::tick()
{
    switch (step++)
    {
        case 0:
            src_data = memory.read(++*registers.PC);
            return InstructionResult::RUNNING;
        case 1:
            memory.write(*dest_addr, src_data);
            return InstructionResult::RUNNING;
            ++*registers.PC;
            [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::LD_r_absnn::tick()
{
    switch (step++)
    {
        case 0:
            load_addr = memory.read(++*registers.PC);
            return InstructionResult::RUNNING;
        case 1:
            load_addr |= memory.read(++*registers.PC) << 8;
            return InstructionResult::RUNNING;
        case 2:
            *dest = memory.read(load_addr);
            return InstructionResult::RUNNING;
        case 3:
            ++*registers.PC;
            [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::LD_absnn_r::tick()
{
    switch (step++)
    {
        case 0:
            write_addr = memory.read(++*registers.PC);
            return InstructionResult::RUNNING;
        case 1:
            write_addr |= memory.read(++*registers.PC) << 8;
            return InstructionResult::RUNNING;
        case 2:
            memory.write(write_addr, *src);
            return InstructionResult::RUNNING;
        case 3:
            ++*registers.PC;
            [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::LD_r_relr::tick()
{
    if (step++ == 0)
    {
        uint16_t read_addr = 0xFF << 8;
        read_addr |= *src_addr_lsb;
        *dest = memory.read(read_addr);
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::LD_relr_r::tick()
{
    if (step++ == 0)
    {
        uint16_t write_addr = 0xFF << 8;
        write_addr |= *dest_addr_lsb;
        memory.write(write_addr, *src);
        return InstructionResult::RUNNING;
    }
    ++*registers.PC;
    return InstructionResult::FINISHED;
}

CPU::InstructionResult CPU::LD_r_reln::tick()
{
    switch (step++)
    {
        case 0:
            load_addr = 0xFF << 8;
            load_addr |= memory.read(++*registers.PC);
            return InstructionResult::RUNNING;
        case 1:
            *dest = memory.read(load_addr);
            return InstructionResult::RUNNING;
        case 2:
            ++*registers.PC;
            [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::LD_reln_r::tick()
{
    switch (step++)
    {
        case 0:
            write_addr = 0xFF << 8;
            write_addr |= memory.read(++*registers.PC);
            return InstructionResult::RUNNING;
        case 1:
            memory.write(write_addr, *src);
            return InstructionResult::RUNNING;
        case 2:
            ++*registers.PC;
            [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

/* 16-bit load instructions */

CPU::InstructionResult CPU::LD_rr_nn::tick()
{
    switch (step++)
    {
        case 0:
            // Load LSB from operand
            operand = memory.read(++*registers.PC);
            return InstructionResult::RUNNING;
        case 1:
            // Load MSB from operand
            operand |= memory.read(++*registers.PC) << 8;
            // Store in 16-bit register
            *dest = operand;
            return InstructionResult::RUNNING;
        case 2:
            ++*registers.PC;
            [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::LD_absnn_rr::tick()
{
    switch (step++)
    {
        case 0:
            dest_addr = memory.read(++*registers.PC);
            return InstructionResult::RUNNING;
        case 1:
            dest_addr |= memory.read(++*registers.PC) << 8;
            return InstructionResult::RUNNING;
        case 2:
        {
            uint8_t register_lsb = (uint8_t)(*src & 0x00FF);
            memory.write(dest_addr, register_lsb);
            return InstructionResult::RUNNING;
        }
        case 3:
        {
            uint8_t register_msb = (uint8_t)((*src & 0xFF00) >> 8);
            memory.write(dest_addr+1, register_msb);
            return InstructionResult::RUNNING;
        }
        case 4:
            ++*registers.PC;
            [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::LD_rr_rr::tick()
{
    switch (step++)
    {
        case 0:
            *dest = *src;
            return InstructionResult::RUNNING;
        case 1:
            ++*registers.PC;
            [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::PUSH_rr::tick()
{
    switch (step++)
    {
        case 0:
            return InstructionResult::RUNNING;
        case 1:
        {
            uint8_t register_msb = (uint8_t)((*src & 0xFF00) >> 8);
            memory.write(--*sp, register_msb);
            return InstructionResult::RUNNING;
        }
        case 2:
        {
            uint8_t register_lsb = (uint8_t)(*src & 0x00FF);
            memory.write(--*sp, register_lsb);
            return InstructionResult::RUNNING;
        }
        case 3:
            ++*registers.PC;
            [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::POP_rr::tick()
{
    switch (step++)
    {
        case 0:
        {
            *dest = memory.read((*sp)++);
            return InstructionResult::RUNNING;
        }
        case 1:
        {
            *dest |= memory.read((*sp)++) << 8;
            return InstructionResult::RUNNING;
        }
        case 2:
            ++*registers.PC;
            [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}

CPU::InstructionResult CPU::POP_AF::tick()
{
    switch (step++)
    {
        case 0:
        {
            uint8_t F = memory.read((*sp)++);
            F &= 0xF0;
            *dest = F;
            return InstructionResult::RUNNING;
        }
        case 1:
        {
            *dest |= memory.read((*sp)++) << 8;
            return InstructionResult::RUNNING;
        }
        case 2:
            ++*registers.PC;
            [[fallthrough]];
        default:
            return InstructionResult::FINISHED;
    }
}
