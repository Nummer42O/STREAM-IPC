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

#define MSG_TYPE_INTERN_PROC_SWITCH_RESPONSE GET_COUNTER
struct NodeSwitchResponse
{
  type_t type;
  primaryKey_t primary_key;
  bool alive;
  uint64_t aliveChangeTime;
  uint32_t bootCounter;
  pid_t pid;
  MAKE_STRING(pub);
  MAKE_STRING(sub);
  MAKE_STRING(service);
  MAKE_STRING(client);
};