#ifndef __CPU_INSTRUCTION_LOAD_H__
#define __CPU_INSTRUCTION_LOAD_H__

#include "cpu_instruction.h"
#include "cpu_instruction_alu.h"
#include "cpu_registers.h"
#include "gbmemory.h"

namespace CPU
{
    enum class AddressMutOperation
    {
        INC,
        DEC,
        NONE
    };

    /* 8-bit load instructions 8 */

    class LD_r_r: public CpuInstruction
    {
    /**
     * @brief 8-bit register to register load instructions
     * All 1 byte opcodes with no further operands
     * All take 1 M-cycle to complete
     */
    private:
        uint8_t* dest;
        uint8_t* src;
        CPU::CpuRegisters& registers;
    public:
        LD_r_r(CpuRegisters& registers, uint8_t* dest, uint8_t* src)
        : registers(registers), dest(dest), src(src) {}
        InstructionResult tick();
    };

    class LD_r_n: public CpuInstruction
    {
    /**
     * @brief 8-bit immediate to register load instructions
     * All 1 byte opcodes with 1 byte operand
     * All take 2 M-cycles to complete
     */
    private:
        uint8_t* dest;
        uint8_t step = 0;
        CPU::CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
    public:
        LD_r_n(CpuRegisters& registers, uint8_t* dest, MEMORY::AddressDispatcher& memory)
        : registers(registers), dest(dest), memory(memory) {}
        InstructionResult tick();
    };

    class LD_r_absrr: public CpuInstruction
    {
    /**
     * @brief Load data from memory at 16-bit absolute address in register to 8-bit register
     * All 1 byte opcodes with no operands
     * All take 2 M-cycles to complete
     * Takes optional AddressMutOperation to allow address src register to be mutated _after_ the memory load
     */
    private:
        uint8_t* dest;
        uint16_t* src_addr;
        uint8_t step = 0;
        CPU::CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
        AddressMutOperation post_operation;
    public:
        LD_r_absrr(CpuRegisters& registers, uint8_t* dest, uint16_t* src_addr, MEMORY::AddressDispatcher& memory, AddressMutOperation post_operation = AddressMutOperation::NONE)
        : registers(registers), dest(dest), src_addr(src_addr), memory(memory), post_operation(post_operation) {}
        InstructionResult tick();
    };

    class LD_absrr_r: public CpuInstruction
    {
    /**
     * @brief Load data from 8-bit register to memory at 16-bit absolute address in register
     * All 1 byte opcodes with no operands
     * All take 2 M-cycles to complete
     * Takes optional AddressMutOperation to allow address dest register to be mutated _after_ the memory write
     */
    private:
        uint16_t* dest_addr;
        uint8_t* src;
        uint8_t step = 0;
        CPU::CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
        AddressMutOperation post_operation;
    public:
        LD_absrr_r(CpuRegisters& registers, uint16_t* dest_addr, uint8_t* src, MEMORY::AddressDispatcher& memory, AddressMutOperation post_operation = AddressMutOperation::NONE)
        : registers(registers), dest_addr(dest_addr), src(src), memory(memory), post_operation(post_operation) {}
        InstructionResult tick();
    };

    class LD_absrr_n: public CpuInstruction
    {
    /**
     * @brief Load data from 8-bit register to memory at 16-bit absolute address in register
     * All 1 byte opcodes with no operands
     * All take 2 M-cycles to complete
     */
    private:
        uint16_t* dest_addr;
        uint8_t step = 0;
        CPU::CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
    public:
        LD_absrr_n(CpuRegisters& registers, uint16_t* dest_addr, MEMORY::AddressDispatcher& memory)
        : registers(registers), dest_addr(dest_addr), memory(memory) {}
        InstructionResult tick();
    };

    class LD_r_absnn: public CpuInstruction
    {
    /**
     * @brief Load data from 16-bit address stored in the operand to 8-bit register
     * All 1 byte opcodes with single 2 byte operand
     * All take 4 M-cycles to complete
     */
    private:
        uint8_t* dest;
        uint16_t load_addr;
        uint8_t step = 0;
        CPU::CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
    public:
        LD_r_absnn(CpuRegisters& registers, uint8_t* dest, MEMORY::AddressDispatcher& memory)
        : registers(registers), dest(dest), memory(memory) {}
        InstructionResult tick();
    };

    class LD_absnn_r: public CpuInstruction
    {
    /**
     * @brief Load data from 8-bit register to 16-bit address stored in the operand
     * All 1 byte opcodes with single 2 byte operand
     * All take 4 M-cycles to complete
     */
    private:
        uint8_t* src;
        uint16_t write_addr;
        uint8_t step = 0;
        CPU::CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
    public:
        LD_absnn_r(CpuRegisters& registers, uint8_t* src, MEMORY::AddressDispatcher& memory)
        : registers(registers), src(src), memory(memory) {}
        InstructionResult tick();
    };

    class LD_r_relr: public CpuInstruction
    {
    /**
     * @brief Load data from 16-bit address constructed from 0xFF MSB + src register as LSB,
     * Data is stored in the target register
     * All 1 byte opcodes with no operands
     * All take 2 M-cycles to complete
     */
    private:
        uint8_t* dest;
        uint8_t* src_addr_lsb;
        uint8_t step = 0;
        CPU::CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
    public:
        LD_r_relr(CpuRegisters& registers, uint8_t* dest, uint8_t* src_addr_lsb, MEMORY::AddressDispatcher& memory)
        : registers(registers), dest(dest), src_addr_lsb(src_addr_lsb), memory(memory) {}
        InstructionResult tick();
    };

    class LD_relr_r: public CpuInstruction
    {
    /**
     * @brief Load data from register,
     * Store in 16-bit address constructed from 0xFF MSB + dest register as LSB
     * All 1 byte opcodes with no operands
     * All take 2 M-cycles to complete
     */
    private:
        uint8_t* dest_addr_lsb;
        uint8_t* src;
        uint8_t step = 0;
        CPU::CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
    public:
        LD_relr_r(CpuRegisters& registers, uint8_t* dest_addr_lsb, uint8_t* src, MEMORY::AddressDispatcher& memory)
        : registers(registers), dest_addr_lsb(dest_addr_lsb), src(src), memory(memory) {}
        InstructionResult tick();
    };

    class LD_r_reln: public CpuInstruction
    {
    /**
     * @brief Load data from 16-bit address constructed from 0xFF MSB + single byte operand
     * Data is stored in the target register
     * All 1 byte opcodes with 1 byte operand
     * All take 3 M-cycles to complete
     */
    private:
        uint8_t* dest;
        uint16_t load_addr;
        uint8_t step = 0;
        CPU::CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
    public:
        LD_r_reln(CpuRegisters& registers, uint8_t* dest, MEMORY::AddressDispatcher& memory)
        : registers(registers), dest(dest), memory(memory) {} // src_addr_lsb(src_addr_lsb)
        InstructionResult tick();
    };

    class LD_reln_r: public CpuInstruction
    {
    /**
     * @brief Load data from register,
     * Store in 16-bit address constructed from 0xFF MSB + single byte operand
     * All 1 byte opcodes with 1 byte operand
     * All take 3 M-cycles to complete
     */
    private:
        uint8_t* src;
        uint16_t write_addr;
        uint8_t step = 0;
        CPU::CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
    public:
        LD_reln_r(CpuRegisters& registers, uint8_t* src, MEMORY::AddressDispatcher& memory)
        : registers(registers), src(src), memory(memory) {}
        InstructionResult tick();
    };

    /* 16-bit load instructions */

    class LD_rr_nn: public CpuInstruction
    {
    /**
     * @brief Load 2 bytes from operand and store in 16-bit register
     * All 1 byte opcodes with 2 byte operand
     * All take 3 M-cycles to complete
     */
    private:
        uint16_t* dest;
        uint16_t operand;
        uint8_t step = 0;
        CPU::CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
    public:
        LD_rr_nn(CpuRegisters& registers, uint16_t* dest, MEMORY::AddressDispatcher& memory)
        : registers(registers), dest(dest), memory(memory) {}
        InstructionResult tick();
    };

    class LD_absnn_rr: public CpuInstruction
    {
    /**
     * @brief Load 2 bytes from 16-bit register and store in memory, at address provided as operand
     * All 1 byte opcodes with 2 byte operand
     * All take 5 M-cycles to complete
     */
    private:
        uint16_t* src;
        uint16_t dest_addr;
        uint8_t step = 0;
        CPU::CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
    public:
        LD_absnn_rr(CpuRegisters& registers, uint16_t* src, MEMORY::AddressDispatcher& memory)
        : registers(registers), src(src), memory(memory) {}
        InstructionResult tick();
    };

    class LD_rr_rr: public CpuInstruction
    {
    /**
     * @brief Load 2 bytes from 16-bit register and store destination 16-bit register
     * All 1 byte opcodes with no operands
     * All take 2 M-cycles to complete
     */
    private:
        uint16_t* src;
        uint16_t* dest;
        uint8_t step = 0;
        CPU::CpuRegisters& registers;
    public:
        LD_rr_rr(CpuRegisters& registers, uint16_t* dest, uint16_t* src)
        : registers(registers), dest(dest), src(src) {}
        InstructionResult tick();
    };

    class PUSH_rr: public CpuInstruction
    {
    /**
     * @brief Push to stack memory from 16-bit register
     * 1 byte opcodes with no operands
     * 4 M-cycles to complete
     */
    private:
        uint16_t* src;
        uint16_t* sp;
        uint8_t step = 0;
        CPU::CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
    public:
        PUSH_rr(CpuRegisters& registers, uint16_t* sp, uint16_t* src, MEMORY::AddressDispatcher& memory)
        : registers(registers), sp(sp), src(src), memory(memory) {}
        InstructionResult tick();
    };

    class POP_rr: public CpuInstruction
    {
    /**
     * @brief Pop to 16-bit register from stack memory
     * 1 byte opcodes with no operands
     * 3 M-cycles to complete
     * Note: in the case of PUSH AF, the FLAGS register is overwritten, changing their values
     */
    private:
        uint16_t* sp;
        uint16_t* dest;
        uint8_t step = 0;
        CPU::CpuRegisters& registers;
        MEMORY::AddressDispatcher& memory;
    public:
        POP_rr(CpuRegisters& registers, uint16_t* dest, uint16_t* sp, MEMORY::AddressDispatcher& memory)
        : registers(registers), dest(dest), sp(sp), memory(memory) {}
        InstructionResult tick();
    };
};

#endif
