// Copyright 2024 solar-mist

#ifndef VIPIR_LIR_INSTRUCTION_RET_H
#define VIPIR_LIR_INSTRUCTION_RET_H 1

#include "vipir/LIR/Value.h"

namespace vipir
{
    namespace lir
    {
        class Ret : public Value
        {
        friend class opt::Peephole;
        public:
            Ret(bool leave, std::vector<int> calleeSaved);

            void print(std::ostream& stream) const override;
            void emit(MC::Builder& builder) override;

            void setLeave(bool leave);
            void setCalleeSaved(std::vector<int> calleeSaved);

        private:
            bool mLeave;
            std::vector<int> mCalleeSaved;
        };
    }
}

#endif // VIPIR_LIR_INSTRUCTION_RET_H