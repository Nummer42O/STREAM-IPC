#pragma once

#include "ipc/common.hpp"

struct InitDataRequest {
  std::string ignoredTopic;
};

struct UnsubscribeDataRequest {
  requestId_t id;
};

struct MsgDataRequest {
  requestId_t id;
  primaryKey_t primaryKey;
  double targetFrequency;
};

struct InitDataResponse {
  char ignoredTopics[MAX_ARRAY_LENGTH][MAX_STRING_LENGTH];
};

struct UnsubscribeDataResponse {
};

struct MsgDataResponse {
  uint32_t shmlPtr;
};