#pragma once

#include "ipc/common.hpp"

#define MSG_TYPE_INIT_REQUEST GET_COUNTER
struct InitRequest {
  MAKE_STRING(ignoredTopic);
};

#define MSG_TYPE_INIT_RESPONSE GET_COUNTER
struct InitResponse {};

#define MSG_TYPE_UNSUBSCRIBE_REQUEST GET_COUNTER
struct UnsubscribeRequest {
  requestId_t id;
};

#define MSG_TYPE_UNSUBSCRIBE_RESPONSE GET_COUNTER
struct UnsubscribeResponse {};

#define MSG_TYPE_MSG_REQUEST GET_COUNTER
struct MsgRequest {
  requestId_t id;
  primaryKey_t(primaryKey);
  double targetFrequency;
};

#define MSG_TYPE_MSG_RESPONSE GET_COUNTER
struct MsgResponse {
  MAKE_STRING(shmAddress);
};