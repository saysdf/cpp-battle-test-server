//
// Created by oleg on 11/11/24.
//
#pragma once
class PositionComponent {
public:
    uint32_t x, y;
    bool isBlocking;

    PositionComponent(uint32_t x, uint32_t y, bool isBlocking = true) : x(x),
                                                                        y(y),
                                                                        isBlocking(isBlocking) {}
};
