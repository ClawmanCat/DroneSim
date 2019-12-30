#pragma once

#include "Core.h"

namespace DroneSim::GPU {
    template <typename D, typename BT, bool Modify, bool Read> class IGPUBuffer {
    public:
        constexpr static bool AllowModify = Modify;
        constexpr static bool AllowRead   = Read;
        using BufferedType = BT;


        template <bool B = AllowModify, typename = std::enable_if_t<B>>
        void modify(std::size_t where, std::vector<BufferedType>& data) {
            static_cast<D>(*this).modify(where, std::move(data));
        }


        template <bool B = AllowRead, typename = std::enable_if_t<B>>
        std::vector<BufferedType> fetch(void) {
            return static_cast<D>(*this).fetch();
        }
    protected:
        IGPUBuffer(void) = default;
    };
}