#pragma once

#include <cstdint>
#include <string>
#include "IEvent.h"
namespace sw::io
{
	struct UnitDied : public IEvent
	{
		explicit UnitDied(uint32_t unit_id) :
				unitId(unit_id)
		{}

		constexpr static const char* Name = "UNIT_DIED";
		const char* getName() const override {
			return Name;
		}
		uint32_t unitId{};

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", unitId);
		}
	};
}
