//
// Created by oleg on 11/12/24.
//
#pragma once
#include "Game/Components/MovementComponent.hpp"
#include "IO/Events/UnitMoved.hpp"
#include "IO/Events/MarchStarted.hpp"
#include "IO/Events/MarchEnded.hpp"

#include <memory>
#include <unordered_map>

class MovementSystem {
private:
    std::unordered_map<uint32_t, std::shared_ptr<PositionComponent>>     positionComponents;
    std::unordered_map<uint32_t, std::shared_ptr<MovementComponent>>     movementComponents;
    std::unordered_map<uint32_t, std::shared_ptr<ActionPointsComponent>> actionPointsComponents;

    EventManager&        eventManager;
    std::shared_ptr<Map> map;

public:
    MovementSystem(EventManager& em) :
        eventManager(em) {
        eventManager.subscribe<sw::io::UnitDied>([this](const sw::io::UnitDied& event) {
            positionComponents.erase(event.unitId);
            movementComponents.erase(event.unitId);
            actionPointsComponents.erase(event.unitId);
        });
    }

    void registerMap(std::shared_ptr<Map> map) {
        this->map = map;
    }

    void addActionPointsComponent(uint32_t id,std::shared_ptr<ActionPointsComponent> ap) {
        actionPointsComponents[id] = ap;
    }

    void addPositionComponent(uint32_t id,std::shared_ptr<PositionComponent> position) {
        positionComponents[id] = position;
    }

    void addMovementComponent(uint32_t id,std::shared_ptr<MovementComponent> movement) {
        movementComponents[id] = movement;
    }

    template <typename TUnit>
    void registerObject(TUnit& visitor) {
        addPositionComponent(visitor->id, visitor->position);
        addMovementComponent(visitor->id, visitor->movement);
        addActionPointsComponent(visitor->id, visitor->actionPoints);
    }

    void setTarget(uint32_t unitId,int targetX,int targetY) {
        auto it = movementComponents.find(unitId);
        if (it != movementComponents.end()) {
            auto movement = it->second;
            movement->setTarget(targetX, targetY);
        }
    }

    bool performAp(uint32_t unitId,uint32_t cost) {
        auto it = actionPointsComponents.find(unitId);
        if (it != actionPointsComponents.end()) {
            return it->second->doAction(cost);
        }
        return false;
    }

    void moveUnit(uint32_t unitId,std::shared_ptr<MovementComponent> movement) {
        auto position = movement->getPosition();
        auto x        = position->x;
        auto y        = position->y;

        // Проверяем, началось ли движение
        if (!movement->hasStarted()) {
            movement->startMovement();
        }

        // Выполняем движение и публикуем событие UnitMoved тратим АП
        if (performAp(unitId, movement->getApCost())) {
            // TODO добавить проверки на летающий не летающий и т.д. что в цвелевой ячейке нет блокирующего юнита (вообще нужно только если у юнитов будут команды или невозможен бой по какой-то причине)
            uint32_t oldx = x;
            uint32_t oldy = y;
            movement->moveTowardsTarget();
            //отменяем движение если в место куда хочет прийти юнит уже занято кем-то кто блокирует ячейку. но если сам юнит не блокирующий можно пустить
            if ((!map->isCellEmpty(position->x, position->y)) && position->isBlocking)
                movement->resetMove(oldx, oldy);
            eventManager.publishEvent(sw::io::UnitMoved{unitId, x, y, oldx, oldy});


            // Проверяем, если цель достигнута, и публикуем событие MarchEnded
            if (movement->atTarget() && !movement->hasFinished()) {
                eventManager.publishEvent(sw::io::MarchEnded{unitId, x, y});
                movement->reachTarget(); // Сбрасываем флаг
            }
        }
    }

    void update(uint32_t unitId) {
        auto it = movementComponents.find(unitId);
        if (it != movementComponents.end()) {
            auto movement = it->second;
            if (movement->hasNewTarget() && !movement->atTarget()) {
                moveUnit(unitId,movement);
            }
        }
    }

    void update() {
        for (auto& [id, movement] : movementComponents) {
            if (movement->hasNewTarget() && !movement->atTarget()) {
                moveUnit(id,movement);
            }
        }
    }
};
