#ifndef __CPU_INSTRUCTION_ALU_H__
#define __CPU_INSTRUCTION_ALU_H__

#include <stdint.h>

#include "cpu_instruction.h"
#include "cpu_registers.h"
#include "memory.h"

namespace CPU
{
    /*
     * 8-bit ALU Instructions
     */
    class ADD_r_r: public CpuInstruction
    {
    /**
     * @brief Add two 8-bit registers and store result in the destination register
     * If check_carry true and carry bit was set from previous operation then additionally add one
     * 1 byte opcodes with no further operands
     * 1 M-cycle to complete
     * Z 0 H C
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 0
     * Half-carry: set to result of computation
     * Carry: set to result of computation
     */
    private:
        uint8_t* src;
        uint8_t* dest;
        CpuRegisters& registers;
        bool check_carry;
    public:
        ADD_r_r(uint8_t* dest, uint8_t* src, CpuRegisters& registers, bool check_carry=false)
        : dest(dest), src(src), registers(registers), check_carry(check_carry) {}
        bool tick();
    };

    class ADD_r_n: public CpuInstruction
    {
    /**
     * @brief Add 8-bit operand to 8-bit register and store result in the destination register
     * If check_carry true and carry bit was set from previous operation then additionally add one
     * 1 byte opcodes with 1 byte operand
     * 2 M-cycle to complete
     * Z 0 H C
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 0
     * Half-carry: set to result of computation
     * Carry: set to result of computation
     */
    private:
        uint8_t* dest;
        MEMORY::AddressDispatcher& memory;
        CpuRegisters& registers;
        bool check_carry;
        uint8_t step = 0;
    public:
        ADD_r_n(uint8_t* dest, CpuRegisters& registers, MEMORY::AddressDispatcher& memory, bool check_carry=false)
        : dest(dest), memory(memory), registers(registers), check_carry(check_carry) {}
        bool tick();
    };

    class ADD_r_absrr: public CpuInstruction
    {
    /**
     * @brief Add 8-bit value from memory at 16-bit address stored in register to 8-bit value in destination register
     * If check_carry true and carry bit was set from previous operation then additionally add one
     * 1 byte opcodes with no further operands
     * 1 M-cycle to complete
     * Z 0 H C
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 0
     * Half-carry: set to result of computation
     * Carry: set to result of computation
     */
    private:
        uint16_t* src_addr;
        uint8_t* dest;
        CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
        bool check_carry;
        uint8_t step = 0;
    public:
        ADD_r_absrr(uint8_t* dest, uint16_t* src_addr, CpuRegisters& registers, MEMORY::AddressDispatcher& memory, bool check_carry=false)
        : dest(dest), src_addr(src_addr), registers(registers), memory(memory), check_carry(check_carry) {}
        bool tick();
    };

    class SUB_r_r: public CpuInstruction
    {
    /**
     * @brief Subtract 8-bit src register from 8-bit destination register
     * If check_carry true and carry bit was set from previous operation then additionally subtract one
     * 1 byte opcodes with no further operands
     * 1 M-cycle to complete
     * Z 1 H C
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 1
     * Half-carry: set to result of computation
     * Carry: set to result of computation
     */
    private:
        uint8_t* src;
        uint8_t* dest;
        CpuRegisters& registers;
        bool check_carry;
    public:
        SUB_r_r(uint8_t* dest, uint8_t* src, CpuRegisters& registers, bool check_carry=false)
        : dest(dest), src(src), registers(registers), check_carry(check_carry) {}
        bool tick();
    };

    class SUB_r_n: public CpuInstruction
    {
    /**
     * @brief Add 8-bit operand to 8-bit register and store result in the destination register
     * If check_carry true and carry bit was set from previous operation then additionally add one
     * 1 byte opcodes with 1 byte operand
     * 2 M-cycle to complete
     * Z 0 H C
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 0
     * Half-carry: set to result of computation
     * Carry: set to result of computation
     */
    private:
        uint8_t* dest;
        MEMORY::AddressDispatcher& memory;
        CpuRegisters& registers;
        bool check_carry;
        uint8_t step = 0;
    public:
        SUB_r_n(uint8_t* dest, CpuRegisters& registers, MEMORY::AddressDispatcher& memory, bool check_carry=false)
        : dest(dest), memory(memory), registers(registers), check_carry(check_carry) {}
        bool tick();
    };

    class SUB_r_absrr: public CpuInstruction
    {
    /**
     * @brief Subtract 8-bit value from memory at 16-bit address stored in register to the 8-bit value in destination register
     * If check_carry true and carry bit was set from previous operation then additionally subtract one
     * 1 byte opcodes with no further operands
     * 1 M-cycle to complete
     * Z 1 H C
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 1
     * Half-carry: set to result of computation
     * Carry: set to result of computation
     */
    private:
        uint16_t* src_addr;
        uint8_t* dest;
        CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
        bool check_carry;
        uint8_t step = 0;
    public:
        SUB_r_absrr(uint8_t* dest, uint16_t* src_addr, CpuRegisters& registers, MEMORY::AddressDispatcher& memory, bool check_carry=false)
        : dest(dest), src_addr(src_addr), registers(registers), memory(memory), check_carry(check_carry) {}
        bool tick();
    };

    class AND_r_r: public CpuInstruction
    {
    /**
     * @brief Bitwise And two 8-bit registers and store result in the destination register
     * If check_carry true and carry bit was set from previous operation then additionally add one
     * 1 byte opcodes with no further operands
     * 1 M-cycle to complete
     * Z 0 1 0
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 0
     * Half-carry: set to 1
     * Carry: set to 0
     */
    private:
        uint8_t* src;
        uint8_t* dest;
        CpuRegisters& registers;
    public:
        AND_r_r(uint8_t* dest, uint8_t* src, CpuRegisters& registers)
        : dest(dest), src(src), registers(registers) {}
        bool tick();
    };

    class AND_r_n: public CpuInstruction
    {
    /**
     * @brief Bitwise And 8-bit operand to 8-bit register and store result in the destination register
     * 1 byte opcodes with 1 byte operand
     * 2 M-cycle to complete
     * Z 0 1 0
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 0
     * Half-carry: set to 1
     * Carry: set to 0
     */
    private:
        uint8_t* dest;
        MEMORY::AddressDispatcher& memory;
        CpuRegisters& registers;
        uint8_t step = 0;
    public:
        AND_r_n(uint8_t* dest, CpuRegisters& registers, MEMORY::AddressDispatcher& memory)
        : dest(dest), memory(memory), registers(registers) {}
        bool tick();
    };

    class AND_r_absrr: public CpuInstruction
    {
    /**
     * @brief Bitwise And 8-bit value from memory at 16-bit address stored in register to the 8-bit value in destination register
     * 1 byte opcodes with no further operands
     * 2 M-cycle to complete
     * Z 0 1 0
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 0
     * Half-carry: set to 1
     * Carry: set to 0
     */
    private:
        uint16_t* src_addr;
        uint8_t* dest;
        CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
        uint8_t step = 0;
    public:
        AND_r_absrr(uint8_t* dest, uint16_t* src_addr, CpuRegisters& registers, MEMORY::AddressDispatcher& memory)
        : dest(dest), src_addr(src_addr), registers(registers), memory(memory) {}
        bool tick();
    };

    class XOR_r_r: public CpuInstruction
    {
    /**
     * @brief Bitwise XOR two 8-bit registers and store result in the destination register
     * If check_carry true and carry bit was set from previous operation then additionally add one
     * 1 byte opcodes with no further operands
     * 1 M-cycle to complete
     * Z 0 1 0
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 0
     * Half-carry: set to 1
     * Carry: set to 0
     */
    private:
        uint8_t* src;
        uint8_t* dest;
        CpuRegisters& registers;
    public:
        XOR_r_r(uint8_t* dest, uint8_t* src, CpuRegisters& registers)
        : dest(dest), src(src), registers(registers) {}
        bool tick();
    };

    class XOR_r_n: public CpuInstruction
    {
    /**
     * @brief Bitwise XOR 8-bit operand to 8-bit register and store result in the destination register
     * 1 byte opcodes with 1 byte operand
     * 2 M-cycle to complete
     * Z 0 1 0
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 0
     * Half-carry: set to 1
     * Carry: set to 0
     */
    private:
        uint8_t* dest;
        MEMORY::AddressDispatcher& memory;
        CpuRegisters& registers;
        uint8_t step = 0;
    public:
        XOR_r_n(uint8_t* dest, CpuRegisters& registers, MEMORY::AddressDispatcher& memory)
        : dest(dest), memory(memory), registers(registers) {}
        bool tick();
    };

    class XOR_r_absrr: public CpuInstruction
    {
    /**
     * @brief Bitwise XOR 8-bit value from memory at 16-bit address stored in register to the 8-bit value in destination register
     * 1 byte opcodes with no further operands
     * 2 M-cycle to complete
     * Z 0 1 0
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 0
     * Half-carry: set to 1
     * Carry: set to 0
     */
    private:
        uint16_t* src_addr;
        uint8_t* dest;
        CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
        uint8_t step = 0;
    public:
        XOR_r_absrr(uint8_t* dest, uint16_t* src_addr, CpuRegisters& registers, MEMORY::AddressDispatcher& memory)
        : dest(dest), src_addr(src_addr), registers(registers), memory(memory) {}
        bool tick();
    };

    class OR_r_r: public CpuInstruction
    {
    /**
     * @brief Bitwise OR two 8-bit registers and store result in the destination register
     * If check_carry true and carry bit was set from previous operation then additionally add one
     * 1 byte opcodes with no further operands
     * 1 M-cycle to complete
     * Z 0 1 0
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 0
     * Half-carry: set to 1
     * Carry: set to 0
     */
    private:
        uint8_t* src;
        uint8_t* dest;
        CpuRegisters& registers;
    public:
        OR_r_r(uint8_t* dest, uint8_t* src, CpuRegisters& registers)
        : dest(dest), src(src), registers(registers) {}
        bool tick();
    };

    class OR_r_n: public CpuInstruction
    {
    /**
     * @brief Bitwise OR 8-bit operand to 8-bit register and store result in the destination register
     * 1 byte opcodes with 1 byte operand
     * 2 M-cycle to complete
     * Z 0 1 0
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 0
     * Half-carry: set to 1
     * Carry: set to 0
     */
    private:
        uint8_t* dest;
        MEMORY::AddressDispatcher& memory;
        CpuRegisters& registers;
        uint8_t step = 0;
    public:
        OR_r_n(uint8_t* dest, CpuRegisters& registers, MEMORY::AddressDispatcher& memory)
        : dest(dest), memory(memory), registers(registers) {}
        bool tick();
    };

    class OR_r_absrr: public CpuInstruction
    {
    /**
     * @brief Bitwise OR 8-bit value from memory at 16-bit address stored in register to the 8-bit value in destination register
     * 1 byte opcodes with no further operands
     * 2 M-cycle to complete
     * Z 0 1 0
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 0
     * Half-carry: set to 1
     * Carry: set to 0
     */
    private:
        uint16_t* src_addr;
        uint8_t* dest;
        CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
        uint8_t step = 0;
    public:
        OR_r_absrr(uint8_t* dest, uint16_t* src_addr, CpuRegisters& registers, MEMORY::AddressDispatcher& memory)
        : dest(dest), src_addr(src_addr), registers(registers), memory(memory) {}
        bool tick();
    };

    class CP_r_r: public CpuInstruction
    {
    /**
     * @brief Compare 8-bit src register from 8-bit destination register, store result in zero flag
     * 1 byte opcodes with no further operands
     * 1 M-cycle to complete
     * Z 1 H C
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 1
     * Half-carry: set to result of computation
     * Carry: set to result of computation
     */
    private:
        uint8_t* src;
        uint8_t* dest;
        CpuRegisters& registers;
    public:
        CP_r_r(uint8_t* dest, uint8_t* src, CpuRegisters& registers)
        : dest(dest), src(src), registers(registers) {}
        bool tick();
    };

    class CP_r_n: public CpuInstruction
    {
    /**
     * @brief Compare 8-bit operand to 8-bit register and store result in zero flag
     * 1 byte opcodes with 1 byte operand
     * 2 M-cycle to complete
     * Z 0 H C
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 0
     * Half-carry: set to result of computation
     * Carry: set to result of computation
     */
    private:
        uint8_t* dest;
        MEMORY::AddressDispatcher& memory;
        CpuRegisters& registers;
        uint8_t step = 0;
    public:
        CP_r_n(uint8_t* dest, CpuRegisters& registers, MEMORY::AddressDispatcher& memory)
        : dest(dest), memory(memory), registers(registers) {}
        bool tick();
    };

    class CP_r_absrr: public CpuInstruction
    {
    /**
     * @brief Compare 8-bit value from memory at 16-bit address stored in register to the 8-bit value in destination register, store result in zero flag
     * 1 byte opcodes with no further operands
     * 1 M-cycle to complete
     * Z 1 H C
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 1
     * Half-carry: set to result of computation
     * Carry: set to result of computation
     */
    private:
        uint16_t* src_addr;
        uint8_t* dest;
        CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
        uint8_t step = 0;
    public:
        CP_r_absrr(uint8_t* dest, uint16_t* src_addr, CpuRegisters& registers, MEMORY::AddressDispatcher& memory)
        : dest(dest), src_addr(src_addr), registers(registers), memory(memory) {}
        bool tick();
    };

    class INC_r: public CpuInstruction
    {
    /**
     * @brief Increment 8-bit register, storing the result back in itself
     * 1 byte opcodes with no further operands
     * 1 M-cycle to complete
     * Z 0 H -
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 0
     * Half-carry: set to result of computation
     * Carry: not modified
     */
    private:
        uint8_t* dest;
        CpuRegisters& registers;
    public:
        INC_r(uint8_t* dest, CpuRegisters& registers)
        : dest(dest), registers(registers) {}
        bool tick();
    };

    class INC_absrr: public CpuInstruction
    {
    /**
     * @brief Increment 8-bit value from memory at 16-bit address stored in register, then store the result back at the memory address
     * 1 byte opcodes with no further operands
     * 3 M-cycle to complete
     * Z 0 H -
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 0
     * Half-carry: set to result of computation
     * Carry: not modified
     */
    private:
        uint16_t* dest_addr;
        uint8_t result;
        CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
        uint8_t step = 0;
    public:
        INC_absrr(uint16_t* dest_addr, CpuRegisters& registers, MEMORY::AddressDispatcher& memory)
        : dest_addr(dest_addr), registers(registers), memory(memory) {}
        bool tick();
    };

    class DEC_r: public CpuInstruction
    {
    /**
     * @brief Decrement 8-bit register, storing the result back in itself
     * 1 byte opcodes with no further operands
     * 1 M-cycle to complete
     * Z 1 H -
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 1
     * Half-carry: set to result of computation
     * Carry: not modified
     */
    private:
        uint8_t* dest;
        CpuRegisters& registers;
    public:
        DEC_r(uint8_t* dest, CpuRegisters& registers)
        : dest(dest), registers(registers) {}
        bool tick();
    };

    class DEC_absrr: public CpuInstruction
    {
    /**
     * @brief Decrement 8-bit value from memory at 16-bit address stored in register, then store the result back at the memory address
     * 1 byte opcodes with no further operands
     * 3 M-cycle to complete
     * Z 1 H -
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 1
     * Half-carry: set to result of computation
     * Carry: not modified
     */
    private:
        uint16_t* dest_addr;
        uint8_t result;
        CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
        uint8_t step = 0;
    public:
        DEC_absrr(uint16_t* dest_addr, CpuRegisters& registers, MEMORY::AddressDispatcher& memory)
        : dest_addr(dest_addr), registers(registers), memory(memory) {}
        bool tick();
    };

    class DAA: public CpuInstruction
    {
    /**
     * @brief Adjust register A to a BCD number after BCD addition or subtraction
     * 1 byte opcode with no further operands
     * 1 M-cycle to complete
     * Z - 0 C
     * Flags:
     * Zero: set to result of computation
     * Subtraction: not modified
     * Half-carry: set to 0
     * Carry: set to result of computation
     */
    private:
        CpuRegisters& registers;
    public:
        DAA(CpuRegisters& registers)
        : registers(registers) {}
        bool tick();
    };

    class CPL: public CpuInstruction
    {
    /**
     * @brief Set A register to bitwise compliment of itself
     * 1 byte opcode with no further operands
     * 1 M-cycle to complete
     * - 1 1 -
     * Flags:
     * Zero: not modified
     * Subtraction: set to 1
     * Half-carry: set to 1
     * Carry: not modified
     */
    private:
        CpuRegisters& registers;
    public:
        CPL(CpuRegisters& registers)
        : registers(registers) {}
        bool tick();
    };

    /*
     * 16-bit ALU Instructions
     */

    class ADD_HL_rr: public CpuInstruction
    {
    /**
     * @brief Add 16-bit register to HL register and store result in the HL register
     * 1 byte opcodes with no further operands
     * 1 M-cycle to complete
     * - 0 H C
     * Flags:
     * Zero: not modified
     * Subtraction: set to 0
     * Half-carry: set to result of computation
     * Carry: set to result of computation
     */
    private:
        uint16_t* src;
        uint16_t* dest;
        CpuRegisters& registers;
    public:
        ADD_HL_rr(uint16_t* src, CpuRegisters& registers)
        : dest(registers.HL), src(src), registers(registers) {}
        bool tick();
    };

    class INC_rr: public CpuInstruction
    {
    /**
     * @brief Increment 16-bit register, storing the result back in itself
     * 1 byte opcodes with no further operands
     * 1 M-cycle to complete
     * Z 0 H -
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 0
     * Half-carry: set to result of computation
     * Carry: not modified
     */
    private:
        uint16_t* dest;
        CpuRegisters& registers;
    public:
        INC_rr(uint16_t* dest, CpuRegisters& registers)
        : dest(dest), registers(registers) {}
        bool tick();
    };

    class DEC_rr: public CpuInstruction
    {
    /**
     * @brief Decrement 16-bit register, storing the result back in itself
     * 1 byte opcodes with no further operands
     * 1 M-cycle to complete
     * Z 1 H -
     * Flags:
     * Zero: set to result of computation
     * Subtraction: set to 1
     * Half-carry: set to result of computation
     * Carry: not modified
     */
    private:
        uint16_t* dest;
        CpuRegisters& registers;
    public:
        DEC_rr(uint16_t* dest, CpuRegisters& registers)
        : dest(dest), registers(registers) {}
        bool tick();
    };

};

#endif