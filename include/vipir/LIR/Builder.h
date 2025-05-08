// Copyright 2024 solar-mist

#ifndef VIPIR_LIR_BUILDER_H
#define VIPIR_LIR_BUILDER_H 1

#include "vipir/LIR/Value.h"
#include "vipir/LIR/Section.h"

#include <memory>

namespace vipir
{
    namespace lir
    {
        class EmitSourceInfo : public Value
        {
        public:
            EmitSourceInfo(int line, int col);

            virtual void print(std::ostream& stream) const override;
            virtual void emit(MC::Builder& builder) override;

        private:
            int mLine;
            int mCol;
        };

        class QueryAddress : public Value
        {
        public:
            QueryAddress(uint64_t* location);

            virtual void print(std::ostream& stream) const override;
            virtual void emit(MC::Builder& builder) override;

        private:
            uint64_t* mLocation;
        };

        using ValuePtr = std::unique_ptr<Value>;
        class Builder
        {
        public:
            void addValue(ValuePtr value);
            void insertValue(ValuePtr value, Value* after);
            void setSection(SectionType sect);

            lir::Value* getLastNode();
            int getPosition();

            std::vector<ValuePtr>& getValues();

        private:
            std::vector<ValuePtr> mValues;

            std::vector<int> mInserts;
        };
    }
}

#endif // VIPIR_LIR_BUILDER_H