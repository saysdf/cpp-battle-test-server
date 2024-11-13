//
// Created by oleg on 11/11/24.
//
#pragma once
#include "Game/Components/HealthComponent.hpp"
#include "Game/EventManager.hpp"
#include "IO/Events/UnitDied.hpp"

#include <memory>
#include <unordered_map>

class HealthSystem {
private:
    std::unordered_map<uint32_t, std::shared_ptr<HealthComponent>> healthComponents;
    EventManager&                                                  eventManager;

public:
    HealthSystem(EventManager& em) :
        eventManager(em) {}

    void addHealthComponent(uint32_t unitId,std::shared_ptr<HealthComponent> health) {
        healthComponents[unitId] = health;
    }

    template <typename TUnit>
    void registerObject(TUnit& visitor) {
        addHealthComponent(visitor->id, visitor->health);
    }

    void removeHealthComponent(int unitId) {
        healthComponents.erase(unitId);
    }

    void update() {
        for (auto it = healthComponents.begin(); it != healthComponents.end();) {
            if (!it->second->isAlive()) {
                eventManager.publishEvent(sw::io::UnitDied{it->first});
                it = healthComponents.erase(it); // Remove dead unit from healthComponents
            } else {
                ++it;
            }
        }
    }
};
