//
// Created by oleg on 11/12/24.
//
#pragma once
#include <list>
#include <memory>
#include <unordered_map>
#include "Unit.hpp"


class Map {
private:
    static uint64_t generatePositionHash(uint32_t x,uint32_t y) {
        return (static_cast<uint64_t>(x) << 32) | y;
    }

    uint32_t width;

public:
    [[nodiscard]]
    uint32_t width1() const {
        return width;
    }

    [[nodiscard]]
    uint32_t height1() const {
        return height;
    }

private:
    uint32_t height;

    // Используем unordered_map, где каждый ключ (позиция) хранит вектор юнитов
    std::unordered_map<uint64_t, std::list<std::shared_ptr<Unit>>> grid;

public:
    Map(uint32_t width,uint32_t height) :
        width(width), height(height) {}

    bool isWithinBounds(uint32_t x,uint32_t y) const {
        return x < width && y < height;
    }

    // Проверка, свободна ли клетка для наземных юнитов (учитывает только блокирующие юниты)
    bool isCellEmpty(uint32_t x,uint32_t y) const {
        auto hash = generatePositionHash(x, y);
        if (grid.find(hash) != grid.end()) {
            // Проверяем, есть ли наземные юниты, занимающие клетку
            for (const auto& unit : grid.at(hash)) {
                if (unit->position->isBlocking) {
                    return false;
                }
            }
        }
        return true;
    }

    bool addUnit(std::shared_ptr<Unit> unit) {
        uint32_t x = unit->position->x;
        uint32_t y = unit->position->y;
        if (isWithinBounds(x, y)) {
            auto hash = generatePositionHash(x, y);
            grid[hash].push_back(unit);
            return true;
        }
        return false;
    }

    bool moveUnit(std::shared_ptr<Unit> unit,uint32_t x,uint32_t y,uint32_t tx,uint32_t ty) {
        auto hash  = generatePositionHash(x, y);
        auto thash = generatePositionHash(tx, ty);
        if (!isCellEmpty(tx, ty))
            return false;
        auto& cellUnits = grid[hash];
        cellUnits.erase(std::remove(cellUnits.begin(), cellUnits.end(), unit), cellUnits.end());
        if (cellUnits.empty()) {
            grid.erase(hash); // Удаляем позицию из map, если она пустая
        }
        grid[thash].push_back(unit);
        return true;
    }

    void removeUnit(std::shared_ptr<Unit> unit) {
        auto hash = generatePositionHash(unit->position->x, unit->position->y);
        if (grid.find(hash) != grid.end()) {
            auto& cellUnits = grid[hash];
            cellUnits.erase(std::remove(cellUnits.begin(), cellUnits.end(), unit), cellUnits.end());
            if (cellUnits.empty()) {
                grid.erase(hash); // Удаляем позицию из map, если она пустая
            }
        }
    }

    // Получение всех юнитов на данной позицииЬ Берем первого из списка.
    const std::list<std::shared_ptr<Unit>>& getUnitsAt(uint32_t x,uint32_t y) const {
        static const std::list<std::shared_ptr<Unit>> empty;
        auto                                          hash = generatePositionHash(x, y);
        if (grid.find(hash) != grid.end()) {
            return grid.at(hash);
        }
        return empty;
    }

    // Поиск ближайшего юнита в радиусе, учитывая все типы юнитов Берем первого из списка.
    uint32_t findNearestUnitInRadius(uint32_t idOfSearcher,uint32_t x,uint32_t y,uint32_t radius) const {
        uint32_t nearestUnitId = 0;
        uint32_t minDistance   = radius + 1;

        for (const auto& [hash, unit] : grid) {
            auto checkableUnit = unit.front();
            // TODO добавить проверки на летающий не летающий и т.д. в листе лежат все юниты в ячейке. Но блокирует ячейку только один.
            if (idOfSearcher == checkableUnit->id)
                continue;
            // Вычисление Чебышёвского расстояния
            int64_t dx       = std::abs(static_cast<int64_t>(checkableUnit->position->x) - static_cast<int64_t>(x));
            int64_t dy       = std::abs(static_cast<int64_t>(checkableUnit->position->y) - static_cast<int64_t>(y));
            int64_t distance = std::max(dx, dy);

            // Проверка, если юнит находится в пределах радиуса и ближе минимального найденного расстояния
            if (distance <= radius && distance < minDistance) {
                nearestUnitId = checkableUnit->id;
                minDistance   = distance;
            }
        }
        return nearestUnitId;
    }

    // Поиск случайного юнита в пределах дальности

    uint32_t findRandomUnitInRange(uint32_t idOfSearcher,uint32_t x,uint32_t y,uint32_t rangeMin,
                                   uint32_t rangeMax) const {
        std::vector<uint32_t> potentialTargets;

        for (const auto& [hash, unit] : grid) {
            auto checkableUnit = unit.front();
            // TODO добавить проверки на летающий не летающий и т.д. в листе лежат все юниты в ячейке. Но блокирует ячейку только один.
            if (idOfSearcher == checkableUnit->id)
                continue;
            // Вычисление Чебышёвского расстояния!!!! МИНУС 1 потому что расстояние это количество клеток между юнитами. А не расстояние до юнита.
            int64_t dx       = std::abs(static_cast<int64_t>(checkableUnit->position->x) - static_cast<int64_t>(x)) - 1;
            int64_t dy       = std::abs(static_cast<int64_t>(checkableUnit->position->y) - static_cast<int64_t>(y)) - 1;
            int64_t distance = std::max(dx, dy);

            // Проверка, если юнит находится в пределах диапазона
            if (distance >= rangeMin && distance <= rangeMax) {
                potentialTargets.push_back(checkableUnit->id);
            }
        }
        if (!potentialTargets.empty()) {
            return potentialTargets[rand() % potentialTargets.size()];
        }
        return 0;
    }
};
