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
            void setStackSize(int stackSize);
            void setSaveFramePointer(bool saveFramePointer);

        private:
            bool mLeave;
            std::vector<int> mCalleeSaved;
            int mStackSize;
            bool mSaveFramePointer;
        };
    }
}

#endif // VIPIR_LIR_INSTRUCTION_RET_H