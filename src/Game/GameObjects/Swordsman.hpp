//
// Created by oleg on 11/12/24.
//
#pragma once
#include "Game/Components/HealthComponent.hpp"
#include "Game/Components/MeleeAttackComponent.hpp"
#include "Game/Components/MovementComponent.hpp"
#include "Unit.hpp"

#include <memory>

class Swordsman : public Unit {
public:
    std::shared_ptr<MovementComponent>    movement;
    std::shared_ptr<HealthComponent>      health;
    std::shared_ptr<MeleeAttackComponent> meleeAttack;

    Swordsman(uint32_t id,uint32_t x,uint32_t y,uint32_t hp,uint32_t strength,
              uint32_t ap = 1) :
        Unit(id, x, y, ap),
        movement(std::make_shared<MovementComponent>(position)),
        health(std::make_shared<HealthComponent>(hp)),
        meleeAttack(std::make_shared<MeleeAttackComponent>(strength)) {}
};
