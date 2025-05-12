#pragma once
#ifndef COMMAND_DEBUGINTRINSICS_H
#define COMMAND_DEBUGINTRINSICS_H

#include "../Command.h"

class Command_DebugIntrinsics : public Command
{
public:
    Command_DebugIntrinsics(Engine* pEngine);
    virtual bool Try(const std::string& commandString) override;
};

#endif // COMMAND_DEBUGINTRINSICS_H