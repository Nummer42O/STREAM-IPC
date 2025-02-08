#pragma once

#include <vector>

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
  std::vector<std::string> ignoredTopics;
};

struct MsgDataResponse {
  uint32_t shmlPtr;
};

struct AckDataResponse {};

using UnsubscribeDataResponse = AckDataResponse;