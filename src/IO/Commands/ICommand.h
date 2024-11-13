//
// Created by oleg on 11/11/24.
//

#pragma once

struct ICommand {
	virtual const char* getName() const = 0;

	virtual ~ICommand() = default;
};


