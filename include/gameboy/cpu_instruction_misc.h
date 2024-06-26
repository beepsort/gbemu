#ifndef __CPU_INSTRUCTION_MISC_H__
#define __CPU_INSTRUCTION_MISC_H__

#include "gameboy/cpu_instruction.h"
#include "gameboy/cpu_registers.h"
#include "gameboy/memory.h"

namespace GAMEBOY
{
    class NOP: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
    public:
        NOP(CpuRegisters& registers)
        : registers(registers) {}
        InstructionResult tick();
    };

    class CB_PREFIX: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        AddressDispatcher& memory;
        CpuInstruction* instruction = nullptr;
    public:
        CB_PREFIX(CpuRegisters& registers, AddressDispatcher& memory)
        : registers(registers), memory(memory) {}
        ~CB_PREFIX();
        InstructionResult tick();
    };

    class RLC_r: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        uint8_t* target;
    public:
        RLC_r(CpuRegisters& registers, uint8_t* target)
        : registers(registers), target(target) {}
        InstructionResult tick();
    };

    class RLC_absHL: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        AddressDispatcher& memory;
        uint16_t shift_register;
        uint8_t step = 0;
    public:
        RLC_absHL(CpuRegisters& registers, AddressDispatcher& memory)
        : registers(registers), memory(memory) {}
        InstructionResult tick();
    };

    class RRC_r: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        uint8_t* target;
    public:
        RRC_r(CpuRegisters& registers, uint8_t* target)
        : registers(registers), target(target) {}
        InstructionResult tick();
    };

    class RRC_absHL: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        AddressDispatcher& memory;
        uint8_t shift_register;
        uint8_t step = 0;
    public:
        RRC_absHL(CpuRegisters& registers, AddressDispatcher& memory)
        : registers(registers), memory(memory) {}
        InstructionResult tick();
    };

    class RL_r: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        uint8_t* target;
    public:
        RL_r(CpuRegisters& registers, uint8_t* target)
        : registers(registers), target(target) {}
        InstructionResult tick();
    };

    class RL_absHL: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        AddressDispatcher& memory;
        uint8_t result;
        uint8_t step = 0;
    public:
        RL_absHL(CpuRegisters& registers, AddressDispatcher& memory)
        : registers(registers), memory(memory) {}
        InstructionResult tick();
    };

    class RR_r: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        uint8_t* target;
    public:
        RR_r(CpuRegisters& registers, uint8_t* target)
        : registers(registers), target(target) {}
        InstructionResult tick();
    };

    class RR_absHL: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        AddressDispatcher& memory;
        uint8_t result;
        uint8_t step = 0;
    public:
        RR_absHL(CpuRegisters& registers, AddressDispatcher& memory)
        : registers(registers), memory(memory) {}
        InstructionResult tick();
    };

    class SLA_r: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        uint8_t* target;
    public:
        SLA_r(CpuRegisters& registers, uint8_t* target)
        : registers(registers), target(target) {}
        InstructionResult tick();
    };

    class SLA_absHL: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        AddressDispatcher& memory;
        uint8_t result;
        uint8_t step = 0;
    public:
        SLA_absHL(CpuRegisters& registers, AddressDispatcher& memory)
        : registers(registers), memory(memory) {}
        InstructionResult tick();
    };

    class SRA_r: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        uint8_t* target;
    public:
        SRA_r(CpuRegisters& registers, uint8_t* target)
        : registers(registers), target(target) {}
        InstructionResult tick();
    };

    class SRA_absHL: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        AddressDispatcher& memory;
        uint8_t loaded;
        uint8_t step = 0;
    public:
        SRA_absHL(CpuRegisters& registers, AddressDispatcher& memory)
        : registers(registers), memory(memory) {}
        InstructionResult tick();
    };

    class SWAP_r: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        uint8_t* target;
    public:
        SWAP_r(CpuRegisters& registers, uint8_t* target)
        : registers(registers), target(target) {}
        InstructionResult tick();
    };

    class SWAP_absHL: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        AddressDispatcher& memory;
        uint8_t loaded;
        uint8_t step = 0;
    public:
        SWAP_absHL(CpuRegisters& registers, AddressDispatcher& memory)
        : registers(registers), memory(memory) {}
        InstructionResult tick();
    };

    class SRL_r: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        uint8_t* target;
    public:
        SRL_r(CpuRegisters& registers, uint8_t* target)
        : registers(registers), target(target) {}
        InstructionResult tick();
    };

    class SRL_absHL: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        AddressDispatcher& memory;
        uint8_t result;
        uint8_t step = 0;
    public:
        SRL_absHL(CpuRegisters& registers, AddressDispatcher& memory)
        : registers(registers), memory(memory) {}
        InstructionResult tick();
    };

    class BIT_r: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        uint8_t* target;
        uint8_t bitnum;
    public:
        BIT_r(CpuRegisters& registers, uint8_t* target, uint8_t bitnum)
        : registers(registers), target(target), bitnum(bitnum) {}
        InstructionResult tick();
    };

    class BIT_absHL: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        AddressDispatcher& memory;
        uint8_t loaded;
        uint8_t bitnum;
        uint8_t step = 0;
    public:
        BIT_absHL(CpuRegisters& registers, AddressDispatcher& memory, uint8_t bitnum)
        : registers(registers), memory(memory), bitnum(bitnum) {}
        InstructionResult tick();
    };

    class RES_r: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        uint8_t* target;
        uint8_t bitnum;
    public:
        RES_r(CpuRegisters& registers, uint8_t* target, uint8_t bitnum)
        : registers(registers), target(target), bitnum(bitnum) {}
        InstructionResult tick();
    };

    class RES_absHL: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        AddressDispatcher& memory;
        uint8_t bitnum;
        uint8_t result;
        uint8_t step = 0;
    public:
        RES_absHL(CpuRegisters& registers, AddressDispatcher& memory, uint8_t bitnum)
        : registers(registers), memory(memory), bitnum(bitnum) {}
        InstructionResult tick();
    };

    class SET_r: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        uint8_t* target;
        uint8_t bitnum;
    public:
        SET_r(CpuRegisters& registers, uint8_t* target, uint8_t bitnum)
        : registers(registers), target(target), bitnum(bitnum) {}
        InstructionResult tick();
    };

    class SET_absHL: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
        AddressDispatcher& memory;
        uint8_t bitnum;
        uint8_t result;
        uint8_t step = 0;
    public:
        SET_absHL(CpuRegisters& registers, AddressDispatcher& memory, uint8_t bitnum)
        : registers(registers), memory(memory), bitnum(bitnum) {}
        InstructionResult tick();
    };
};

#endif
