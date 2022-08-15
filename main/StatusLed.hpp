#pragma once

#include "wrappers/Task.hpp"

using util::wrappers::TaskWithMemberFunctionBase;

class StatusLed : public TaskWithMemberFunctionBase
{
public:
    explicit StatusLed(bool &isConnected)
        : TaskWithMemberFunctionBase("statusLedTask", 256, osPriorityBelowNormal3),
          isConnected(isConnected) //
          {};

protected:
    void taskMain() override;

private:
    bool &isConnected;
};