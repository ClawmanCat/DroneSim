#pragma once

#include "Core.h"
#include "IGPURunnable.h"

#include <string>

namespace DroneSim::GPU {
    template <typename D, typename PT> class IGPUCompiler {
    public:
        using ProgramType = PT;


        static D& instance(void) {
            return static_cast<D&>(*this).instance();
        }

        ProgramType compile(const std::string& name) {
            return static_cast<D&>(*this).compile(name);
        }
    protected:
        IGPUCompiler(void) = default;
    };
}