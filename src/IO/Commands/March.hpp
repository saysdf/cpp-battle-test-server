#pragma once

#include <cstdint>
#include <iosfwd>
#include "ICommand.h"

namespace sw::io {
    struct March : public ICommand {
        constexpr static const char* Name = "MARCH";

        const char* getName() const override {
            return Name;
        }

        uint32_t unitId{};
        uint32_t targetX{};
        uint32_t targetY{};

        template <typename Visitor>
        void visit(Visitor& visitor) {
            visitor.visit("unitId", unitId);
            visitor.visit("targetX", targetX);
            visitor.visit("targetY", targetY);
        }
    };
}
