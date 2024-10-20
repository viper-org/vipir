// Copyright 2024 solar-mist

#ifndef VIPIR_LIR_INSTRUCTION_ENTER_FUNC_H
#define VIPIR_LIR_INSTRUCTION_ENTER_FUNC_H 1

#include "vipir/LIR/Value.h"

namespace vipir
{
    namespace lir
    {
        class EnterFunc : public Value
        {
        friend class opt::Peephole;
        public:
            EnterFunc(int stackSize, std::vector<int> calleeSaved);

            void print(std::ostream& stream) const override;
            void emit(MC::Builder& builder) override;

            void setStackSize(int newSize);
            void setCalleeSaved(std::vector<int> calleeSaved);
            void setSaveFramePointer(bool saveFramePointer);

        private:
            int mStackSize;
            std::vector<int> mCalleeSaved;
            bool mSaveFramePointer;
        };
    }
}

#endif // VIPIR_LIR_INSTRUCTION_ENTER_FUNC_H