//
// Created by oleg on 11/11/24.
//
#pragma once
#include "EventManager.hpp"
#include "Game/GameObjects/Hunter.hpp"
#include "Game/GameObjects/Map.hpp"
#include "Game/GameObjects/Swordsman.hpp"
#include "Game/Systems/CombatSystem.hpp"
#include "Game/Systems/HealthSystem.hpp"
#include "Game/Systems/MovementSystem.hpp"
#include "IO/Commands/CommandProcessor.hpp"
#include "IO/Commands/CreateMap.hpp"
#include "IO/Commands/March.hpp"
#include "IO/Commands/SpawnHunter.hpp"
#include "IO/Commands/SpawnSwordsman.hpp"
#include "IO/Events/MapCreated.hpp"
#include "IO/Events/MarchEnded.hpp"
#include "IO/Events/MarchStarted.hpp"
#include "IO/Events/UnitAttacked.hpp"
#include "IO/Events/UnitDied.hpp"
#include "IO/Events/UnitMoved.hpp"
#include "IO/Events/UnitSpawned.hpp"
#include "IO/System/CommandParser.hpp"
#include "IO/System/EventLog.hpp"
#include "Systems/ActionSystem.hpp"

#include <iostream>

using namespace sw::io;

class GameEngine {
private:
    EventManager eventManager;
    sw::EventLog eventLog;

    HealthSystem   healthSystem;
    MovementSystem movementSystem;
    CombatSystem   combatSystem;
    ActionSystem   actionSystem;

    CommandParser                                                   commandParser;
    CommandProcessor<SpawnSwordsman, March, SpawnHunter, CreateMap> commandProcessor;

    std::unordered_map<uint32_t, std::shared_ptr<Unit>> units;
    std::list<uint32_t>                                 orderOfAction;
    std::shared_ptr<Map>                                map;
    uint64_t                                            tick             = 1;
    bool                                                anythingHappened = false;

public:
    GameEngine() :
        eventManager(),
        healthSystem(eventManager),
        movementSystem(eventManager),
        combatSystem(eventManager),
        actionSystem(eventManager),
        commandProcessor(),
        map(nullptr) {
        // Регистрация обработчиков команд
        commandProcessor.registerHandler<CreateMap>([this](const CreateMap& command) {
            map = std::make_shared<Map>(command.width, command.height);
            movementSystem.registerMap(map);
            combatSystem.registerMap(map);
            eventManager.publishEvent(MapCreated{command.width, command.height});
        });
        commandProcessor.registerHandler<SpawnSwordsman>([this](const SpawnSwordsman& command) {
            auto swordsman = std::make_shared<Swordsman>(command.unitId, command.x, command.y, command.hp,
                                                         command.strength);
            if (!map->addUnit(swordsman)) {
                //юнит автоудаляется если не удалось добавить на карту
                return false;
            }
            units[command.unitId] = swordsman;
            combatSystem.registerObject(swordsman);
            movementSystem.registerObject(swordsman);
            healthSystem.registerObject(swordsman);
            actionSystem.registerObject(swordsman);
            eventManager.publishEvent(UnitSpawned{command.unitId, "Swordsman", command.x, command.y});
            return true;
        });

        commandProcessor.registerHandler<SpawnHunter>([this](const SpawnHunter& command) {
            auto hunter = std::make_shared<Hunter>(command.unitId, command.x, command.y, command.hp, command.strength,
                                                   command.agility, command.range);
            if (!map->addUnit(hunter)) {
                //юнит автоудаляется если не удалось добавить на карту
                return false;
            }
            units[command.unitId] = hunter;
            combatSystem.registerObject(hunter);
            movementSystem.registerObject(hunter);
            healthSystem.registerObject(hunter);
            actionSystem.registerObject(hunter);
            eventManager.publishEvent(UnitSpawned{command.unitId, "Hunter", command.x, command.y});
            return true;
        });
        commandProcessor.registerHandler<March>([this](const March& command) {
            movementSystem.setTarget(command.unitId, command.targetX, command.targetY);
            auto unit = units[command.unitId];
            eventManager.publishEvent(sw::io::MarchStarted{
                                          command.unitId, unit->position->x, unit->position->y, command.targetX,
                                          command.targetY
                                      });
        });

        commandParser.add<CreateMap>([this](CreateMap command) {
            executeCommand(command);
        });
        commandParser.add<SpawnSwordsman>([this](SpawnSwordsman command) {
            executeCommand(command);
        });
        commandParser.add<SpawnHunter>([this](SpawnHunter command) {
            executeCommand(command);
        });
        commandParser.add<March>([this](March command) {
            executeCommand(command);
        });
        // Подписка на события для логов
        eventManager.subscribe<UnitSpawned>([this](UnitSpawned event) {
            orderOfAction.push_back(event.unitId);
            eventLog.log(tick, event);
            anythingHappened = true;
        });
        eventManager.subscribe<UnitAttacked>([this](UnitAttacked event) {
            eventLog.log(tick, event);
            anythingHappened = true;
            healthSystem.update(event.targetUnitId);
        });
        eventManager.subscribe<UnitDied>([this](UnitDied event) {
            auto it = units.find(event.unitId);
            if (it != units.end()) {
                auto unit = it->second;
                map->removeUnit(unit);
                units.erase(event.unitId);
                orderOfAction.remove(event.unitId);
            }
            eventLog.log(tick, event);
            anythingHappened = true;
        });
        eventManager.subscribe<UnitMoved>([this](UnitMoved event) {
            map->moveUnit(units[event.unitId], event.oldx, event.oldy, event.x, event.y);
            eventLog.log(tick, event);
            anythingHappened = true;
        });
        eventManager.subscribe<MapCreated>([this](MapCreated event) {
            eventLog.log(tick, event);
            anythingHappened = true;
        });
        eventManager.subscribe<MarchStarted>([this](MarchStarted event) {
            eventLog.log(tick, event);
            anythingHappened = true;
        });
        eventManager.subscribe<MarchEnded>([this](MarchEnded event) {
            eventLog.log(tick, event);
            anythingHappened = true;
        });
    }

    // Выполнение команды
    void processCommands(std::istream& stream) {
        commandParser.parse(stream);
    }

    template <typename Command>
    void executeCommand(const Command& command) {
        commandProcessor.executeCommand(command);
    }

    void update() {
        tick++;
        actionSystem.update();
        for (auto order : orderOfAction) {
            combatSystem.update(order);
            movementSystem.update(order);
        }
    }

    void loop() {
        while (anythingHappened) {
            anythingHappened = false;
            update();
        }
    }
};
