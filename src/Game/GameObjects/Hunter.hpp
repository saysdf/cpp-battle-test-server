//
// Created by oleg on 11/12/24.
//
#pragma once
#include "Game/Components/HealthComponent.hpp"
#include "Game/Components/MeleeAttackComponent.hpp"
#include "Game/Components/MovementComponent.hpp"
#include "Game/Components/RangedAttackComponent.hpp"
#include "Unit.hpp"

#include <memory>

class Hunter : public Unit {
public:
    std::shared_ptr<MovementComponent>     movement;
    std::shared_ptr<HealthComponent>       health;
    std::shared_ptr<MeleeAttackComponent>  meleeAttack;
    std::shared_ptr<RangedAttackComponent> rangedAttack;

    Hunter(uint32_t id,uint32_t x,uint32_t y,uint32_t hp,uint32_t strength,uint32_t agility,uint32_t range,
           uint32_t ap = 1) :
        Unit(id, x, y, ap),
        movement(std::make_shared<MovementComponent>(position)),
        health(std::make_shared<HealthComponent>(hp)),
        meleeAttack(std::make_shared<MeleeAttackComponent>(strength)),
        rangedAttack(std::make_shared<RangedAttackComponent>(agility, range)) {}
};
