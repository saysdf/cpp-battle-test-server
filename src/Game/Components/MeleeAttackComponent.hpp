//
// Created by oleg on 11/12/24.
//
#pragma once
#include <cstdint>

class MeleeAttackComponent {
private:
    int strength;
    uint32_t apCost;

public:
    explicit MeleeAttackComponent(int attackStrength, uint32_t apCost = 1) : strength(attackStrength),
                                                                             apCost(apCost) {}

    const uint32_t getApCost() const { return apCost; }
    int getDamage() const { return strength; }
};
