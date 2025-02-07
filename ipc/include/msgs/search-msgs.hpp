#pragma once

#include "ipc/common.hpp"


struct SearchRequest {
  msgKey_t key;
  enum Type : uint8_t {
    NODE,
    TOPIC
  } type;
  char name[MAX_STRING_LENGTH];
};
#define SEARCH_REQUEST_MSG_TYPE __COUNTER__

struct SearchResponse {
  msgKey_t key;
  primaryKey_t primaryKey;
};
#define SEARCH_RESPONSE_MSG_TYPE __COUNTER__
