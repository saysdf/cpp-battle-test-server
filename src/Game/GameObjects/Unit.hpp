//
// Created by oleg on 11/11/24.
//
#pragma once
#include "Game/Components/ActionPointsComponent.hpp"
#include "Game/Components/PositionComponent.hpp"

#include <memory>

class Unit {
public:
    uint32_t                               id;
    std::shared_ptr<PositionComponent>     position;
    std::shared_ptr<ActionPointsComponent> actionPoints;

    Unit(uint32_t id,uint32_t x,uint32_t y,uint32_t ap = 1) :
        id(id),
        position(std::make_shared<PositionComponent>(x, y)),
        actionPoints(
                     std::make_shared<ActionPointsComponent>(ap)) {}

    virtual ~Unit() = default;
};
