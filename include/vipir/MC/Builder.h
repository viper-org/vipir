// Copyright 2024 solar-mist

#ifndef VIPIR_MC_BUILDER_H
#define VIPIR_MC_BUILDER_H 1

#include "vasm/instruction/Value.h"

#include <memory>

namespace vipir
{
    class DIBuilder;

    namespace MC
    {
        class EmitSourceInfo : public instruction::Value
        {
        friend class vipir::DIBuilder;
        public:
            EmitSourceInfo(int line, int col);

            virtual void emit(codegen::OpcodeBuilder& builder, codegen::Section section) override;
            virtual void print(std::ostream& stream) override;

        private:
            int mLine;
            int mCol;

            int mAddress{ -1 };
        };

        class QueryAddress : public instruction::Value
        {
        public:
            QueryAddress(uint64_t* location);

            virtual void emit(codegen::OpcodeBuilder& builder, codegen::Section section) override;
            virtual void print(std::ostream& stream) override;

        private:
            uint64_t* mLocation;
        };


        class Builder
        {
        public:
            Builder();

            void addValue(instruction::ValuePtr value);

            std::vector<instruction::ValuePtr>& getValues();
            std::vector<EmitSourceInfo*>& getSourceInfo();

        private:
            std::vector<instruction::ValuePtr> mValues;
            std::vector<EmitSourceInfo*> mSourceInfo;
        };
    }
}

#endif // VIPIR_MC_BUILDER_H