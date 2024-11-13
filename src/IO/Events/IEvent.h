//
// Created by oleg on 11/11/24.
//
#pragma once

struct IEvent {
    virtual const char* getName() const = 0;

    virtual ~IEvent() = default;
};
