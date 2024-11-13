//
// Created by oleg on 11/11/24.
//
#pragma once
#include <cstdint>

class HealthComponent {
private:
    int64_t hp;
    uint32_t maxHp;

public:
    HealthComponent(uint32_t maxHp) : hp(maxHp),
                                      maxHp(maxHp) {}

    void takeDamage(uint32_t amount) {
        hp -= amount;
        if (hp < 0) hp = 0;
    }

    bool isAlive() const { return hp > 0; }

    uint32_t getHP() const { return hp; }
};
