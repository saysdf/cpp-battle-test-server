//
// Created by oleg on 11/11/24.
//
#pragma once
#include "IO/Events/IEvent.h"

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

class EventManager {
private:
    using EventHandler = std::function<void(const IEvent &)>;
    std::unordered_map<std::string, std::vector<EventHandler> > listeners;

public:
    template<typename EventType>
    void subscribe(std::function<void(const EventType &)> handler) {
        listeners[EventType::Name].emplace_back([handler](const IEvent &event) {
            handler(static_cast<const EventType &>(event));
        });
    }

    void publishEvent(const IEvent &event) const {
        auto it = listeners.find(event.getName());
        if (it != listeners.end()) { for (const auto &handler: it->second) { handler(event); } }
    }
};
