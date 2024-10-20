// Copyright 2024 solar-mist

#ifndef VIPIR_LIR_OPERAND_H
#define VIPIR_LIR_OPERAND_H 1

#include "vipir/Optimizer/RegAlloc/VReg.h"

#include "vasm/instruction/Operand.h"
#include "vasm/codegen/Opcodes.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace vipir
{
    namespace opt
    {
        class Peephole;
        class PeepholeV2;
    }

    namespace lir
    {
        class Operand;
        using OperandPtr = std::unique_ptr<Operand>;
        class Operand
        {
        friend class opt::Peephole;
        friend class opt::PeepholeV2;
        public:
            virtual ~Operand() { }
            
            virtual std::string ident() const = 0;

            virtual instruction::OperandPtr asmOperand() = 0;

            virtual OperandPtr clone() = 0;
            virtual bool operator==(OperandPtr& other) = 0;

            virtual bool isMemory() { return false; }
            virtual codegen::OperandSize size() = 0;
        };

        class Immediate : public Operand
        {
        friend class opt::Peephole;
        friend class opt::PeepholeV2;
        public:
            Immediate(std::intmax_t value);

            std::string ident() const override;
            instruction::OperandPtr asmOperand() override;
            OperandPtr clone() override;
            bool operator==(OperandPtr& other) override;
            codegen::OperandSize size() override;

            std::intmax_t value() const;

        private:
            std::intmax_t mValue;
        };

        class PhysicalReg : public Operand
        {
        friend class opt::Peephole;
        friend class opt::PeepholeV2;
        friend class VirtualReg;
        public:
            PhysicalReg(int id, codegen::OperandSize size);

            std::string ident() const override;
            instruction::OperandPtr asmOperand() override;
            OperandPtr clone() override;
            bool operator==(OperandPtr& other) override;
            codegen::OperandSize size() override;

        private:
            int mId;
            codegen::OperandSize mSize;
        };

        class VirtualReg : public Operand
        {
        friend class opt::Peephole;
        friend class opt::PeepholeV2;
        friend class PhysicalReg;
        public:
            VirtualReg(opt::VReg* reg, codegen::OperandSize size);

            std::string ident() const override;
            instruction::OperandPtr asmOperand() override;
            OperandPtr clone() override;
            bool operator==(OperandPtr& other) override;
            bool isMemory() override;
            codegen::OperandSize size() override;

        private:
            opt::VReg* mVreg;
            codegen::OperandSize mSize;
        };

        class Lbl : public Operand
        {
        friend class opt::PeepholeV2;
        public:
            Lbl(std::string name, bool plt);

            std::string ident() const override;
            instruction::OperandPtr asmOperand() override;
            OperandPtr clone() override;
            bool operator==(OperandPtr& other) override;
            codegen::OperandSize size() override;

        private:
            std::string mName;
            bool mPlt;
        };

        class CMP : public Operand
        {
        friend class opt::Peephole;
        friend class opt::PeepholeV2;
        public:
            enum class Operator
            {
                EQ, NE,
                LT, GT,
                LE, GE
            };

            CMP(Operator op);

            std::string ident() const override;
            instruction::OperandPtr asmOperand() override;
            OperandPtr clone() override;
            bool operator==(OperandPtr& other) override;
            codegen::OperandSize size() override;

            std::string operatorName();

        private:
            Operator mOperator;
        };

        class Memory : public Operand
        {
        friend class opt::Peephole;
        friend class opt::PeepholeV2;
        public:
            Memory(codegen::OperandSize size, OperandPtr base, std::optional<int> displacement, OperandPtr index, std::optional<int> scale);

            std::string ident() const override;
            instruction::OperandPtr asmOperand() override;
            OperandPtr clone() override;
            bool operator==(OperandPtr& other) override;
            bool isMemory() override;
            codegen::OperandSize size() override;

            OperandPtr base();

        private:
            codegen::OperandSize mSize;
            OperandPtr mBase;
            std::optional<int> mDisplacement;
            OperandPtr mIndex;
            std::optional<int> mScale;
        };

        class Compound : public Operand
        {
        friend class opt::Peephole;
        friend class opt::PeepholeV2;
        public:
            Compound(std::vector<OperandPtr> values);

            std::string ident() const override;
            instruction::OperandPtr asmOperand() override;
            OperandPtr clone() override;
            bool operator==(OperandPtr& other) override;
            codegen::OperandSize size() override;

            std::vector<OperandPtr>& getValues();

        private:
            std::vector<OperandPtr> mValues;
        };
    }
}

#endif // VIPIR_LIR_OPERAND_H