//
// Created by oleg on 11/12/24.
//
#pragma once
#include <cstdint>

class ActionPointsComponent {
private:
    int64_t currentActionPoints;
    uint32_t maxActionPoints;

public:
    ActionPointsComponent(uint32_t maxActionPoints = 1) : maxActionPoints(maxActionPoints),
                                                          currentActionPoints(0)
    {}

    void changeCurrentActionPoints(uint32_t value) { currentActionPoints = value; }
    void changeMaxActionPoints(uint32_t value) { maxActionPoints = value; }
    void refillActionPoints() { currentActionPoints = maxActionPoints; }

    bool doAction(uint32_t cost = 1) {
        if (currentActionPoints >= cost) {
            currentActionPoints -= cost;
            return true;
        }
        return false;
    }
};
