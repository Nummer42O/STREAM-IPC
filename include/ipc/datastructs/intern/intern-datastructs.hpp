#pragma once

#include "ipc/common.hpp"


#define MSG_TYPE_INTERN_PROC_SWITCH_REQUEST GET_COUNTER
struct ProcSwitchRequest
{
  bool updates;
};

#define MSG_TYPE_INTERN_PROC_SWITCH_RESPONSE GET_COUNTER
struct ProcSwitchResponse
{
  pid_t pid;
};