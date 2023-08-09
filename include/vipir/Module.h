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
    class Global;
    using GlobalPtr = std::unique_ptr<Global>;

    class Module
    {
    public:
        Module(std::string name);

        std::string_view getName() const;
        const std::vector<GlobalPtr>& getGlobals() const;
        
        void insertGlobal(Global* global);

        void print(std::ostream& stream) const;
    private:
        std::string mName;
        std::vector<GlobalPtr> mGlobals;
    };
}

#endif // VIPIR_MODULE_H