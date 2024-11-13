#pragma once

#include <cstdint>
#include <string>
#include "IEvent.h"
namespace sw::io
{
	struct UnitAttacked : public IEvent
	{
		UnitAttacked(uint32_t attacker_unit_id, uint32_t target_unit_id, uint32_t damage, uint32_t target_hp) :
				attackerUnitId(attacker_unit_id),
				targetUnitId(target_unit_id),
				damage(damage),
				targetHp(target_hp)
		{}

		constexpr static const char* Name = "UNIT_ATTACKED";
		const char* getName() const override {
			return Name;
		}
		uint32_t attackerUnitId{};
		uint32_t targetUnitId{};
		uint32_t damage{};
		uint32_t targetHp{};

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("attackerUnitId", attackerUnitId);
			visitor.visit("targetUnitId", targetUnitId);
			visitor.visit("damage", damage);
			visitor.visit("targetHp", targetHp);
		}
	};
}
