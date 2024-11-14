//
// Created by oleg on 11/12/24.
//
#pragma once
#include "PositionComponent.hpp"
#include <memory>

class MovementComponent {
private:
    std::shared_ptr<PositionComponent> position;
    uint32_t                           targetX, targetY;
    uint32_t                           apCost;
    uint32_t                           maxRange;
    bool                               newTargetSet;
    bool                               hasStartedMovement; // Флаг для события начала движения
    bool                               hasReachedTarget;   // Флаг для события завершения движения

public:
    MovementComponent(std::shared_ptr<PositionComponent> position,uint32_t maxRange = 1,uint32_t apCost = 1) :
        position(position),
        targetX(0), targetY(0),
        apCost(apCost),
        maxRange(maxRange),
        newTargetSet(false),
        hasStartedMovement(false),
        hasReachedTarget(false) {}

    const uint32_t getApCost() const {
        return apCost;
    }

    std::shared_ptr<PositionComponent> getPosition() {
        return position;
    }

    void setTarget(uint32_t x,uint32_t y) {
        targetX            = x;
        targetY            = y;
        newTargetSet       = true;
        hasStartedMovement = false; // Сбрасываем флаг начала движения
        hasReachedTarget   = false; // Сбрасываем флаг завершения движения
    }

    void moveTowardsTarget() {
        if (!newTargetSet || atTarget())
            return;

        uint32_t dx       = std::abs(static_cast<int>((targetX) - position->x));
        uint32_t dy       = std::abs(static_cast<int>((targetY) - position->y));
        uint32_t distance = std::min(maxRange, std::max(dx, dy));

        if (position->x < targetX)
            position->x = std::min(position->x + distance, targetX);
        else if (position->x > targetX)
            position->x = std::max(position->x - distance, targetX);

        if (position->y < targetY)
            position->y = std::min(position->y + distance, targetY);
        else if (position->y > targetY)
            position->y = std::max(position->y - distance, targetY);
    }

    void resetMove(uint32_t x,uint32_t y) {
        position->x        = x;
        position->y        = y;
        newTargetSet       = false;
        hasStartedMovement = false;
        hasReachedTarget   = false;
    }

    bool atTarget() {
        if (position->x == targetX && position->y == targetY) {
            newTargetSet = false;
            return true;
        }
        return false;
    }

    uint32_t getTargetX() const {
        return targetX;
    }

    uint32_t getTargetY() const {
        return targetY;
    }

    bool hasNewTarget() const {
        return newTargetSet;
    }

    bool hasStarted() const {
        return hasStartedMovement;
    }

    bool hasFinished() const {
        return hasReachedTarget;
    }

    void startMovement() {
        hasStartedMovement = true;
    } // начало движения
    void reachTarget() {
        hasReachedTarget = true;
    }
};
