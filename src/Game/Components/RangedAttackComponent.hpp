//
// Created by oleg on 11/12/24.
//
#pragma once
#include <cstdint>

class RangedAttackComponent {
private:
    int agility;
    int range;
    uint32_t apCost;

public:
    RangedAttackComponent(int attackAgility, int attackRange, uint32_t apCost = 1) : agility(attackAgility),
        range(attackRange),
        apCost(apCost) {}

    const uint32_t getApCost() const { return apCost; }
    int getDamage() const { return agility; }
    int getRange() const { return range; }
    int getRangeMin() const { return 0; }
};
