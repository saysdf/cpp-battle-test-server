#pragma once

#include <cstdint>
#include <string>
#include "IEvent.h"
namespace sw::io
{
	struct UnitSpawned : public IEvent
	{
		UnitSpawned(uint32_t unit_id, const std::string& unit_type, uint32_t x, uint32_t y) :
				unitId(unit_id),
				unitType(unit_type),
				x(x),
				y(y)
		{}

		constexpr static const char* Name = "UNIT_SPAWNED";
		const char* getName() const override {
			return Name;
		}
		uint32_t unitId{};
		std::string unitType{};
		uint32_t x{};
		uint32_t y{};

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", unitId);
			visitor.visit("unitType", unitType);
			visitor.visit("x", x);
			visitor.visit("y", y);
		}
	};
}
