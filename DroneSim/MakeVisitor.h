#pragma once

#include "Core.h"


namespace DroneSim::Traits {
    template <typename... Preds> struct Visitor : public Preds... {
        Visitor(const Preds&... preds) : Preds(preds)... {}

        using Preds::operator()...;
    };


    template <typename... Preds> constexpr inline auto MakeVisitor(const Preds&... preds) {
        return Visitor<Preds...>(preds...);
    }
}