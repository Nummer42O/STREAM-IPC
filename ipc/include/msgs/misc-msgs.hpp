#pragma once

#include "ipc/common.hpp"

struct InitRequest {
  msgKey_t key;
  char ignoredTopic[MAX_STRING_LENGTH];
};

struct UnsubscribeRequest {
  msgKey_t key;
  requestId_t id;
};

struct MsgRequest {
  msgKey_t key;
  requestId_t id;
  primaryKey_t primaryKey;
  double targetFrequency;
};