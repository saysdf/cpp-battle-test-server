#pragma once

#include <cstdint>
#include <iosfwd>
#include "ICommand.h"

namespace sw::io {
    struct CreateMap : public ICommand {
        constexpr static const char* Name = "CREATE_MAP";

        const char* getName() const override {
            return Name;
        }

        uint32_t width{};
        uint32_t height{};

        template <typename Visitor>
        void visit(Visitor& visitor) {
            visitor.visit("width", width);
            visitor.visit("height", height);
        }
    };
}
