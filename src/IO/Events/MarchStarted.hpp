#pragma once

#include <cstdint>
#include "IEvent.h"
namespace sw::io
{
	struct MarchStarted : public IEvent
	{
		MarchStarted(uint32_t unit_id, uint32_t x, uint32_t y, uint32_t target_x, uint32_t target_y) :
				unitId(unit_id),
				x(x),
				y(y),
				targetX(target_x),
				targetY(target_y)
		{}
		constexpr static const char* Name = "MARCH_STARTED";
		const char* getName() const override {
			return Name;
		}
		uint32_t unitId{};
		uint32_t x{};
		uint32_t y{};



		uint32_t targetX{};
		uint32_t targetY{};

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", unitId);
			visitor.visit("x", x);
			visitor.visit("y", y);
			visitor.visit("targetX", targetX);
			visitor.visit("targetY", targetY);
		}
	};
}
