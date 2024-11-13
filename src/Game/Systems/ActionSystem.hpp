//
// Created by oleg on 11/12/24.
//
#pragma once
#include "Game/Components/ActionPointsComponent.hpp"
#include "Game/EventManager.hpp"

class ActionSystem {
private:
    std::unordered_map<uint32_t, std::shared_ptr<ActionPointsComponent>> actionPointsComponent;
    EventManager&                                                        eventManager;

public:
    ActionSystem(EventManager& em):
        eventManager(em) {
        eventManager.subscribe<sw::io::UnitDied>([this](const sw::io::UnitDied& event) {
            actionPointsComponent.erase(event.unitId);
        });
    }

    void addActionPointsComponent(uint32_t id,std::shared_ptr<ActionPointsComponent> ap) {
        actionPointsComponent[id] = ap;
    }

    template <typename TUnit>
    void registerObject(TUnit& visitor) {
        addActionPointsComponent(visitor->id, visitor->actionPoints);
    }

    void update() {
        for (const auto& [id, actionComponent] : actionPointsComponent) {
            actionComponent->refillActionPoints();
        }
    }
};
