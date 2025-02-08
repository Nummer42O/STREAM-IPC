#pragma once

#include "ipc/common.hpp"

struct InitRequest {
  msgKey_t key;
  char ignoredTopic[MAX_STRING_LENGTH];
};
#define INIT_REQUEST_MSG_TYPE __COUNTER__

struct UnsubscribeRequest {
  msgKey_t key;
  requestId_t id;
};
#define UNSUBSCRIBE_REQUEST_MSG_TYPE __COUNTER__

struct MsgRequest {
  msgKey_t key;
  requestId_t id;
  primaryKey_t primaryKey;
  double targetFrequency;
};
#define MSG_REQUEST_MSG_TYPE __COUNTER__

struct InitResponse {
  msgKey_t key;
  char ignoredTopics[MAX_ARRAY_LENGTH][MAX_STRING_LENGTH];
};
#define INIT_RESPONSE_MSG_TYPE __COUNTER__

struct MsgResponse {
  msgKey_t key;
  uint32_t shmlPtr;
};
#define MSG_RESPONSE_MSG_TYPE __COUNTER__

struct AckResponse {
  msgKey_t key;
};
#define ACK_RESPONSE_MSG_TYPE __COUNTER__

using UnsubscribeResponse = AckResponse;
#define UNSUBSCRIBE_RESPONSE_MSG_TYPE ACK_RESPONSE_MSG_TYPE
