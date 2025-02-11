#pragma once

#include "ipc/common.hpp"


struct SearchRequest {
  MAKE_MSG_TYPE;

  enum Type: uint8_t {
    NODE,
    TOPIC
  } type;
  primaryKey_t primaryKey;
};

struct SearchResponse {
  MAKE_MSG_TYPE;

  primaryKey_t primaryKey;
};
