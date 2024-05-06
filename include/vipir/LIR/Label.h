// Copyright 2024 solar-mist

#ifndef VIPIR_LIR_LABEL_H
#define VIPIR_LIR_LABEL_H 1

#include "vipir/LIR/Value.h"

namespace vipir
{
    namespace lir
    {
        class Label : public Value
        {
        public:
            Label(std::string name, bool global);

            void print(std::ostream& stream) const override;
            void emit(MC::Builder& builder) override;
        
        private:
            std::string mName;
            bool mGlobal;
        };

        class ExternLabel : public Value
        {
        public:
            ExternLabel(std::string name);

            void print(std::ostream& stream) const override;
            void emit(MC::Builder& builder) override;
        
        private:
            std::string mName;
        };
    }
}

#endif // VIPIR_LIR_LABEL_H