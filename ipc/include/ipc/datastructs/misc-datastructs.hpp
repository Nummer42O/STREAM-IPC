#pragma once

#include "ipc/common.hpp"

struct InitData {
  char ignoredTopic[MAX_STRING_LENGTH];
};

struct UnsubscribeData {
  requestId_t id;
};

struct MsgData {
  requestId_t id;
  primaryKey_t primaryKey;
  double targetFrequency;
};