#ifndef __REGISTERS_H__
#define __REGISTERS_H__

#include <stdint.h>

namespace CPU
{
    class CpuRegisters
    {
    private:
        static const uint8_t FLAG_ZERO = 0b0001;
        static const uint8_t FLAG_SUB = 0b0010;
        static const uint8_t FLAG_HALFCARRY = 0b0100;
        static const uint8_t FLAG_CARRY = 0b1000;
    public:
        uint16_t* AF = new uint16_t(0x0001);
        uint8_t* A = (uint8_t*) AF;
        uint8_t* FLAGS = (uint8_t*) AF + 1;
        uint16_t* BC = new uint16_t(0x0000);
        uint8_t* B = (uint8_t*) BC;
        uint8_t* C = (uint8_t*) BC + 1;
        uint16_t* DE = new uint16_t(0x0000);
        uint8_t* D = (uint8_t*) DE;
        uint8_t* E = (uint8_t*) DE + 1;
        uint16_t* HL = new uint16_t(0x0000);
        uint8_t* H = (uint8_t*) HL;
        uint8_t* L = (uint8_t*) HL + 1;
        uint16_t* SP = new uint16_t(0x0000);
        uint16_t* PC = new uint16_t(0x0100);
        // Interupt Master Enable
        bool IME = true;

        /* CPU Flags */
        bool get_flag_zero()
        {
            return *FLAGS & FLAG_ZERO;
        }

        bool get_flag_sub()
        {
            return *FLAGS & FLAG_SUB;
        }

        bool get_flag_halfcarry()
        {
            return *FLAGS & FLAG_HALFCARRY;
        }

        bool get_flag_carry()
        {
            return *FLAGS & FLAG_CARRY;
        }

        void set_flag_zero(bool value)
        {
            if (value==true) *FLAGS |= FLAG_ZERO;
            else *FLAGS &= ~FLAG_ZERO;
        }

        void set_flag_sub(bool value)
        {
            if (value==true) *FLAGS |= FLAG_SUB;
            else *FLAGS &= ~FLAG_SUB;
        }

        void set_flag_halfcarry(bool value)
        {
            if (value==true) *FLAGS |= FLAG_HALFCARRY;
            else *FLAGS &= ~FLAG_HALFCARRY;
        }

        void set_flag_carry(bool value)
        {
            if (value==true) *FLAGS |= FLAG_CARRY;
            else *FLAGS &= ~FLAG_CARRY;
        }
    };
};

#endif
