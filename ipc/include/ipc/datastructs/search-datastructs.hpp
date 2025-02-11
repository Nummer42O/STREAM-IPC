#pragma once

#include "ipc/common.hpp"

#define MSG_TYPE_SEARCH_REQUEST GET_COUNTER
struct SearchRequest {
  enum Type: uint8_t {
    NODE,
    TOPIC
  } type;
  primaryKey_t primaryKey;
};

#define MSG_TYPE_SEARCH_RESPONSE GET_COUNTER
struct SearchResponse {
  primaryKey_t primaryKey;
};