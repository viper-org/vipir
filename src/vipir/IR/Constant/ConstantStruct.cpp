// Coypright 2024 solar-mist


#include "vipir/IR/Constant/ConstantStruct.h"

#include "vipir/Module.h"

#include "vipir/MC/StructOperand.h"

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

    bool ConstantStruct::isConstant() const
    {
        return true;
    }

    void ConstantStruct::emit(MC::Builder& builder)
    {
        std::vector<instruction::OperandPtr> values;
        for (auto value : mValues)
        {
            values.push_back(value->getEmittedValue());
        }

        mEmittedValue = std::make_unique<StructOperand>(std::move(values));
    }

    ConstantStruct::ConstantStruct(Module& module, Type* type, std::vector<Value*> values)
        : Value(module)
        , mValues(std::move(values))
    {
        mType = type;
        assert(mType->isStructType());
    }
}