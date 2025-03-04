#pragma once

#include "ipc/common.hpp"


enum type_t {
  NODE,
  PUB,
  SUB,
  SERVICE,
  CLIENT,
};

#define MSG_TYPE_INTERN_PROC_SWITCH_REQUEST GET_COUNTER
struct NodeSwitchRequest
{
  bool updates;
};