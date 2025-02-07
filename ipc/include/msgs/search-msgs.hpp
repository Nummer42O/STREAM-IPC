#pragma once

#include "ipc/common.hpp"

struct SearchRequest {
  msgKey_t key;
  enum Type {
    NODE,
    TOPIC
  } type;
  char name[MAX_STRING_LENGTH];
};