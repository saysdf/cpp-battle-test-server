#pragma once

#include <cstdint>
#include "IEvent.h"

namespace sw::io {
	struct MarchEnded : public IEvent {
		MarchEnded(uint32_t unit_id,uint32_t x,uint32_t y) :
			unitId(unit_id),
			x(x),
			y(y) {}

		constexpr static const char* Name = "MARCH_ENDED";

		const char* getName() const override {
			return Name;
		}

		uint32_t unitId{};
		uint32_t x{};
		uint32_t y{};

		template <typename Visitor>
		void visit(Visitor& visitor) {
			visitor.visit("unitId", unitId);
			visitor.visit("x", x);
			visitor.visit("y", y);
		}
	};
}
