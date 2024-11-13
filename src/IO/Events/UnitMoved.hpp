#pragma once

#include <cstdint>
#include <string>
#include "IEvent.h"

namespace sw::io {
    struct UnitMoved : public IEvent {
        UnitMoved(uint32_t unit_id,uint32_t x,uint32_t y,uint32_t oldx = 0,uint32_t oldy = 0) :
            unitId(unit_id),
            x(x),
            y(y),
            oldx(oldx),
            oldy(oldy) {}

        constexpr static const char* Name = "UNIT_MOVED";

        const char* getName() const override {
            return Name;
        }

        uint32_t unitId{};
        uint32_t x{};
        uint32_t y{};
        uint32_t oldx{};
        uint32_t oldy{};

        template <typename Visitor>
        void visit(Visitor& visitor) {
            visitor.visit("unitId", unitId);
            visitor.visit("x", x);
            visitor.visit("y", y);
        }
    };
}
