#pragma once

#include "ipc/common.hpp"

struct InitRequest {
  char ignoredTopic[MAX_STRING_LENGTH];
};

struct UnsubscribeRequest {
  requestId_t id;
};

struct MsgRequest {
  requestId_t id;
  primaryKey_t primaryKey;
  double targetFrequency;
};