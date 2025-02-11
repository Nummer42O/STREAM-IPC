#pragma once

#include "ipc/common.hpp"


struct InitRequest {
  MAKE_MSG_TYPE;

  MAKE_STRING(ignoredTopic);
};

struct InitResponse {
  MAKE_MSG_TYPE;
};

struct UnsubscribeRequest {
  MAKE_MSG_TYPE;

  requestId_t id;
};

struct UnsubscribeResponse {
  MAKE_MSG_TYPE;
};

struct MsgRequest {
  MAKE_MSG_TYPE;

  requestId_t id;
  primaryKey_t primaryKey;
  double targetFrequency;
};

struct MsgResponse {
  MAKE_MSG_TYPE;

  sharedMemoryLocation_t sharedMemPtr;
};
