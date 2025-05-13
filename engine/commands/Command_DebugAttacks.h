#pragma once
#ifndef COMMAND_DEBUGATTACKS_H
#define COMMAND_DEBUGATTACKS_H

#include "../Command.h"

class Command_DebugAttacks : public Command
{
public:
	Command_DebugAttacks(Engine* pEngine);
	virtual bool Try(const std::string& commandString) override;
};


#endif