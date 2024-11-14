//
// Created by oleg on 11/11/24.
//
#pragma once
#include "Game/Components/HealthComponent.hpp"
#include "Game/Components/MeleeAttackComponent.hpp"
#include "Game/Components/PositionComponent.hpp"
#include "Game/Components/RangedAttackComponent.hpp"
#include "Game/EventManager.hpp"
#include "IO/Events/UnitAttacked.hpp"
#include "IO/Events/UnitDied.hpp"

#include <memory>
#include <unordered_map>

class CombatSystem {
private:
    std::unordered_map<uint32_t, std::shared_ptr<MeleeAttackComponent>>  meleeAttackComponents;
    std::unordered_map<uint32_t, std::shared_ptr<RangedAttackComponent>> rangedAttackComponents;
    std::unordered_map<uint32_t, std::shared_ptr<PositionComponent>>     positionComponents;
    std::unordered_map<uint32_t, std::shared_ptr<HealthComponent>>       healthComponents;
    std::unordered_map<uint32_t, std::shared_ptr<ActionPointsComponent>> actionPointsComponent;
    EventManager&                                                        eventManager;
    std::shared_ptr<Map>                                                 map;

public:
    CombatSystem(EventManager& em) :
        eventManager(em) {
        eventManager.subscribe<sw::io::UnitDied>([this](const sw::io::UnitDied& event) {
            meleeAttackComponents.erase(event.unitId);
            rangedAttackComponents.erase(event.unitId);
            positionComponents.erase(event.unitId);
            healthComponents.erase(event.unitId);
            actionPointsComponent.erase(event.unitId);
        });
    }

    void registerMap(std::shared_ptr<Map> map) {
        this->map = map;
    }

    // Регистрируем варианты для ближнего и дальнего боя.
    template <typename T, typename = void>
    struct has_melee_attack : std::false_type {};

    template <typename T>
    struct has_melee_attack<T, std::void_t<decltype((*std::declval<T>()).meleeAttack)>> : std::true_type {};

    template <typename T, typename = void>
    struct has_ranged_attack : std::false_type {};

    template <typename T>
    struct has_ranged_attack<T, std::void_t<decltype((*std::declval<T>()).rangedAttack)>> : std::true_type {};

    template <typename TUnit>
    void registerObject(TUnit& visitor) {
        //TODO проверка нужна при добавлении нового типа атаки или подобных фичей чтобы проверить на этапе компиляции что такой темплейт создался.
        // static_assert(has_ranged_attack<TUnit>::value, "TUnit does not have ranged_attack");
        if constexpr (has_melee_attack<TUnit>::value) {
            addMeleeAttackComponent<TUnit>(visitor->id, visitor->meleeAttack);
        }
        if constexpr (has_ranged_attack<TUnit>::value) {
            addRangedAttackComponent<TUnit>(visitor->id, visitor->rangedAttack);
        }
        addPositionComponent(visitor->id, visitor->position);
        addHealthComponent(visitor->id, visitor->health);
        addActionPointsComponent(visitor->id, visitor->actionPoints);
    }

    template <typename TUnit>
    std::enable_if_t<has_melee_attack<TUnit>::value> addMeleeAttackComponent(
        uint32_t unitId,std::shared_ptr<MeleeAttackComponent> component) {
        meleeAttackComponents[unitId] = component;
    }

    template <typename TUnit>
    std::enable_if_t<has_ranged_attack<TUnit>::value> addRangedAttackComponent(
        uint32_t unitId,std::shared_ptr<RangedAttackComponent> component) {
        rangedAttackComponents[unitId] = component;
    }

    void addActionPointsComponent(uint32_t unitId,std::shared_ptr<ActionPointsComponent> ap) {
        actionPointsComponent[unitId] = ap;
    }

    void addPositionComponent(uint32_t unitId,std::shared_ptr<PositionComponent> position) {
        positionComponents[unitId] = position;
    }

    void addHealthComponent(uint32_t unitId,std::shared_ptr<HealthComponent> health) {
        healthComponents[unitId] = health;
    }

    bool performAp(uint32_t unitId,uint32_t cost) {
        auto it = actionPointsComponent.find(unitId);
        if (it != actionPointsComponent.end()) {
            auto action = it->second;
            return action->doAction(cost);
        }
        return false;
    }

    void update(uint32_t attackerId) {
        auto itm = meleeAttackComponents.find(attackerId);
        if (itm != meleeAttackComponents.end()) {
            auto attack = itm->second;
            if (executeMeleeAttack(attackerId, attack))
                return;
        }
        auto itr = rangedAttackComponents.find(attackerId);
        if (itr != rangedAttackComponents.end()) {
            auto attack = itr->second;
            if (executeRangedAttack(attackerId, attack))
                return;
        }
    }

    void update() {
        for (const auto& [attackerId, meleeComponent] : positionComponents) {
            auto itm = meleeAttackComponents.find(attackerId);
            if (itm != meleeAttackComponents.end()) {
                auto attack = itm->second;
                if (executeMeleeAttack(attackerId, attack))
                    continue;
            }
            auto itr = rangedAttackComponents.find(attackerId);
            if (itr != rangedAttackComponents.end()) {
                auto attack = itr->second;
                if (executeRangedAttack(attackerId, attack))
                    continue;
            }
        }
    }

private:
    bool canPerformRangedAttack(uint32_t attackerId) {
        auto position = positionComponents[attackerId];
        return map->findNearestUnitInRadius(attackerId, position->x, position->y, 1) == 0;
    }

    bool executeRangedAttack(uint32_t attackerId,std::shared_ptr<RangedAttackComponent> rangedComponent) {
        auto     position = positionComponents[attackerId];
        uint32_t rangeMin = rangedComponent->getRangeMin();
        uint32_t rangeMax = rangedComponent->getRange();

        uint32_t targetId = map->findRandomUnitInRange(attackerId, position->x, position->y, rangeMin, rangeMax);
        auto     it       = healthComponents.find(targetId);
        if (targetId && it != healthComponents.end()) {
            auto healthTarget = it->second;
            if (performAp(attackerId, rangedComponent->getApCost())) {
                uint32_t damage = rangedComponent->getDamage();
                healthTarget->takeDamage(damage);
                eventManager.publishEvent(sw::io::UnitAttacked{
                                              attackerId, targetId, damage, healthComponents[targetId]->getHP()
                                          });
                return true;
            }
        }
        return false;
    }

    bool executeMeleeAttack(uint32_t attackerId,std::shared_ptr<MeleeAttackComponent> meleeComponent) {
        auto     position = positionComponents[attackerId];
        uint32_t radius   = 1; // Радиус для ближнего боя — соседние клетки

        uint32_t targetId = map->findNearestUnitInRadius(attackerId, position->x, position->y, radius);
        auto     it       = healthComponents.find(targetId);
        if (targetId && it != healthComponents.end()) {
            auto healthTarget = it->second;
            if (performAp(attackerId, meleeComponent->getApCost())) {
                uint32_t damage = meleeComponent->getDamage();
                healthTarget->takeDamage(damage);
                eventManager.publishEvent(sw::io::UnitAttacked{
                                              attackerId, targetId, damage, healthComponents[targetId]->getHP()
                                          });
                return true;
            }
        }
        return false;
    }
};
