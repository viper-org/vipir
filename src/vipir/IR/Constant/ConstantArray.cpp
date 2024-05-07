// Coypright 2024 solar-mist


#include "vipir/IR/Constant/ConstantArray.h"

#include "vipir/Module.h"

#include "vipir/MC/CompoundOperand.h"

#include "vipir/Type/ArrayType.h"

#include <format>

namespace vipir
{

    ConstantArray* ConstantArray::Get(Module& module, Type* type, std::vector<Value*> values)
    {
        ConstantArray* constant = new ConstantArray(module, type, std::move(values));
        module.insertConstant(constant);
        return constant;
    }

    void ConstantArray::print(std::ostream&)
    {
    }

    std::string ConstantArray::ident() const
    {
        std::string ret = std::format("array {} [", mType->getName());
        for (auto value : mValues)
        {
            ret += value->ident() + ' ';
        }
        return ret + "]";
    }

    bool ConstantArray::isConstant() const
    {
        return true;
    }

    void ConstantArray::emit(lir::Builder& builder)
    {
        std::vector<lir::OperandPtr> values;
        for (auto value : mValues)
        {
            value->lateEmit(builder);
            values.push_back(value->getEmittedValue());
        }

        mEmittedValue = std::make_unique<lir::Compound>(std::move(values));
    }

    ConstantArray::ConstantArray(Module& module, Type* type, std::vector<Value*> values)
        : Value(module)
        , mValues(std::move(values))
    {
        mType = type;
        assert(mType->isArrayType());
        ArrayType* arrayType = static_cast<ArrayType*>(mType);
        assert(mValues.size() == arrayType->getCount());
        for (auto value : mValues)
        {
            assert(value->getType() == arrayType->getBaseType());
        }
    }
}