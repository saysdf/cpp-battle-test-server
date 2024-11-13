//
// Created by oleg on 11/11/24.
//
#pragma once
#include <tuple>
#include <type_traits>
#include <functional>
#include <memory>
#include "ICommand.h"

template <typename... Commands>
class CommandProcessor {
public:
    // Основной метод для выполнения команды
    template <typename Command>
    void executeCommand(const Command& command) const {
        executeCommandImpl(command, std::make_index_sequence<sizeof...(Commands)>{});
    }

    // Метод регистрации обработчиков команд
    template <typename Command, typename Handler>
    void registerHandler(Handler&& handler) {
        getHandler<Command>() = std::forward<Handler>(handler);
    }

private:
    // хранилище для обработчиков команд
    mutable std::tuple<std::function<void(const Commands&)>...> handlers;

    // получение ссылки на конкретный обработчик
    template <typename Command>
    auto& getHandler() const {
        return std::get<std::function<void(const Command&)>>(handlers);
    }

    // Вспомогательная функция для поиска и вызова подходящего обработчика
    template <typename Command, std::size_t... Is>
    void executeCommandImpl(const Command& command,std::index_sequence<Is...>) const {
        // Рекурсивно вызывает обработчик если типы совпадают
        (invokeHandler<Commands>(command) || ...);
    }

    // Вызывает обработчик команды если тип совпадает
    template <typename CommandType, typename Command>
    bool invokeHandler(const Command& command) const {
        if constexpr (std::is_same_v<CommandType, Command>) {
            auto& handler = getHandler<CommandType>();
            if (handler) {
                handler(command);
                return true;
            }
        }
        return false;
    }
};
