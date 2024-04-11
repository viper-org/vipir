// Copyright 2023 solar-mist

/*
 * A module represents a single translation unit(a file)
 * to be compiled using vipir
*/

#ifndef VIPIR_MODULE_H
#define VIPIR_MODULE_H 1

#include <memory>
#include <string>
#include <vector>

namespace vipir
{
    enum class OutputFormat
    {
        ELF,
        PE,
    };

    class Module
    {
    public:
        Module(std::string name);

        std::string_view getName() const;

        void print(std::ostream& stream) const;

        void emit(std::ostream& stream, OutputFormat outputFormat) const;
    private:
        std::string mName;
    };
}

#endif // VIPIR_MODULE_H