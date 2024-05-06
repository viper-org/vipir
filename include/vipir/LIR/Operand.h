// Copyright 2024 solar-mist

#ifndef VIPIR_LIR_OPERAND_H
#define VIPIR_LIR_OPERAND_H 1

#include "vipir/Optimizer/RegAlloc/VReg.h"

#include "vasm/instruction/Operand.h"
#include "vasm/codegen/Opcodes.h"

#include <cstdint>
#include <memory>
#include <string>

namespace vipir
{
    namespace lir
    {
        class Operand;
        using OperandPtr = std::unique_ptr<Operand>;
        class Operand
        {
        public:
            virtual std::string ident() const = 0;

            virtual instruction::OperandPtr asmOperand() = 0;

            virtual OperandPtr clone() = 0;
            virtual bool operator==(OperandPtr& other) = 0;
        };

        class Immediate : public Operand
        {
        public:
            Immediate(std::intmax_t value);

            std::string ident() const override;
            instruction::OperandPtr asmOperand() override;
            OperandPtr clone() override;
            bool operator==(OperandPtr& other) override;

        private:
            std::intmax_t mValue;
        };

        class PhysicalReg : public Operand
        {
        friend class VirtualReg;
        public:
            PhysicalReg(int id, codegen::OperandSize size);

            std::string ident() const override;
            instruction::OperandPtr asmOperand() override;
            OperandPtr clone() override;
            bool operator==(OperandPtr& other) override;

        private:
            int mId;
            codegen::OperandSize mSize;
        };

        class VirtualReg : public Operand
        {
        friend class PhysicalReg;
        public:
            VirtualReg(opt::VReg* reg, codegen::OperandSize size);

            std::string ident() const override;
            instruction::OperandPtr asmOperand() override;
            OperandPtr clone() override;
            bool operator==(OperandPtr& other) override;

        private:
            opt::VReg* mVreg;
            codegen::OperandSize mSize;
        };
    }
}

#endif // VIPIR_LIR_OPERAND_H