#pragma once

#include <cstdint>
#include <iosfwd>
#include "ICommand.h"

namespace sw::io {
    struct SpawnSwordsman : public ICommand {
        constexpr static const char* Name = "SPAWN_SWORDSMAN";

        const char* getName() const override {
            return Name;
        }

        uint32_t unitId{};
        uint32_t x{};
        uint32_t y{};
        uint32_t hp{};
        uint32_t strength{};

        template <typename Visitor>
        void visit(Visitor& visitor) {
            visitor.visit("unitId", unitId);
            visitor.visit("x", x);
            visitor.visit("y", y);
            visitor.visit("hp", hp);
            visitor.visit("strength", strength);
        }
    };
}
