cmake_minimum_required(VERSION 3.27)
project(sw_battle_test)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.hpp)
add_executable(sw_battle_test ${SOURCES}
        src/Game/Components/HealthComponent.hpp
        src/Game/Components/MovementComponent.hpp
        src/Game/Components/PositionComponent.hpp
        src/Game/Components/MeleeAttackComponent.hpp
        src/Game/Components/RangedAttackComponent.hpp
        src/Game/GameObjects/Unit.hpp
        src/Game/GameObjects/Swordsman.hpp
        src/Game/GameObjects/Hunter.hpp
        src/Game/GameObjects/Map.hpp
        src/Game/Systems/HealthSystem.hpp
        src/Game/Systems/MovementSystem.hpp
        src/Game/Systems/CombatSystem.hpp
        src/Game/GameEngine.hpp
        src/Game/EventManager.hpp
        src/IO/Commands/ICommand.h
        src/IO/Events/IEvent.h
        src/IO/Commands/CommandProcessor.hpp
        src/Game/Components/ActionPointsComponent.hpp
        src/Game/Systems/ActionSystem.hpp
)

target_include_directories(sw_battle_test PUBLIC src/)
