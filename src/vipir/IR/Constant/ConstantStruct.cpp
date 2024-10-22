// Coypright 2024 solar-mist


#include "vipir/IR/Constant/ConstantStruct.h"

#include "vipir/Module.h"

#include "vipir/MC/CompoundOperand.h"

#include <format>

namespace vipir
{

    ConstantStruct* ConstantStruct::Get(Module& module, Type* type, std::vector<Value*> values)
    {
        ConstantStruct* constant = new ConstantStruct(module, type, std::move(values));
        module.insertConstant(constant);
        return constant;
    }

    void ConstantStruct::print(std::ostream&)
    {
    }

    std::string ConstantStruct::ident() const
    {
        std::string ret = std::format("struct {} {{", mType->getName());
        for (auto value : mValues)
        {
            ret += value->ident() + ' ';
        }
        return ret + "}";
    }

    void ConstantStruct::doConstantFold()
    {
    }

    bool ConstantStruct::isConstant() const
    {
        return true;
    }

    void ConstantStruct::emit(lir::Builder& builder)
    {
        std::vector<lir::OperandPtr> values;
        for (auto value : mValues)
        {
            value->lateEmit(builder);
            values.push_back(value->getEmittedValue());
        }

        mEmittedValue = std::make_unique<lir::Compound>(std::move(values));
    }

    ConstantStruct::ConstantStruct(Module& module, Type* type, std::vector<Value*> values)
        : Value(module)
        , mValues(std::move(values))
    {
        mType = type;
        assert(mType->isStructType());
    }
}